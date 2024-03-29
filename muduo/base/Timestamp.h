#ifndef MUDUO_BASE_TIMESTAMP_H
#define MUDUO_BASE_TIMESTAMP_H

#include "muduo/base/copyable.h"
#include "muduo/base/Types.h"
#include <boost/operators.hpp>

namespace muduo
{

class Timestamp : public muduo::copyable,
				  public boost::equality_comparable<Timestamp>,
				  public boost::less_than_comparable<Timestamp>
{
public:
	Timestamp()
		: microSecondsSinceEpoch_(0)
	{
	}

	explicit Timestamp(int64_t microSecondsSinceEpochArg)
		: microSecondsSinceEpoch_(microSecondsSinceEpochArg)
	{
	}

	void swap(Timestamp& that)
	{
		std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
	}	

	string toString() const;
	string toFormattedString(bool showMicroseconds = true) const;
	bool valid() const { return microSecondsSinceEpoch_ > 0; }
	
	// 内部使用
	int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
	time_t secondsSinceEpoch() const
	{
		return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
	}

	// 获取当前时间
	static Timestamp now();
	static Timestamp invalid() { return Timestamp(); }

	static Timestamp fromUnixTime(time_t t)
	{
		return fromUnixTime(t, 0);
	}

	static Timestamp fromUnixTime(time_t t, int microseconds)
	{
		return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
	}

	static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
	int64_t microSecondsSinceEpoch_;	
};							  

inline bool operator < (Timestamp lhs, Timestamp rhs)
{
	return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator == (Timestamp lhs, Timestamp rhs)
{
	return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

// 求两个时间戳相差的秒数
inline double timeDifference(Timestamp high, Timestamp low)
{
	int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
	return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

// 对时间戳添加秒数
inline Timestamp addTime(Timestamp timestamp, double seconds)
{
	int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
	return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

}

#endif
