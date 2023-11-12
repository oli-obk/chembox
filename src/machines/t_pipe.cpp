#include "t_pipe.hpp"
#include "defines.hpp"
#include <sstream>
#include <Gosu/Font.hpp>
#include <Gosu/Text.hpp>

std::weak_ptr<Gosu::Font> TPipe::s_pFont;

std::mt19937 TPipe::engine;

TPipe::TPipe(Gosu::Graphics& g, ReceiveFromDir dir)
:ClonableMachine(dir)
,ImageStore(g, L"t_pipe.png", true)
,m_pFont(s_pFont.lock())
{
    if (!m_pFont) {
		m_pFont.reset(new Gosu::Font(g, Gosu::defaultFontName(), 20));
		s_pFont = m_pFont;
    }
    createConnector(ReceiveFromDir::Left);
    createConnector(ReceiveFromDir::Up);
    createConnector(ReceiveFromDir::Right);
}

TPipe::~TPipe()
{
}

void TPipe::draw(double x, double y, double z, double w, double h)
{
    size_t count = particles.count()
        + up.count()
        + left.count()
        + right.count()
        ;

    if (count != 0) {
        std::wstringstream wss;
        wss << count;
        m_pFont->drawRel(wss.str(), x + 0.5*w, y + 0.5*h, RenderLayer::Particles+1, 0.5, 0.4, 1.0, 1.0, Gosu::Color::RED);
    }
    auto col = Gosu::Color::WHITE;
    switch (get_rotation()) {
        case ReceiveFromDir::Left:
            Image().getData().draw(x+w, y+h, col,
                                   x, y+h, col,
                                   x, y, col,
                                   x+w, y, col,
                                   z, Gosu::amDefault);
            break;
        case ReceiveFromDir::Up:
            Image().getData().draw(x, y+h, col,
                                   x, y, col,
                                   x+w, y, col,
                                   x+w, y+h, col,
                                   z, Gosu::amDefault);
            break;
        case ReceiveFromDir::Right:
            Image().getData().draw(x, y, col,
                                   x+w, y, col,
                                   x+w, y+h, col,
                                   x, y+h, col,
                                   z, Gosu::amDefault);
            break;
        case ReceiveFromDir::Down:
            Image().getData().draw(x+w, y, col,
                                   x+w, y+h, col,
                                   x, y+h, col,
                                   x, y, col,
                                   z, Gosu::amDefault);
            break;
    }
}

void TPipe::receive()
{
    left = std::move(getConnector(ReceiveFromDir::Right)->pop());
    right = std::move(getConnector(ReceiveFromDir::Left)->pop());
    intersect(left, right, up);
    auto from_up = std::move(getConnector(ReceiveFromDir::Up)->pop());
    intersect(up, from_up, particles);
    auto split = from_up.split<2>();
    std::shuffle(std::begin(split), std::end(split), engine);
    left += std::move(split[0]);
    right += std::move(split[1]);
}

void TPipe::send()
{
    auto split = particles.split<4>();

    auto con = getConnector(ReceiveFromDir::Up);
    con->push(std::move(up));
    con->push(std::move(split[0]));

    con = getConnector(ReceiveFromDir::Left);
    con->push(std::move(left));
    con->push(std::move(split[1]));

    con = getConnector(ReceiveFromDir::Right);
    con->push(std::move(right));
    con->push(std::move(split[2]));

    particles = std::move(split[3]);
}

char TPipe::serialize() const
{
    switch (get_rotation())
    {
        case ReceiveFromDir::Left:
            return '<';
        case ReceiveFromDir::Right:
            return '>';
        case ReceiveFromDir::Up:
            return '^';
        case ReceiveFromDir::Down:
            return 'v';
    }
    assert(false);
}

TPipe::TPipe(const TPipe& rhs)
:ClonableMachine(rhs)
,ImageStore(rhs)
,m_pFont(rhs.m_pFont)
{
}

ReceiveFromDir tpipe_char_to_dir(char c)
{
    switch(c) {
        case '<': return ReceiveFromDir::Left;
        case '>': return ReceiveFromDir::Right;
        case '^': return ReceiveFromDir::Up;
        case 'v': return ReceiveFromDir::Down;
    }
    assert(false);
}

TPipe::TPipe(char c, Gosu::Graphics& g)
:TPipe(g, tpipe_char_to_dir(c))
{
}
