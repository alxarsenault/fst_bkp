#pragma once

#include <sys/time.h>

namespace fst {

class time {
public:
	inline time()
	{
		gettimeofday(&_tv, nullptr);
	}

	const struct timeval& get_timeval() const
	{
		return _tv;
	}

	// inline long GetMs() const { return _tv.tv_sec * 1000 +
	// (long)ceil(_tv.tv_usec / 1000.0); }
	inline long ms() const
	{
		return _tv.tv_sec * 1000 + (long)(_tv.tv_usec / 1000.0);
	}

	inline time& operator=(const time& time)
	{
		_tv = time._tv;
		return *this;
	}

	inline bool operator==(const time& time) const
	{
		return !(bool)timercmp(&_tv, &time._tv, !=);
	}

	inline bool operator!=(const time& time) const
	{
		return (bool)timercmp(&_tv, &time._tv, !=);
	}

	inline bool operator<(const time& time) const
	{
		return (bool)timercmp(&_tv, &time._tv, <);
	}

	inline bool operator>(const time& time) const
	{
		return (bool)timercmp(&_tv, &time._tv, >);
	}

	inline bool operator<=(const time& time) const
	{
		return operator<(time) || operator==(time);
	}

	inline bool operator>=(const time& time) const
	{
		return operator>(time) || operator==(time);
	}

	inline time operator-(const time& time) const
	{
		class time c;
		timersub(&_tv, &time._tv, &c._tv);
		return c;
	}

	inline time operator+(const time& time) const
	{
		class time c;
		timeradd(&_tv, &time._tv, &c._tv);
		return c;
	}

	inline time& operator-=(const time& time)
	{
		class time c;
		timersub(&_tv, &time._tv, &c._tv);
		_tv = c._tv;
		return *this;
	}

	inline time& operator+=(const time& time)
	{
		class time c;
		timeradd(&_tv, &time._tv, &c._tv);
		_tv = c._tv;
		return *this;
	}

private:
	struct timeval _tv;
};
} // fst.
