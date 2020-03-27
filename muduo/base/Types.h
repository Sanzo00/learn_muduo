#ifndef MUDUO_BASE_TYPES_H
#define MUDUO_BASE_TYPES_H

#include <stdint.h>
#include <string.h>
#include <string>

#ifndef NOEBUG
#include <assert.h>
#endif

namespace muduo
{

using std::string;

inline void memZero(void* p, size_t n) {
	memset(p, 0, n);
}

// static_cast显式的类型转换, 凡是隐式执行的类型转换都可以用static_cast来显式完成同时消除编译器对隐式转化的警告
// up_cast(派生类到基类的转化)和down_cast(基类到派生类的转化)
// 无论是向上还是向下转型, 编译器只看继承关系, 如果存在继承关系就会放行, 不能透过现象看本质
// down-cast往往是不正确的转化, static_cast不能做真正类型的检查, 编译通过但会在运行时会崩溃
// implicit_cast可以解决这个问题, 在编译期给出错误信息
template<typename To, typename From>
inline To implicit_cast(From const &f)  // implicit_cast<ToType>(expr)
{
	return f;
}

// dynamic_cast依赖于RTTI(Runtime Type Information), 被转化的类型必须时多态(含有虚函数), 在运行时进行类型转换, 将基类指针或引用转化为派生类指针或引用
// dynamic_cast在运行时可能crash, 运行时RTTI不是好的选择
// 
template<typename To, typename From>
inline To down_cast(From* f) // 只接受指针类型down_cast<T*>(foo)
{

#if !defined(NOEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
	assert(f == NULL || dynamic_cast<To>(f) != NULL); // RTTI: 调试输出 
#endif
	return static_cast<To>(f);
}

}

#endif
