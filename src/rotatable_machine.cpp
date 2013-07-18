#include "rotatable_machine.hpp"

RotatableMachine::RotatableMachine(Gosu::Graphics& g, ReceiveFromDir dir)
:Machine(g)
,rotation(ReceiveFromDir::Up)
{
    set_rotation(dir);
}

RotatableMachine::~RotatableMachine()
{
}

void RotatableMachine::createConnector(ReceiveFromDir dir)
{
    Machine::createConnector(dir + rotation);
}

void RotatableMachine::destroyConnector(ReceiveFromDir dir)
{
    Machine::destroyConnector(dir + rotation);
}

void RotatableMachine::set_rotation(ReceiveFromDir rot)
{
    bool dirs[] = {
        getConnector(ReceiveFromDir::Up),
        getConnector(ReceiveFromDir::Right),
        getConnector(ReceiveFromDir::Down),
        getConnector(ReceiveFromDir::Left)
        };
	destroyConnectors();
    rotation = rot;
    for (ReceiveFromDir i:{ReceiveFromDir::Up, ReceiveFromDir::Right, ReceiveFromDir::Down, ReceiveFromDir::Left}) {
        if (!dirs[static_cast<int>(i)]) continue;
        createConnector(i);
    }
}

optional<Connector&> RotatableMachine::getConnector(ReceiveFromDir dir)
{
    return Machine::getConnector(dir + rotation);
}

optional<const Connector&> RotatableMachine::getConnector(ReceiveFromDir dir) const
{
    return Machine::getConnector(dir + rotation);
}

RotatableMachine::RotatableMachine(const RotatableMachine& rhs)
:Machine(rhs)
,rotation(rhs.rotation)
{
}

ReceiveFromDir RotatableMachine::get_rotation() const
{
    return rotation;
}

double RotatableMachine::getXDir(ReceiveFromDir dir) const
{
    switch (dir + rotation) {
        case ReceiveFromDir::Down:
        case ReceiveFromDir::Up:
        return 0.0;
        case ReceiveFromDir::Left:
        return -1.0;
        case ReceiveFromDir::Right:
        return 1.0;
    }
    assert(false);
}

double RotatableMachine::getYDir(ReceiveFromDir dir) const
{
    switch (dir + rotation) {
        case ReceiveFromDir::Right:
        case ReceiveFromDir::Left:
        return 0.0;
        case ReceiveFromDir::Up:
        return -1.0;
        case ReceiveFromDir::Down:
        return 1.0;
    }
    assert(false);
}
