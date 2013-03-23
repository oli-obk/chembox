#ifndef CHEMBOX_PARTICLE_HPP
#define CHEMBOX_PARTICLE_HPP

#include <cstdint>
#include "helpers.hpp"
#include <cstddef>
#include <cassert>

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
	std::map<ParticleState, std::map<ParticleType, builtin_wrapper<uint32_t>>> data;
public:
	ParticleMap() {};
	size_t count(ParticleState state, ParticleType type) const
	{
		auto it = data.find(state);
		if (it == data.end()) return 0;
		auto it2 = it->second.find(type);
		if (it2 == it->second.end()) return 0;
		return it2->second;
	}
	ParticleMap(ParticleState state, ParticleType type, unsigned count)
	{
		data[state][type] = count;
	}
	void add(ParticleState state, ParticleType type, unsigned count)
	{
		data[state][type] += count;
	}

	void subtract(ParticleState state, ParticleType type, unsigned count)
	{
		assert(data[state][type] >= count);
		data[state][type] -= count;
	}

	ParticleMap& operator += (const ParticleMap& rhs)
	{
		for (auto& a:rhs.data) {
			for (auto& b:a.second) {
				data[a.first][b.first] += b.second;
			}
		}
		return *this;
	}

	ParticleMap& operator -= (const ParticleMap& rhs)
	{
		for (auto& a:rhs.data) {
			for (auto& b:a.second) {
				assert(b.second <= data[a.first][b.first]);
				data[a.first][b.first] -= b.second;
			}
		}
		return *this;
	}

	ParticleMap operator/(size_t divisor)
	{
		ParticleMap ret;
		for (auto& a:data) {
			for (auto& b:a.second) {
				ret.data[a.first][b.first] = b.second/divisor;
			}
		}
		return ret;
	}

	ParticleMap operator%(size_t divisor)
	{
		ParticleMap ret;
		for (auto& a:data) {
			for (auto& b:a.second) {
				ret.data[a.first][b.first] = b.second%divisor;
			}
		}
		return ret;
	}

	ParticleMap operator*(size_t mul)
	{
		ParticleMap ret;
		for (auto& a:data) {
			for (auto& b:a.second) {
				ret.data[a.first][b.first] = b.second*mul;
			}
		}
		return ret;
	}
};

#endif // CHEMBOX_PARTICLE_HPP
