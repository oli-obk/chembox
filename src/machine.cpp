#include "machine.hpp"

Machine::Machine(Gosu::Graphics& g)
:particle_engine(g)
{
	m_destroyed = true;
}

Connector& Machine::createConnector(ReceiveFromDir dir)
{
	assert(!connectors[int(dir)]);
	connectors[int(dir)].construct();
	return *connectors[int(dir)];
}

Machine::~Machine()
{
}

void Machine::insert(ParticleState state, ParticleType type, int count)
{
	particles[state][type] += count;
}
