#include "pipe.hpp"
#include <sstream>
#include <Gosu/Font.hpp>
#include <Gosu/Text.hpp>
#include "defines.hpp"
#include <Gosu/Math.hpp>

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
    particles_to_render.fill(0);
    particles_to_render_interpolated.fill(0);
    if (!m_pFont) {
		m_pFont.reset(new Gosu::Font(g, Gosu::defaultFontName(), 10));
		s_pFont = m_pFont;
    }
}

Pipe::Pipe(const Pipe& rhs)
:RotatableVersionedMachine(rhs)
,m_pFont(rhs.m_pFont)
{
    particles_to_render.fill(0);
    particles_to_render_interpolated.fill(0);
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
        auto parts = con->pop();
        
        particles_to_render[int(dir)] -= parts.count() + flowing_particles[int(dir)].count();
        
        if (getConnector(flip(dir))) {
            auto split = parts.split<2>();
            size_t v[] = {0, 1, 2, 3, 4};
            std::shuffle(std::begin(v), v+2, engine);
            flowing_particles[int(flip(dir))] += std::move(split[v[0]]);
            particles += std::move(split[v[1]]);
        } else if(getConnector(dir+1) && getConnector(dir-1)) {
            auto split = parts.split<3>();
            size_t v[] = {0, 1, 2, 3, 4};
            std::shuffle(std::begin(v), v+3, engine);
            flowing_particles[int(dir+1)] += std::move(split[v[0]]);
            flowing_particles[int(dir-1)] += std::move(split[v[1]]);
            particles += std::move(split[v[2]]);
        } else if(getConnector(dir+1)) {
            auto split = parts.split<2>();
            size_t v[] = {0, 1, 2, 3, 4};
            std::shuffle(std::begin(v), v+2, engine);
            flowing_particles[int(dir+1)] += std::move(split[v[0]]);
            particles += std::move(split[v[1]]);
        } else if(getConnector(dir-1)) {
            auto split = parts.split<2>();
            size_t v[] = {0, 1, 2, 3, 4};
            std::shuffle(std::begin(v), v+2, engine);
            flowing_particles[int(dir-1)] += std::move(split[v[0]]);
            particles += std::move(split[v[1]]);
        } else {
            particles += std::move(parts);
        }
        auto& a = particles_to_render_interpolated[int(dir)];
        a = Gosu::interpolate(a, particles_to_render[int(dir)], 0.01);
    }
}

void Pipe::send()
{
    // count connections
    size_t connections = 1;
	for (ReceiveFromDir dir:{ReceiveFromDir::Up, ReceiveFromDir::Down, ReceiveFromDir::Left, ReceiveFromDir::Right}) {
        auto con = getConnector(dir);
        if (con) connections++;
    }
    // prepare sending buffer
    auto distr = particles.split(connections);

    size_t v[] = {0, 1, 2, 3, 4};
    std::shuffle(std::begin(v), v+connections, engine);
    // send stuff from buffer
    size_t i = 0;
	for (ReceiveFromDir dir:{ReceiveFromDir::Up, ReceiveFromDir::Down, ReceiveFromDir::Left, ReceiveFromDir::Right}) {
        auto con = getConnector(dir);
        if (!con) {
            particles_to_render[int(dir)] = 0;
            continue;
        }
        auto& parts = distr[v[i++]];
        particles_to_render[int(dir)] = parts.count() + flowing_particles[int(dir)].count();
        con->push(std::move(parts));
        con->push(std::move(flowing_particles[int(dir)]));
    }
    particles = std::move(distr[v[i]]);
}

void Pipe::draw(double x, double y)
{
    RotatableVersionedMachine::draw(x, y);
    size_t count = particles.count();
    
    if (count != 0) {
        std::wstringstream wss;
        wss << count;
        m_pFont->drawRel(wss.str(), x + 0.5, y + 0.5, RenderLayer::Particles+1, 0.5, 0.4, 0.05, 0.05, Gosu::Color::RED);
    }
    
    for (ReceiveFromDir dir:{ReceiveFromDir::Up, ReceiveFromDir::Down, ReceiveFromDir::Left, ReceiveFromDir::Right}) {
        const auto a = particles_to_render_interpolated[int(dir)];
        // epsilon = 0.1, only draw outgoing particles
        if (a <= 0.1) continue;
        // sparse particles
        if (Gosu::random(0, 100) > 10) continue;
        Particle p;
        p.x = x + 0.5 + Gosu::random(-0.1, 0.1)*getYDir(dir);
        p.y = y + 0.5 + Gosu::random(-0.1, 0.1)*getXDir(dir);
        double vel = 0.05;
        p.velocity_x = vel*getXDir(dir);
        p.velocity_y = vel*getYDir(dir);
        p.time_to_live = 1.0/vel;
        p.center_x = 0.5;
        p.center_y = 0.5;
        p.scale = log(a+1);
        p.color.alpha = 1.0;
        p.color.red = 0.0;
        p.color.blue = 1.0;
        p.color.green = 0.5;
        p.friction = 0.0;
        p.angle = 0.0;
        p.angular_velocity = 0.0;
        p.zoom = 0.0;
        p.fade = 0.2;
        effects().emit(L"particle_gas.png", p);
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