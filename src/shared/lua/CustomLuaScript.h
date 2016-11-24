#ifndef __CUSTOMLUASCRIPT_H_
#define __CUSTOMLUASCRIPT_H_


/*****************************************************************/
/*
/*                     LUA脚本对象基础类
/*
/*	提供对脚本的加载、函数库注册、错误输出以及函数调用的规则定义。
/*	对于错误输出的捕获，子类应当覆盖showError函数以便决定如何处理
/*	脚本或调用中产生的错误。
/*  
/*****************************************************************/
// #include "BufferAllocator.h"
#include <string>
#include "ByteBuffer.h"

struct lua_State;

class CCustomLuaScript
{
protected:
	lua_State		*m_pLua;			//LUA虚拟机对象
	int				m_nLastError;		//上一个错误号
	std::string		m_sLastErrDesc;		//上一个错误的描述
	char			m_sLastFnName[128];	//上一次调用的函数名称

protected:
	//创建虚拟机对象
	virtual lua_State* createLuaVM();
	//打开基础库。默认会打开base、string、math以及table库。返回true表示成功。
	virtual bool openBaseLibs();
	//注册本地函数库。返回true表示成功。
	virtual bool registLocalLibs();
	//调用脚本的初始化函数。函数返回true表示没有错误发生。本类未提供调用初始化函数的实际功能。
	virtual bool callInit();
	//调用脚本的卸载函数。函数返回true表示没有错误发生。本类未提供调用卸载函数的实际功能。
	virtual bool callFinal();
	//显示脚本错误内容
	//virtual void showError(const char * sError);
protected:
	//设置当前调用函数名称，记录调用函数名称的目的是为了输出更详细的错误
	inline void setFnName(const char * sName){ if(sName) strcpy(m_sLastFnName, sName); }
	//格式化错误内容并显示，格式化后错误内容字符串的长度被限制为1024个字符。
	//void showErrorFormat(const char * sFmt, ...); 
	/*	脚本调用函数
		在讲函数以及参数名称入栈后调用从函数用于替代直接调用lua_pcall的行为以便集中错误处理。
		函数中包含对调用函数前后的堆栈平衡检查。
		函数返回true表示没有错误。
	*/
	bool pcall(const int args, const int results, const int nErrFunc);
	//检查脚本调用返回值，如果nError不为成功值则会输出错误内容并返回false，且nError的值被保存在m_nLastError中。
	inline bool lcCheck(int nError);
	
	/*
	* Comments: 编译脚本为字节码
	* Param lua_State * L:
	* Param const char * szContent: 文本形式的lua脚本内容
	* Param CDataPacket & packet: 二进制输出流
	* @Return bool:成功返回true；失败返回false
	* @Remark:
	*/
	bool CompileLua(lua_State* L, const char* szContent, CDataPacket &packet);

	/*
	* Comments: 二进制输出流Writer
	* Param lua_State * L: Lua状态机
	* Param const void * p: 待写入的二进制内容
	* Param size_t size: 二进制内容长度
	* Param void * u: userData。 这里是CDataPacket*
	* @Return int: 成功返回0；失败非0
	* @Remark:
	*/
	static int StreamWriter(lua_State* L, const void* p, size_t size, void* u);

public:
	CCustomLuaScript();
	virtual ~CCustomLuaScript();

	/* 设置脚本内容，会完成如下核心操作：
	   1、调用当前脚本的卸载函数
	   2、删除当前的虚拟机对象
	   3、重新创建虚拟机
	   4、打开基础函数库
	   5、注册本地函数库
	   6、调用初始化函数
	   如果参数sText为NULL或为空字符串则会销毁当前虚拟机且不创建新虚拟机。
	   函数返回true表示设置脚本成功，其他值表示发生错误。
	*/
	bool setScript(const char * sText);

	/*
	* Comments: 设置二进制脚本内容，完成大部分操作同setScript(const char * sText)，指示脚本内容是编译过的二进制字节码
	* Param const char * szScript: 脚本（文本形式）
	* Param CDataPacket& packet: 如果bCompile为true，它是编译后的二进制输出流；否则是二进制输入流
	* Param const char * name: 代码块名称（一般指脚本文件名）
	* Param bool bCompile: 为true会先编译脚本为二进制，然后加载二进制；否则直接加载二进制数据
	* @Return bool: 成功返回true；失败返回false
	* @Remark:
	*/
	bool setBinScript(const char * szScript, CDataPacket& packet, const char * name = NULL, bool bCompile = false);
	
	/*
	* Comments: 重置二进制脚本
	* Param CDataPacket & packet: 脚本二进制数据
	* @Return bool:
	* @Remark:
	*/
	bool resetBinScript(CDataPacket& packet);

	//获取虚拟机的内存使用量，单位是字节
	int getAvaliableMemorySize();
	//进行垃圾回收，释放内存。函数返回回收了多少字节的内存
	int gc();
	//获取错误号
	inline int getLastErrorNo(){ return m_nLastError; }
	//获取错误描述
	inline const std::string& getLastErrorDesc() const { return m_sLastErrDesc; }
};

#endif //__CUSTOMLUASCRIPT_H_