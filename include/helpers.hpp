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

#endif // CHEMBOX_HELPERS_HPP
