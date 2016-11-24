#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <_ast.h>
#include <_memchk.h>
#include <Tick.h>
#include <List.h>
#include <RefClass.hpp>
#include <RefString.hpp>
#include <Stream.h>
#include "../include/ShareUtil.h"
#include "../include/CustomLuaPreProcess.h"

using namespace wylib::container;
using namespace wylib::string;
using namespace wylib::stream;


CCustomLuaPreProcessor::CCustomLuaPreProcessor()
	:Inherited()
{
}

CCustomLuaPreProcessor::~CCustomLuaPreProcessor()
{
	clearIncludedFileList();
	clearIncludeDirectory();
	clearMacroList();
}

inline bool fileExists(LPCTSTR sFilePath)
{
	DWORD dwAttr = GetFileAttributes(sFilePath);
	if ( dwAttr == INVALID_FILE_ATTRIBUTES )
		return false;
	if ( dwAttr & FILE_ATTRIBUTE_DIRECTORY )
		return false;
	return true;
}

LPCTSTR getFileNamePart(LPCTSTR sFullFilePath)
{
	LPCTSTR sRet = sFullFilePath;
	while (*sFullFilePath)
	{
		if (*sFullFilePath == '/' || *sFullFilePath == '\\')
			sRet = sFullFilePath + 1;
		sFullFilePath++;
	}
	return sRet;
}

LPCTSTR CCustomLuaPreProcessor::parse(LPCTSTR sSourceText, LPCTSTR sFilePath, const TCHAR cNewLine)
{
	setSize(0);
	m_ConditionList.clear();//清空条件表
	m_LineRangeList.clear();//清空源文件行号表
	m_nLineNo = 1;
	m_NewLineChar = cNewLine;
	m_boErrorAbort = false;
	m_boCurrCondition = true;
	clearIncludedFileList();
	return SaveFileDirAndParse(sSourceText, sFilePath);
}

LPCTSTR CCustomLuaPreProcessor::ParseSource(LPCTSTR sSourceText)
{
	LONGLONG dwPos = getPosition();
	LPTSTR sLine = NULL;
	SIZE_T dwMaxLineLen = 0, dwLineLen;

	__try
	{
		//跳过UTF-8 BOM
		if ( (*(PINT)sSourceText & 0x00FFFFFF) == 0xBFBBEF )
			sSourceText += 3;

		m_ParseEnvir.sParsePtr = sSourceText;
		m_ParseEnvir.nLineNo = 1;
		while (*m_ParseEnvir.sParsePtr && !m_boErrorAbort)
		{
			m_ParseEnvir.sNewLinePtr = _tcschr(m_ParseEnvir.sParsePtr, m_NewLineChar);
			//定位新行位置
			if ( m_ParseEnvir.sNewLinePtr )
			{
				m_ParseEnvir.sLineEndPtr = m_ParseEnvir.sNewLinePtr - 1;
				m_ParseEnvir.sNewLinePtr++;//调过新行字符
			}
			else
			{
				m_ParseEnvir.sNewLinePtr = m_ParseEnvir.sParsePtr + _tcslen(m_ParseEnvir.sParsePtr);
				m_ParseEnvir.sLineEndPtr = m_ParseEnvir.sNewLinePtr - 1;
			}
			//定位当前行尾，调过\n\r以及制表符
			while (m_ParseEnvir.sLineEndPtr >= m_ParseEnvir.sParsePtr && (UINT_PTR)*m_ParseEnvir.sLineEndPtr < 0x20)
			{
				m_ParseEnvir.sLineEndPtr--;
			}
			if ( m_ParseEnvir.sLineEndPtr >= m_ParseEnvir.sParsePtr )
			{
				//终止行尾有效字符之后的后一个字符
				m_ParseEnvir.sLineEndPtr++;
				dwLineLen = m_ParseEnvir.sLineEndPtr - m_ParseEnvir.sParsePtr;

				//分析并处理文本行，如果前3个字符预处理指令字符（--#）则进行预处理，否则直接写入文本内容
#ifdef UNICODE
				if ( *(PINT)m_ParseEnvir.sParsePtr == MAKEFOURCC('-', 0, '-', 0)
					&& (*(((PINT)m_ParseEnvir.sParsePtr) + 1) & 0x0000FFFF) == MAKEFOURCC('#', 0, 0, 0))
#else
				if ( (*(PINT)m_ParseEnvir.sParsePtr & 0x00FFFFFF) == MAKEFOURCC('-', '-', '#', 0) )
#endif
				{
					//拷贝行内容
					if (dwLineLen > dwMaxLineLen)
					{
						//sLine = (LPTSTR)realloc(sLine, sizeof(*sLine) * (dwLineLen + 1));
						char * new_data = (LPTSTR)realloc(sLine, sizeof(*sLine) * (dwLineLen + 1));
						if (new_data != NULL)
							sLine = new_data;
						else
							return "";

						dwMaxLineLen = dwLineLen;
					}
					memcpy(sLine, m_ParseEnvir.sParsePtr, dwLineLen * sizeof(*sLine));
					sLine[dwLineLen] = 0;
					//保存当前行索引
					INT_PTR nLineNoBefore = m_nLineNo;
					//执行行预处理语句
					ProcessLine(sLine);
					//在预处理指令没有写入新内容的情况下，必须写入一个空行占据改预处理行，才能使得预处理后的行号能够与源文件匹配
					if (nLineNoBefore == m_nLineNo)
					{
						//在代码中写入换行符
						write(&m_NewLineChar, sizeof(TCHAR));
						//调整行号
						m_nLineNo++;
					}
				}
				else if ( m_boCurrCondition )//只能在预处理条件为true的情况下写入文本
				{
					write(m_ParseEnvir.sParsePtr, dwLineLen * sizeof(*m_ParseEnvir.sParsePtr));
					//在代码中写入换行符
					write(&m_NewLineChar, sizeof(TCHAR));
					//调整行号
					m_nLineNo++;
				}
			}
			else
			{
				//在代码中写入换行符
				write(&m_NewLineChar, sizeof(TCHAR));
				//调整行号
				m_nLineNo++;
			}
			//将输入处理指针调整到下一行
			m_ParseEnvir.sParsePtr = m_ParseEnvir.sNewLinePtr;
			m_ParseEnvir.nLineNo++;//调整原始文件行号
		}
	}
	__finally
	{
		free(sLine);
	}

	if ( m_boErrorAbort )
		return NULL;
	return (getSize() > 0) ? (LPCTSTR)getMemory() + dwPos : _T("");
}

VOID CCustomLuaPreProcessor::clearIncludeDirectory()
{
	INT_PTR i;
	for (i=m_IncludeDirList.count()-1; i>-1; --i)
	{
		delete m_IncludeDirList[i];
	}
	m_IncludeDirList.clear();
}

VOID CCustomLuaPreProcessor::addMacro(LPCTSTR sName, LPCTSTR sValue /* = NULL */)
{
	//如果宏存在则替换，否则则添加新的宏对象
	PPREPROC_MACRO pMacro = getMacro(sName);
	if ( pMacro )
	{
		showErrorFormat(_T("Macro %s Redefine and replaced"), sName);
		pMacro->sValue[0] = 0;
		if ( sValue )
		{
			_asncpyt(pMacro->sValue, sValue);
		}
	}
	else
	{
		PREPROC_MACRO macro;
		_asncpyt(macro.sName, sName);
		macro.sValue[0] = 0;
		if ( sValue )
		{
			_asncpyt(macro.sValue, sValue);
		}
		m_MacroList.add(macro);
	}
}

VOID CCustomLuaPreProcessor::removeMacro(LPCTSTR sName)
{
	for (INT_PTR i=m_MacroList.count()-1; i>-1; --i)
	{
		if ( _tcscmp(m_MacroList[i].sName, sName) == 0 )
		{
			m_MacroList.remove(i);
		}
	}
}

VOID CCustomLuaPreProcessor::clearMacroList()
{
	m_MacroList.clear();
}

INT_PTR CCustomLuaPreProcessor::getLineRangeData(LineRange *pLineRange, INT_PTR nCount)
{
	if (!pLineRange)
		return m_LineRangeList.count();
	if (nCount >= m_LineRangeList.count())
		nCount = m_LineRangeList.count();
	memcpy(pLineRange, &m_LineRangeList[0], nCount * sizeof(m_LineRangeList[0]));
	return nCount;
}

LPCTSTR CCustomLuaPreProcessor::getSourceLineNumber(INT_PTR nLineNo, INT_PTR &nSrcLineNo)
{
	return m_LineRangeList.getSourceLineNumber(nLineNo, nSrcLineNo);
}

const CCustomLuaPreProcessor::PPREPROC_MACRO CCustomLuaPreProcessor::getMacro(LPCTSTR sName)
{
	for (INT_PTR i=m_MacroList.count()-1; i>-1; --i)
	{
		if ( _tcscmp(m_MacroList[i].sName, sName) == 0 )
		{
			return &m_MacroList[i];
		}
	}
	return NULL;
}

VOID CCustomLuaPreProcessor::ProcessLine(LPCTSTR sLineText)
{
	INT_PTR nIdx = 0;
	TCHAR sDirective[128];

	//取得预处理指令以及指令内容
	sLineText += 3;//调过--#
	while (nIdx < ArrayCount(sDirective)-1)
	{
		if ( *sLineText == 0 || (UINT_PTR)*sLineText <= 0x20 )
		{
			break;
		}
		sDirective[nIdx] = *sLineText;
		sLineText++;
		nIdx++;
	}
	if ( nIdx > 0 )
	{
		sDirective[nIdx] = 0;
		//调过预处理指令数据部分开头的不可见字符
		while (*sLineText && (UINT_PTR)*sLineText <= 0x20)
			sLineText++;
		//执行预处理指令
		ProcessDirective(sDirective, sLineText);
	}
}

VOID CCustomLuaPreProcessor::ProcessDirective(LPCTSTR sDirective, LPCTSTR sData)
{
	//代码文件包含指令--#include
	if ( _tcsnicmp(sDirective, _T("include"), 7) == 0 )
	{
		if (m_boCurrCondition == true) 
			DirectiveOfInclude(sData);
	}
	//宏定义指令--#define
	else if ( _tcsnicmp(sDirective, _T("define"), 6) == 0 )
	{
		if (m_boCurrCondition == true) 
			DirectiveOfDefine(sData);
	}
	//取消宏定义指令--#undef
	else if ( _tcsnicmp(sDirective, _T("undef"), 5) == 0 )
	{
		if (m_boCurrCondition == true) 
			DirectiveOfUndef(sData);
	}
	//条件指令--#ifdef
	else if ( _tcsnicmp(sDirective, _T("ifdef"), 5) == 0 )
	{
		DirectiveOfIfdef(sData);
	}
	//条件指令--#ifndef
	else if ( _tcsnicmp(sDirective, _T("ifndef"), 6) == 0 )
	{
		DirectiveOfIfndef(sData);
	}
	//条件指令--#if
	else if ( _tcsnicmp(sDirective, _T("if"), 2) == 0 )
	{
		DirectiveOfIf(sData);
	}
	//条件指令--#else
	else if ( _tcsnicmp(sDirective, _T("else"), 4) == 0 )
	{
		DirectiveOfElse(sData);
	}
	//条件指令--#elif
	else if ( _tcsnicmp(sDirective, _T("elif"), 4) == 0 )
	{
		DirectiveOfElif(sData);
	}
	//条件指令--#endif
	else if ( _tcsnicmp(sDirective, _T("endif"), 5) == 0 )
	{
		DirectiveOfEndif(sData);
	}
	else
	{
		showErrorFormat(_T("invalid directive %s"), sData);
		abort();
	}
}

VOID CCustomLuaPreProcessor::DirectiveOfInclude(LPCTSTR sData)
{
	TCHAR sFile[1024];
	size_t dwSize;

	if ( !m_boCurrCondition )
		return;

	//取出包含文件中引号之内的文件名称
	if (*sData != '"')
		return;

	sData++;
	if ( *sData == '"' )
		return;

	LPCTSTR sPtr = _tcschr(sData, '"');
	if ( !sPtr )
		return;

	//将文件名拷贝到sFile缓冲区中
	dwSize = sPtr - sData;
	if ( dwSize > ArrayCount(sFile)-1 )
		dwSize = ArrayCount(sFile)-1;
	_tcsncpy(sFile, sData, dwSize);
	sFile[dwSize] = 0;
	//定位到包含限定词的位置
	while (*sPtr)
	{
		if (*sPtr == 0x20 || *sPtr == 0x09)
		{
			do 
			{
				sPtr++;
			}
			while (*sPtr == 0x20 || *sPtr == 0x09);
			break;
		}
		sPtr++;
	}
	SearchAndLoadIncludeFile(sFile, _tcsnicmp(sPtr, _T("once"), 4) != 0);
}

VOID CCustomLuaPreProcessor::DirectiveOfDefine(LPCTSTR sData)
{
	if ( !m_boCurrCondition )
		return;

	PPToken sTokens[2];
	INT_PTR nCount = ArrayCount(sTokens);

	delimiteLine(sData, sTokens, nCount);
	if ( nCount >= 2 )
		addMacro(sTokens[0], sTokens[1]);
	else if ( nCount >= 1 )
		addMacro(sTokens[0]);
	else
	{
		showError(_T("missing macro name"));
		abort();
	}
}

VOID CCustomLuaPreProcessor::DirectiveOfUndef(LPCTSTR sData)
{
	if ( !m_boCurrCondition )
		return;

	PPToken sTokens[1];
	INT_PTR nCount = ArrayCount(sTokens);

	delimiteLine(sData, sTokens, nCount);
	if ( nCount >= 1 )
		removeMacro(sTokens[0]);
	else 
	{
		showError(_T("missing macro name on #undef"));
		abort();
	}
}

VOID CCustomLuaPreProcessor::DirectiveOfIfdef(LPCTSTR sData)
{
	PPToken sTokens[1];
	INT_PTR nCount = ArrayCount(sTokens);

	delimiteLine(sData, sTokens, nCount);
	if ( nCount >= 1 )
	{
		m_ConditionList.add(getMacro(sTokens[0]) != NULL);
		refreshCondition();
	}
	else
	{
		showError(_T("missing macro name on #ifdef"));
		abort();
	}
}

VOID CCustomLuaPreProcessor::DirectiveOfIfndef(LPCTSTR sData)
{
	PPToken sTokens[1];
	INT_PTR nCount = ArrayCount(sTokens);

	delimiteLine(sData, sTokens, nCount);
	if ( nCount >= 1 )
	{
		m_ConditionList.add(getMacro(sTokens[0]) == NULL);
		refreshCondition();
	}
	else
	{
		showError(_T("missing macro name on #ifndef"));
		abort();
	}
}

VOID CCustomLuaPreProcessor::DirectiveOfIf(LPCTSTR sData)
{
	PPToken sTokens[3];
	INT_PTR nCount = ArrayCount(sTokens);

	delimiteLine(sData, sTokens, nCount);
	if ( nCount >= 3 )
	{
		PPREPROC_MACRO pMacro = getMacro(sTokens[0]);
		if ( pMacro )
		{
			if ( _tcscmp(_T("=="), sTokens[1]) == 0 )
			{
				m_ConditionList.add(_tcscmp(pMacro->sValue, sTokens[2]) == 0);
				refreshCondition();
			}
			else if ( _tcscmp(_T("!="), sTokens[1]) == 0 )
			{
				m_ConditionList.add(_tcscmp(pMacro->sValue, sTokens[2]) != 0);
				refreshCondition();
			}
			else
			{
				showErrorFormat(_T("invalid macro compare operator %s, default is false"), sTokens[1]);
				abort();
			}
		}
		else
		{
			showErrorFormat(_T("macro %s was not defined, default comparison is false"), sTokens[0]);
			m_ConditionList.add(false);
			refreshCondition();
		}
	}
	else
	{
		showError(_T("missing operator/operand on #if"));
		abort();
	}
}

VOID CCustomLuaPreProcessor::DirectiveOfElse(LPCTSTR sData)
{
	INT_PTR nCount = m_ConditionList.count();
	if ( nCount > 0 )
	{
		m_ConditionList[nCount-1] = !m_ConditionList.get(nCount-1);
		refreshCondition();
	}
	else
	{
		showError(_T("illegal #else missing #if/#ifdef/#ifndef"));
		abort();
	}
}

VOID CCustomLuaPreProcessor::DirectiveOfElif(LPCTSTR sData)
{
	INT_PTR nCondCount = m_ConditionList.count();
	if ( nCondCount <= 0 )
	{
		showError(_T("illegal #elif missing #if/#ifdef/#ifndef"));
		abort();
		return;
	}
	
	//如果当前条件为true则改变条件为false
	if ( m_ConditionList[nCondCount-1] )
	{
		m_ConditionList[nCondCount-1] = false;
		refreshCondition();
		return;
	}

	PPToken sTokens[3];
	INT_PTR nCount = ArrayCount(sTokens);

	delimiteLine(sData, sTokens, nCount);
	if ( nCount >= 3 )
	{

		PPREPROC_MACRO pMacro = getMacro(sTokens[0]);
		if ( pMacro )
		{
			if ( _tcscmp(_T("=="), sTokens[1]) == 0 )
			{
				if (_tcscmp(pMacro->sValue, sTokens[2]) == 0)
					m_ConditionList[nCondCount-1] = true;
				refreshCondition();
			}
			else if ( _tcscmp(_T("!="), sTokens[1]) == 0 )
			{
				if (_tcscmp(pMacro->sValue, sTokens[2]) != 0)
					m_ConditionList[nCondCount-1] = true;
				refreshCondition();
			}
			else
			{
				showErrorFormat(_T("invalid macro compare operator %s, default is still false"), sTokens[1]);
				abort();
			}
		}
		else
		{
			showErrorFormat(_T("macro %s was not defined, default comparison is false"), sTokens[0]);
		}
	}
	else
	{
		showError(_T("missing operator/operand on #elif"));
		abort();
	}
}

VOID CCustomLuaPreProcessor::DirectiveOfEndif(LPCTSTR sData)
{
	INT_PTR nCount = m_ConditionList.count();
	if ( nCount > 0 )
	{
		m_ConditionList.trunc(nCount-1);
		refreshCondition();
	}
	else
	{
		showError(_T("illegal #endif missing #if/#ifdef/#ifndef"));
		abort();
	}
}

LPCTSTR CCustomLuaPreProcessor::delimiteLine(LPCTSTR sLinePtr, IN OUT PPToken *sTokens, IN OUT INT_PTR &nTokenCount)
{
	INT_PTR nCount = 0;
	LPTSTR sPtr = (LPTSTR)sLinePtr;
	LPCTSTR sBegin;
	SIZE_T nLen;
	while (*sPtr && nCount < nTokenCount )
	{
		//跳过行首的不可见字符
		while(*sPtr && (unsigned)*sPtr <= 0x20 )
		{
			sPtr++;
		}
		sBegin = sPtr;
		//取得当前的词结束的位置
		while (*sPtr && (unsigned)*sPtr > 0x20 )
		{
			sPtr++;
		}
		//如果成功的取到了词，则保存改词到sTokens中
		if ( sPtr > sBegin )
		{
			nLen = sPtr - sBegin;
			if (nLen >= ArrayCount(sTokens[0]))
				nLen = ArrayCount(sTokens[0])-1;
			memcpy(sTokens[nCount], sBegin, nLen * sizeof(*sBegin));
			sTokens[nCount][nLen] = 0;

			nCount++;
			if ( *sPtr )
			{
				*sPtr = 0;//在该词结束的位置写入终止符
				sPtr++;
			}
		}
	}
	nTokenCount = nCount;
	return sPtr;
}

void CCustomLuaPreProcessor::refreshCondition()
{
	m_boCurrCondition = true;
	INT_PTR nCount = m_ConditionList.count();
	while(nCount > 0)
	{
		nCount--;
		if ( !m_ConditionList[nCount] )
		{
			m_boCurrCondition = false;
			break;
		}
	}
}

void CCustomLuaPreProcessor::clearIncludedFileList()
{
	for (INT_PTR i=m_IncludedFileList.count()-1; i>-1; --i)
	{
		free(m_IncludedFileList[i]);
	}
	m_IncludedFileList.trunc(0);
}

VOID CCustomLuaPreProcessor::showError(LPCTSTR sError)
{
	RefString sText;
	sText.format(_T("%s  in:\n  %s:%d"), sError, m_ParseEnvir.sFilePath, m_ParseEnvir.nLineNo);

	TCHAR sBuf[1024];
	SIZE_T nLen;
	for (INT_PTR i=m_ParseEnvirStack.count()-1; i>0; --i)//循环的终止条件是索引大于0，因为第0个环境是未初始化的自身数据
	{
		PREPROC_PARSERENVIR &ev = m_ParseEnvirStack[i];
		nLen = _sntprintf(sBuf, ArrayCount(sBuf) - 1, _T("\n  %s:%d"), ev.sFilePath, ev.nLineNo); 
		sBuf[nLen] = 0; 
		sText += sBuf; 
	}

	_tprintf(sText);
	throw sText;
}

VOID CCustomLuaPreProcessor::showErrorFormat(LPCTSTR sFmt, ...)
{
	TCHAR sBuf[1024];
	va_list	args;

	va_start(args, sFmt);
	_vsntprintf(sBuf, ArrayCount(sBuf) - 1, sFmt, args);
	va_end(args);

	showError(sBuf);
}

LPCTSTR CCustomLuaPreProcessor::SaveFileDirAndParse(LPCTSTR sSourceText, LPCTSTR sFullFilePath)
{
	//生成文件路径字符串并将路径压入预处理器包含文件分析栈
	String sDir = sFullFilePath;
	LPTSTR sDirPtr = (LPTSTR)sDir.rawStr();
	sDirPtr[getFileNamePart(sDirPtr)-sDirPtr] = 0;
	INT_PTR nPathIdx = m_FilePathStack.add(&sDir);


	//保存原始文件行信息
	INT_PTR nLineIdx = m_LineRangeList.count() + 1;
	m_LineRangeList.reserve((nLineIdx + 15) & ~(15));
	m_LineRangeList.trunc(nLineIdx);
	nLineIdx--;
	LineRange *pLn = &m_LineRangeList[nLineIdx];
	GetShortPathName(sFullFilePath, pLn->sFileName, ArrayCount(pLn->sFileName) - 1);
	pLn->nLineStart = m_nLineNo;

	//分析文件中的代码
	m_ParseEnvir.sFilePath = sFullFilePath;
	LPCTSTR sResult = ParseSource(sSourceText);

	//设置结束行索引
	/** 
		必须重新取得ln的指针，因为ParseSource是个递归调用，
	    如果后续再次调用SaveFileDirAndParse而导致m_LineRangeList的内存空间被重新申请，
		那么本次调用ParseSource前取得的ln将不再有效！
		这个BUG调了数个小时！！！
	**/
	pLn = &m_LineRangeList[nLineIdx];
	pLn->nLineEnd = m_nLineNo - 1;

	//从预处理器包含文件分析栈从弹出文件路径
	m_FilePathStack.trunc(nPathIdx);

	return sResult;
}

bool CCustomLuaPreProcessor::SearchAndLoadIncludeFile(LPCTSTR sIncludeFileName, bool boEnableMultipleInclude)
{
	INT_PTR i, nCount;
	String sPath;

	//开始搜索文件，优先从当前文件的目录开始搜索
	nCount = m_FilePathStack.count();
	if ( nCount > 0 )
	{
		nCount--;
		sPath = m_FilePathStack[nCount]->rawStr();
		sPath += sIncludeFileName;
		if ( fileExists(sPath.rawStr()) )
		{
			return LoadIncludeFile(sPath.rawStr(), boEnableMultipleInclude);
		}
	}
	//无法从当前文件的目录找到文件则从包含文件目录列表中降序循环搜索
	for (i=m_IncludeDirList.count()-1; i>-1; --i)
	{
		sPath = m_IncludeDirList[i]->rawStr();
		sPath += sIncludeFileName;
		if ( fileExists(sPath.rawStr()) )
		{
			return LoadIncludeFile(sPath.rawStr(), boEnableMultipleInclude);
		}
	}
	//最后将文件视为绝对路径中的文件
	if ( fileExists(sIncludeFileName) )
	{
		return LoadIncludeFile(sIncludeFileName, boEnableMultipleInclude);
	}
	showErrorFormat(_T("include file \"%s\" can not be found"), sIncludeFileName);
	return false;
}

bool CCustomLuaPreProcessor::LoadIncludeFile(LPCTSTR sIncludeFilePath, bool boEnableMultipleInclude)
{
	static const DWORD dwBufLen = 1024;

	//整理文件完整路径
	LPTSTR sFilePath = (LPTSTR)malloc(dwBufLen * sizeof(sFilePath[0]));
	DWORD dwLen = GetFullPathName(sIncludeFilePath, dwBufLen - 1, sFilePath, NULL);
	sFilePath[dwLen] = 0;

	//如果不允许多次包含，则判断文件是否已经被加载过
	if (!boEnableMultipleInclude)
	{
		INT_PTR i;
		for (i=m_IncludedFileList.count()-1; i>-1; --i)
		{
			//文件被加载过了，放弃再度包含
			if (_tcscmp(m_IncludedFileList[i], sFilePath) == 0)
			{
				free(sFilePath);
				return false;
			}
		}
	}
	//将文完整路径添加到已加载过的文件列表中
	m_IncludedFileList.add(sFilePath);

	//加载并分析文件
	CMemoryStream ms;
	if ( ms.loadFromFile(sFilePath) <= 0 )
		return false;

	SaveParseEnvir();
#ifdef UNICODE
	LPCTSTR sSourceText = (LPCTSTR)ms.getMemory();
	LPTSTR sConvertedSource = NULL;
	int nSrcCodePage = -1;
	//进行编码转换，转换为UNICODE
	if ( (*(PINT)sSourceText & 0x00FFFFFF) != 0x0061FEFF )
	{
		//UTF-8
		if ( (*(PINT)sSourceText & 0x00FFFFFF) == 0xBFBBEF )
		{
			sSourceText = (LPCTSTR)(((LPCSTR)sSourceText) + 3);
			nSrcCodePage = CP_UTF8;
		}
		//Local Ansic (not UCS)
		else if ((*sSourceText & 0xFF00) != 0)
		{
			nSrcCodePage = CP_ACP;
		}

		//从确定的代码页中转换编码
		if (nSrcCodePage != -1)
		{
			DWORD dwSrcLen = strlen((LPCSTR)sSourceText);
			sConvertedSource = (LPTSTR)malloc(sizeof(*sConvertedSource) * (dwSrcLen + 1));
			dwSrcLen = MultiByteToWideChar(nSrcCodePage, 0, (LPCSTR)sSourceText, dwSrcLen, sConvertedSource, dwSrcLen);
			sConvertedSource[dwSrcLen] = 0;
		}
		sSourceText = sConvertedSource;
	}
	SaveFileDirAndParse(sSourceText, sFilePath);
	free(sConvertedSource);
#else
	SaveFileDirAndParse((LPCTSTR)ms.getMemory(), sFilePath);
#endif

	RestorsParseEnvir();
	return true;
}

//----------------------CCustomLuaPreProcessor::CLineRangeList---------------------------

LPCTSTR CCustomLuaPreProcessor::CLineRangeList::getSourceLineNumber(INT_PTR nLineNo, INT_PTR &nSrcLineNo)
{
	INT_PTR nLineStart = 1, nLineCount = 0, nLineEnd = 0, nCount = count();
	LPCTSTR sSrcFile = NULL;

	for (INT_PTR i=0; i<nCount; ++i)
	{
		LineRange &ln = (*this)[i];

		if (ln.nLineStart > nLineNo)
			break;

		if (ln.nLineEnd >= nLineNo)
		{
			sSrcFile = ln.sFileName;
			nLineStart = ln.nLineStart;
			nLineCount = 0;
		}
		else if (nLineEnd < ln.nLineEnd)
		{
			nLineEnd = ln.nLineEnd;
			nLineCount += (nLineEnd - ln.nLineStart);
		}
	}

	if (sSrcFile)
	{
		nSrcLineNo = nLineNo - nLineStart - nLineCount + 1;//行号从1开始
		return sSrcFile;
	}
	else return NULL;
}
