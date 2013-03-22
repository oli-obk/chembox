#ifndef CHEMBOX_PARTICLE_HPP
#define CHEMBOX_PARTICLE_HPP

#include <cstdint>
#include "helpers.hpp"

enum class ParticleState
{
	Plasma, Gas, Liquid, Solid
};

typedef uint8_t ParticleType;
typedef builtin_wrapper<uint64_t> ParticleEnergy;


#include <map>
typedef std::map<ParticleType, int> ParticleCountMap;
typedef std::map<ParticleState, ParticleCountMap> ParticleMap;

#endif // CHEMBOX_PARTICLE_HPP
