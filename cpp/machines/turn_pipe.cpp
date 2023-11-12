#include "turn_pipe.hpp"
#include "defines.hpp"
#include <sstream>
#include <Gosu/Font.hpp>
#include <Gosu/Text.hpp>

std::weak_ptr<Gosu::Font> TurnPipe::s_pFont;

TurnPipe::TurnPipe(Gosu::Graphics& g, ReceiveFromDir dir)
:ClonableMachine(dir)
,ImageStore(g, L"turn_pipe.png", true)
,m_pFont(s_pFont.lock())
{
    if (!m_pFont) {
		m_pFont.reset(new Gosu::Font(g, Gosu::defaultFontName(), 20));
		s_pFont = m_pFont;
    }
    createConnector(ReceiveFromDir::Up);
    createConnector(ReceiveFromDir::Right);
}

TurnPipe::~TurnPipe()
{
}

void TurnPipe::draw(double x, double y, double z, double w, double h)
{

    size_t count = particles.count()
        + up.count()
        + right.count()
        ;

    if (count != 0) {
        std::wstringstream wss;
        wss << count;
        m_pFont->drawRel(wss.str(), x + 0.5*w, y + 0.5*h, RenderLayer::Particles+1, 0.5, 0.4, 1.0, 1.0, Gosu::Color::RED);
    }
    auto col = Gosu::Color::WHITE;
    switch (get_rotation()) {
        case ReceiveFromDir::Down:
            Image().getData().draw(x+w, y+h, col,
                                   x, y+h, col,
                                   x, y, col,
                                   x+w, y, col,
                                   z, Gosu::amDefault);
            break;
        case ReceiveFromDir::Left:
            Image().getData().draw(x, y+h, col,
                                   x, y, col,
                                   x+w, y, col,
                                   x+w, y+h, col,
                                   z, Gosu::amDefault);
            break;
        case ReceiveFromDir::Up:
            Image().getData().draw(x, y, col,
                                   x+w, y, col,
                                   x+w, y+h, col,
                                   x, y+h, col,
                                   z, Gosu::amDefault);
            break;
        case ReceiveFromDir::Right:
            Image().getData().draw(x+w, y, col,
                                   x+w, y+h, col,
                                   x, y+h, col,
                                   x, y, col,
                                   z, Gosu::amDefault);
            break;
    }

}

void TurnPipe::receive()
{
    right = std::move(getConnector(ReceiveFromDir::Up)->pop());
    up = std::move(getConnector(ReceiveFromDir::Right)->pop());
    intersect(up, right, particles);
}

void TurnPipe::send()
{
    auto split = particles.split<3>();
    auto up_con = getConnector(ReceiveFromDir::Up);
    up_con->push(std::move(up));
    up_con->push(std::move(split[0]));
    auto right_con = getConnector(ReceiveFromDir::Right);
    right_con->push(std::move(right));
    right_con->push(std::move(split[1]));
    particles = std::move(split[2]);
}

char TurnPipe::serialize() const
{
    switch (get_rotation())
    {
        case ReceiveFromDir::Left:
            return '\\';
        case ReceiveFromDir::Right:
            return '/';
        case ReceiveFromDir::Up:
            return 'L';
        case ReceiveFromDir::Down:
            return '7';
    }
    assert(false);
}

TurnPipe::TurnPipe(const TurnPipe& rhs)
:ClonableMachine(rhs)
,ImageStore(rhs)
,m_pFont(rhs.m_pFont)
{
}

ReceiveFromDir char_to_dir(char c)
{
    switch(c) {
        case '\\': return ReceiveFromDir::Left;
        case '/': return ReceiveFromDir::Right;
        case 'L': return ReceiveFromDir::Up;
        case '7': return ReceiveFromDir::Down;
    }
    assert(false);
}

TurnPipe::TurnPipe(char c, Gosu::Graphics& g)
:TurnPipe(g, char_to_dir(c))
{
}
