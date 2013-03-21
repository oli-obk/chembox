#ifndef MACHINE_HPP
#define MACHINE_HPP

enum class ReceiveFromDir
{
	Up, Down, Left, Right
};

#include "Particle.hpp"
#include "optional.hpp"

class Machine
{
public:
	virtual void draw() = 0;
	virtual bool accepts(ParticleState, ReceiveFromDir) const { return false; }
	virtual void update(optional<Machine&> up, optional<Machine&> down, optional<Machine&> left, optional<Machine&> right) = 0;
	virtual void receive(ParticleState, ParticleType, int count, ParticleEnergy energysum, ReceiveFromDir) = 0;
	virtual ~Machine();
};

#endif // MACHINE_HPP
