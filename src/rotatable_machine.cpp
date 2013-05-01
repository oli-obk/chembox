#include "rotatable_machine.hpp"

RotatableMachine::RotatableMachine(Gosu::Graphics& g, int dir)
:Machine(g)
{
    rotation = 0;
    set_rotation(dir);
}

RotatableMachine::~RotatableMachine()
{
}

void RotatableMachine::createConnector(ReceiveFromDir dir)
{
    Machine::createConnector(dir.rotate(rotation));
}

void RotatableMachine::destroyConnector(ReceiveFromDir dir)
{
    Machine::destroyConnector(dir.rotate(rotation));
}

void RotatableMachine::set_rotation(size_t rot)
{
    bool dirs[] = {
        getConnector(ReceiveFromDir::Up),
        getConnector(ReceiveFromDir::Right),
        getConnector(ReceiveFromDir::Down),
        getConnector(ReceiveFromDir::Left)
        };
	destroyConnectors();
    rot %= 4;
    rotation = rot;
    for (ReceiveFromDir i:{ReceiveFromDir::Up, ReceiveFromDir::Right, ReceiveFromDir::Down, ReceiveFromDir::Left}) {
        if (!dirs[static_cast<int>(i)]) continue;
        createConnector(i);
    }
}

optional<Connector&> RotatableMachine::getConnector(ReceiveFromDir dir)
{
    return Machine::getConnector(dir.rotate(rotation));
}

optional<const Connector&> RotatableMachine::getConnector(ReceiveFromDir dir) const
{
    return Machine::getConnector(dir.rotate(rotation));
}

RotatableMachine::RotatableMachine(const RotatableMachine& rhs)
:Machine(rhs)
{
    set_rotation(rhs.rotation);
}

size_t RotatableMachine::get_rotation() const
{
    return rotation;
}
