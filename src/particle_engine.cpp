#include "particle_engine.hpp"
#include "defines.hpp"
#include <Gosu/Graphics.hpp>

ParticleEngine::ParticleEngine(Gosu::Graphics& g)
:Gas(g, L"particle_gas.png"), t(0)
,graphics(g)
{
}

void ParticleEngine::drawGasFlow(double x, double y, double dir, double length, double width)
{
	graphics.pushTransform(Gosu::translate(x, y));
	graphics.pushTransform(Gosu::rotate(dir));
	for (double dx = 0; dx < length; dx++) {
		for (double dy = 0; dy < width; dy++) {
			Gas.draw(dy, -dx-t, RenderLayer::Particles, 0.01, 0.01, Gosu::Color::WHITE);
		}
	}
	graphics.popTransform();
	graphics.popTransform();
}

void ParticleEngine::update()
{
	t += 0.1;
	if (t >= 1.0) t = 0.0;
}
