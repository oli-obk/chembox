#include "fourway_pipe.hpp"
#include <Gosu/Image.hpp>
#include "defines.hpp"
#include <Gosu/Math.hpp>

std::weak_ptr<Gosu::Image> FourwayPipe::s_pImage;

FourwayPipe::FourwayPipe(Gosu::Graphics& g)
:m_pImage(s_pImage.lock())
,particles(g)
{
	if (!m_pImage) {
		m_pImage.reset(new Gosu::Image(g,L"fourway_pipe.png", true));
		s_pImage = m_pImage;
	}
}

FourwayPipe::~FourwayPipe()
{
}

bool FourwayPipe::accepts(ParticleState state, ReceiveFromDir) const
{
	if (state == ParticleState::Solid) return false;
	return true;
}

void FourwayPipe::draw()
{
	m_pImage->draw(0, 0, RenderLayer::Machines, 1.0/double(m_pImage->width()), 1.0/double(m_pImage->height()));
	particles.drawGasFlow(0.5, 0.5, Gosu::angle(0, 0, 1, 0), 0.4, 0.15, Gosu::Colors::red, 0.05);
}

void FourwayPipe::receive(ParticleState, ParticleType, int, ParticleEnergy, ReceiveFromDir)
{
}

void FourwayPipe::update(optional<Machine&> up, optional<Machine&> down, optional<Machine&> left, optional<Machine&> right)
{
	particles.update();
}
