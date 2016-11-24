//#pragma once
#ifndef __CUSTOMLUACLASS_H_
#define __CUSTOMLUACLASS_H_

/************************************************************************/
/* 
/*                C++��ע�ᵽ�ű��е����ʹ������
/*
/* ����������������������������������������������������
/*   CustomLuaClass�����˼���ϲ������ƣ������ʹ���ҹ��ܱȽ�ȫ�棬�����б���ʵ����һ��
/* ���Ͳ��ܴ�������ȱ�ݣ�����ʵ�ʵĴ���Ӧ���Ͻ�Ϊ������
/* ����������������������������������������������������
/* 
/*   ��ʵ���˶�new��__gc��__get��__set��__eq��__add��__sub��__mul��__div��
/* __mod��__unm��__len��__lt��__le��__concat��ЩԪ�����Ĵ���ģ�͡�����ͨ���������
/* ��Ԫ����������������ʵ���Լ���Ԫ��������
/*
/*   ����Ҫ��һ�������Ǳ�ʵ��ģʽ֧����ű�����һ�����������е�����ʵ���������ô����µ�ʵ
/* �����߱��ű�gc���գ��ܹ����õ�ʵ��������Ϊ�������ڴ�����ű�Ϊ�����й�����չ�ļܹ���
/* ʽ���������tolua++��ʱû��ʵ�ֵģ�������ص��ǶԸ���Ԫ������֧�ָ�Ϊȫ�档
/*
/*  ��ʵ�ַ�ʽ�У����͵�Ԫ���к���������Ҫ���ݣ�
/*
/*    _typename : string ���͵������ַ���
/*    _typeid : number ���͵�IDֵ��IDֵ�ǻ������������ַ���ͨ����ϣ�㷨���ɵ�
/*    _classInst : lightuserdata ����Ԫ����������
/*
/************************************************************************/


#define CLC_CS_TYPENAME		"_typename"	//����metatable�е��������Ƽ�
#define CLC_CS_TYPEID		"_typeid"	//����metatable�е�����ID��
#define CLC_CS_CLASSINST	"_classInst"//����metatable�е�����ʵ����

template <class T, class CHILD>
class CCustomLuaClass
{
public:
	typedef T DataType;
	typedef CCustomLuaClass<T, CHILD> ClassType;

public:
	/*
	* Comments: ��LUA�������ע�������
	* Param lua_State * L:
	* @Return void:
	*/
	virtual void regist(lua_State *L)
	{
		registClassName(L, ClassTypeName);
		registClassName(L, ClassRefTypeName);
	}
	/*
	* Comments: ��LUA����һ�����͵�ʵ�����ã���������һ���û�����Ķ�����˸�ʵ�����ᱻgcɾ���ڴ�
	* Param lua_State * L:
	* Param T & ref:
	* @Return int:
	*/
	virtual int newReference(lua_State *L, T &ref)
	{
		T **ppRef = (T**)lua_newuserdata(L, sizeof(*ppRef));
		//�������ͱ�
		setType(L, ClassRefTypeName);
		*ppRef = &ref;
		return 1;
	}

protected:
	typedef int (CHILD::*ExportCallFn)(lua_State *L, DataType *pInst);
	/* �����������ݽṹ���� */
	struct FuncCall
	{
		const char *name;
		ExportCallFn call;
	};

private:
	/** �ڲ��������ýṹ **/
	struct RawCallFunc
	{
		unsigned int name;//hash��ĺ�������
		union
		{
			ExportCallFn call;//���ú���
			void *pfn;
		};
	};

protected:
	/*
	* Comments: ����һ���ɽű�T:new��������Ҫ����������
	* Param lua_State * L:
	* @Return T*:
	*/
	virtual T* create(lua_State *L) 
	{
		//Ĭ�ϵ�ʵ����ͨ��lua_newuserdata��������ڴ棬���ʵ�����ڴ��ɽű�����ᱻgc
		return (T*)lua_newuserdata(L, sizeof(T));
	}
	/*
	* Comments: ����һ���ɽű�T:new����������������
	* Param lua_State * L:
	* Param T * ptr:
	* @Return void:
	*/
	virtual void destroy(lua_State *L, T *ptr)
	{
		//���������ͨ��lua_newuserdata��������ڴ棬���ʵ�����ڴ��ɽű�����ᱻgc���û����ù��Ĵ��ڴ�
	}
	/*
	* Comments: ��__getԪ������ʵ��
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int get(lua_State *L, T* pInst)
	{
		if (NumCall > 0)
		{
			const char *sName = lua_tostring(L, 1);
			unsigned int nh = hashstr(sName);
			RawCallFunc *pCall = CallList;
			for (int i=NumCall-1; i>=0; --i)
			{
				if (pCall->name == nh)
				{
					//���غ���
					lua_pushlightuserdata(L, this);
					lua_pushlightuserdata(L, pCall->pfn);
					lua_pushvalue(L, 1);
					lua_pushcclosure(L, _callFn, 3);
					return 1;
				}
				pCall++;
			}
		}
		return 0;
	}
	/*
	* Comments: ��__setԪ������ʵ��
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int set(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: ��__eqԪ������ʵ��
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int eq(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: ��__addԪ������ʵ��
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int add(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: ��__subԪ������ʵ��
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int sub(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: ��__mulԪ������ʵ��
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int mul(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: ��__divԪ������ʵ��
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int div(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: ��__modԪ������ʵ��
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int mod(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: ��__unmԪ������ʵ��
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int unm(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: ��__lenԪ������ʵ��
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int len(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: ��__ltԪ������ʵ��
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int lt(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: ��__leԪ������ʵ��
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int le(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: ��__concatԪ������ʵ��
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int concat(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: ��������ͨ���ض�����ע�ᵽ�������
	* Param lua_State * L:
	* Param const char * className:
	* @Return void:
	*/
	virtual void registClassName(lua_State *L, const char* className)
	{
		lua_createtable(L, 0, 18 + NumCall);
		//_typename
		lua_pushstring(L, CLC_CS_TYPENAME);
		lua_pushstring(L, className);
		lua_rawset(L, -3);
		//_typeid
		lua_pushstring(L, CLC_CS_TYPEID);
		lua_pushnumber(L, hashstr(className));
		lua_rawset(L, -3);
		//_classInst
		lua_pushstring(L, CLC_CS_CLASSINST);
		lua_pushlightuserdata(L, this);
		lua_rawset(L, -3);
		//constructor
		setMetaNameClosure(L, "new", _new);
		//__gc
		setMetaClosure(L, TM_GC, _gc);
		//__index
		setMetaClosure(L, TM_INDEX, _get);
		//__newindex
		setMetaClosure(L, TM_NEWINDEX, _set);
		//__eq
		setMetaClosure(L, TM_EQ, _eq);
		//__add
		setMetaClosure(L, TM_ADD, _add);
		//__sub
		setMetaClosure(L, TM_SUB, _sub);
		//__mul
		setMetaClosure(L, TM_MUL, _mul);
		//__div
		setMetaClosure(L, TM_DIV, _div);
		//__mod
		setMetaClosure(L, TM_MOD, _mod);
		//__unm
		setMetaClosure(L, TM_UNM, _unm);
		//__len
		setMetaClosure(L, TM_LEN, _len);
		//__lt
		setMetaClosure(L, TM_LT, _lt);
		//__le
		setMetaClosure(L, TM_LE, _le);
		//__concat
		setMetaClosure(L, TM_CONCAT, _concat);

		lua_setglobal(L, className);
	}

	/*
	* Comments: ��ȡ����ʵ��
	* Param lua_State * L:
	* Param int index:
	* @Return CCLVariant*:
	*/
	virtual T* getDataPtr(lua_State *L, int index = 1)
	{
		if (lua_type(L, index) != LUA_TUSERDATA)
			return NULL;
		T *result = NULL;
		/*
		int lua_getmetatable (lua_State *L, int index);
		�Ѹ�������ָ���ֵ��Ԫ��ѹ���ջ�����������Ч���������ֵû��Ԫ������������ 0 ���Ҳ�����ջ��ѹ�κζ�����
		*/
		if (lua_getmetatable(L, index))
		{
			lua_pushstring(L, CLC_CS_TYPEID);
			lua_rawget(L, -2);
			if (lua_isnumber(L, -1))
			{
				unsigned int tpid = (unsigned int)lua_tonumber(L, -1);
				if (ClassRefTypeId == tpid)
					result = *(T**)lua_touserdata(L, (index > 0) ? index : index - 2);
				else if (ClassTypeId == tpid)
					result = (T*)lua_touserdata(L, (index > 0) ? index : index - 2);
			}
			//����_typeid�Լ�metatable
			lua_pop(L, 2);
		}
		return result;
	}

	/*
	* Comments: ������������������ñ�
	* Param const FuncCall * funcs:
	* Param const size_t count:
	* @Return void:
	*/
	inline void exportFunctions(const FuncCall *funcs, const size_t count)
	{
		RawCallFunc *pCall = CallList = (RawCallFunc *)realloc(CallList, sizeof(*CallList) * count);
		pCall += NumCall;
		for (size_t i=0; i<count; ++i)
		{
			pCall->name = hashstr(funcs[i].name);
			pCall->call = funcs[i].call;
			NumCall++;
			pCall++;
		}
	}
public:
	//�ַ���hash����
	inline static unsigned int hashstr(const char* str)
	{
		return hashlstr(str, strlen(str));
	}
	//�ַ���hash��������Ҫ�ṩ����
	inline static unsigned int hashlstr(const char* str, size_t len)
	{
		unsigned int h = (unsigned int)len;
		size_t step = (len>>5)+1;  /* if string is too long, don't hash all its chars */
		size_t l1;
		for (l1=len; l1>=step; l1-=step)  /* compute hash */
			h = h ^ ((h<<5)+(h>>2)+(unsigned char)str[l1-1]);
		return h;
	}

protected:

	/*
	* Comments: ��Ԫ������������Ԫ���Ԫ����ʵ�ֺ���
	* Param lua_State * L:
	* Param const char * metaName: Ԫ��������
	* Param const void * fn: ʵ�ֺ���ָ��
	* Param int tidx: Ԫ������
	* @Return void:
	*/
	inline static void setMetaNameClosure(lua_State *L, const char* metaName, const void* fn, int tidx = -1)
	{  
		lua_pushstring(L, metaName);
		lua_pushcfunction(L, (lua_CFunction)fn);
		lua_rawset(L, (tidx < 0) ? tidx - 2 : tidx);
	}

	/*
	* Comments: ��Ԫ������������Ԫ���Ԫ����ʵ�ֺ���
	* Param lua_State * L:
	* Param TMS tm:
	* Param const void * fn:
	* Param int tidx:
	* @Return void:
	*/
	inline static void setMetaClosure(lua_State *L, TMS tm, const void* fn, int tidx = -1)
	{  
		const char* sTMName = getTMName(tm);
		if (sTMName)
		{
			setMetaNameClosure(L, sTMName, fn, tidx);
		}
	}

	/*
	* Comments: ��ȡԪ������������
	* Param TMS tm:
	* @Return const char *:
	*/
	inline static const char * getTMName(TMS tm)
	{
		static const char *const luaT_eventname[] = {  /* ORDER TM */
			"__index", "__newindex",
			"__gc", "__mode", "__eq",
			"__add", "__sub", "__mul", "__div", "__mod",
			"__pow", "__unm", "__len", "__lt", "__le",
			"__concat", "__call"
		};
		if (tm < 0 || tm >= ArrayCount(luaT_eventname) )
			return NULL;
		return luaT_eventname[tm];
	}
	/*
	* Comments: ��ȡջ��userdata��������Ĵ�����ʵ��
	* Param lua_State * L:
	* Param int vidx: userdata��ջ�е�����
	* @Return ClassType*:
	*/
	inline static ClassType* getClassTypeInstance(lua_State*L, int vidx = 1)
	{
		if (!lua_isuserdata(L, vidx))
			return NULL;
		//���û��Ԫ����������0�Ҳ���ջ�д洢�κ�����
		if (!lua_getmetatable(L, vidx))
			return NULL;
		ClassType *result = NULL;
		//��ȡ����ʵ��
		lua_pushstring(L, CLC_CS_CLASSINST);
		lua_rawget(L, -2);
		result = (ClassType *)lua_touserdata(L, -1);
		//��������ID
		//����_classInst�Լ�metatable
		lua_pop(L, 2);
		return result;
	}

public:
	CCustomLuaClass(const char* sClassName)
	{
		NumCall = 0;
		CallList = NULL;

		size_t len = strlen(sClassName);
		if (len > sizeof(ClassTypeName))
			len = sizeof(ClassTypeName)-4;
		memcpy(ClassTypeName, sClassName, len);
		ClassTypeName[len] = 0;

		memcpy(ClassRefTypeName, sClassName, len);
		ClassRefTypeName[len+0] = 'R';
		ClassRefTypeName[len+1] = 'e';
		ClassRefTypeName[len+2] = 'f';
		ClassRefTypeName[len+3] = 0;

		ClassTypeId = hashstr(ClassTypeName);
		ClassRefTypeId = hashstr(ClassRefTypeName);
	}
	virtual ~CCustomLuaClass()
	{
		if (CallList)
		{
			free(CallList);
		}
	}
private:
	/*
	* Comments: Ϊ��ű����ص����͹���Ԫ������
	* Param lua_State * L:
	* Param const char * classeName: Ԫ����������
	* Param int tidx: ʵ����ջ�е�����
	* @Return void:
	*/
	inline static void setType(lua_State *L, const char *classeName, int tidx = -1)
	{
		lua_getglobal(L, classeName);
		if (lua_istable(L, -1))
			lua_setmetatable(L, (tidx > 0) ? tidx : tidx - 1);
		else lua_pop(L, 1);
	}

private:
	inline static int _new(lua_State *L)
	{
		ClassType* pthis;

		if (!lua_istable(L, 1))
			return 0;

		//��ȡ����ʵ��
		lua_pushstring(L, CLC_CS_CLASSINST);
		lua_rawget(L, 1);
		pthis = (ClassType *)lua_touserdata(L, -1);
		//��������ID
		//����_classInst
		lua_pop(L, 1);
		
		if (pthis)
		{
			T *pData = pthis->create(L);
			//�������ͱ�
			setType(L, pthis->ClassTypeName);
			return 1;
		}
		return 0;
	}
	inline static int _gc(lua_State *L)
	{
		ClassType* pthis = getClassTypeInstance(L);
		/*
		int lua_getmetatable (lua_State *L, int index);
		�Ѹ�������ָ���ֵ��Ԫ��ѹ���ջ�����������Ч���������ֵû��Ԫ������������ 0 ���Ҳ�����ջ��ѹ�κζ�����
		*/
		if (pthis && lua_getmetatable(L, 1))
		{
			lua_pushlstring(L, CLC_CS_TYPEID, 7);
			lua_rawget(L, -2);
			if (lua_isnumber(L, -1))
			{
				unsigned int tpid = (unsigned int)lua_tonumber(L, -1);
				if (pthis->ClassRefTypeId == tpid)
					*(T**)lua_touserdata(L, 1) = NULL;
				else if (pthis->ClassTypeId == tpid)
					pthis->destroy(L, (T*)lua_touserdata(L, 1));
			}
			//����_typeid�Լ�metatable
			lua_pop(L, 2);
		}
		return 0;
	}
	inline static int _get(lua_State *L)
	{
		ClassType* pthis = getClassTypeInstance(L);
		if (pthis)
		{
			T *pInst = pthis->getDataPtr(L);
			if (pInst)
			{
				lua_remove(L, 1);
				return pthis->get(L, pInst);
			}
		}
		return 0;
	}
	inline static int _set(lua_State *L)
	{
		ClassType* pthis = getClassTypeInstance(L);
		if (pthis)
		{
			T *pInst = pthis->getDataPtr(L);
			if (pInst)
			{
				lua_remove(L, 1);
				return pthis->set(L, pInst);
			}
		}
		return 0;
	}
	inline static int _eq(lua_State *L)
	{
		ClassType* pthis = getClassTypeInstance(L);
		if (pthis)
		{
			T *pInst = pthis->getDataPtr(L);
			if (pInst)
			{
				lua_remove(L, 1);
				return pthis->eq(L, pInst);
			}
		}
		return 0;
	}
	inline static int _add(lua_State *L)
	{
		ClassType* pthis = getClassTypeInstance(L);
		if (pthis)
		{
			T *pInst = pthis->getDataPtr(L);
			if (pInst)
			{
				lua_remove(L, 1);
				return pthis->add(L, pInst);
			}
		}
		return 0;
	}
	inline static int _sub(lua_State *L)
	{
		ClassType* pthis = getClassTypeInstance(L);
		if (pthis)
		{
			T *pInst = pthis->getDataPtr(L);
			if (pInst)
			{
				lua_remove(L, 1);
				return pthis->sub(L, pInst);
			}
		}
		return 0;
	}
	inline static int _mul(lua_State *L)
	{
		ClassType* pthis = getClassTypeInstance(L);
		if (pthis)
		{
			T *pInst = pthis->getDataPtr(L);
			if (pInst)
			{
				lua_remove(L, 1);
				return pthis->mul(L, pInst);
			}
		}
		return 0;
	}
	inline static int _div(lua_State *L)
	{
		ClassType* pthis = getClassTypeInstance(L);
		if (pthis)
		{
			T *pInst = pthis->getDataPtr(L);
			if (pInst)
			{
				lua_remove(L, 1);
				return pthis->div(L, pInst);
			}
		}
		return 0;
	}
	inline static int _mod(lua_State *L)
	{
		ClassType* pthis = getClassTypeInstance(L);
		if (pthis)
		{
			T *pInst = pthis->getDataPtr(L);
			if (pInst)
			{
				lua_remove(L, 1);
				return pthis->mod(L, pInst);
			}
		}
		return 0;
	}
	inline static int _unm(lua_State *L)
	{
		ClassType* pthis = getClassTypeInstance(L);
		if (pthis)
		{
			T *pInst = pthis->getDataPtr(L);
			if (pInst)
			{
				lua_remove(L, 1);
				return pthis->unm(L, pInst);
			}
		}
		return 0;
	}
	inline static int _len(lua_State *L)
	{
		ClassType* pthis = getClassTypeInstance(L);
		if (pthis)
		{
			T *pInst = pthis->getDataPtr(L);
			if (pInst)
			{
				lua_remove(L, 1);
				return pthis->len(L, pInst);
			}
		}
		return 0;
	}
	inline static int _lt(lua_State *L)
	{
		ClassType* pthis = getClassTypeInstance(L);
		if (pthis)
		{
			T *pInst = pthis->getDataPtr(L);
			if (pInst)
			{
				lua_remove(L, 1);
				return pthis->lt(L, pInst);
			}
		}
		return 0;
	}
	inline static int _le(lua_State *L)
	{
		ClassType* pthis = getClassTypeInstance(L);
		if (pthis)
		{
			T *pInst = pthis->getDataPtr(L);
			if (pInst)
			{
				lua_remove(L, 1);
				return pthis->le(L, pInst);
			}
		}
		return 0;
	}
	inline static int _concat(lua_State *L)
	{
		ClassType* pthis = getClassTypeInstance(L);
		if (pthis)
		{
			T *pInst = pthis->getDataPtr(L);
			if (pInst)
			{
				lua_remove(L, 1);
				return pthis->concat(L, pInst);
			}
		}
		return 0;
	}
	inline static int _callFn(lua_State *L)
	{
		//��get������Ϊcclosure���õĵ�һ��������thisָ��
		CHILD *pthis = (CHILD*)lua_touserdata(L, lua_upvalueindex(1));
		//��get������Ϊcclosure���õĵڶ��������Ǻ���ָ��
		union { ExportCallFn call; void* pfn; } c; 
		c.pfn = lua_touserdata(L, lua_upvalueindex(2));
		T *pInst = pthis->getDataPtr(L, 1);
		if (pInst)
		{
			lua_remove(L, 1);
			return (pthis->*(c.call))(L, pInst);
		}
		else
		{
			const char *sName = lua_tostring(L, lua_upvalueindex(3));
			lua_pushfstring(L, "%s:%s arg 1 is not %s, please use o:%s().", 
				pthis->ClassTypeName, sName, pthis->ClassTypeName, sName);
			lua_error(L);
		}
		return 0;
	}

protected:
	char			ClassTypeName[64];		//��������
	char			ClassRefTypeName[64];	//������������
	unsigned int	ClassTypeId;			//����ID
	unsigned int	ClassRefTypeId;			//��������ID

private:
	RawCallFunc		*CallList;
	int				NumCall;
};

#endif //__CUSTOMLUACLASS_H_