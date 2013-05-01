#ifndef ROTATABLEMACHINE_HPP
#define ROTATABLEMACHINE_HPP

#include "machine.hpp" // Base class: Machine

class RotatableMachine : public Machine
{
    size_t rotation;
protected:
    RotatableMachine(Gosu::Graphics& g, int dir);
    RotatableMachine(const RotatableMachine& rhs);
public:
    void set_rotation(size_t rot);
    size_t get_rotation() const;
	virtual optional<Connector&> getConnector(ReceiveFromDir dir);
	virtual optional<const Connector&> getConnector(ReceiveFromDir dir) const;
	virtual void createConnector(ReceiveFromDir dir);
	virtual void destroyConnector(ReceiveFromDir dir);
    virtual ~RotatableMachine();

};

#endif // ROTATABLEMACHINE_HPP
