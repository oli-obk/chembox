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
            particles.add(ParticleState::Gas, ParticleType::Hydrogen, 10);
        break;
		default:
		break;
	}
}

std::weak_ptr<Gosu::Font> Pipe::s_pFont;
std::mt19937 Pipe::engine;

Pipe::Pipe(Gosu::Graphics& g)
:ImageStore(g, L"pipe4.png", true)
,PipeParticles(g)
,m_pFont(s_pFont.lock())
{
    if (!m_pFont) {
		m_pFont.reset(new Gosu::Font(g, Gosu::defaultFontName(), 20));
		s_pFont = m_pFont;
    }
    createConnector(ReceiveFromDir::Left);
    createConnector(ReceiveFromDir::Up);
    createConnector(ReceiveFromDir::Right);
    createConnector(ReceiveFromDir::Down);
}

Pipe::Pipe(const Pipe& rhs)
:ClonableMachine(rhs)
,ImageStore(rhs)
,PipeParticles(rhs)
,m_pFont(rhs.m_pFont)
{
}

size_t Pipe::numActions() const
{
    return 1;
}

void Pipe::receive()
{
    // receive incoming
	for (ReceiveFromDir dir:{ReceiveFromDir::Up, ReceiveFromDir::Down, ReceiveFromDir::Left, ReceiveFromDir::Right}) {
        auto con = getConnector(dir);
        if (!con) continue;
        auto parts = con->pop();

        PipeParticles::at(dir) -= parts.count();
        PipeParticles::update(dir);

        flowing_particles[int(dir)] = std::move(parts);
    }

	ParticleMap rl;
	intersect(flowing_particles[int(ReceiveFromDir::Up)],
                flowing_particles[int(ReceiveFromDir::Down)],
                rl);
    std::swap(  flowing_particles[int(ReceiveFromDir::Up)],
                flowing_particles[int(ReceiveFromDir::Down)]
                );

    ParticleMap ud;
	intersect(flowing_particles[int(ReceiveFromDir::Right)],
                flowing_particles[int(ReceiveFromDir::Left)],
                ud);
	std::swap(  flowing_particles[int(ReceiveFromDir::Right)],
                flowing_particles[int(ReceiveFromDir::Left)]
                );

    intersect(rl, ud, particles);
    auto rl_split = rl.split<2>();
    flowing_particles[int(ReceiveFromDir::Right)] += std::move(rl_split[0]);
    flowing_particles[int(ReceiveFromDir::Left)] += std::move(rl_split[1]);
    auto ud_split = ud.split<2>();
    flowing_particles[int(ReceiveFromDir::Up)] += std::move(ud_split[0]);
    flowing_particles[int(ReceiveFromDir::Down)] += std::move(ud_split[1]);
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
            PipeParticles::at(dir) = 0;
            continue;
        }
        auto& parts = distr[v[i++]];
        PipeParticles::at(dir) = parts.count() + flowing_particles[int(dir)].count();
        con->push(std::move(parts));
        con->push(std::move(flowing_particles[int(dir)]));
    }
    particles = std::move(distr[v[i]]);
}

void Pipe::draw(double x, double y, double z, double w, double h)
{

    ImageStore::draw(x, y, z, w, h);

    size_t count = particles.count()
        + flowing_particles[0].count()
        + flowing_particles[1].count()
        + flowing_particles[2].count()
        + flowing_particles[3].count()
        ;

    if (count != 0) {
        std::wstringstream wss;
        wss << count;
        m_pFont->drawRel(wss.str(), x + 0.5*w, y + 0.5*h, RenderLayer::Particles+1, 0.5, 0.4, 1.0, 1.0, Gosu::Color::RED);
    }

    PipeParticles::draw(x, y, z, w, h);
}

char Pipe::serialize() const
{
    return '+';
}

Pipe::Pipe(char, Gosu::Graphics& g)
:Pipe(g)
{
}

