#include "end_pipe.hpp"
#include "defines.hpp"

EndPipe::EndPipe(Gosu::Graphics& g, int dir, size_t version)
:RotatableVersionedMachine(g, dir, version, L"end_pipe")
{
}

EndPipe::~EndPipe()
{
}

void EndPipe::update()
{
	for (auto dir:{ReceiveFromDir::Up, ReceiveFromDir::Down, ReceiveFromDir::Left, ReceiveFromDir::Right}) {
		auto con = getConnector(dir);
		if (!con) continue;
		auto parts = con->pop();
		con->push(parts);
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
