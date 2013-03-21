#include "particle_engine.hpp"
#include "defines.hpp"
#include <Gosu/Graphics.hpp>

ParticleEngine::ParticleEngine(Gosu::Graphics& g)
:Gas(g, L"particle_gas.png"), t(0)
,graphics(g)
{
}

void ParticleEngine::drawGasFlow(double x, double y, double dir, double length, double width, Gosu::Color col, double step)
{
	graphics.pushTransform(Gosu::translate(x, y));
	graphics.pushTransform(Gosu::rotate(dir, width/2));
	for (double dx = 0; dx < length; dx+=step) {
		for (double dy = 0; dy < width; dy+=step) {
			Gosu::Color c = col;
			double dxt = dx+t*step;
			double val = dxt/(length+step)*2-1;
			c.setAlpha(255-val*val*255);
			Gas.draw(dy, -dxt, RenderLayer::Particles, 0.1/double(Gas.width()), 0.1/double(Gas.height()), c);
		}
	}
	graphics.popTransform();
	graphics.popTransform();
}

void ParticleEngine::update()
{
	t += 0.1;
	if (t >= 1.0) t -= 1.0;
}
