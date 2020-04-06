#ifndef MUDUO_NET_CALLBACKS_H
#define MUDUO_NET_CALLBACKS_H

#include "muduo/base/Timestamp.h"
#include <functional>
#include <memory>

namespace muduo
{

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

template<typename T>
inline T* get_pointer(const std::shared_ptr<T>& ptr)
{
	return ptr.get();
}

template<typename T>
inline T* get_pointer(const std::unique_ptr<T>& ptr)
{
	return ptr.get();
}

template<typename To, typename From>
inline ::std::shared_ptr<To> down_pointer_cast(const ::std::shared_ptr<From>& f) {
#ifndef NOEBUG
	assert(f == NULL || dynamic_cast<To*>(get_pointer(f)) != NULL);
#endif
	return ::std::static_pointer_cast<To>(f);
}

namespace net
{
// All client visible callbacks go here.
class Buffer;
class TcpConne

} // namespace net

} // namespace muduo

#endif // MUDUO_NET_CALLBACKS_H
