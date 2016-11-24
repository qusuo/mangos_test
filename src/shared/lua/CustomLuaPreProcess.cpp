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
	m_ConditionList.clear();//���������
	m_LineRangeList.clear();//���Դ�ļ��кű�
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
		//����UTF-8 BOM
		if ( (*(PINT)sSourceText & 0x00FFFFFF) == 0xBFBBEF )
			sSourceText += 3;

		m_ParseEnvir.sParsePtr = sSourceText;
		m_ParseEnvir.nLineNo = 1;
		while (*m_ParseEnvir.sParsePtr && !m_boErrorAbort)
		{
			m_ParseEnvir.sNewLinePtr = _tcschr(m_ParseEnvir.sParsePtr, m_NewLineChar);
			//��λ����λ��
			if ( m_ParseEnvir.sNewLinePtr )
			{
				m_ParseEnvir.sLineEndPtr = m_ParseEnvir.sNewLinePtr - 1;
				m_ParseEnvir.sNewLinePtr++;//���������ַ�
			}
			else
			{
				m_ParseEnvir.sNewLinePtr = m_ParseEnvir.sParsePtr + _tcslen(m_ParseEnvir.sParsePtr);
				m_ParseEnvir.sLineEndPtr = m_ParseEnvir.sNewLinePtr - 1;
			}
			//��λ��ǰ��β������\n\r�Լ��Ʊ��
			while (m_ParseEnvir.sLineEndPtr >= m_ParseEnvir.sParsePtr && (UINT_PTR)*m_ParseEnvir.sLineEndPtr < 0x20)
			{
				m_ParseEnvir.sLineEndPtr--;
			}
			if ( m_ParseEnvir.sLineEndPtr >= m_ParseEnvir.sParsePtr )
			{
				//��ֹ��β��Ч�ַ�֮��ĺ�һ���ַ�
				m_ParseEnvir.sLineEndPtr++;
				dwLineLen = m_ParseEnvir.sLineEndPtr - m_ParseEnvir.sParsePtr;

				//�����������ı��У����ǰ3���ַ�Ԥ����ָ���ַ���--#�������Ԥ��������ֱ��д���ı�����
#ifdef UNICODE
				if ( *(PINT)m_ParseEnvir.sParsePtr == MAKEFOURCC('-', 0, '-', 0)
					&& (*(((PINT)m_ParseEnvir.sParsePtr) + 1) & 0x0000FFFF) == MAKEFOURCC('#', 0, 0, 0))
#else
				if ( (*(PINT)m_ParseEnvir.sParsePtr & 0x00FFFFFF) == MAKEFOURCC('-', '-', '#', 0) )
#endif
				{
					//����������
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
					//���浱ǰ������
					INT_PTR nLineNoBefore = m_nLineNo;
					//ִ����Ԥ�������
					ProcessLine(sLine);
					//��Ԥ����ָ��û��д�������ݵ�����£�����д��һ������ռ�ݸ�Ԥ�����У�����ʹ��Ԥ�������к��ܹ���Դ�ļ�ƥ��
					if (nLineNoBefore == m_nLineNo)
					{
						//�ڴ�����д�뻻�з�
						write(&m_NewLineChar, sizeof(TCHAR));
						//�����к�
						m_nLineNo++;
					}
				}
				else if ( m_boCurrCondition )//ֻ����Ԥ��������Ϊtrue�������д���ı�
				{
					write(m_ParseEnvir.sParsePtr, dwLineLen * sizeof(*m_ParseEnvir.sParsePtr));
					//�ڴ�����д�뻻�з�
					write(&m_NewLineChar, sizeof(TCHAR));
					//�����к�
					m_nLineNo++;
				}
			}
			else
			{
				//�ڴ�����д�뻻�з�
				write(&m_NewLineChar, sizeof(TCHAR));
				//�����к�
				m_nLineNo++;
			}
			//�����봦��ָ���������һ��
			m_ParseEnvir.sParsePtr = m_ParseEnvir.sNewLinePtr;
			m_ParseEnvir.nLineNo++;//����ԭʼ�ļ��к�
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
	//�����������滻������������µĺ����
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

	//ȡ��Ԥ����ָ���Լ�ָ������
	sLineText += 3;//����--#
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
		//����Ԥ����ָ�����ݲ��ֿ�ͷ�Ĳ��ɼ��ַ�
		while (*sLineText && (UINT_PTR)*sLineText <= 0x20)
			sLineText++;
		//ִ��Ԥ����ָ��
		ProcessDirective(sDirective, sLineText);
	}
}

VOID CCustomLuaPreProcessor::ProcessDirective(LPCTSTR sDirective, LPCTSTR sData)
{
	//�����ļ�����ָ��--#include
	if ( _tcsnicmp(sDirective, _T("include"), 7) == 0 )
	{
		if (m_boCurrCondition == true) 
			DirectiveOfInclude(sData);
	}
	//�궨��ָ��--#define
	else if ( _tcsnicmp(sDirective, _T("define"), 6) == 0 )
	{
		if (m_boCurrCondition == true) 
			DirectiveOfDefine(sData);
	}
	//ȡ���궨��ָ��--#undef
	else if ( _tcsnicmp(sDirective, _T("undef"), 5) == 0 )
	{
		if (m_boCurrCondition == true) 
			DirectiveOfUndef(sData);
	}
	//����ָ��--#ifdef
	else if ( _tcsnicmp(sDirective, _T("ifdef"), 5) == 0 )
	{
		DirectiveOfIfdef(sData);
	}
	//����ָ��--#ifndef
	else if ( _tcsnicmp(sDirective, _T("ifndef"), 6) == 0 )
	{
		DirectiveOfIfndef(sData);
	}
	//����ָ��--#if
	else if ( _tcsnicmp(sDirective, _T("if"), 2) == 0 )
	{
		DirectiveOfIf(sData);
	}
	//����ָ��--#else
	else if ( _tcsnicmp(sDirective, _T("else"), 4) == 0 )
	{
		DirectiveOfElse(sData);
	}
	//����ָ��--#elif
	else if ( _tcsnicmp(sDirective, _T("elif"), 4) == 0 )
	{
		DirectiveOfElif(sData);
	}
	//����ָ��--#endif
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

	//ȡ�������ļ�������֮�ڵ��ļ�����
	if (*sData != '"')
		return;

	sData++;
	if ( *sData == '"' )
		return;

	LPCTSTR sPtr = _tcschr(sData, '"');
	if ( !sPtr )
		return;

	//���ļ���������sFile��������
	dwSize = sPtr - sData;
	if ( dwSize > ArrayCount(sFile)-1 )
		dwSize = ArrayCount(sFile)-1;
	_tcsncpy(sFile, sData, dwSize);
	sFile[dwSize] = 0;
	//��λ�������޶��ʵ�λ��
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
	
	//�����ǰ����Ϊtrue��ı�����Ϊfalse
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
		//�������׵Ĳ��ɼ��ַ�
		while(*sPtr && (unsigned)*sPtr <= 0x20 )
		{
			sPtr++;
		}
		sBegin = sPtr;
		//ȡ�õ�ǰ�Ĵʽ�����λ��
		while (*sPtr && (unsigned)*sPtr > 0x20 )
		{
			sPtr++;
		}
		//����ɹ���ȡ���˴ʣ��򱣴�Ĵʵ�sTokens��
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
				*sPtr = 0;//�ڸôʽ�����λ��д����ֹ��
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
	for (INT_PTR i=m_ParseEnvirStack.count()-1; i>0; --i)//ѭ������ֹ��������������0����Ϊ��0��������δ��ʼ������������
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
	//�����ļ�·���ַ�������·��ѹ��Ԥ�����������ļ�����ջ
	String sDir = sFullFilePath;
	LPTSTR sDirPtr = (LPTSTR)sDir.rawStr();
	sDirPtr[getFileNamePart(sDirPtr)-sDirPtr] = 0;
	INT_PTR nPathIdx = m_FilePathStack.add(&sDir);


	//����ԭʼ�ļ�����Ϣ
	INT_PTR nLineIdx = m_LineRangeList.count() + 1;
	m_LineRangeList.reserve((nLineIdx + 15) & ~(15));
	m_LineRangeList.trunc(nLineIdx);
	nLineIdx--;
	LineRange *pLn = &m_LineRangeList[nLineIdx];
	GetShortPathName(sFullFilePath, pLn->sFileName, ArrayCount(pLn->sFileName) - 1);
	pLn->nLineStart = m_nLineNo;

	//�����ļ��еĴ���
	m_ParseEnvir.sFilePath = sFullFilePath;
	LPCTSTR sResult = ParseSource(sSourceText);

	//���ý���������
	/** 
		��������ȡ��ln��ָ�룬��ΪParseSource�Ǹ��ݹ���ã�
	    ��������ٴε���SaveFileDirAndParse������m_LineRangeList���ڴ�ռ䱻�������룬
		��ô���ε���ParseSourceǰȡ�õ�ln��������Ч��
		���BUG��������Сʱ������
	**/
	pLn = &m_LineRangeList[nLineIdx];
	pLn->nLineEnd = m_nLineNo - 1;

	//��Ԥ�����������ļ�����ջ�ӵ����ļ�·��
	m_FilePathStack.trunc(nPathIdx);

	return sResult;
}

bool CCustomLuaPreProcessor::SearchAndLoadIncludeFile(LPCTSTR sIncludeFileName, bool boEnableMultipleInclude)
{
	INT_PTR i, nCount;
	String sPath;

	//��ʼ�����ļ������ȴӵ�ǰ�ļ���Ŀ¼��ʼ����
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
	//�޷��ӵ�ǰ�ļ���Ŀ¼�ҵ��ļ���Ӱ����ļ�Ŀ¼�б��н���ѭ������
	for (i=m_IncludeDirList.count()-1; i>-1; --i)
	{
		sPath = m_IncludeDirList[i]->rawStr();
		sPath += sIncludeFileName;
		if ( fileExists(sPath.rawStr()) )
		{
			return LoadIncludeFile(sPath.rawStr(), boEnableMultipleInclude);
		}
	}
	//����ļ���Ϊ����·���е��ļ�
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

	//�����ļ�����·��
	LPTSTR sFilePath = (LPTSTR)malloc(dwBufLen * sizeof(sFilePath[0]));
	DWORD dwLen = GetFullPathName(sIncludeFilePath, dwBufLen - 1, sFilePath, NULL);
	sFilePath[dwLen] = 0;

	//����������ΰ��������ж��ļ��Ƿ��Ѿ������ع�
	if (!boEnableMultipleInclude)
	{
		INT_PTR i;
		for (i=m_IncludedFileList.count()-1; i>-1; --i)
		{
			//�ļ������ع��ˣ������ٶȰ���
			if (_tcscmp(m_IncludedFileList[i], sFilePath) == 0)
			{
				free(sFilePath);
				return false;
			}
		}
	}
	//��������·����ӵ��Ѽ��ع����ļ��б���
	m_IncludedFileList.add(sFilePath);

	//���ز������ļ�
	CMemoryStream ms;
	if ( ms.loadFromFile(sFilePath) <= 0 )
		return false;

	SaveParseEnvir();
#ifdef UNICODE
	LPCTSTR sSourceText = (LPCTSTR)ms.getMemory();
	LPTSTR sConvertedSource = NULL;
	int nSrcCodePage = -1;
	//���б���ת����ת��ΪUNICODE
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

		//��ȷ���Ĵ���ҳ��ת������
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
		nSrcLineNo = nLineNo - nLineStart - nLineCount + 1;//�кŴ�1��ʼ
		return sSrcFile;
	}
	else return NULL;
}
