#ifndef END_PIPE_HPP
#define END_PIPE_HPP

#include "machine.hpp" // Base class: Machine
#include <memory>
#include "Particle.hpp"
#include <Gosu/Graphics.hpp>
#include "rotatable_versioned_machine.hpp"

class EndPipe : public RotatableVersionedMachine
{
private:
	EndPipe(const EndPipe& rhs);
    std::array<ParticleMap, 4> particles;
public:
	virtual void Initialize(optional<Machine&> up, optional<Machine&> down, optional<Machine&> left, optional<Machine&> right);
	EndPipe(Gosu::Graphics& g, ReceiveFromDir dir, size_t version = 0);
	EndPipe(char c, Gosu::Graphics& g);
	virtual ~EndPipe();

public:
	virtual void send();
	virtual void receive();
	virtual std::unique_ptr<Machine> clone() const { return std::unique_ptr<EndPipe>(new EndPipe(*this)); }

    char serialize() const { return 'X'; }
    static const std::string deserializes() { return "X"; }
};

#endif // END_PIPE_HPP
