#ifndef CHEMBOX_PARTICLE_HPP
#define CHEMBOX_PARTICLE_HPP

enum class ParticleState
{
	Plasma, Gas, Liquid, Solid
};

typedef uint8_t ParticleType;
typedef uint64_t ParticleEnergy;


#include <map>
typedef std::map<ParticleType, int> ParticleCountMap;
typedef std::map<ParticleState, ParticleCountMap> ParticleMap;

#endif // CHEMBOX_PARTICLE_HPP
