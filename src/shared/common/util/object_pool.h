#ifndef JXS_OBJECTPOOL_H_
#define JXS_OBJECTPOOL_H_
//author:hsj
//简单的对象池
#include "../include/basic_type_def.h"

template<class T>
class ObjectCreater
{
public:
	virtual T* CreateObject();
	ObjectCreater(){}
	virtual ~ObjectCreater(){}

private:

};

template<class T>
T* ObjectCreater<T>::CreateObject()
{
	return new T;
}

template<class T>
class ObjectPool
{
public:
	ObjectPool();
	~ObjectPool();
	static ObjectPool<T>& GetSingleton();
	bool Init(int obj_num, ObjectCreater<T>* obj_creator);

	T* GetFreeObject();
	bool ReleaseObject(T* obj); //返还
	
private:
	void Expand(int add_num);

private:
	T** m_objects;
	int m_objects_num;
	int m_free_object_num;
	ObjectCreater<T>* m_obj_creator;
};

template<class T>
ObjectPool<T>& ObjectPool<T>::GetSingleton()
{
	static ObjectPool<T> instance;
	return instance;
}

template<class T>
void ObjectPool<T>::Expand(int add_num)
{
	Int32 total_num = m_objects_num + add_num;
	T** new_objects = (T**)malloc(sizeof(T*) * total_num);
	if (NULL == new_objects)
	{
		return;
	}

	for (int i = 0; i < add_num; ++i)
	{
		new_objects[i] = m_obj_creator->CreateObject();
	}

	if (NULL != m_objects)
	{
		memcpy(&(new_objects[add_num]), &(m_objects[0]), sizeof(T*) * m_objects_num);
		free((void*)m_objects);
	}
	m_free_object_num += add_num;
	m_objects_num = total_num;
	m_objects = new_objects;
}

template<class T>
bool ObjectPool<T>::ReleaseObject(T* obj)
{
	if (m_free_object_num >= m_objects_num)
	{
		return false;
	}
	else
	{
		m_objects[m_free_object_num] = obj;
		++m_free_object_num;
		return true;
	}
}

template<class T>
T* ObjectPool<T>::GetFreeObject()
{
	if (m_free_object_num <= 0)
	{
		Expand(m_objects_num);
	}
	if (m_free_object_num <= 0)
	{
		return NULL;
	}
	else
	{
		return m_objects[--m_free_object_num];
	}
}

template<class T>
bool ObjectPool<T>::Init(int obj_num, ObjectCreater<T>* obj_creator)
{
	if (NULL != m_objects || obj_num <= 0)
	{
		return false;
	}

	if (NULL == obj_creator)
	{
		obj_creator = new ObjectCreater<T>();
	}

	m_objects = (T**)malloc(sizeof(T*) * obj_num); //new (T*)[obj_num];
	if (NULL == m_objects)
	{
		return false;
	}
	m_obj_creator = obj_creator;

	for (int i = 0; i < obj_num; ++i)
	{
		m_objects[i] = m_obj_creator->CreateObject();
	}
	m_free_object_num = obj_num;
	m_objects_num = obj_num;
	return true;
}

template<class T>
ObjectPool<T>::~ObjectPool()
{
	for (int i = 0; i < m_free_object_num; ++i)
	{
		if (NULL != m_objects[i])
		{
			delete m_objects[i];
		}
	}
	if (NULL != m_obj_creator)
	{
		delete m_obj_creator;
	}
}

template<class T>
ObjectPool<T>::ObjectPool()
	:m_objects(NULL)
	, m_objects_num(0)
	, m_free_object_num(0)
	, m_obj_creator(NULL)
{
	
};




#endif