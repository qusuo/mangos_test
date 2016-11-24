#ifndef __CUSTOMLUASCRIPT_H_
#define __CUSTOMLUASCRIPT_H_


/*****************************************************************/
/*
/*                     LUA�ű����������
/*
/*	�ṩ�Խű��ļ��ء�������ע�ᡢ��������Լ��������õĹ����塣
/*	���ڴ�������Ĳ�������Ӧ������showError�����Ա������δ���
/*	�ű�������в����Ĵ���
/*  
/*****************************************************************/
// #include "BufferAllocator.h"
#include <string>
#include "ByteBuffer.h"

struct lua_State;

class CCustomLuaScript
{
protected:
	lua_State		*m_pLua;			//LUA���������
	int				m_nLastError;		//��һ�������
	std::string		m_sLastErrDesc;		//��һ�����������
	char			m_sLastFnName[128];	//��һ�ε��õĺ�������

protected:
	//�������������
	virtual lua_State* createLuaVM();
	//�򿪻����⡣Ĭ�ϻ��base��string��math�Լ�table�⡣����true��ʾ�ɹ���
	virtual bool openBaseLibs();
	//ע�᱾�غ����⡣����true��ʾ�ɹ���
	virtual bool registLocalLibs();
	//���ýű��ĳ�ʼ����������������true��ʾû�д�����������δ�ṩ���ó�ʼ��������ʵ�ʹ��ܡ�
	virtual bool callInit();
	//���ýű���ж�غ�������������true��ʾû�д�����������δ�ṩ����ж�غ�����ʵ�ʹ��ܡ�
	virtual bool callFinal();
	//��ʾ�ű���������
	//virtual void showError(const char * sError);
protected:
	//���õ�ǰ���ú������ƣ���¼���ú������Ƶ�Ŀ����Ϊ���������ϸ�Ĵ���
	inline void setFnName(const char * sName){ if(sName) strcpy(m_sLastFnName, sName); }
	//��ʽ���������ݲ���ʾ����ʽ������������ַ����ĳ��ȱ�����Ϊ1024���ַ���
	//void showErrorFormat(const char * sFmt, ...); 
	/*	�ű����ú���
		�ڽ������Լ�����������ջ����ôӺ����������ֱ�ӵ���lua_pcall����Ϊ�Ա㼯�д�����
		�����а����Ե��ú���ǰ��Ķ�ջƽ���顣
		��������true��ʾû�д���
	*/
	bool pcall(const int args, const int results, const int nErrFunc);
	//���ű����÷���ֵ�����nError��Ϊ�ɹ�ֵ�������������ݲ�����false����nError��ֵ��������m_nLastError�С�
	inline bool lcCheck(int nError);
	
	/*
	* Comments: ����ű�Ϊ�ֽ���
	* Param lua_State * L:
	* Param const char * szContent: �ı���ʽ��lua�ű�����
	* Param CDataPacket & packet: �����������
	* @Return bool:�ɹ�����true��ʧ�ܷ���false
	* @Remark:
	*/
	bool CompileLua(lua_State* L, const char* szContent, CDataPacket &packet);

	/*
	* Comments: �����������Writer
	* Param lua_State * L: Lua״̬��
	* Param const void * p: ��д��Ķ���������
	* Param size_t size: ���������ݳ���
	* Param void * u: userData�� ������CDataPacket*
	* @Return int: �ɹ�����0��ʧ�ܷ�0
	* @Remark:
	*/
	static int StreamWriter(lua_State* L, const void* p, size_t size, void* u);

public:
	CCustomLuaScript();
	virtual ~CCustomLuaScript();

	/* ���ýű����ݣ���������º��Ĳ�����
	   1�����õ�ǰ�ű���ж�غ���
	   2��ɾ����ǰ�����������
	   3�����´��������
	   4���򿪻���������
	   5��ע�᱾�غ�����
	   6�����ó�ʼ������
	   �������sTextΪNULL��Ϊ���ַ���������ٵ�ǰ������Ҳ��������������
	   ��������true��ʾ���ýű��ɹ�������ֵ��ʾ��������
	*/
	bool setScript(const char * sText);

	/*
	* Comments: ���ö����ƽű����ݣ���ɴ󲿷ֲ���ͬsetScript(const char * sText)��ָʾ�ű������Ǳ�����Ķ������ֽ���
	* Param const char * szScript: �ű����ı���ʽ��
	* Param CDataPacket& packet: ���bCompileΪtrue�����Ǳ����Ķ�����������������Ƕ�����������
	* Param const char * name: ��������ƣ�һ��ָ�ű��ļ�����
	* Param bool bCompile: Ϊtrue���ȱ���ű�Ϊ�����ƣ�Ȼ����ض����ƣ�����ֱ�Ӽ��ض���������
	* @Return bool: �ɹ�����true��ʧ�ܷ���false
	* @Remark:
	*/
	bool setBinScript(const char * szScript, CDataPacket& packet, const char * name = NULL, bool bCompile = false);
	
	/*
	* Comments: ���ö����ƽű�
	* Param CDataPacket & packet: �ű�����������
	* @Return bool:
	* @Remark:
	*/
	bool resetBinScript(CDataPacket& packet);

	//��ȡ��������ڴ�ʹ��������λ���ֽ�
	int getAvaliableMemorySize();
	//�����������գ��ͷ��ڴ档�������ػ����˶����ֽڵ��ڴ�
	int gc();
	//��ȡ�����
	inline int getLastErrorNo(){ return m_nLastError; }
	//��ȡ��������
	inline const std::string& getLastErrorDesc() const { return m_sLastErrDesc; }
};

#endif //__CUSTOMLUASCRIPT_H_