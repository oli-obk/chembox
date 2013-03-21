#ifndef PARTICLEENGINE_HPP
#define PARTICLEENGINE_HPP

#include <Gosu/Image.hpp>
#include "Particle.hpp"

class ParticleEngine
{
	Gosu::Image Gas;
	double t;
	Gosu::Graphics& graphics;
public:
	ParticleEngine(Gosu::Graphics&);
	void drawGasFlow(double x, double y, double dir, double length, double width);
	void update();
};

#endif // PARTICLEENGINE_HPP
