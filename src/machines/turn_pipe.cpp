#include "turn_pipe.hpp"
#include "defines.hpp"

TurnPipe::TurnPipe(Gosu::Graphics& g, ReceiveFromDir dir)
:ClonableMachine(g, dir)
,ImageStore(g, L"turn_pipe.png", true)
{
    createConnector(ReceiveFromDir::Up);
    createConnector(ReceiveFromDir::Right);
}

TurnPipe::~TurnPipe()
{
}

void TurnPipe::draw(double x, double y)
{
    auto col = Gosu::Color::WHITE;
    switch (get_rotation()) {
        case ReceiveFromDir::Down:
            Image().getData().draw(x+1, y+1, col,
                                   x, y+1, col,
                                   x, y, col,
                                   x+1, y, col,
                                   RenderLayer::Machines, Gosu::amDefault);
            break;
        case ReceiveFromDir::Left:
            Image().getData().draw(x, y+1, col,
                                   x, y, col,
                                   x+1, y, col,
                                   x+1, y+1, col,
                                   RenderLayer::Machines, Gosu::amDefault);
            break;
        case ReceiveFromDir::Up:
            Image().getData().draw(x, y, col,
                                   x+1, y, col,
                                   x+1, y+1, col,
                                   x, y+1, col,
                                   RenderLayer::Machines, Gosu::amDefault);
            break;
        case ReceiveFromDir::Right:
            Image().getData().draw(x+1, y, col,
                                   x+1, y+1, col,
                                   x, y+1, col,
                                   x, y, col,
                                   RenderLayer::Machines, Gosu::amDefault);
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
