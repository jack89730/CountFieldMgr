#ifndef INC_SINGLETON_H
#define INC_SINGLETON_H
/////////////////////////////////////////////////////////////////////////////////////////////
// �ļ���:		singleton.h
// ������:		�ں��
// ����ʱ��:	2010.03.29
// ��������:	ʵ�����ģʽ Singleton ����һ�ָĽ���ȫ�ֱ�����
//              Ensure a class only has one instance, and provide a global point of access to it.
/*
	������ʵ��������Ĳ���
	1��֧�ֶ��̴߳���
	2��Singleton ����������
	   1�����������ڡ�һ����Զ���ᱻ���ٵĶ���
	   2����ѭC++ԭ�򣬺󴴽����ȱ�����
*/
/////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
//__NS_BARATOL_START
//////////////////////////////////////////////////////////////////////////////////////////////////
//����
#define ST_LIFETIME_LIFO		0		//last-in, first-out (LIFO) order
#define ST_LIFETIME_NODESTROY	-1		//�����ٶ���

template<class T, int nLifetime = ST_LIFETIME_LIFO>
class CTLSingleton 
{
public:
	static T& Instance() // Unique point of access
	{ 
		if (0 == _instance) 
		{
			CSingleLock lock(&GetMutex());
			lock.Lock();
			if (0 == _instance) 
			{
				_instance = new T();
				if(nLifetime == ST_LIFETIME_LIFO)
				{
					atexit(Destroy);  //ע�����ٺ�����ʵ�� LIFO
				}
			}
		}
		return *_instance;
	}
	static T* GetInstance()
	{
		return _instance;
	}
	static void Release()
	{
		Destroy();
	}
protected:
	CTLSingleton(){}
	~CTLSingleton(){}
private:
	CTLSingleton(const CTLSingleton&);
	CTLSingleton& operator=(const CTLSingleton&);
private:
	static void Destroy() // Destroy the only instance
	{ 
		if ( _instance != 0 ) 
		{
			delete _instance;
			_instance = 0;
		}
	}
	static CCriticalSection& GetMutex()
	{
		static CCriticalSection _mutex;
		return _mutex;
	}
	
	static T * volatile _instance;				// The one and only instance
};

//template<class T, int nLifetime> CCriticalSection CTLSingleton<T, nLifetime>::_mutex;
template<class T, int nLifetime> T * volatile CTLSingleton<T, nLifetime>::_instance = 0;

//__NS_BARATOL_END
//////////////////////////////////////////////////////////////////////////////////////////////////
#endif  //INC_SINGLETON_H
