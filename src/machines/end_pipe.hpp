#ifndef END_PIPE_HPP
#define END_PIPE_HPP

#include "machine.hpp" // Base class: Machine
#include <memory>
#include "Particle.hpp"
#include <Gosu/Graphics.hpp>
#include "particle_engine.hpp"
#include "rotatable_versioned_machine.hpp"

class EndPipe : public RotatableVersionedMachine
{
private:
	EndPipe(const EndPipe& rhs);
    std::array<ParticleMap, 4> particles;
public:
	EndPipe(Gosu::Graphics& g, ReceiveFromDir dir, size_t version = 0);
	virtual ~EndPipe();

public:
	virtual void send();
	virtual void receive();
	virtual std::unique_ptr<Machine> clone() { return std::unique_ptr<EndPipe>(new EndPipe(*this)); }
	size_t numActions() const;
	void Action(size_t id);
};

#endif // END_PIPE_HPP
