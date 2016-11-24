#ifndef JXS_MEMOBJREGISTER_H_
#define JXS_MEMOBJREGISTER_H_
#include "../include/basic_type_def.h"
#include "mem_pool.h"
#include "../util/obj_counter.h"
//author:hsj

namespace jxsmem
{
class MemObjRegister
{
public:
	MemObjRegister(const char* obj_name, Int32 obj_size, Int32 init_obj_num);
	~MemObjRegister();
	Int32 GetChunkIndex() const { return m_chunk_index; }
private:
	Int32 m_chunk_index;
};

#ifdef MEMPOOL_DEBUG
extern ObjCounter g_block_counter;
#define INCREASE_MEM_OBJ(obj_name)\
	jxsmem::g_block_counter.IncreaseObj(obj_name);
#define DECREASE_MEM_OBJ(obj_name)\
	jxsmem::g_block_counter.DecreaseObj(obj_name);
#else
#define INCREASE_MEM_OBJ(obj_name) 0;
#define DECREASE_MEM_OBJ(obj_name) 0;
#endif



}//end namespace jxsmem

#define REGISTER_MEMOBJ(obj, obj_num, obj_name)\
	jxsmem::MemObjRegister g_##obj(obj_name, sizeof(obj), obj_num);\
	\
	\
	void * obj::operator new(size_t size)\
	{\
		void * m = jxsmem::MemPool::Singelton()->AllocMem(g_##obj.GetChunkIndex());\
		INCREASE_MEM_OBJ(obj_name);\
		return m;\
	}\
	\
	\
	void obj::operator delete(void *p)\
	{\
		jxsmem::MemPool::Singelton()->FreeMem(g_##obj.GetChunkIndex(), p);\
		DECREASE_MEM_OBJ(obj_name);\
	}


#define EXPAN_MEMOBJ(obj, obj_num, obj_name) \
	MemPool::Singelton()->RegisterMemObject(obj_name, sizeof(obj), obj_num);

#endif