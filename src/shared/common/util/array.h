#ifndef JXS_ARRAY_H_
#define JXS_ARRAY_H_
//定长数组
//数据有序连续
#include "../include/basic_type_def.h"

namespace jxs
{
	template<class T, int N>
	class Array
	{
	public:
		Array();
		~Array();
		int GetLength()const;//获取对象当前元素数目  
		int GetTotalSize()const;//获取对象当前申请的总内存   
		int FindFirst(const T& t);
		T RemoveAt(const int& pos);//删除指定位置的元素（索引从0开始),返回删除的元素，并保持有原数据有序
		void RemoveFirst(const T& t);
		void RemoveAll();//删除所有元素
		void RawRemoveAt(const int& pos);
		bool IsEmpty()const;//判断当前容器是否为空  
		T& operator [](int i);
		T Pop();//尾部删除一个元素,返回该元素    
		void Push(const T& t); //尾部加入一个元素
		void IncNum();
		void DecNum();
	private:
		int m_num;//当前的实际元素个数 
		T m_temp[N];
		 
	};

	template<class T, int N>
	Array<T, N>::Array()
		:m_num(0)
	{
		memset(m_temp, 0, sizeof(m_temp));
	}

	template<class T, int N>
	Array<T, N>::~Array()
	{
	}

	template<class T, int N>
	T& Array<T, N>::operator [](int i)
	{
		if (i<0 || i>N - 1)
		{
			i = 0;
		}
		return m_temp[i];
	}
	template<class T, int N>
	int Array<T, N>::GetLength()const
	{
		return m_num;

	}

	template<class T, int N>
	T Array<T, N>::Pop()
	{
		if (m_num > 0)
		{
			return m_temp[m_num--];
		}
		else
		{
			return (T)NULL;
		}
	}

	template<class T, int N>
	int Array<T, N>::GetTotalSize()const
	{
		return N;
	}

	template<class T, int N>
	int Array<T, N>::FindFirst(const T& t)
	{
		for (int i = 0; i < m_num; ++i)
		{
			if (m_temp[i] == t)
			{
				return i;
			}
		}
		return -1;
	}

	template<class T, int N>
	void Array<T, N>::Push(const T& t)
	{
		if (m_num < N)
		{
			m_temp[m_num++] = t;
		}
	}

	template<class T, int N>
	T Array<T, N>::RemoveAt(const int& pos)
	{
		if (pos < 0 || pos >= m_num)
		{
			return (T)0;
		}
		else
		{
			T ret = m_temp[pos];
			for (int i = pos; i < m_num; ++i)
			{
				m_temp[i] = m_temp[i + 1];
			}

			--m_num;
			return ret;
		}
	}

	template<class T, int N>
	void Array<T, N>::RawRemoveAt(const int& pos)
	{
		if (pos < 0 || pos >= m_num)
			return;
		
		for (int i = pos; i < m_num; ++i)
			m_temp[i] = m_temp[i + 1];

		--m_num;
	}

	template<class T, int N>
	void Array<T, N>::RemoveFirst(const T& t)
	{
		int find_pos = FindFirst(t);
		if (find_pos >= 0)
		{
			RemoveAt(find_pos);
		}
	}

	template<class T, int N>
	void Array<T, N>::RemoveAll()
	{
		m_num = 0;
	}
	template<class T, int N>
	bool Array<T, N>::IsEmpty()const
	{
		return 0 == m_num;
	}

	template<class T, int N>
	void Array<T, N>::IncNum()
	{
		m_num++;
	}

	template<class T, int N>
	void Array<T, N>::DecNum()
	{
		m_num--;
	}
}


#endif