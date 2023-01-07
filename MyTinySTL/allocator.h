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
  typedef ptrdiff_t    difference_type;         // 数据类型指针距离 ？？

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
template <class T>
T* allocator<T>::allocate()
{
  return static_cast<T*>(::operator new(sizeof(T)));
}

template <class T>
T* allocator<T>::allocate(size_type n)
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
  ::operator delete(ptr);
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
  mystl::construct(ptr, value);
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

