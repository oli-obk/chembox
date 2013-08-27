#include "turn_pipe.hpp"
#include "defines.hpp"

std::weak_ptr<Gosu::Image> TurnPipe::s_pImg;

TurnPipe::TurnPipe(Gosu::Graphics& g, ReceiveFromDir dir)
:RotatableMachine(g, dir)
,m_pImg(s_pImg.lock())
{
    createConnector(ReceiveFromDir::Up);
    createConnector(ReceiveFromDir::Right);
    if (!m_pImg) {
        m_pImg.reset(new Gosu::Image(g, L"turn_pipe.png"));
        s_pImg = m_pImg;
    }
}

TurnPipe::~TurnPipe()
{
}

void TurnPipe::draw(double x, double y)
{
    double angles[] = { 0, 90, 180, -90 };
    m_pImg->drawRot(x+0.5,
                    y+0.5,
                    RenderLayer::Machines,
                    angles[static_cast<int>(get_rotation())],
                    0.5,
                    0.5,
                    1.0/double(m_pImg->width()),
                    1.0/double(m_pImg->height())
                    );
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

std::unique_ptr<Machine> TurnPipe::clone() const
{
    return std::unique_ptr<Machine>(new TurnPipe(*this));
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
:RotatableMachine(rhs)
,m_pImg(rhs.m_pImg)
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