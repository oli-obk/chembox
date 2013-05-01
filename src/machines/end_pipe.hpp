#ifndef END_PIPE_HPP
#define END_PIPE_HPP

#include "machine.hpp" // Base class: Machine
#include <memory>
#include "Particle.hpp"
#include <Gosu/Graphics.hpp>
#include "particle_engine.hpp"
#include "rotatable_machine.hpp"

class EndPipe : public RotatableMachine
{
private:
	static std::array<std::weak_ptr<Gosu::Image>, 5> s_pImage;
	std::array<std::shared_ptr<Gosu::Image>, 5> m_pImage;
	size_t version;
	EndPipe(const EndPipe& rhs);
    void set_version(size_t version);
    size_t get_version() const;
public:
	EndPipe(Gosu::Graphics& g, int dir, size_t version = 0);
	virtual ~EndPipe();

public:
	virtual void draw();
	virtual void update();
	virtual std::unique_ptr<Machine> clone() { return std::unique_ptr<EndPipe>(new EndPipe(*this)); }
	size_t numActions() const;
	void Action(size_t id);
};

#endif // END_PIPE_HPP
