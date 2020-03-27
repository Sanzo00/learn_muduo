#ifndef MUDUO_BASE_NONCOPYABLE_H
#define MUDUO_BASE_NONCOPYABLE_H

namespace muduo
{

// 继承noncopyable的类, 不能拷贝构造和赋值, 对象语义
class noncopyable
{
public:
	// 删除拷贝构造和赋值操作
	noncopyable(const noncopyable&) = delete;
	void operator=(const noncopyable&) = delete;

protected:
	noncopyable() = default;
	~noncopyable() = default;
};

}

#endif
