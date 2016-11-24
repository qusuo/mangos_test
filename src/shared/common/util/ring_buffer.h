#ifndef JXS_RINGBUFFER_H_
#define JXS_RINGBUFFER_H_
//author:hsj
//用链表实现的环形缓冲区
//保证在数据末结点写在头结点读

#include "../include/basic_type_def.h"

class RingBuffer
{
public:
	static const Int32 MIN_BUFF_LEN = 128;
	static const Int32 MIN_NODE_NUM = 5;
	static const Int32 MAX_HEAD_LEN = 64;
public:
	RingBuffer();
	~RingBuffer();
	//节点数应该有一个最小值，以保证内存的利用率
	bool Init(Int32 data_buff_len, Int32 node_num);
	
	bool ReadData(Int32 read_len, char* out_data);
	bool WriteData(const char* data, Int32 data_len);
	bool ViewHeadData(void* head, Int32 head_len);
	Int32 GetDataLen();

private:
	Int32 GetWritableBuffLen();
	bool Expand(Int32 node_num);

private:
	struct BufferNode
	{
		BufferNode() :data_buff(NULL), next_node(NULL){}
		~BufferNode()
		{
			if (NULL != data_buff)
			{
				free(data_buff);
			}
			next_node = NULL;
		}
		char* data_buff;
		BufferNode* next_node;
	};
	BufferNode* CreateNode(Int32 buff_len);
	BufferNode* AddNodes(Int32 node_num);
	void DeleteNodeList(BufferNode* header_node);
	void AddNodeToNodeList(BufferNode* node_list, BufferNode* node);
private:
	/** 读和写可能会在同一个节点 **/
	Int32 m_node_buff_len;			//一个结点的长度
	Int32 m_data_len;				//写入的数据长度
	BufferNode* m_head;				//链表头
	Int32 m_read_pos;				//读位置
	BufferNode* m_write_node;		//写节点
	Int32 m_write_pos;				//写入位置
	Int32 m_total_node_num;			//总结点个数
};

#endif