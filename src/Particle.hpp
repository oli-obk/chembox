#ifndef CHEMBOX_PARTICLE_HPP
#define CHEMBOX_PARTICLE_HPP

#include <cstdint>
#include "helpers.hpp"

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
typedef std::map<ParticleType, builtin_wrapper<uint32_t>> ParticleCountMap;
typedef std::map<ParticleState, ParticleCountMap> ParticleMap;

#endif // CHEMBOX_PARTICLE_HPP
