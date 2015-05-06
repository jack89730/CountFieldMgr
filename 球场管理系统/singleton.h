#ifndef INC_SINGLETON_H
#define INC_SINGLETON_H
/////////////////////////////////////////////////////////////////////////////////////////////
// 文件名:		singleton.h
// 创建者:		于浩淼
// 创建时间:	2010.03.29
// 内容描述:	实现设计模式 Singleton ，是一种改进的全局变量。
//              Ensure a class only has one instance, and provide a global point of access to it.
/*
	本代码实现了下面的策略
	1、支持多线程创建
	2、Singleton 的生命周期
	   1）无限生命期。一个永远不会被销毁的对象
	   2）遵循C++原则，后创建者先被销毁
*/
/////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
//__NS_BARATOL_START
//////////////////////////////////////////////////////////////////////////////////////////////////
//声明
#define ST_LIFETIME_LIFO		0		//last-in, first-out (LIFO) order
#define ST_LIFETIME_NODESTROY	-1		//不销毁对象

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
					atexit(Destroy);  //注册销毁函数，实现 LIFO
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
