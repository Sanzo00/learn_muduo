// Taken from PCRE pcre_stringpiece.h
//
// Copyright (c) 2005, Google Inc.
// All rights reserved.

#ifndef MUDUO_BASE_STRINGPIECE_H
#define MUDUO_BASE_STRINGPIECE_H

#include <string.h>
#include <iosfwd>
#include "muduo/base/Types.h"

namespace muduo
{

class StringArg
{
public:
	StringArg(const char* str)
		: str_(str)
	{
	}

	StringArg(const string& str)
		: str_(str.c_str())
	{
	}

	const char* c_str() const { return str_; }
	
private:
	const char* str_;
}; // class StringArg


// StringPiece实际上是字符串的proxy类, 构造函数是const, 只用于提取, 不用于修改
// 为什么要有这个StringPiece类?
// C++里面有string和char*，如果你用const string &s 做函数形参，可以同时兼容两种字符串。但当你传入一个很长的char * 时，char*转string，创建一个临时string对象,同时开辟一个内存空间，拷贝字符串, 开销比较大。如果你的目的仅仅是读取字符串的值，用这个StringPiece的话，仅仅是4+一个指针的内存开销，而且也保证了兼容性。所以这个类的目的是传入字符串的字面值，它内部的ptr_ 这块内存不归他所有。所以不能做任何改动。归根结底，是处于性能的考虑，用以实现高效的字符串传递，这里既可以用const char*，也可以用std::string类型作为参数，并且不涉及内存拷贝。
// 实际上，这个类是google提供的一个类。

class StringPiece 
{
private:
	const char* ptr_;
	int length_;

public:
// We provide non-explicit singleton constructors so users can pass
// in a "const char*" or a "string" wherever a "StringPiece" is
// expected.	
	StringPiece()
		: ptr_(NULL), length_(0) { }
	StringPiece(const char* str)
		: ptr_(str), length_(static_cast<int>(strlen(ptr_))) { } 
	StringPiece(const unsigned char* str)
		: ptr_(reinterpret_cast<const char*>(str)),
		  length_(static_cast<int>(strlen(ptr_))) { }
	StringPiece(const string& str)
		: ptr_(str.data()), length_(static_cast<int>(str.size())) { }
	StringPiece(const char* offset, int len)
		: ptr_(offset), length_(len) { }

// data() may return a pointer to a buffer with embedded NULs, and the
// returned buffer may or may not be null terminated.  Therefore it is
// typically a mistake to pass data() to a routine that expects a NUL
// terminated string.  Use "as_string().c_str()" if you really need to do
// this.  Or better yet, change your routine so it does not rely on NUL
// termination.
	const char* data() const { return ptr_; }
	int size() const { return length_; }
	bool empty() const { return length_ == 0; }
	const char* begin() const { return ptr_; }
	const char* end() const { return ptr_ + length_; }

	void clear() { ptr_ = NULL; length_ = 0; }
	void set(const char* buffer, int len) { ptr_ = buffer; length_ = len; }
	void set(const char* str) {
		ptr_ = str;
		length_ = static_cast<int>(strlen(str));
	}

	void set(const void* buffer, int len) {
		ptr_ = reinterpret_cast<const char*>(buffer);
		length_ = len;
	}

	char operator[] (int i) const { return ptr_[i]; }
	void remove_prefix(int n) {
		ptr_ += n;
		length_ -= n;
	}

	void remove_suffix(int n) {
		length_ -= n;
	}

	bool operator == (const StringPiece& x) const {
		return ((length_ == x.length_) && (memcmp(ptr_, x.ptr_, length_) == 0));
	}

	bool operator != (const StringPiece& x) const {
		return !(*this == x);
	}

#define STRINGPIECE_BINARY_PREDICATE(cmp, auxcmp)									\
	bool operator cmp(const StringPiece& x) const{									\
		int r = memcmp(ptr_, x.ptr_, length_ < x.length_ ? length_ : x.length_);	\
		return ((r auxcmp 0) || ((r == 0) && (length_ cmp x.length_)));				\
	}									

	STRINGPIECE_BINARY_PREDICATE(<,  <);
	STRINGPIECE_BINARY_PREDICATE(<=, <);
	STRINGPIECE_BINARY_PREDICATE(>,  >);
	STRINGPIECE_BINARY_PREDICATE(>=, >);
#undef STRINGPIECE_BINARY_PREDICATE

	int compare(const StringPiece& x) const {
		int r = memcmp(ptr_, x.ptr_, length_ < x.length_ ? length_ : x.length_);
		if (r == 0) {
			if (length_ < x.length_) r = -1;
			else if (length_ > x.length_) r = +1;
		}
		return r;
	}

	string as_string() const{
		return string(data(), size());
	}

	void CopyToString(string* target) const {
		// ptr_拷贝到
		target->assign(ptr_, length_);
	}

	// Does "this" start "x"
	bool starts_with(const StringPiece& x) const{
		return ((length_ >= x.length_) && (memcmp(ptr_, x.ptr_, x.length_) == 0));
	}
}; // class StringPiece

} // namespace muduo

#ifdef HAVE_TYPE_TRAITS
// This makes vector<StringPiece> really fast for some STL implementations
template<> struct __type_traits<muduo::StringPiece> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type has_trivial_destructor;
	typedef __true_type is_POD_type;
};
#endif

// allow StringPiece to be logged
std::ostream& operator << (std::ostream& o, const muduo::StringPiece& piece);

#endif // MUDUO_BASE_STRINGPIECE_H
