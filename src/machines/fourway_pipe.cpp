#include "fourway_pipe.hpp"
#include <Gosu/Image.hpp>
#include "defines.hpp"

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

void FourwayPipe::draw(double x, double y)
{
	m_pImage->draw(x, y, RenderLayer::Machines, 1.0/double(m_pImage->width()), 1.0/double(m_pImage->height()));
}

void FourwayPipe::receive(ParticleState, ParticleType, int, ParticleEnergy, ReceiveFromDir)
{
}

void FourwayPipe::update(optional<Machine&> up, optional<Machine&> down, optional<Machine&> left, optional<Machine&> right)
{
}
