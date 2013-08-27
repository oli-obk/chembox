#include "t_pipe.hpp"
#include "defines.hpp"

std::weak_ptr<Gosu::Image> TPipe::s_pImg;
std::mt19937 TPipe::engine;

TPipe::TPipe(Gosu::Graphics& g, ReceiveFromDir dir)
:RotatableMachine(g, dir)
,m_pImg(s_pImg.lock())
{
    createConnector(ReceiveFromDir::Left);
    createConnector(ReceiveFromDir::Up);
    createConnector(ReceiveFromDir::Right);
    if (!m_pImg) {
        m_pImg.reset(new Gosu::Image(g, L"t_pipe.png", true));
        s_pImg = m_pImg;
    }
}

TPipe::~TPipe()
{
}

void TPipe::draw(double x, double y)
{
    auto col = Gosu::Color::WHITE;
    switch (get_rotation()) {
        case ReceiveFromDir::Left:
            m_pImg->getData().draw(x+1, y+1, col,
                                   x, y+1, col,
                                   x, y, col,
                                   x+1, y, col,
                                   RenderLayer::Machines, Gosu::amDefault);
            break;
        case ReceiveFromDir::Up:
            m_pImg->getData().draw(x, y+1, col,
                                   x, y, col,
                                   x+1, y, col,
                                   x+1, y+1, col,
                                   RenderLayer::Machines, Gosu::amDefault);
            break;
        case ReceiveFromDir::Right:
            m_pImg->getData().draw(x, y, col,
                                   x+1, y, col,
                                   x+1, y+1, col,
                                   x, y+1, col,
                                   RenderLayer::Machines, Gosu::amDefault);
            break;
        case ReceiveFromDir::Down:
            m_pImg->getData().draw(x+1, y, col,
                                   x+1, y+1, col,
                                   x, y+1, col,
                                   x, y, col,
                                   RenderLayer::Machines, Gosu::amDefault);
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

std::unique_ptr<Machine> TPipe::clone() const
{
    return std::unique_ptr<Machine>(new TPipe(*this));
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
:RotatableMachine(rhs)
,m_pImg(rhs.m_pImg)
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
