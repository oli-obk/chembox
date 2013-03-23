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
	static std::weak_ptr<Gosu::Font> s_pFont;
	std::shared_ptr<Gosu::Image> m_pImage;
	std::shared_ptr<Gosu::Font> m_pFont;
	std::reference_wrapper<Connector> connectors[4];
	ParticleEnergy energy;
public:
	ParticleMap particles;
	FourwayPipe(Gosu::Graphics&);
	virtual ~FourwayPipe();

public:
	virtual bool accepts(ParticleState, ReceiveFromDir) const;
	virtual void draw();
	virtual void update();
};

#endif // FOURWAYPIPE_HPP
