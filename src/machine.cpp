#include "machine.hpp"

Machine::Machine(Gosu::Graphics& g)
{
	m_destroyed = true;
	m_initialized = false;
}

void Machine::createConnector(ReceiveFromDir dir)
{
	ReInitialize();
	assert(!connectors[int(dir)]);
	connectors[int(dir)].construct();
}

Machine::~Machine()
{
}

Machine::Machine(const Machine& rhs)
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
	optional<Machine&> machines[] = {up, right, down, left};
	for (auto dir:{ReceiveFromDir::Up, ReceiveFromDir::Right, ReceiveFromDir::Down, ReceiveFromDir::Left})
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
		auto& con = machines[d]->connectors[static_cast<int>(flip(dir))];
		if (!con) {
			// there is a cell but no connector
			return;
		}
		connectors[d]->connect(*con);
	}
	m_initialized = true;
}


optional<Connector&> Machine::getConnector(ReceiveFromDir dir)
{
	return optional<Connector&>(connectors[static_cast<int>(dir)]);
}

optional<const Connector&> Machine::getConnector(ReceiveFromDir dir) const
{
	return optional<const Connector&>(connectors[static_cast<int>(dir)]);
}

void Machine::destroyConnector(ReceiveFromDir dir)
{
	ReInitialize();
	connectors[int(dir)].destruct();
}

void Machine::destroyConnectors()
{
	ReInitialize();
	for (auto& c:connectors) {
		c.destruct();
	}
}
