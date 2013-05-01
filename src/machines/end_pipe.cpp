#include "end_pipe.hpp"
#include "defines.hpp"

EndPipe::EndPipe(Gosu::Graphics& g, int dir, size_t version)
:RotatableVersionedMachine(g, dir, version, L"end_pipe")
{
}

EndPipe::~EndPipe()
{
}

void EndPipe::send()
{
	for (auto dir:{ReceiveFromDir::Up, ReceiveFromDir::Down, ReceiveFromDir::Left, ReceiveFromDir::Right}) {
		auto con = getConnector(dir);
		if (!con) continue;
		con->push(particles[static_cast<size_t>(dir)]);
        particles[static_cast<size_t>(dir)].clear();
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
:RotatableVersionedMachine(rhs)
{
}

void EndPipe::Action(size_t id)
{
	switch (id) {
		case 0:
            set_rotation(get_rotation()+1);
		break;
		case 1:
			set_version(get_version()+1);
		break;
		default:
		break;
	}
}

size_t EndPipe::numActions() const
{
	return 2;
}
