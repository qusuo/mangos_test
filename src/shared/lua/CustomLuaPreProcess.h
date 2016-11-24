#pragma once

/****************************************************************

                   通用LUA脚本文本内容预处理器

    由于LUA词法分析器不具有代码文本预处理的功能且在实际是用中需要用到代码包含等
  类似C语言的预处理功能，因此提供此类用于对脚本文本内容进行简单的预处理。为了使
  预处理指令的代码语法兼容lua语法，采用类似在HTML中书写JavaScript代码的方式，
  使用原生语言的注释语法实现预处理语法。在lua预处理器中预处理指令的语法规则如下：
  --#directive implemente data
    即，预处理指令必须在行开始的第一个字符处书写，一个预处理指令不得超过一行。预
  处理指令是不区分大小写的。

  预处理目前实现了下列功能：
    1）代码文件包含，使用“--#include "FilePath"”来包含一个代码文件。
	  代码文件的搜索顺序是首先从当前文件的目录开始使用相对路径搜索，如果找不到
	  文件则在预设的代码文件搜索目录中降序进行相对目录搜索。代码包含指令最多支持
	  对最终长度为1024个字符的路径进行处理
	2）宏定义，使用“--#define NAME [VALUE]”来定义一个宏，宏的值是字符串。
	  “--#undef NAME”来取消一个宏的定义。宏的名称长度不得超过30个字符，宏的值
	  长度不得超过126个字符。
    3）基于宏的条件检测，使用“--#ifdef”、“--#ifndef”、“--#if MACRO == VALUE”、
	   “--#if MACRO != VALUE”、“--#else”、“--#elif”、“--#endif”。

****************************************************************/
class CCustomLuaPreProcessor :
	protected wylib::stream::CMemoryStream
{
public:
	typedef CMemoryStream Inherited;
	/* 定义源文件行号信息 */
	typedef struct LineRange
	{
		TCHAR sFileName[MAX_PATH]; //源文件路径，超过MAX_PATH则使用短文件名
		INT_PTR nLineStart; //预处理后的起始行号
		INT_PTR nLineEnd;	//预处理后的结束行号
	};
	class CLineRangeList : public wylib::container::CBaseList<LineRange>
	{
	public:
		/* 获取预处理后的行号对应的原始文件路径以及行号
		* nLineNo 预处理后的行号，行号从1开始
		* nSrcLineNo 用于保存原始文件行号，行号从1开始
		* @return 如果找到对应的行号，则函数返回文件路径字符串指针并且填充nSrcLineNo为原始文件行号，否则函数返回NULL
		*/
		LPCTSTR getSourceLineNumber(INT_PTR nLineNo, INT_PTR &nSrcLineNo);
	};
public:
	CCustomLuaPreProcessor();
	~CCustomLuaPreProcessor();

	/* 对输入脚本内容进行预处理
	 *sSourceText	输入字符串起始指针，输入字符串必须是0结尾的
	 *sFilePath		代码所在文件包含文件名的完整路径，用于进行优先包含目录搜索
	 *cNewLine		换行字符，默认值为'\n'
	 *@return 返回处理后的字符串指针，字符串在预处理器销毁或进行下次分析之前一直有效
	 */
	LPCTSTR parse(LPCTSTR sSourceText, LPCTSTR sFilePath, const TCHAR cNewLine = '\n');

	//添加包含文件搜索目录
	VOID addIncludeDirectory(LPCTSTR sPath)
	{
		m_IncludeDirList.add(new String(sPath));
	}
	//清空包含文件搜索目录
	VOID clearIncludeDirectory();
	/* 添加预处理宏定义
	 * sName		宏名称
	 * sValue		宏参数值
	 */
	VOID addMacro(LPCTSTR sName, LPCTSTR sValue = NULL);
	//移除一个宏定义
	VOID removeMacro(LPCTSTR sName);
	//清空预处理宏定义
	VOID clearMacroList();
	/* 获取源文件行号对照表
	 * pLineRange 用于保存文件行号信息的缓冲区
	 * nCount 缓冲区中可存储的行号信息数量
	 *@return 如果pLineRange为空，则函数返回保存所有的行信息所需的行号信息数量，否则函数返回值为实际向pLineRange中拷贝行信息数量
	 */
	INT_PTR getLineRangeData(LineRange *pLineRange, INT_PTR nCount);
	/* 获取预处理后的行号对应的原始文件路径以及行号
	* nLineNo 预处理后的行号，行号从1开始
	* nSrcLineNo 用于保存原始文件行号，行号从1开始
	* @return 如果找到对应的行号，则函数返回文件路径字符串指针并且填充nSrcLineNo为原始文件行号，否则函数返回NULL
	*/
	LPCTSTR getSourceLineNumber(INT_PTR nLineNo, INT_PTR &nSrcLineNo);

protected:
	/* 保留当前处理结果内并对新的输入脚本内容进行预处理
	 *sSourceText	输入字符串起始指针，输入字符串必须是0结尾的
	 */
	LPCTSTR ParseSource(LPCTSTR sSourceText);
	//保存当前预处理器分析环境，将分析环境压入环境栈中
	inline VOID SaveParseEnvir()
	{
		m_ParseEnvirStack.add(m_ParseEnvir);
	}
	//恢复上一个预处理器分析环境，恢复后将删除栈顶的分析环境对象
	inline VOID RestorsParseEnvir()
	{
		m_ParseEnvir = m_ParseEnvirStack[m_ParseEnvirStack.count()-1];
		m_ParseEnvirStack.trunc(m_ParseEnvirStack.count()-1);
	}
	//分析以“--#”文本开头的预处理行
	VOID ProcessLine(LPCTSTR sLineText);
	/* 处理并执行预处理指令的功能
	 * 如果子类需要对已实现更多的预处理指令或修改现有预处理指令的实现规则，则可以覆盖此函数
	 */
	VOID ProcessDirective(LPCTSTR sDirective, LPCTSTR sData);
	//对#include预处理指令功能的实现
	VOID DirectiveOfInclude(LPCTSTR sData);
	//对#define预处理指令的实现
	VOID DirectiveOfDefine(LPCTSTR sData);
	//对#undef预处理指令的实现
	VOID DirectiveOfUndef(LPCTSTR sData);
	//对#ifdef预处理指令的实现
	VOID DirectiveOfIfdef(LPCTSTR sData);
	//对#ifndef预处理指令的实现
	VOID DirectiveOfIfndef(LPCTSTR sData);
	//对#if预处理指令的实现
	VOID DirectiveOfIf(LPCTSTR sData);
	//对#else预处理指令的实现
	VOID DirectiveOfElse(LPCTSTR sData);
	//对#elif预处理指令的实现
	VOID DirectiveOfElif(LPCTSTR sData);
	//对#endif预处理指令的实现
	VOID DirectiveOfEndif(LPCTSTR sData);

protected:
	//预处理器词对象定义
	typedef TCHAR PPToken[128];

	/*** 语法分析副主函数集合 ***/
	/*
	* Comments: 按空格或WhiteSpace分割词
	* Param LPCTSTR sLinePtr: 输入文本行字符串指针
	* Param IN OUT LPCTSTR * sTokens: 用于存储每个词的指针的字符串数组
	* Param IN OUT INT_PTR & nTokenCount: 限定最大取词数量。取词完成后此参数将保存实际取的词数量
	* @Return LPCTSTR: 返回经过取词后的输入参数sLinePtr的位置，以便基于返回值继续取词
	*/
	LPCTSTR delimiteLine(LPCTSTR sLinePtr, IN OUT PPToken *sTokens, IN OUT INT_PTR &nTokenCount);
	//获取完全的预处理取词条件值
	void refreshCondition();
	//出现错误，标记终止分析
	inline void abort(){ m_boErrorAbort = true; }
	//情况已包含文件列表
	void clearIncludedFileList();

protected:
	//输出错误的函数
	virtual VOID showError(LPCTSTR sError);
	//带参数格式化的输出错误的函数
	virtual VOID showErrorFormat(LPCTSTR sFmt, ...);
private:
	/* 保留当前处理结果内并对新的输入脚本内容进行预处理
	 * 分析代码前会取得代码文件的目录并压入预处理器包含文件分析栈中，处理后会从栈中将目录移除
	 *sSourceText	输入字符串起始指针，输入字符串必须是0结尾的
	 *sFillFilePath	代码文件的完整路径
	 */
	LPCTSTR SaveFileDirAndParse(LPCTSTR sSourceText, LPCTSTR sFillFilePath);
	//搜索并加载包含文件
	bool SearchAndLoadIncludeFile(LPCTSTR sIncludeFileName, bool boEnableMultipleInclude);
	//加载并分析包含文件
	bool LoadIncludeFile(LPCTSTR sIncludeFilePath, bool boEnableMultipleInclude);
	
protected:
	//预处理器分析环境
	typedef struct tagPreProcParseEnvir
	{
		LPCTSTR sFilePath;		//原始文件路径
		LPCTSTR	sParsePtr;		//输入内容分析位置
		LPCTSTR sNewLinePtr;	//新行位置
		LPCTSTR sLineEndPtr;	//行尾位置
		INT_PTR nLineNo;		//当前分析的行号，从1开始
	}PREPROC_PARSERENVIR, *PPREPROC_PARSERENVIR;
	//预处理器中的宏定义
	typedef struct tagMacro
	{
		TCHAR	sName[32];
		TCHAR	sValue[128];
	}PREPROC_MACRO, *PPREPROC_MACRO;

	PREPROC_PARSERENVIR									m_ParseEnvir;		//当前分析环境
	wylib::container::CBaseList<PREPROC_PARSERENVIR>	m_ParseEnvirStack;	//预处理器代码分析环境列表，后进先出
	wylib::container::CBaseList<String*>				m_FilePathStack;	//预处理器包含文件分析栈，后进先出
	wylib::container::CBaseList<String*>				m_IncludeDirList;	//调用者预设的预处理包含文件搜索目录列表
	wylib::container::CBaseList<PREPROC_MACRO>			m_MacroList;		//预定义宏列表
	wylib::container::CBaseList<bool>					m_ConditionList;	//当前预处理条件表
	wylib::container::CBaseList<LPTSTR>					m_IncludedFileList;	//预处理器已经包含过的文件列表
	CLineRangeList										m_LineRangeList;	//源文件行号信息表
	INT_PTR												m_nLineNo;			//当前总计文件行号
	TCHAR												m_NewLineChar;		//新行符，默认是'\n'
	bool												m_boErrorAbort;		//是否因错误而需要终止分析
	bool												m_boCurrCondition;	//当前的预处理条件值
public:
	//通过宏名称获取宏指针
	const PPREPROC_MACRO getMacro(LPCTSTR sName);
};
