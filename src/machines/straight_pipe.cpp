#include "straight_pipe.hpp"
#include "defines.hpp"

std::weak_ptr<Gosu::Image> StraightPipe::s_pImg;

StraightPipe::StraightPipe(Gosu::Graphics& g, ReceiveFromDir dir)
:RotatableMachine(g, dir)
,m_pImg(s_pImg.lock())
{
    createConnector(ReceiveFromDir::Up);
    createConnector(ReceiveFromDir::Down);
    if (!m_pImg) {
        m_pImg.reset(new Gosu::Image(g, L"straight_pipe.png"));
        s_pImg = m_pImg;
    }
}

StraightPipe::~StraightPipe()
{
}

void StraightPipe::draw(double x, double y)
{

    auto col = Gosu::Color::WHITE;
    switch (get_rotation()) {
        case ReceiveFromDir::Up:
        case ReceiveFromDir::Down:
            m_pImg->getData().draw(x+1, y+1, col,
                                   x, y+1, col,
                                   x, y, col,
                                   x+1, y, col,
                                   RenderLayer::Machines, Gosu::amDefault);
            break;
        case ReceiveFromDir::Left:
        case ReceiveFromDir::Right:
            m_pImg->getData().draw(x, y+1, col,
                                   x, y, col,
                                   x+1, y, col,
                                   x+1, y+1, col,
                                   RenderLayer::Machines, Gosu::amDefault);
            break;
    }
}

void StraightPipe::receive()
{
    down = std::move(getConnector(ReceiveFromDir::Up)->pop());
    up = std::move(getConnector(ReceiveFromDir::Down)->pop());
    intersect(up, down, particles);
}

void StraightPipe::send()
{
    auto split = particles.split<3>();
    auto up_con = getConnector(ReceiveFromDir::Up);
    up_con->push(std::move(up));
    up_con->push(std::move(split[0]));
    auto down_con = getConnector(ReceiveFromDir::Down);
    down_con->push(std::move(down));
    down_con->push(std::move(split[1]));
    particles = std::move(split[2]);
}

std::unique_ptr<Machine> StraightPipe::clone() const
{
    return std::unique_ptr<Machine>(new StraightPipe(*this));
}

char StraightPipe::serialize() const
{
    switch (get_rotation())
    {
        case ReceiveFromDir::Left:
        case ReceiveFromDir::Right:
            return '-';
        case ReceiveFromDir::Up:
        case ReceiveFromDir::Down:
            return '|';
    }
    assert(false);
}

StraightPipe::StraightPipe(const StraightPipe& rhs)
:RotatableMachine(rhs)
,m_pImg(rhs.m_pImg)
{
}

StraightPipe::StraightPipe(char c, Gosu::Graphics& g)
:StraightPipe(g, (c=='|') ? ReceiveFromDir::Up : ReceiveFromDir::Left)
{
}
