#include "fourway_pipe.hpp"
#include <Gosu/Image.hpp>

std::weak_ptr<Gosu::Image> FourwayPipe::s_pImage;

FourwayPipe::FourwayPipe(Gosu::Graphics& g)
:m_pImage(s_pImage.lock())
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

void FourwayPipe::draw(double x, double y, double wdt, double hgt)
{
	m_pImage->draw(x, y, 0, wdt/double(m_pImage->width()), hgt/double(m_pImage->width()));
}

void FourwayPipe::receive(ParticleState, ParticleType, int, ParticleEnergy, ReceiveFromDir)
{
}
void FourwayPipe::update(optional<Machine&> up, optional<Machine&> down, optional<Machine&> left, optional<Machine&> right)
{
}
