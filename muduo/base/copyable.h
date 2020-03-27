#ifndef MUDUO_BASE_COPYABLE_H
#define MUDUO_BASE_COPYABLE_H

namespace muduo
{

// copyable是空基类,标识类. 继承copyable的类为值语义
class copyable
{

protected:
	copyable() = default; // 默认存在构造函数
	~copyable() = default; // 默认窜在析构函数
};

}

#endif
