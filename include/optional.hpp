/*
 * taken from http://stackoverflow.com/questions/11875045/implementing-boostoptional-in-c11
 */
#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP
#include <exception>
#include <array>
#include <type_traits>
#include <cassert>

template<typename T, class Enable = void>
class optional;

template<typename T>
optional<T> make_optional(const T& val)
{
	return optional<T>(val);
}

template<typename T>
optional<T> make_optional(T&& val)
{
	return optional<T>(std::move(val));
}

template<typename T>
class optional<T, typename std::enable_if<!std::is_reference<T>::value>::type>
{
	friend class optional<T&>;
	friend class optional<const T&>;
private:
    bool valid;
    // needs to be a union, b/c in a union nothing is initialized by default
    union {
        T value;
    };
    
    void destruct_if_valid()
    {
        if (valid) {
            value.~T();
        }
    }
    
public:
    // default constructor
    optional() : valid(false) {}
    
    // initializer
    explicit optional(const T& _v)
    {
        valid = true;
        new (&value) T(_v);
    }

	explicit optional(T&& _v)
    {
        valid = true;
        new (&value) T(std::move(_v));
    }
    
    // copy constructor
    optional(const optional& other)
    {
        valid = other.valid;
        if (valid) {
            new (&value) T(other.value);
        }
    }
    
    optional& operator=(const optional& other)
    {
        destruct_if_valid();
        valid = other.valid;
        new (&value) T(other.value);
        return *this;
    }
    
    optional& operator=(optional&& other)
    {
        destruct_if_valid();
        valid = other.valid;
        if (valid) {
            new (&value) T(std::move(other.value));
            other.valid = false;
        }
        return *this;
    }
    
    // move constructor
    optional(optional&& other)
    {
        valid = other.valid;
        if (valid) {
            new (&value) T(std::move(other.value));
            other.valid = false;
        }
    }

    ~optional()
    {
        destruct_if_valid();
    }

    bool operator!() const { return !valid; }
    
    operator bool() const { return valid; }

    T& operator*()
    {
        assert(valid);
        if (valid) return value;
        throw std::bad_exception();
    }
    
    const T& operator*() const
    {
        assert(valid);
        if (valid) return value;
        throw std::bad_exception();
    }
    
    T* operator->()
    {
        assert(valid);
        if (valid) return &value;
        throw std::bad_exception();
    }
    
    const T* operator->() const
    {
        assert(valid);
        if (valid) return &value;
        throw std::bad_exception();
    }
    
    template<typename... Args>
    void construct(Args... args)
    {
        destruct_if_valid();
        valid = true;
        new (&value) T(args...);
    }
    
    void destruct()
    {
        destruct_if_valid();
        valid = false;
    }
};

template<typename T3>
class optional<T3, typename std::enable_if<std::is_reference<T3>::value>::type >
{
private:
    typedef typename std::remove_reference<T3>::type T;
    T* value;

public:
    // default constructor
    optional() : value(nullptr) {}
    
    // initializer
    explicit optional(const T3 _v) : value(&_v) {}
    
    // copy constructor
    optional(const optional& other)
    {
        value = other.value;
    }

	template<typename T5>
	explicit optional(const optional<T5>& other)
	{
		if (!other) {
			value = nullptr;
		} else {
			value = const_cast<T*>(&other.value);
		}
	}
    
    friend class optional<const T&>;
    
    template<typename T4 = T>
    optional(const optional<typename std::remove_const<T4>::type&> other, typename std::enable_if<std::is_const<T4>::value>::type* = nullptr)
    {
        value = other.value;
    }
    
    optional& operator=(const optional& other)
    {
        value = other.value;
        return *this;
    }
	
	optional& operator=(optional&& other)
    {
        value = other.value;
		other.value = nullptr;
        return *this;
    }
    
    // move constructor
    optional(optional&& other)
	#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)
	:optional()
    {
    #else
    {
        value = nullptr;
	#endif
        std::swap(value, other.value);
    }

    ~optional()
    {
    }

    bool operator!() const { return !value; }
    
    operator bool() const { return value; }

    T& operator*()
    {
        assert(value);
        if (value) return *value;
        throw std::bad_exception();
    }
    
    const T& operator*() const
    {
        assert(value);
        if (value) return *value;
        throw std::bad_exception();
    }
    
    T* operator->()
    {
        assert(value);
        if (value) return value;
        throw std::bad_exception();
    }
    
    const T* operator->() const
    {
        assert(value);
        if (value) return value;
        throw std::bad_exception();
    }
    
    void construct(T3 t)
    {
        value = &t;
    }
    
    void destruct()
    {
        value = nullptr;
    }
};
#endif // OPTIONAL_HPP
