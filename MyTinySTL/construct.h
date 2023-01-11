#ifndef MYTINYSTL_CONSTRUCT_H_
#define MYTINYSTL_CONSTRUCT_H_

// 这个头文件包含两个函数 construct，destroy
// construct : 负责对象的构造
// destroy   : 负责对象的析构

#include <new>

#include "type_traits.h"
#include "iterator.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)  // unused parameter
#endif // _MSC_VER

namespace mystl
{

// construct 构造对象

//construct调用的是placement new, 在一个已经获得的内存里建立一个对象
template <class Ty>
void construct(Ty* ptr)
{
  ::new ((void*)ptr) Ty();     //::全局作用域符号，当全局变量在局部函数中与其中某个变量重名，那么就可以用::来区分
}

template <class Ty1, class Ty2>
void construct(Ty1* ptr, const Ty2& value)
{
  //placement new（定位new）:在用户指定的内存位置上（这个内存是已经预先分配好的）构建新的对象，因此这个构建过程不需要额外分配内存，只需要调用对象的构造函数在该内存位置上构造对象即可
  //Object * p = new (address) ClassConstruct(...);
  //address：placement new所指定的内存地址
  //ClassConstruct：对象的构造函数

  //new操作符为分配内存所调用函数的名字是operator new，就是间接性的调用了malloc函数。

  //new operator就是new操作符，平时使用的new（如int *pt = new object(...);）主要完成两个工作：
  //1、分配足够的内存以便容纳所需类型的对象（operator new）
  //2、调用构造函数初始化内存中的对象（placement new）


  //(new operator)的效果就是(operator new)+(placement new)。
  ::new ((void*)ptr) Ty1(value);        
}

template <class Ty, class... Args>
void construct(Ty* ptr, Args&&... args)
{
  ::new ((void*)ptr) Ty(mystl::forward<Args>(args)...);
}

// destroy 将对象析构


//“trivial destructor”一般是指用户没有自定义析构函数，而由系统生成的，这种析构函数在《STL源码解析》中成为“无关痛痒”的析构函数。

//反之，用户自定义了析构函数，则称之为“non-trivial destructor”，这种析构函数如果申请了新的空间一定要显式的释放，否则会造成内存泄露

//对于trivial destructor，如果每次都进行调用，显然对效率是一种伤害，如何进行判断呢？《STL源码解析》中给出的说明是：

//首先利用value_type()获取所指对象的型别，再利用type_traits判断该型别的析构函数是否trivial，若是(true_type)，则什么也不做，若为(__false_type)，则去调用destory()函数
//也就是说，在实际的应用当中，STL库提供了相关的判断方法__type_traits，感兴趣的读者可以自行查阅使用方式。除了trivial destructor，还有trivial construct、trivial copy construct等，如果能够对是否trivial进行区分，可以采用内存处理函数memcpy()、malloc()等更加高效的完成相关操作，提升效率。



template <class Ty>
void destroy_one(Ty*, std::true_type) {}

template <class Ty>
void destroy_one(Ty* pointer, std::false_type)
{
  if (pointer != nullptr)
  {
    pointer->~Ty();
  }
}

template <class ForwardIter>
// 当__type_traits为__true_type时, 什么也不做, 因为这样效率很高效, 并不需要执行析构函数
void destroy_cat(ForwardIter , ForwardIter , std::true_type) {}

//// 当__type_traits为__false_type时，通过迭代所有的对象并调用版本一的函数执行析构函数进行析构
template <class ForwardIter>
void destroy_cat(ForwardIter first, ForwardIter last, std::false_type)
{
  for (; first != last; ++first)
    destroy(&*first);
}

//destroy调用析构函数并且有两个版本
//版本一传入指针直接就调用了析构函数：
template <class Ty>
void destroy(Ty* pointer)
{
  //调用了std::is_trivially_destructible用来判断是否有系统默认的析构函数，如果是系统默认的析构函数就没有必要执行。
  destroy_one(pointer, std::is_trivially_destructible<Ty>{});
}

//版本二需要传入两个迭代器，并且根据是否有自定义的析构函数来执行析构
template <class ForwardIter>
void destroy(ForwardIter first, ForwardIter last)
{
  destroy_cat(first, last, std::is_trivially_destructible<
              typename iterator_traits<ForwardIter>::value_type>{});
}

} // namespace mystl

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // !MYTINYSTL_CONSTRUCT_H_

