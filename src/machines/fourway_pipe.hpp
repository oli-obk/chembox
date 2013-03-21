#ifndef FOURWAYPIPE_HPP
#define FOURWAYPIPE_HPP

#include "machine.hpp" // Base class: Machine
#include <Gosu/Graphics.hpp>
#include <memory>
#include "particle_engine.hpp"

class FourwayPipe : public Machine
{
private:
	static std::weak_ptr<Gosu::Image> s_pImage;
	std::shared_ptr<Gosu::Image> m_pImage;
	ParticleEngine particles;
public:
	FourwayPipe(Gosu::Graphics&);
	virtual ~FourwayPipe();

public:
	virtual bool accepts(ParticleState, ReceiveFromDir) const;
	virtual void draw();
	virtual void receive(ParticleState, ParticleType, int count, ParticleEnergy energysum, ReceiveFromDir);
	virtual void update(optional<Machine&> up, optional<Machine&> down, optional<Machine&> left, optional<Machine&> right);
};

#endif // FOURWAYPIPE_HPP
