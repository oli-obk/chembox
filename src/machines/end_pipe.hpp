#ifndef END_PIPE_HPP
#define END_PIPE_HPP

#include "machine.hpp" // Base class: Machine
#include <memory>
#include "Particle.hpp"
#include <Gosu/Graphics.hpp>
#include "particle_engine.hpp"

class EndPipe : public Machine
{
private:
	static std::array<std::weak_ptr<Gosu::Image>, 5> s_pImage;
	std::array<std::shared_ptr<Gosu::Image>, 5> m_pImage;
	double render_dir;
	size_t version, rotation;
	EndPipe(const EndPipe& rhs);
	void updateSettings(size_t rotation, size_t version);
public:
	EndPipe(Gosu::Graphics& g, ReceiveFromDir dir, size_t version = 0);
	virtual ~EndPipe();

public:
	virtual void draw();
	virtual void update();
	virtual std::unique_ptr<Machine> clone() { return std::unique_ptr<EndPipe>(new EndPipe(*this)); }
	size_t numActions() const;
	void Action(size_t id);
};

#endif // END_PIPE_HPP
