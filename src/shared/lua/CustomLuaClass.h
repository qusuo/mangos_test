//#pragma once
#ifndef __CUSTOMLUACLASS_H_
#define __CUSTOMLUACLASS_H_

/************************************************************************/
/* 
/*                C++类注册到脚本中的类型处理基类
/*
/* ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
/*   CustomLuaClass在设计思想上不够完善，随可以使用且功能比较全面，但仍有必须实例化一个
/* 类型才能处理导出的缺陷，这在实际的代码应用上较为不方便
/* ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
/* 
/*   类实现了对new、__gc、__get、__set、__eq、__add、__sub、__mul、__div、
/* __mod、__unm、__len、__lt、__le、__concat这些元方法的处理模型。子类通过覆盖相关
/* 的元方法处理函数，可以实现自己的元方法处理。
/*
/*   最重要的一个方面是本实现模式支持向脚本传递一个宿主程序中的类型实例，而不用创建新的实
/* 例或者被脚本gc回收，能够更好的实现以宿主为主进行内存管理、脚本为辅进行功能扩展的架构方
/* 式，而这点是tolua++暂时没有实现的，另外的特点是对各种元方法的支持更为全面。
/*
/*  该实现方式中，类型的元表中含有如下重要内容：
/*
/*    _typename : string 类型的名称字符串
/*    _typeid : number 类型的ID值，ID值是基于类型名称字符串通过哈希算法生成的
/*    _classInst : lightuserdata 类型元方法处理函数
/*
/************************************************************************/


#define CLC_CS_TYPENAME		"_typename"	//定义metatable中的类型名称键
#define CLC_CS_TYPEID		"_typeid"	//定义metatable中的类型ID键
#define CLC_CS_CLASSINST	"_classInst"//定义metatable中的类型实例键

template <class T, class CHILD>
class CCustomLuaClass
{
public:
	typedef T DataType;
	typedef CCustomLuaClass<T, CHILD> ClassType;

public:
	/*
	* Comments: 向LUA虚拟机中注册此类型
	* Param lua_State * L:
	* @Return void:
	*/
	virtual void regist(lua_State *L)
	{
		registClassName(L, ClassTypeName);
		registClassName(L, ClassRefTypeName);
	}
	/*
	* Comments: 向LUA返回一个类型的实例引用，由于这是一个用户管理的对象，因此该实例不会被gc删除内存
	* Param lua_State * L:
	* Param T & ref:
	* @Return int:
	*/
	virtual int newReference(lua_State *L, T &ref)
	{
		T **ppRef = (T**)lua_newuserdata(L, sizeof(*ppRef));
		//设置类型表
		setType(L, ClassRefTypeName);
		*ppRef = &ref;
		return 1;
	}

protected:
	typedef int (CHILD::*ExportCallFn)(lua_State *L, DataType *pInst);
	/* 函数调用数据结构名称 */
	struct FuncCall
	{
		const char *name;
		ExportCallFn call;
	};

private:
	/** 内部函数调用结构 **/
	struct RawCallFunc
	{
		unsigned int name;//hash后的函数名称
		union
		{
			ExportCallFn call;//调用函数
			void *pfn;
		};
	};

protected:
	/*
	* Comments: 创建一个由脚本T:new调用所需要产生的类型
	* Param lua_State * L:
	* @Return T*:
	*/
	virtual T* create(lua_State *L) 
	{
		//默认的实现是通过lua_newuserdata申请对象内存，这个实例的内存由脚本管理会被gc
		return (T*)lua_newuserdata(L, sizeof(T));
	}
	/*
	* Comments: 销毁一个由脚本T:new调用所产生的类型
	* Param lua_State * L:
	* Param T * ptr:
	* @Return void:
	*/
	virtual void destroy(lua_State *L, T *ptr)
	{
		//如果对象是通过lua_newuserdata申请对象内存，这个实例的内存由脚本管理会被gc，用户不用关心此内存
	}
	/*
	* Comments: 对__get元方法的实现
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
					//返回函数
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
	* Comments: 对__set元方法的实现
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int set(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: 对__eq元方法的实现
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int eq(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: 对__add元方法的实现
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int add(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: 对__sub元方法的实现
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int sub(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: 对__mul元方法的实现
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int mul(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: 对__div元方法的实现
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int div(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: 对__mod元方法的实现
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int mod(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: 对__unm元方法的实现
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int unm(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: 对__len元方法的实现
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int len(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: 对__lt元方法的实现
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int lt(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: 对__le元方法的实现
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int le(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: 对__concat元方法的实现
	* Param lua_State * L:
	* @Return int:
	*/
	virtual int concat(lua_State *L, T* pInst)
	{
		return 0;
	}
	/*
	* Comments: 将此类型通过特定名称注册到虚拟机中
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
	* Comments: 获取对象实例
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
		把给定索引指向的值的元表压入堆栈。如果索引无效，或是这个值没有元表，函数将返回 0 并且不会向栈上压任何东西。
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
			//弹出_typeid以及metatable
			lua_pop(L, 2);
		}
		return result;
	}

	/*
	* Comments: 向虚拟机导出函数调用表
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
	//字符串hash函数
	inline static unsigned int hashstr(const char* str)
	{
		return hashlstr(str, strlen(str));
	}
	//字符串hash函数，需要提供长度
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
	* Comments: 按元方法名称设置元表的元方法实现函数
	* Param lua_State * L:
	* Param const char * metaName: 元方法名称
	* Param const void * fn: 实现函数指针
	* Param int tidx: 元表索引
	* @Return void:
	*/
	inline static void setMetaNameClosure(lua_State *L, const char* metaName, const void* fn, int tidx = -1)
	{  
		lua_pushstring(L, metaName);
		lua_pushcfunction(L, (lua_CFunction)fn);
		lua_rawset(L, (tidx < 0) ? tidx - 2 : tidx);
	}

	/*
	* Comments: 按元方法类型设置元表的元方法实现函数
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
	* Comments: 获取元方法类型名称
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
	* Comments: 获取栈中userdata对象关联的处理类实例
	* Param lua_State * L:
	* Param int vidx: userdata在栈中的索引
	* @Return ClassType*:
	*/
	inline static ClassType* getClassTypeInstance(lua_State*L, int vidx = 1)
	{
		if (!lua_isuserdata(L, vidx))
			return NULL;
		//如果没有元表，则函数返回0且不向栈中存储任何数据
		if (!lua_getmetatable(L, vidx))
			return NULL;
		ClassType *result = NULL;
		//获取类型实例
		lua_pushstring(L, CLC_CS_CLASSINST);
		lua_rawget(L, -2);
		result = (ClassType *)lua_touserdata(L, -1);
		//消炎类型ID
		//弹出_classInst以及metatable
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
	* Comments: 为向脚本返回的类型关联元表类型
	* Param lua_State * L:
	* Param const char * classeName: 元表类型名称
	* Param int tidx: 实例在栈中的索引
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

		//获取类型实例
		lua_pushstring(L, CLC_CS_CLASSINST);
		lua_rawget(L, 1);
		pthis = (ClassType *)lua_touserdata(L, -1);
		//消炎类型ID
		//弹出_classInst
		lua_pop(L, 1);
		
		if (pthis)
		{
			T *pData = pthis->create(L);
			//设置类型表
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
		把给定索引指向的值的元表压入堆栈。如果索引无效，或是这个值没有元表，函数将返回 0 并且不会向栈上压任何东西。
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
			//弹出_typeid以及metatable
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
		//在get函数中为cclosure设置的第一个参数是this指针
		CHILD *pthis = (CHILD*)lua_touserdata(L, lua_upvalueindex(1));
		//在get函数中为cclosure设置的第而个参数是函数指针
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
	char			ClassTypeName[64];		//类型名称
	char			ClassRefTypeName[64];	//引用类型名称
	unsigned int	ClassTypeId;			//类型ID
	unsigned int	ClassRefTypeId;			//引用类型ID

private:
	RawCallFunc		*CallList;
	int				NumCall;
};

#endif //__CUSTOMLUACLASS_H_