#ifndef ROTATABLEMACHINE_HPP
#define ROTATABLEMACHINE_HPP

#include "machine.hpp" // Base class: Machine

class RotatableMachine : public Machine
{
    ReceiveFromDir rotation;
protected:
    RotatableMachine(Gosu::Graphics& g, ReceiveFromDir dir);

    void set_rotation(ReceiveFromDir rot);
    ReceiveFromDir get_rotation() const;
	optional<Connector&> getConnector(ReceiveFromDir dir);
	optional<const Connector&> getConnector(ReceiveFromDir dir) const;
	void createConnector(ReceiveFromDir dir);
	void destroyConnector(ReceiveFromDir dir);
public:
    double getXDir(ReceiveFromDir dir) const;
    double getYDir(ReceiveFromDir dir) const;
    virtual ~RotatableMachine();

};

#endif // ROTATABLEMACHINE_HPP
