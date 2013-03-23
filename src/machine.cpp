#include "machine.hpp"

Machine::Machine(Gosu::Graphics& g)
:particle_engine(g)
{
	m_destroyed = true;
	m_initialized = false;
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

void Machine::communicate()
{
	for (auto& c:connectors) {
		if (!c) continue;
		c->communicate();
	}
}

void Machine::Initialize(optional<Machine&> up, optional<Machine&> down, optional<Machine&> left, optional<Machine&> right)
{
	assert(!m_initialized);
	ReceiveFromDir opposite[4] = {ReceiveFromDir::Down, ReceiveFromDir::Up, ReceiveFromDir::Right, ReceiveFromDir::Left};
	optional<Machine&> machines[] = {up, down, left, right};
	for (auto dir:{ReceiveFromDir::Up, ReceiveFromDir::Down, ReceiveFromDir::Left, ReceiveFromDir::Right})
	{
		int d = static_cast<int>(dir);
		if (!connectors[d]) continue;
		if (!machines[d]) continue;
		connectors[d]->connect(*(machines[d]->connectors[int(opposite[d])]));
	}
	m_initialized = true;
}
