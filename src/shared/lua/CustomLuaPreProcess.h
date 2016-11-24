#pragma once

/****************************************************************

                   ͨ��LUA�ű��ı�����Ԥ������

    ����LUA�ʷ������������д����ı�Ԥ����Ĺ�������ʵ����������Ҫ�õ����������
  ����C���Ե�Ԥ�����ܣ�����ṩ�������ڶԽű��ı����ݽ��м򵥵�Ԥ����Ϊ��ʹ
  Ԥ����ָ��Ĵ����﷨����lua�﷨������������HTML����дJavaScript����ķ�ʽ��
  ʹ��ԭ�����Ե�ע���﷨ʵ��Ԥ�����﷨����luaԤ��������Ԥ����ָ����﷨�������£�
  --#directive implemente data
    ����Ԥ����ָ��������п�ʼ�ĵ�һ���ַ�����д��һ��Ԥ����ָ��ó���һ�С�Ԥ
  ����ָ���ǲ����ִ�Сд�ġ�

  Ԥ����Ŀǰʵ�������й��ܣ�
    1�������ļ�������ʹ�á�--#include "FilePath"��������һ�������ļ���
	  �����ļ�������˳�������ȴӵ�ǰ�ļ���Ŀ¼��ʼʹ�����·������������Ҳ���
	  �ļ�����Ԥ��Ĵ����ļ�����Ŀ¼�н���������Ŀ¼�������������ָ�����֧��
	  �����ճ���Ϊ1024���ַ���·�����д���
	2���궨�壬ʹ�á�--#define NAME [VALUE]��������һ���꣬���ֵ���ַ�����
	  ��--#undef NAME����ȡ��һ����Ķ��塣������Ƴ��Ȳ��ó���30���ַ������ֵ
	  ���Ȳ��ó���126���ַ���
    3�����ں��������⣬ʹ�á�--#ifdef������--#ifndef������--#if MACRO == VALUE����
	   ��--#if MACRO != VALUE������--#else������--#elif������--#endif����

****************************************************************/
class CCustomLuaPreProcessor :
	protected wylib::stream::CMemoryStream
{
public:
	typedef CMemoryStream Inherited;
	/* ����Դ�ļ��к���Ϣ */
	typedef struct LineRange
	{
		TCHAR sFileName[MAX_PATH]; //Դ�ļ�·��������MAX_PATH��ʹ�ö��ļ���
		INT_PTR nLineStart; //Ԥ��������ʼ�к�
		INT_PTR nLineEnd;	//Ԥ�����Ľ����к�
	};
	class CLineRangeList : public wylib::container::CBaseList<LineRange>
	{
	public:
		/* ��ȡԤ�������кŶ�Ӧ��ԭʼ�ļ�·���Լ��к�
		* nLineNo Ԥ�������кţ��кŴ�1��ʼ
		* nSrcLineNo ���ڱ���ԭʼ�ļ��кţ��кŴ�1��ʼ
		* @return ����ҵ���Ӧ���кţ����������ļ�·���ַ���ָ�벢�����nSrcLineNoΪԭʼ�ļ��кţ�����������NULL
		*/
		LPCTSTR getSourceLineNumber(INT_PTR nLineNo, INT_PTR &nSrcLineNo);
	};
public:
	CCustomLuaPreProcessor();
	~CCustomLuaPreProcessor();

	/* ������ű����ݽ���Ԥ����
	 *sSourceText	�����ַ�����ʼָ�룬�����ַ���������0��β��
	 *sFilePath		���������ļ������ļ���������·�������ڽ������Ȱ���Ŀ¼����
	 *cNewLine		�����ַ���Ĭ��ֵΪ'\n'
	 *@return ���ش������ַ���ָ�룬�ַ�����Ԥ���������ٻ�����´η���֮ǰһֱ��Ч
	 */
	LPCTSTR parse(LPCTSTR sSourceText, LPCTSTR sFilePath, const TCHAR cNewLine = '\n');

	//��Ӱ����ļ�����Ŀ¼
	VOID addIncludeDirectory(LPCTSTR sPath)
	{
		m_IncludeDirList.add(new String(sPath));
	}
	//��հ����ļ�����Ŀ¼
	VOID clearIncludeDirectory();
	/* ���Ԥ����궨��
	 * sName		������
	 * sValue		�����ֵ
	 */
	VOID addMacro(LPCTSTR sName, LPCTSTR sValue = NULL);
	//�Ƴ�һ���궨��
	VOID removeMacro(LPCTSTR sName);
	//���Ԥ����궨��
	VOID clearMacroList();
	/* ��ȡԴ�ļ��кŶ��ձ�
	 * pLineRange ���ڱ����ļ��к���Ϣ�Ļ�����
	 * nCount �������пɴ洢���к���Ϣ����
	 *@return ���pLineRangeΪ�գ��������ر������е�����Ϣ������к���Ϣ����������������ֵΪʵ����pLineRange�п�������Ϣ����
	 */
	INT_PTR getLineRangeData(LineRange *pLineRange, INT_PTR nCount);
	/* ��ȡԤ�������кŶ�Ӧ��ԭʼ�ļ�·���Լ��к�
	* nLineNo Ԥ�������кţ��кŴ�1��ʼ
	* nSrcLineNo ���ڱ���ԭʼ�ļ��кţ��кŴ�1��ʼ
	* @return ����ҵ���Ӧ���кţ����������ļ�·���ַ���ָ�벢�����nSrcLineNoΪԭʼ�ļ��кţ�����������NULL
	*/
	LPCTSTR getSourceLineNumber(INT_PTR nLineNo, INT_PTR &nSrcLineNo);

protected:
	/* ������ǰ�������ڲ����µ�����ű����ݽ���Ԥ����
	 *sSourceText	�����ַ�����ʼָ�룬�����ַ���������0��β��
	 */
	LPCTSTR ParseSource(LPCTSTR sSourceText);
	//���浱ǰԤ��������������������������ѹ�뻷��ջ��
	inline VOID SaveParseEnvir()
	{
		m_ParseEnvirStack.add(m_ParseEnvir);
	}
	//�ָ���һ��Ԥ�����������������ָ���ɾ��ջ���ķ�����������
	inline VOID RestorsParseEnvir()
	{
		m_ParseEnvir = m_ParseEnvirStack[m_ParseEnvirStack.count()-1];
		m_ParseEnvirStack.trunc(m_ParseEnvirStack.count()-1);
	}
	//�����ԡ�--#���ı���ͷ��Ԥ������
	VOID ProcessLine(LPCTSTR sLineText);
	/* ����ִ��Ԥ����ָ��Ĺ���
	 * ���������Ҫ����ʵ�ָ����Ԥ����ָ����޸�����Ԥ����ָ���ʵ�ֹ�������Ը��Ǵ˺���
	 */
	VOID ProcessDirective(LPCTSTR sDirective, LPCTSTR sData);
	//��#includeԤ����ָ��ܵ�ʵ��
	VOID DirectiveOfInclude(LPCTSTR sData);
	//��#defineԤ����ָ���ʵ��
	VOID DirectiveOfDefine(LPCTSTR sData);
	//��#undefԤ����ָ���ʵ��
	VOID DirectiveOfUndef(LPCTSTR sData);
	//��#ifdefԤ����ָ���ʵ��
	VOID DirectiveOfIfdef(LPCTSTR sData);
	//��#ifndefԤ����ָ���ʵ��
	VOID DirectiveOfIfndef(LPCTSTR sData);
	//��#ifԤ����ָ���ʵ��
	VOID DirectiveOfIf(LPCTSTR sData);
	//��#elseԤ����ָ���ʵ��
	VOID DirectiveOfElse(LPCTSTR sData);
	//��#elifԤ����ָ���ʵ��
	VOID DirectiveOfElif(LPCTSTR sData);
	//��#endifԤ����ָ���ʵ��
	VOID DirectiveOfEndif(LPCTSTR sData);

protected:
	//Ԥ�������ʶ�����
	typedef TCHAR PPToken[128];

	/*** �﷨���������������� ***/
	/*
	* Comments: ���ո��WhiteSpace�ָ��
	* Param LPCTSTR sLinePtr: �����ı����ַ���ָ��
	* Param IN OUT LPCTSTR * sTokens: ���ڴ洢ÿ���ʵ�ָ����ַ�������
	* Param IN OUT INT_PTR & nTokenCount: �޶����ȡ��������ȡ����ɺ�˲���������ʵ��ȡ�Ĵ�����
	* @Return LPCTSTR: ���ؾ���ȡ�ʺ���������sLinePtr��λ�ã��Ա���ڷ���ֵ����ȡ��
	*/
	LPCTSTR delimiteLine(LPCTSTR sLinePtr, IN OUT PPToken *sTokens, IN OUT INT_PTR &nTokenCount);
	//��ȡ��ȫ��Ԥ����ȡ������ֵ
	void refreshCondition();
	//���ִ��󣬱����ֹ����
	inline void abort(){ m_boErrorAbort = true; }
	//����Ѱ����ļ��б�
	void clearIncludedFileList();

protected:
	//�������ĺ���
	virtual VOID showError(LPCTSTR sError);
	//��������ʽ�����������ĺ���
	virtual VOID showErrorFormat(LPCTSTR sFmt, ...);
private:
	/* ������ǰ�������ڲ����µ�����ű����ݽ���Ԥ����
	 * ��������ǰ��ȡ�ô����ļ���Ŀ¼��ѹ��Ԥ�����������ļ�����ջ�У��������ջ�н�Ŀ¼�Ƴ�
	 *sSourceText	�����ַ�����ʼָ�룬�����ַ���������0��β��
	 *sFillFilePath	�����ļ�������·��
	 */
	LPCTSTR SaveFileDirAndParse(LPCTSTR sSourceText, LPCTSTR sFillFilePath);
	//���������ذ����ļ�
	bool SearchAndLoadIncludeFile(LPCTSTR sIncludeFileName, bool boEnableMultipleInclude);
	//���ز����������ļ�
	bool LoadIncludeFile(LPCTSTR sIncludeFilePath, bool boEnableMultipleInclude);
	
protected:
	//Ԥ��������������
	typedef struct tagPreProcParseEnvir
	{
		LPCTSTR sFilePath;		//ԭʼ�ļ�·��
		LPCTSTR	sParsePtr;		//�������ݷ���λ��
		LPCTSTR sNewLinePtr;	//����λ��
		LPCTSTR sLineEndPtr;	//��βλ��
		INT_PTR nLineNo;		//��ǰ�������кţ���1��ʼ
	}PREPROC_PARSERENVIR, *PPREPROC_PARSERENVIR;
	//Ԥ�������еĺ궨��
	typedef struct tagMacro
	{
		TCHAR	sName[32];
		TCHAR	sValue[128];
	}PREPROC_MACRO, *PPREPROC_MACRO;

	PREPROC_PARSERENVIR									m_ParseEnvir;		//��ǰ��������
	wylib::container::CBaseList<PREPROC_PARSERENVIR>	m_ParseEnvirStack;	//Ԥ������������������б�����ȳ�
	wylib::container::CBaseList<String*>				m_FilePathStack;	//Ԥ�����������ļ�����ջ������ȳ�
	wylib::container::CBaseList<String*>				m_IncludeDirList;	//������Ԥ���Ԥ��������ļ�����Ŀ¼�б�
	wylib::container::CBaseList<PREPROC_MACRO>			m_MacroList;		//Ԥ������б�
	wylib::container::CBaseList<bool>					m_ConditionList;	//��ǰԤ����������
	wylib::container::CBaseList<LPTSTR>					m_IncludedFileList;	//Ԥ�������Ѿ����������ļ��б�
	CLineRangeList										m_LineRangeList;	//Դ�ļ��к���Ϣ��
	INT_PTR												m_nLineNo;			//��ǰ�ܼ��ļ��к�
	TCHAR												m_NewLineChar;		//���з���Ĭ����'\n'
	bool												m_boErrorAbort;		//�Ƿ���������Ҫ��ֹ����
	bool												m_boCurrCondition;	//��ǰ��Ԥ��������ֵ
public:
	//ͨ�������ƻ�ȡ��ָ��
	const PPREPROC_MACRO getMacro(LPCTSTR sName);
};
