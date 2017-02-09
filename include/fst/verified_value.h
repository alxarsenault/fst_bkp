#pragma once

#include <utility>

namespace fst {
template <typename T>
class verified_value {
public:
	inline verified_value(const T& value, bool is_valid = true);

	inline verified_value(T&& value, bool is_valid = true) noexcept;

	inline T& get();

	inline const T& get() const;

	inline T&& acquire();

	inline void set_validity(bool validity);

	inline void validate();

	inline void invalidate();

	inline bool is_valid() const;

	inline operator bool() const;

	inline bool operator==(bool b) const;

	inline bool operator!=(bool b) const;

	inline bool operator!() const;

private:
	T _value;
	bool _is_valid;
};

////
template <typename T>
inline verified_value<T> make_verified_value(const T& value, bool is_valid = true)
{
	return verified_value<T>(value, is_valid);
}

template <typename T>
inline verified_value<T> make_verified_value(T&& value, bool is_valid = true)
{
	return verified_value<T>(std::move(value), is_valid);
}
////

//
template <typename T>
inline verified_value<T>::verified_value(const T& value, bool is_valid)
	: _value(value)
	, _is_valid(is_valid)
{
}

template <typename T>
inline verified_value<T>::verified_value(T&& value, bool is_valid) noexcept
	: _value(std::move(value))
	, _is_valid(is_valid)
{
}

template <typename T>
inline T& verified_value<T>::get()
{
	return _value;
}

template <typename T>
inline const T& verified_value<T>::get() const
{
	return _value;
}

template <typename T>
inline T&& verified_value<T>::acquire()
{
	_is_valid = false;
	return std::move(_value);
}

template <typename T>
inline void verified_value<T>::set_validity(bool validity)
{
	_is_valid = validity;
}

template <typename T>
inline void verified_value<T>::validate()
{
	_is_valid = true;
}

template <typename T>
inline void verified_value<T>::invalidate()
{
	_is_valid = false;
}

template <typename T>
inline bool verified_value<T>::is_valid() const
{
	return _is_valid;
}

template <typename T>
inline verified_value<T>::operator bool() const
{
	return _is_valid;
}

template <typename T>
inline bool verified_value<T>::operator==(bool b) const
{
	return _is_valid == b;
}

template <typename T>
inline bool verified_value<T>::operator!=(bool b) const
{
	return _is_valid != b;
}

template <typename T>
inline bool verified_value<T>::operator!() const
{
	return !_is_valid;
}
}
