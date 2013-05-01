#include "end_pipe.hpp"
#include "defines.hpp"


std::array<std::weak_ptr<Gosu::Image>, 5> EndPipe::s_pImage;

EndPipe::EndPipe(Gosu::Graphics& g, int dir, size_t version)
:RotatableMachine(g, dir)
,version(0)
{
	for (size_t i = 0; i < s_pImage.size(); i++) {
		m_pImage[i] = s_pImage[i].lock();
	}
	if (!m_pImage[0]) {
		assert(!m_pImage[1]);
		assert(!m_pImage[2]);
		assert(!m_pImage[3]);
		assert(!m_pImage[4]);
		m_pImage[0].reset(new Gosu::Image(g,L"end_pipe.png", true));
		m_pImage[1].reset(new Gosu::Image(g,L"end_pipe2.png", true));
		m_pImage[2].reset(new Gosu::Image(g,L"end_pipe2opposite.png", true));
		m_pImage[3].reset(new Gosu::Image(g,L"end_pipe3.png", true));
		m_pImage[4].reset(new Gosu::Image(g,L"end_pipe4.png", true));

		for (size_t i = 0; i < s_pImage.size(); i++) {
			s_pImage[i] = m_pImage[i];
		}
	}
    set_version(version);
}

EndPipe::~EndPipe()
{
}

void EndPipe::draw()
{
	double angles[] = { 0, 90, 180, -90 };
	m_pImage[get_version()]->drawRot(0.5, 0.5, RenderLayer::Machines, angles[get_rotation()], 0.5, 0.5, 1.0/double(m_pImage[get_version()]->width()), 1.0/double(m_pImage[get_version()]->height()));
}

void EndPipe::update()
{
	for (auto dir:{ReceiveFromDir::Up, ReceiveFromDir::Down, ReceiveFromDir::Left, ReceiveFromDir::Right}) {
		auto con = getConnector(dir);
		if (!con) continue;
		auto parts = con->pop();
		con->push(parts);
	}
}

EndPipe::EndPipe(const EndPipe& rhs)
:RotatableMachine(rhs)
{
	for (int i:{0, 1, 2, 3, 4}) {
		m_pImage[i] = rhs.m_pImage[i];
	}
    set_version(rhs.version);
}

void EndPipe::Action(size_t id)
{
	switch (id) {
		case 0:
            set_rotation(get_rotation()+1);
		break;
		case 1:
			set_version(version+1);
		break;
		default:
		break;
	}
}

size_t EndPipe::numActions() const
{
	return 2;
}

void EndPipe::set_version(size_t v)
{
	v %= 5;
	destroyConnectors();
	switch (v) {
		case 0:
			createConnector(ReceiveFromDir::Up);
		break;
		case 1:
			createConnector(ReceiveFromDir::Up);
			createConnector(ReceiveFromDir::Right);
		break;
		case 2:
			createConnector(ReceiveFromDir::Up);
			createConnector(ReceiveFromDir::Down);
		break;
		case 3:
			createConnector(ReceiveFromDir::Up);
			createConnector(ReceiveFromDir::Down);
			createConnector(ReceiveFromDir::Right);
		break;
		case 4:
			createConnector(ReceiveFromDir::Up);
			createConnector(ReceiveFromDir::Down);
			createConnector(ReceiveFromDir::Right);
			createConnector(ReceiveFromDir::Left);
		break;
	}
	version = v;
}

size_t EndPipe::get_version() const
{
    return version;
}
