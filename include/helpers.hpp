#ifndef CHEMBOX_HELPERS_HPP
#define CHEMBOX_HELPERS_HPP

template<unsigned int t, unsigned int u>
struct constpow {
	static const long unsigned int value = t*constpow<t, u-1>::value;
};

template<unsigned int t>
struct constpow<t, 0> {
	static const long unsigned int value = 1;
};

template<typename T, T def = 0>
class builtin_wrapper
{
private:
	T val;
public:
	builtin_wrapper(T v = def):val(v) {}
	operator T() const { return val; }
	T& operator+= (const T& rhs)
	{
		return val += rhs;
	}
};

template<typename T>
T operator+(const builtin_wrapper<T>& lhs, const T& rhs)
{
	return static_cast<T>(lhs) + rhs;
}

template<typename T>
T operator+(const T& lhs, const builtin_wrapper<T>& rhs)
{
	return lhs + static_cast<T>(rhs);
}

#endif // CHEMBOX_HELPERS_HPP
