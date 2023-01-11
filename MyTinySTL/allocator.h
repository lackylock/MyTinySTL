#ifndef MYTINYSTL_ALLOCATOR_H_
#define MYTINYSTL_ALLOCATOR_H_

// 这个头文件包含一个模板类 allocator，用于管理内存的分配、释放，对象的构造、析构


//分配器将new的申请空间（allocate）和调用构造函数（construct）的两个功能分开实现。


#include "construct.h"
#include "util.h"

namespace mystl
{

// 模板类：allocator
// 模板函数代表数据类型
template <class T>
class allocator
{
public:
  typedef T            value_type;              // 数据类型
  typedef T*           pointer;                 // 数据类型指针
  typedef const T*     const_pointer;           // const数据类型指针
  typedef T&           reference;               // 数据类型引用
  typedef const T&     const_reference;         // const数据类型引用
  typedef size_t       size_type;               // 数据类型大小

  //long long最早是C99标准引进的，然而VC6.0推出于1998年，在C99标准之前。所以当时微软就自己搞出来一个变量叫做__int64来表示64位整数。
  //很多同学使用的第一个C++的编译器就是VC6.0，所以记得在VC6.0当中要使用__int64而非long long。
  //所以一个比较简单的区分方法是，判断编译器运行的操作系统是否是windows，如果是windows使用__int64，否则使用long long
  typedef ptrdiff_t    difference_type;         // 数据类型指针距离（typedef __int64    ptrdiff_t;）   

public:
  // 分配内存
  static T*   allocate();
  static T*   allocate(size_type n);

  // 释放内存
  static void deallocate(T* ptr);
  static void deallocate(T* ptr, size_type n);

  // 构造对象
  static void construct(T* ptr);
  static void construct(T* ptr, const T& value);
  static void construct(T* ptr, T&& value);

  template <class... Args>
  static void construct(T* ptr, Args&& ...args);

  // 析构对象
  static void destroy(T* ptr);
  static void destroy(T* first, T* last);
};


// 分配内存直接使用的operator new

// operator new()：指对new的重载形式，它是一个函数，并不是运算符。对于operator new来说，分为全局重载和类重载，
// 全局重载是void* ::operator new(size_t size)，在类中重载形式 void* A::operator new(size_t size)。
// 还要注意的是这里的operator new()完成的操作一般只是分配内存，事实上系统默认的全局  ::operator new(size_t size)也只是调用malloc分配内存，并且返回一个void*指针。
// 而构造函数的调用(如果需要)是在new运算符中完成的

template <class T>
T* allocator<T>::allocate()   //默认构造函数
{
  return static_cast<T*>(::operator new(sizeof(T)));    //::operator new 返回一个void*,使用static_cast强转
}

template <class T>
T* allocator<T>::allocate(size_type n)  //带参构造函数 -- 分配n个T对象内存空间
{
  if (n == 0)
    return nullptr;
  return static_cast<T*>(::operator new(n * sizeof(T)));
}


// 回收内存使用的operator delete
template <class T>
void allocator<T>::deallocate(T* ptr)
{
  if (ptr == nullptr)
    return;
  ::operator delete(ptr);   //使用 ::operator new 分配内存，则用 ::operator delete(ptr) 释放内存
}

template <class T>
void allocator<T>::deallocate(T* ptr, size_type /*size*/)
{
  if (ptr == nullptr)
    return;
  ::operator delete(ptr);
}

template <class T>
void allocator<T>::construct(T* ptr)
{
  //construct直接调用了placement new
  mystl::construct(ptr);
}

template <class T>
void allocator<T>::construct(T* ptr, const T& value)
{
  mystl::construct(ptr, value);   //在起始地址ptr处调用带参构造函数T(value)构造一个对象
}

template <class T>
 void allocator<T>::construct(T* ptr, T&& value)
{
  mystl::construct(ptr, mystl::move(value));
}

template <class T>
template <class ...Args>
 void allocator<T>::construct(T* ptr, Args&& ...args)
{
  mystl::construct(ptr, mystl::forward<Args>(args)...);
}

template <class T>
void allocator<T>::destroy(T* ptr)
{
  mystl::destroy(ptr);
}

template <class T>
void allocator<T>::destroy(T* first, T* last)
{
  mystl::destroy(first, last);
}

} // namespace mystl
#endif // !MYTINYSTL_ALLOCATOR_H_

