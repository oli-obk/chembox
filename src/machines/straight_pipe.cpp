#include "straight_pipe.hpp"
#include "defines.hpp"

StraightPipe::StraightPipe(Gosu::Graphics& g, ReceiveFromDir dir)
:ClonableMachine(dir)
,ImageStore(g, L"straight_pipe.png", true)
{
    createConnector(ReceiveFromDir::Up);
    createConnector(ReceiveFromDir::Down);
}

StraightPipe::~StraightPipe()
{
}

void StraightPipe::draw(double x, double y, double z, double w, double h)
{

    auto col = Gosu::Color::WHITE;
    switch (get_rotation()) {
        case ReceiveFromDir::Up:
        case ReceiveFromDir::Down:
            Image().getData().draw(x+w, y+h, col,
                                   x, y+h, col,
                                   x, y, col,
                                   x+w, y, col,
                                   z, Gosu::amDefault);
            break;
        case ReceiveFromDir::Left:
        case ReceiveFromDir::Right:
            Image().getData().draw(x, y+h, col,
                                   x, y, col,
                                   x+w, y, col,
                                   x+w, y+h, col,
                                   z, Gosu::amDefault);
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
:ClonableMachine(rhs)
,ImageStore(rhs)
{
}

StraightPipe::StraightPipe(char c, Gosu::Graphics& g)
:StraightPipe(g, (c=='|') ? ReceiveFromDir::Up : ReceiveFromDir::Left)
{
}
