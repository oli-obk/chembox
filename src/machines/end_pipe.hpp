#ifndef END_PIPE_HPP
#define END_PIPE_HPP

#include "machine.hpp" // Base class: Machine
#include <memory>
#include "Particle.hpp"
#include <Gosu/Graphics.hpp>
#include "particle_engine.hpp"

class end_pipe : public Machine
{
private:
	static std::weak_ptr<Gosu::Image> s_pImage;
	std::shared_ptr<Gosu::Image> m_pImage;
	double render_dir;
	ReceiveFromDir receive_dir;
	Connector& con;
public:
	end_pipe(Gosu::Graphics& g, ReceiveFromDir dir);
	virtual ~end_pipe();

public:
	virtual bool accepts(ParticleState, ReceiveFromDir) const;
	virtual void draw();
	virtual void update();
};

#endif // END_PIPE_HPP
