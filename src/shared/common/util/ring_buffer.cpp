#include "ring_buffer.h"
#include "../include/function_def.h"
#include <stdio.h>

RingBuffer::RingBuffer()
	:m_node_buff_len(0)
	, m_data_len(0)
	, m_head(0)
	, m_read_pos(0)
	, m_write_node(0)
	, m_write_pos(0)
	, m_total_node_num(0)
{
}

RingBuffer::~RingBuffer()
{
	DeleteNodeList(m_head);
}

void RingBuffer::DeleteNodeList(BufferNode* header_node)
{
	BufferNode* tmp_node = header_node;
	while (NULL != tmp_node)
	{
		BufferNode* next_node = tmp_node->next_node;
		delete tmp_node;
		tmp_node = next_node;
	}
}

bool RingBuffer::Init(Int32 data_buff_len, Int32 node_num)
{
	if (data_buff_len < 0 || node_num < 5)
	{
		return false;
	}
	m_node_buff_len = data_buff_len;

	m_write_node = AddNodes(node_num);
	m_head = m_write_node;
	m_total_node_num = node_num;

	return true;
}

bool RingBuffer::ReadData(Int32 read_len, char* out_data)
{
	if (read_len <= 0 || m_data_len < read_len)
	{
		return false;
	}

	Int32 copy_len = 0;
	while (copy_len < read_len)
	{
		Int32 left_len = read_len - copy_len;
		Int32 tmp_len = Min(m_node_buff_len - m_read_pos, left_len);
		memcpy(out_data + copy_len, m_head->data_buff + m_read_pos, tmp_len);
		copy_len += tmp_len;
		m_read_pos += tmp_len;

		if (m_read_pos >= m_node_buff_len)
		{
			BufferNode* tmp_node = m_head;
			m_head = m_head->next_node;
			AddNodeToNodeList(m_write_node, tmp_node);

			m_read_pos = 0;

			printf("ring buffer switch node ############## \n");
		}
			
	}
	m_data_len -= read_len;
	return true;
}

bool RingBuffer::WriteData(const char* data, Int32 data_len)
{
	Int32 space_len = GetWritableBuffLen();
	if (space_len <= data_len)
	{
		//JXS_ASSERT(m_total_node_num < 100);
		if (m_total_node_num >= 100)
		{
			printf("############## ringbuff node over max num ##########\n");
			return false;
		}

		//计算需要新增数量
		Int32 add_num = (data_len - space_len) / m_node_buff_len + 1;
		if (!Expand(add_num))
		{
			return false;
		}
	}

	Int32 copy_len = 0;
	while (copy_len < data_len)
	{
		Int32 left_len = data_len - copy_len;
		Int32 tmp_len = Min(m_node_buff_len - m_read_pos, left_len);
		memcpy(m_write_node->data_buff + m_write_pos, data + copy_len, tmp_len);
		copy_len += tmp_len;
		m_write_pos += tmp_len;

		if (m_write_pos >= m_node_buff_len)
		{
			m_write_node = m_write_node->next_node;
			m_write_pos -= m_node_buff_len;
		}
	}

	m_data_len += data_len;
	return true;
}

bool RingBuffer::ViewHeadData(void* head, Int32 head_len)
{
	static char buff[MAX_HEAD_LEN] = { 0 };

	if (m_data_len < head_len || head_len > MAX_HEAD_LEN)
	{
		return false;
	}

	Int32 cur_node_data_len = Min(m_node_buff_len - m_read_pos, m_data_len);
	if (cur_node_data_len < head_len)
	{	
		memcpy(buff, &(m_head->data_buff[m_read_pos]), cur_node_data_len);
		memcpy(&(buff[cur_node_data_len]), (m_head->next_node->data_buff), head_len - cur_node_data_len);	

		memcpy(head, buff, head_len);
	}
	else
	{
		memcpy(head, &(m_head->data_buff[m_read_pos]), head_len);
	}
	
	return true;
}

RingBuffer::BufferNode* RingBuffer::AddNodes(Int32 node_num)
{
	BufferNode* pre_node = CreateNode(m_node_buff_len);
	if (NULL == pre_node)
	{
		return NULL;
	}
	BufferNode* tnp_head_node = pre_node;

	for (Int32 i = 0; i < node_num; ++i)
	{
		BufferNode* tmp_node = CreateNode(m_node_buff_len);
		if (NULL == tmp_node)
		{
			DeleteNodeList(tnp_head_node);
			return NULL;
		}
		pre_node->next_node = tmp_node;
		pre_node = tmp_node;
	}
	return tnp_head_node;
}

Int32 RingBuffer::GetWritableBuffLen()
{
	Int32 len = 0;
	Int32 pos = m_write_pos;
	BufferNode* tmp_node = m_write_node;
	while (NULL != tmp_node)
	{
		len += (m_node_buff_len - pos);
		tmp_node = tmp_node->next_node;
		pos = 0;
	}
	return len;
}

Int32 RingBuffer::GetDataLen()
{
	return m_data_len;
}

RingBuffer::BufferNode* RingBuffer::CreateNode(Int32 buff_len)
{
	BufferNode* tmp_node = new BufferNode();
	tmp_node->data_buff = (char*)malloc(buff_len);
	return tmp_node;
}

bool RingBuffer::Expand(Int32 node_num)
{
	BufferNode* tmp_node = AddNodes(node_num);
	if (NULL == tmp_node)
	{
		return false;
	}


	if (NULL == m_write_node)
	{
		m_write_node = tmp_node;
	}
	else
	{
		BufferNode* next_node = m_write_node;
		while (NULL != next_node->next_node)
		{
			next_node = next_node->next_node;
		}
		next_node->next_node = tmp_node;	
	}
	m_total_node_num += node_num;
	return true;
}

void RingBuffer::AddNodeToNodeList(BufferNode* node_list, BufferNode* node)
{
	BufferNode* tmp_node = node_list;
	while (NULL != tmp_node->next_node)
	{
		tmp_node = tmp_node->next_node;
	}
	tmp_node->next_node = node;
	node->next_node = NULL;
}

