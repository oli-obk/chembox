#include "defines.hpp"
#include "rotatable_versioned_machine.hpp"
#include <iostream>
#include <Gosu/Image.hpp>

std::map<const wchar_t*, std::array<std::weak_ptr<Gosu::Image>, 6>> RotatableVersionedMachine::s_pImage;

RotatableVersionedMachine::RotatableVersionedMachine(Gosu::Graphics& g, ReceiveFromDir rot, size_t version, const wchar_t* basename)
:RotatableMachine(rot)
{
	for (size_t i = 0; i < s_pImage[basename].size(); i++) {
		m_pImage[i] = s_pImage[basename][i].lock();
	}
	if (!m_pImage[0]) {
		assert(!m_pImage[1]);
		assert(!m_pImage[2]);
		assert(!m_pImage[3]);
		assert(!m_pImage[4]);
		assert(!m_pImage[5]);
		m_pImage[0].reset(new Gosu::Image(g,std::wstring(basename) + L".png", true));
		m_pImage[1].reset(new Gosu::Image(g,std::wstring(basename) + L"2.png", true));
		m_pImage[2].reset(new Gosu::Image(g,std::wstring(basename) + L"2opposite.png", true));
		m_pImage[3].reset(new Gosu::Image(g,std::wstring(basename) + L"3.png", true));
		m_pImage[4].reset(new Gosu::Image(g,std::wstring(basename) + L"4.png", true));
		m_pImage[5].reset(new Gosu::Image(g,std::wstring(basename) + L"0.png", true));

		for (size_t i = 0; i < s_pImage[basename].size(); i++) {
			s_pImage[basename][i] = m_pImage[i];
		}
	}
    assert(m_pImage[0]);
    assert(m_pImage[1]);
    assert(m_pImage[2]);
    assert(m_pImage[3]);
    assert(m_pImage[4]);
    assert(m_pImage[5]);
    set_version(version);
}

void RotatableVersionedMachine::set_version(size_t v)
{
	v %= m_pImage.size();
	destroyConnectors();
	switch (v) {
		case 5:
        break;
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

size_t RotatableVersionedMachine::get_version() const
{
    return version;
}

void RotatableVersionedMachine::draw(double x, double y, double z, double w, double h)
{
	double angles[] = { 0, 90, 180, -90 };
	m_pImage[get_version()]->drawRot(
                                        x+0.5*w, y+0.5*h, z,
                                        angles[static_cast<int>(get_rotation())],
                                        0.5, 0.5,
                                        w/double(m_pImage[get_version()]->width()),
                                        h/double(m_pImage[get_version()]->height())
                                        );
}
