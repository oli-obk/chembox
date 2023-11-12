#include "end_pipe.hpp"
#include "defines.hpp"
#include <Gosu/Font.hpp>
#include <Gosu/Text.hpp>

std::weak_ptr<Gosu::Font> EndPipe::s_pFont;

EndPipe::EndPipe(Gosu::Graphics& g, ReceiveFromDir dir, size_t version)
:ClonableMachine(g, dir, version, L"end_pipe")
,m_pFont(s_pFont.lock())
{
    if (!m_pFont) {
		m_pFont.reset(new Gosu::Font(g, Gosu::defaultFontName(), 20));
		s_pFont = m_pFont;
    }
}

EndPipe::~EndPipe()
{
}

void EndPipe::send()
{
	for (auto dir:{ReceiveFromDir::Up, ReceiveFromDir::Down, ReceiveFromDir::Left, ReceiveFromDir::Right}) {
		auto con = getConnector(dir);
		if (!con) continue;
		con->push(std::move(particles[static_cast<size_t>(dir)]));
	}
}

void EndPipe::receive()
{
	for (auto dir:{ReceiveFromDir::Up, ReceiveFromDir::Down, ReceiveFromDir::Left, ReceiveFromDir::Right}) {
		auto con = getConnector(dir);
		if (!con) continue;
		particles[static_cast<size_t>(dir)] = con->pop();
	}
}

EndPipe::EndPipe(const EndPipe& rhs)
:ClonableMachine(rhs)
,m_pFont(rhs.m_pFont)
{
}

EndPipe::EndPipe(char, Gosu::Graphics& g)
:EndPipe(g, ReceiveFromDir::Up, 0)
{
}

void EndPipe::Initialize(optional<Machine&> up_, optional<Machine&> down_, optional<Machine&> left_, optional<Machine&> right_)
{
    int count = 0;
    bool up = up_ && up_->getConnector(ReceiveFromDir::Down);
    bool down = down_ && down_->getConnector(ReceiveFromDir::Up);
    bool left = left_ && left_->getConnector(ReceiveFromDir::Right);
    bool right = right_ && right_->getConnector(ReceiveFromDir::Left);
    if (up) count++;
    if (down) count++;
    if (left) count++;
    if (right) count++;
    set_version(5);
    switch (count) {
        case 0:
            break;
        case 4:
        set_version(4);
        break;
        case 1:
        set_version(0);
        if (up) set_rotation(ReceiveFromDir::Up);
        if (down) set_rotation(ReceiveFromDir::Down);
        if (left) set_rotation(ReceiveFromDir::Left);
        if (right) set_rotation(ReceiveFromDir::Right);
        break;
        case 2:
        if (up && down) {
            set_version(2);
            set_rotation(ReceiveFromDir::Up);
            break;
        }
        if (right && left) {
            set_version(2);
            set_rotation(ReceiveFromDir::Left);
            break;
        }
        set_version(1);
        if (up && right) {
            set_rotation(ReceiveFromDir::Up);
            break;
        }
        if (right && down) {
            set_rotation(ReceiveFromDir::Right);
            break;
        }
        if (down && left) {
            set_rotation(ReceiveFromDir::Down);
            break;
        }
        set_rotation(ReceiveFromDir::Left);
        break;
        case 3:
        set_version(3);
        if (!up) set_rotation(ReceiveFromDir::Right);
        if (!down) set_rotation(ReceiveFromDir::Left);
        if (!left) set_rotation(ReceiveFromDir::Up);
        if (!right) set_rotation(ReceiveFromDir::Down);
        break;
    }
    Machine::Initialize(up_, down_, left_, right_);
}
