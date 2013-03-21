#include "particle_engine.hpp"
#include "defines.hpp"
#include <Gosu/Graphics.hpp>
#include <Gosu/Math.hpp>

std::weak_ptr<Gosu::Image> ParticleEngine::s_pGas;

ParticleEngine::ParticleEngine(Gosu::Graphics& g)
:m_pGas(s_pGas.lock()), t(Gosu::random(0.0, 1.0))
,graphics(g)
{
	if (!m_pGas) {
		m_pGas.reset(new Gosu::Image(g, L"particle_gas.png"));
		s_pGas = m_pGas;
	}
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
			m_pGas->draw(dy, -dxt, RenderLayer::Particles, 0.1/double(m_pGas->width()), 0.1/double(m_pGas->height()), c);
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
