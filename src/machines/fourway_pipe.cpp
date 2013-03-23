#include "fourway_pipe.hpp"
#include <Gosu/Image.hpp>
#include "defines.hpp"
#include <Gosu/Math.hpp>
#include <Gosu/Text.hpp>
#include <Gosu/Font.hpp>
#include <sstream>

std::weak_ptr<Gosu::Image> FourwayPipe::s_pImage;
std::weak_ptr<Gosu::Font> FourwayPipe::s_pFont;

FourwayPipe::FourwayPipe(Gosu::Graphics& g)
:Machine(g)
,m_pImage(s_pImage.lock())
,m_pFont(s_pFont.lock())
,connectors({
	createConnector(ReceiveFromDir::Up),
	createConnector(ReceiveFromDir::Down),
	createConnector(ReceiveFromDir::Left),
	createConnector(ReceiveFromDir::Right),
	})
{
	if (!m_pImage) {
		assert(!m_pFont);
		m_pImage.reset(new Gosu::Image(g,L"fourway_pipe.png", true));
		s_pImage = m_pImage;
		m_pFont.reset(new Gosu::Font(g, Gosu::defaultFontName(), 10));
		s_pFont = m_pFont;
	}
}

FourwayPipe::~FourwayPipe()
{
}

bool FourwayPipe::accepts(ParticleState state, ReceiveFromDir) const
{
	if (isDestroyed()) return false;
	if (state == ParticleState::Solid) return false;
	return true;
}

void FourwayPipe::draw()
{
	m_pImage->draw(0, 0, RenderLayer::Machines, 1.0/double(m_pImage->width()), 1.0/double(m_pImage->height()));
	size_t count = particles.count(ParticleState::Gas, ParticleType::Hydrogen);
	if (count != 0) {
		std::wstringstream wss;
		wss << count;
		m_pFont->drawRel(wss.str(), 0.5, 0.5, RenderLayer::Machines+1, 0.5, 0.4, 0.05, 0.05, Gosu::Colors::red);
	}
}

void FourwayPipe::update()
{
	for (Connector& c:connectors) {
		particles += c.pop();
	}
	ParticleMap distribution = particles / 4;
	particles -= distribution*4;
	for (Connector& c:connectors) {
		c.push(distribution);
	}
	particle_engine.update();
}
