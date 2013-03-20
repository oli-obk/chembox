#ifndef FOURWAYPIPE_HPP
#define FOURWAYPIPE_HPP

#include "machine.hpp" // Base class: Machine
#include <Gosu/Graphics.hpp>
#include <memory>

class FourwayPipe : public Machine
{
private:
	static std::weak_ptr<Gosu::Image> s_pImage;
	std::shared_ptr<Gosu::Image> m_pImage;
public:
	FourwayPipe(Gosu::Graphics&);
	virtual ~FourwayPipe();

public:
	virtual bool accepts(ParticleState, ReceiveFromDir) const;
	virtual void draw(double x, double y, double wdt, double hgt);
	virtual void receive(ParticleState, ParticleType, int count, ParticleEnergy energysum, ReceiveFromDir);
	virtual void update(optional<Machine&> up, optional<Machine&> down, optional<Machine&> left, optional<Machine&> right);
};

#endif // FOURWAYPIPE_HPP
