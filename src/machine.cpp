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

Machine::Machine(const Machine& rhs)
:particle_engine(rhs.particle_engine)
{
	m_destroyed = true;
	m_initialized = false;
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
	m_initialized = false;
	ReceiveFromDir opposite[4] = {ReceiveFromDir::Down, ReceiveFromDir::Up, ReceiveFromDir::Right, ReceiveFromDir::Left};
	optional<Machine&> machines[] = {up, down, left, right};
	for (auto dir:{ReceiveFromDir::Up, ReceiveFromDir::Down, ReceiveFromDir::Left, ReceiveFromDir::Right})
	{
		int d = static_cast<int>(dir);
		// no connector? nothing to do
		if (!connectors[d]) continue;
		// already connected? nothing to do
		if (connectors[d]->connected()) continue;
		if (!machines[d]) {
			// there is nothing here to connect to
			return;
		}
		auto& con = machines[d]->connectors[int(opposite[d])];
		if (!con) {
			// there is a cell but no connector
			return;
		}
		connectors[d]->connect(*con);
	}
	m_initialized = true;
}
