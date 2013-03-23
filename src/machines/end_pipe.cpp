#include "end_pipe.hpp"
#include "defines.hpp"


std::weak_ptr<Gosu::Image> end_pipe::s_pImage;

end_pipe::end_pipe(Gosu::Graphics& g, ReceiveFromDir dir)
:Machine(g)
,m_pImage(s_pImage.lock())
,con(createConnector(dir))
{
	if (!m_pImage) {
		m_pImage.reset(new Gosu::Image(g,L"end_pipe.png", true));
		s_pImage = m_pImage;
	}
	receive_dir = dir;
	double angles[] = { 0, 180, -90, 90 };
	render_dir = angles[int(dir)];
}

end_pipe::~end_pipe()
{
}

bool end_pipe::accepts(ParticleState state, ReceiveFromDir dir) const
{
	if (state == ParticleState::Solid) return false;
	return dir == receive_dir;
}

void end_pipe::draw()
{
	m_pImage->drawRot(0.5, 0.5, RenderLayer::Machines, render_dir, 0.5, 0.5, 1.0/double(m_pImage->width()), 1.0/double(m_pImage->height()));
}

void end_pipe::update()
{
	auto parts = con.pop();
	con.push(parts);
}

end_pipe::end_pipe(const end_pipe& rhs)
:Machine(rhs)
,m_pImage(rhs.m_pImage)
,render_dir(rhs.render_dir)
,receive_dir(rhs.receive_dir)
,con(createConnector(receive_dir))
{
}
