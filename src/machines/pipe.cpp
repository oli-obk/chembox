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
,SharedEffects(g)
,m_pFont(s_pFont.lock())
{
    particles_to_render.fill(0);
    particles_to_render_interpolated.fill(0);
    if (!m_pFont) {
		m_pFont.reset(new Gosu::Font(g, Gosu::defaultFontName(), 10));
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
,SharedEffects(rhs)
,m_pFont(rhs.m_pFont)
{
    particles_to_render.fill(0);
    particles_to_render_interpolated.fill(0);
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

    ImageStore::draw(x, y, RenderLayer::Machines);

    size_t count = particles.count()
        + flowing_particles[0].count()
        + flowing_particles[1].count()
        + flowing_particles[2].count()
        + flowing_particles[3].count()
        ;
    
    if (count != 0) {
        std::wstringstream wss;
        wss << count;
        m_pFont->drawRel(wss.str(), x + 0.5, y + 0.5, RenderLayer::Particles+1, 0.5, 0.4, 0.05, 0.05, Gosu::Color::RED);
    }

    struct {double x, y; ReceiveFromDir dir;} items[] = {
        {-1, 0, ReceiveFromDir::Left},
        {1,  0, ReceiveFromDir::Right},
        {0, -1, ReceiveFromDir::Up},
        {0,  1, ReceiveFromDir::Down},
        };

    for (auto item : items) {
        const auto a = particles_to_render_interpolated[int(item.dir)];
        // epsilon = 0.1, only draw outgoing particles
        if (a <= 0.1) continue;
        // sparse particles
        if (Gosu::random(0, 100) > 10) continue;
        Particle p;
        p.x = x + 0.5 + Gosu::random(-0.1, 0.1)*item.y;
        p.y = y + 0.5 + Gosu::random(-0.1, 0.1)*item.x;
        double vel = 0.05;
        p.velocity_x = vel*item.x;
        p.velocity_y = vel*item.y;
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

char Pipe::serialize() const
{
    return '+';
}

Pipe::Pipe(char, Gosu::Graphics& g)
:Pipe(g)
{
}

