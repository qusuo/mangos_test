//#ifndef JXS_OBJPOOLREGISTER_H_
//#define JXS_OBJPOOLREGISTER_H_
//
//
//
////预分配内存，内存池对象，缓存对象，预分配个数，缓存对象名称
//#define REGISTER_MEMOBJPOOL(mempool, obj, obj_num, obj_name)\
//	mempool g_##obj(obj_name, sizeof(obj), obj_num);\
//	\
//	\
//	void * obj::operator new(size_t size)\
//																{\
//		void * m = g_##obj.AllocMem();\
//		return m;\
//																}\
//	\
//	\
//	void obj::operator delete(void *p)\
//																{\
//		g_##obj.FreeMem(p);\
//																}
//
//
//#endif