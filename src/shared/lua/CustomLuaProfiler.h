#pragma once

/************************************************************************/
/*
/*                    LUA �ű�������������ͳ����
/*
/*    ����ͨ��lua_hook�ķ�ʽʵ���˼�¼��ͳ�ƽű��к�������ʱ�����ĵĹ��ܡ��Ժ�������ʱ��
/* ���ĵ�Ĭ�ϼ��㷽ʽΪ���㺯������ǰ����úõ�ʱ����������ܱȽϴ󣬵������ҳ�����
/* ���ĵ�ʱ��ϴ�ĺ�����ͬʱʵ���˽�ͳ����Ϣ����Ϊ�ı����Ĺ��ܣ�Ĭ�ϵ������ʽΪ����ͳ����
/* Ϣ��
/*
/*    ����ͨ���������ಢ����HandleDebugHook����������ʵ�ֶԺ���������Ϣ�ļ�¼�����ܵ�ͳ
/* �ƹ���
/*
/*    ����ͨ���������ಢ����Save����������ʵ�ֶ�ͳ����Ϣ����Ϊ�ı����������ʽ��
/*
/************************************************************************/

#include "ObjectCounter.h"
class CCustomLuaProfiler
{
public:
	typedef wylib::misc::CRefObject<CCustomLuaPreProcessor::CLineRangeList> CSourceLineList;

public:
	CCustomLuaProfiler();
	~CCustomLuaProfiler();

	/* 
	* Comments:��������ͳ�ƶ���
	* Param lua_State * L:
	* Param CSourceLineList * pLRList: ����ȡ��
	* @Return bool:���Ŀ������Ѿ�����װ������ͳ�ƣ���������false
	*/
	bool SetState(lua_State *L, CSourceLineList *pLRList = NULL);
	/*
	* ��������������ͳ�Ʊ��浽����
	* ��������������д������ֽ�
	*/
	size_t Save(wylib::stream::CBaseStream &stm);
	/*
	* Comments: ��ȡ����ʱ�����һ������ջ��ʱ��
	* @Return LONGLONG:
	*/
	LONGLONG GetMaxTotalTime();

protected:
	//��lua�д洢��ǰ����ͳ�Ƶ����������ֵ����
	static const char szPerStaticDebuggerName[];
	//��̬hook������
	static void __cdecl StaticCallTimeHook(lua_State *L, lua_Debug *ar);

protected:
	/** �������HOOK�ص� **/
	virtual void HandleDebugHook(lua_State *L, lua_Debug *ar);
public:
	/** Lua�����������ܼ�¼ **/
	class CLuaCallRcd : public Counter<CLuaCallRcd>
	{
	public:
		char	 sName[40];		//��������
		LONG	 nLineNo;		//�к�
		LONGLONG dwLastTime;	//�ϴε���ʱ��
		LONGLONG dwMaxTime;		//������ʱ��
		LONGLONG dwMinTime;		//��С����ʱ��
		LONGLONG dwCallCount;	//���ô���
		LONGLONG dwTotalTime;	//�������ܼƵ��ú�ʱ
		LPVOID	 pSubCallList;	//�Ӽ����ú���������ΪSubCallTable
	public:
		~CLuaCallRcd();
	};
	//�Ӽ����ú�����
	typedef CCustomHashTable<CLuaCallRcd> CSubCallTable;
	//��ǰ���ö�ջ��
	typedef wylib::container::CBaseList<CLuaCallRcd*> CCallStack;

protected:
	lua_State*		m_pLua;
	CCallStack		m_CallStack;		//��ǰ���ö�ջ
	CSubCallTable	m_RootCallTable;	//�����ü�¼��
	LONGLONG		m_dwIgoreRecordTime;//�ڱ��溯���������ܼ�¼ʱ������ʱ����ڴ�ֵ�ļ�¼
	CSourceLineList m_LRList;//�з�Χ��¼�������ڸ�ʽ������ͳ��ʱȡ�ô�����ļ������к�
};