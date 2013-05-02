#include "pipe.hpp"
#include <sstream>
#include <Gosu/Font.hpp>
#include <Gosu/Text.hpp>
#include "defines.hpp"

Pipe::~Pipe()
{
}

void Pipe::Action(size_t id)
{
	switch (id) {
		case 0:
            set_rotation(get_rotation() + 1);
		break;
		case 1:
			set_version(get_version()+1);
		break;
        case 2:
            particles.add(ParticleState::Gas, ParticleType::Hydrogen, 10);
        break;
		default:
		break;
	}
}

std::weak_ptr<Gosu::Font> Pipe::s_pFont;
std::mt19937 Pipe::engine;

Pipe::Pipe(Gosu::Graphics& g, ReceiveFromDir dir, size_t version)
:RotatableVersionedMachine(g, dir, version, L"pipe")
,m_pFont(s_pFont.lock())
{
    if (!m_pFont) {
		m_pFont.reset(new Gosu::Font(g, Gosu::defaultFontName(), 10));
		s_pFont = m_pFont;
    }
}

Pipe::Pipe(const Pipe& rhs)
:RotatableVersionedMachine(rhs)
,m_pFont(rhs.m_pFont)
,particles(particles)
{
}

size_t Pipe::numActions() const
{
    return 3;
}

void Pipe::receive()
{
    // receive incoming
	for (ReceiveFromDir dir:{ReceiveFromDir::Up, ReceiveFromDir::Down, ReceiveFromDir::Left, ReceiveFromDir::Right}) {
        auto con = getConnector(dir);
        if (!con) continue;
		particles += con->pop();
	}
}

void Pipe::send()
{
    // count connections
    size_t connections = 0;
	for (ReceiveFromDir dir:{ReceiveFromDir::Up, ReceiveFromDir::Down, ReceiveFromDir::Left, ReceiveFromDir::Right}) {
        auto con = getConnector(dir);
        if (con) connections++;
    }
    // prepare sending buffer
    auto distr = particles.split(connections);
    particles.clear();

    size_t v[] = {0, 1, 2, 3};
    std::shuffle(std::begin(v), v+connections, engine);
    // send stuff from buffer
    size_t i = 0;
	for (ReceiveFromDir dir:{ReceiveFromDir::Up, ReceiveFromDir::Down, ReceiveFromDir::Left, ReceiveFromDir::Right}) {
        auto con = getConnector(dir);
        if (!con) continue;
		con->push(distr[v[i++]]);
	}
	particle_engine.update();
}

void Pipe::draw()
{
    RotatableVersionedMachine::draw();
	size_t count = particles.count(ParticleState::Gas, ParticleType::Hydrogen);
	if (count != 0) {
		std::wstringstream wss;
		wss << count;
		m_pFont->drawRel(wss.str(), 0.5, 0.5, RenderLayer::Machines+1, 0.5, 0.4, 0.05, 0.05, Gosu::Color::RED);
	}
}

char Pipe::serialize()
{
    switch (get_version()) {
        case 0:
            return 'X'; // this is more of an end pipe tbh
        case 1:
            switch (get_rotation()) {
                case ReceiveFromDir::Up:
                    return 'L';
                break;
                case ReceiveFromDir::Right:
                    return '/';
                break;
                case ReceiveFromDir::Down:
                    return '7';
                break;
                case ReceiveFromDir::Left:
                    return '\\';
                break;
            }
        case 2:
            if (get_rotation() == ReceiveFromDir::Up || get_rotation() == ReceiveFromDir::Down) {
                return '|';
            } else {
                return '-';
            }
        case 3:
            switch (get_rotation()) {
                case ReceiveFromDir::Up:
                    return '>';
                break;
                case ReceiveFromDir::Right:
                    return 'v';
                break;
                case ReceiveFromDir::Down:
                    return '<';
                break;
                case ReceiveFromDir::Left:
                    return '^';
                break;
            }
        case 4:
            return '+';
    }
    return 0;
}

ReceiveFromDir get_serialization_rotation(char c)
{
    switch (c) {
        case '+':
        case '|':
        case '>':
        case 'L':
            return ReceiveFromDir::Up;
        case '-':
        case 'v':
        case '/':
            return ReceiveFromDir::Right;
        case '<':
        case '7':
            return ReceiveFromDir::Down;
        case '^':
        case '\\':
            return ReceiveFromDir::Left;
        default:
        throw std::runtime_error("Pipe got an invalid char to deserialize rotation from");
    }
}

size_t get_serialization_version(char c)
{
    switch (c) {
        case 'L':
        case '/':
        case '\\':
        case '7':
            return 1;
        case '|':
        case '-':
            return 2;
        case '<':
        case 'v':
        case '^':
        case '>':
            return 3;
        case '+':
            return 4;
        default:
        throw std::runtime_error("Pipe got an invalid char to deserialize version from");
    }
}

Pipe::Pipe(char c, Gosu::Graphics& g)
:Pipe(g, get_serialization_rotation(c), get_serialization_version(c))
{
}
