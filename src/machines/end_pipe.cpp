#include "end_pipe.hpp"
#include "defines.hpp"


std::array<std::weak_ptr<Gosu::Image>, 5> EndPipe::s_pImage;

EndPipe::EndPipe(Gosu::Graphics& g, ReceiveFromDir dir, size_t version)
:Machine(g)
,version(0)
,rotation(0)
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
	updateSettings(int(dir), version);
}

EndPipe::~EndPipe()
{
}

void EndPipe::draw()
{
	m_pImage[version]->drawRot(0.5, 0.5, RenderLayer::Machines, render_dir, 0.5, 0.5, 1.0/double(m_pImage[version]->width()), 1.0/double(m_pImage[version]->height()));
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
:Machine(rhs)
{
	for (int i:{0, 1, 2, 3, 4}) {
		m_pImage[i] = rhs.m_pImage[i];
	}
	updateSettings(rhs.rotation, rhs.version);
}

void EndPipe::Action(size_t id)
{
	switch (id) {
		case 0:
			{
				int next[] = { 3, 2, 0, 1};
				updateSettings(next[rotation], version);
			}
		break;
		case 1:
			updateSettings(rotation, version+1);
		break;
		default:
		break;
	}
}

size_t EndPipe::numActions() const
{
	return 2;
}

void EndPipe::updateSettings(size_t rot, size_t v)
{
	rot %= 4;
	v %= 5;
	int next[] = { 3, 2, 0, 1};
	int opposite[] = { 1, 0, 3, 2};
	int prev[] = { 2, 3, 1, 0};
	destroyConnectors();
	switch (v) {
		case 0:
			createConnector(static_cast<ReceiveFromDir>(rot));
		break;
		case 1:
			createConnector(static_cast<ReceiveFromDir>(rot));
			createConnector(static_cast<ReceiveFromDir>(next[rot]));
		break;
		case 2:
			createConnector(static_cast<ReceiveFromDir>(rot));
			createConnector(static_cast<ReceiveFromDir>(opposite[rot]));
		break;
		case 3:
			createConnector(static_cast<ReceiveFromDir>(rot));
			createConnector(static_cast<ReceiveFromDir>(opposite[rot]));
			createConnector(static_cast<ReceiveFromDir>(next[rot]));
		break;
		case 4:
			createConnector(static_cast<ReceiveFromDir>(rot));
			createConnector(static_cast<ReceiveFromDir>(opposite[rot]));
			createConnector(static_cast<ReceiveFromDir>(next[rot]));
			createConnector(static_cast<ReceiveFromDir>(prev[rot]));
		break;
	}
	double angles[] = { 0, 180, -90, 90 };
	render_dir = angles[rot];
	rotation = rot;
	version = v;
}
