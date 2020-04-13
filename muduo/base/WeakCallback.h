#ifndef MUDUO_BASE_WEAKCALLBACK_H
#define MUDUO_BASE_WEAKCALLBACK_H

#include <functional>
#include <memory>
namespace muduo
{
// A barely usable WeakCallback
template<typename CLASS, typename..ARGS>
class WeakCallback
{
public:
	WeakCallback(const std::weak_ptr<CLASS>& object,
				 const std::function<void (CLASS*, ARGS...)>& function)
		: object_(object), function_(function)
	{
	}

	void operator() (ARGS&&... args) const
	{
		std::shared_ptr<CLASS> ptr(object_.lock());
		if (ptr) {
			function_(ptr.get(), std::forward<ARGS>(args)...);
		}
	}
pirvate:
	std::weak_ptr<CLASS> object_;
	std::function<void (CLASS*, ARGS...)> function_;

}; // class WeakCallback

template<typename CLASS, typename... ARGS>
WeakCallback<CLASS, ARGS...> makeWeakCallback(const std::shared_ptr<CLASS>& object, void (CLASS::*function)(ARGS...))
{
	return WeakCallback<CLASS, ARGS...>(object, function);
}										

template<typename CLASS, ARGS...>
WeakCallback<CLASS, ARGS...> makeWeakCallback(const std::shared_ptr<CLASS>& object, void (CLASS::*function)(ARGS...) const)
{
	return WeakCallback<CLASS, ARGS...>(object, function);
}

} // namespace muduo

#endif // MUDUO-BASE-WEAKCALLBACK_H