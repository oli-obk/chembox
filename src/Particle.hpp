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

#endif // CHEMBOX_PARTICLE_HPP
