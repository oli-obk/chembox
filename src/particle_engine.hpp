#ifndef PARTICLEENGINE_HPP
#define PARTICLEENGINE_HPP

#include <Gosu/Image.hpp>
#include "Particle.hpp"

class ParticleEngine
{
	static std::weak_ptr<Gosu::Image> s_pGas;
	std::shared_ptr<Gosu::Image> m_pGas;
	double t;
	Gosu::Graphics& graphics;
public:
	ParticleEngine(Gosu::Graphics&);
	void drawGasFlow(double x, double y, double dir, double length, double width, Gosu::Color col, double step = 1.0);
	void update();
};

#endif // PARTICLEENGINE_HPP
