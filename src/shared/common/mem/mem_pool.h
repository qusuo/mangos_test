#ifndef JXS_MEMPOOL_H_
#define JXS_MEMPOOL_H_
#include "../include/basic_type_def.h"
//author: hsj

namespace jxsmem
{

const Int32 MAX_MEM_OBJ_SIZE = 100000000; //100M
const Int32 DEFAULT_CHUNK_NUM = 30;
class MemChunk;
class MemPool
{
public:	
	~MemPool();
	static MemPool* Singelton();
	//if succeed will return the index error code:-1 size too small, -2 size too big 
	Int32 RegisterMemObject(const char* obj_name, Int32 obj_size, Int32 init_obj_num);

	char* AllocMem(Int32 trunk_index);
	void FreeMem(Int32 trunk_index, void* p);

private:
	MemPool();
	MemPool(const MemPool& rhs){}
	MemPool& operator = (const MemPool& rhs){return *this; }

private:
	typedef MemChunk* MemChunkPtr;
	MemChunkPtr m_chunks[DEFAULT_CHUNK_NUM]; 
	Int32 m_chunk_list_size;
};
}

#endif