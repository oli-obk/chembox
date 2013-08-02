#ifndef CHEMBOX_PARTICLE_HPP
#define CHEMBOX_PARTICLE_HPP

#include <cstdint>
#include "helpers.hpp"
#include <cstddef>
#include <cassert>
#include <vector>
#include <array>

enum class ParticleState
{
	Plasma, Gas, Liquid, Solid
};

enum class ParticleType : uint8_t
{
	Hydrogen = 1,
	Helium = 2
};

typedef builtin_wrapper<uint64_t> ParticleEnergy;

#include <map>
class ParticleMap
{
private:
    friend void intersect(ParticleMap& left, ParticleMap& right, ParticleMap& intersection);
	std::map<ParticleState, std::map<ParticleType, builtin_wrapper<int>>> data;
    ParticleMap& operator=(const ParticleMap&) = delete;
    ParticleMap(const ParticleMap&) = delete;
public:
    ParticleMap(ParticleMap&& rhs)
    {
        data = std::move(rhs.data);
    }
    
    ParticleMap& operator=(ParticleMap&& rhs)
    {
        assert(data.empty());
        data = std::move(rhs.data);
        return *this;
    }
    
    void swap(ParticleMap& other)
    {
        std::swap(other.data, data);
    }
    
	ParticleMap() {};

	int count(ParticleState state, ParticleType type) const
	{
		auto it = data.find(state);
		if (it == data.end()) return 0;
		auto it2 = it->second.find(type);
		if (it2 == it->second.end()) return 0;
		return it2->second;
	}
	
	int count(ParticleType type) const
	{
		int ret = 0;
		for (auto& a:data) {
			auto it = a.second.find(type);
			if (it == a.second.end()) continue;
			ret += it->second;
		}
		return ret;
	}
	
	int count(ParticleState state) const
	{
		auto it = data.find(state);
		if (it == data.end()) return 0;
		int ret = 0;
		for (auto& b:it->second) {
			ret += b.second;
		}
		return ret;
	}
	
	int count() const
	{
		int ret = 0;
		for (auto& a:data) {
			for (auto& b:a.second) {
				ret += b.second;
			}
		}
		return ret;
	}
	
	ParticleMap(ParticleState state, ParticleType type, int count)
	{
		data[state][type] = count;
	}
	void add(ParticleState state, ParticleType type, unsigned count)
	{
		data[state][type] += count;
	}

	void subtract(ParticleState state, ParticleType type, unsigned count)
	{
		data[state][type] -= count;
	}

	ParticleMap& operator += (ParticleMap&& rhs)
	{
		for (auto& a:rhs.data) {
			for (auto& b:a.second) {
				data[a.first][b.first] += b.second;
			}
		}
        rhs.data.clear();
		return *this;
	}

	ParticleMap& operator -= (ParticleMap&& rhs)
	{
		for (auto& a:rhs.data) {
			for (auto& b:a.second) {
				data[a.first][b.first] -= b.second;
			}
		}
        rhs.data.clear();
		return *this;
	}
    
    void negate()
    {
		for (auto& a:data) {
			for (auto& b:a.second) {
				b.second *= -1;
			}
		}
    }

	template<size_t divisor>
	std::array<ParticleMap, divisor> split()
	{
		std::array<ParticleMap, divisor> ret;
		for (auto& a:data) {
			for (auto& b:a.second) {
                for (size_t i = 0; i < divisor; i++) {
                    ret[i].data[a.first][b.first] = b.second/divisor;
                }
                // increase the first "rest" bins
                size_t rest = b.second%divisor;
                for (size_t i = 0; i < rest; i++) {
                    ret[i].data[a.first][b.first]++;
                }
			}
		}
        data.clear();
		return ret;
	}

	std::vector<ParticleMap> split(size_t divisor)
	{
		std::vector<ParticleMap> ret(divisor);
		for (auto& a:data) {
			for (auto& b:a.second) {
                for (size_t i = 0; i < divisor; i++) {
                    ret[i].data[a.first][b.first] = b.second/divisor;
                }
                // increase the first "rest" bins
                size_t rest = b.second%divisor;
                for (size_t i = 0; i < rest; i++) {
                    ret[i].data[a.first][b.first]++;
                }
			}
		}
        data.clear();
		return ret;
	}
};

inline ParticleMap operator+ (ParticleMap&& lhs, ParticleMap&& rhs)
{
    ParticleMap ret = std::move(lhs);
    ret += std::move(rhs);
    return ret;
}

inline ParticleMap operator- (ParticleMap&& lhs, ParticleMap&& rhs)
{
    ParticleMap ret = std::move(lhs);
    ret -= std::move(rhs);
    return ret;
}

inline void intersect(ParticleMap& left, ParticleMap& right, ParticleMap& intersection)
{
    for (auto& a:left.data) {
        for (auto& b:a.second) {
            auto ra = right.data.find(a.first);
            if (ra == right.data.end()) continue;
            auto rb = ra->second.find(b.first);
            if (rb == ra->second.end()) continue;
            
            if (b.second == rb->second) {
                intersection.data[a.first][b.first] += b.second*2;
                a.second.erase(b.first);
                ra->second.erase(b.first);
            } else if (b.second > rb->second) {
                int diff = b.second - rb->second;
                intersection.data[a.first][b.first] += rb->second*2;
                b.second = diff;
                ra->second.erase(b.first);
            } // other case handled in second loop
        }
    }
    for (auto& a:right.data) {
        for (auto& b:a.second) {
            auto la = left.data.find(a.first);
            if (la == left.data.end()) continue;
            auto lb = la->second.find(b.first);
            if (lb == la->second.end()) continue;
            
            if (b.second > lb->second) {
                int diff = b.second - lb->second;
                intersection.data[a.first][b.first] += lb->second*2;
                b.second = diff;
                la->second.erase(b.first);
            } // other cases handled in first loop
        }
    }
}

#endif // CHEMBOX_PARTICLE_HPP
