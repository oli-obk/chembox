#include "pipeparticles.hpp"
#include <Gosu/Math.hpp>

void PipeParticles::draw(double x, double y, double z, double w, double h)
{
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
        if (Gosu::random(0, 100) > 1) continue;
        Particle p;
        p.x = x + (0.5 + Gosu::random(-0.1, 0.1)*item.y)*h;
        p.y = y + (0.5 + Gosu::random(-0.1, 0.1)*item.x)*w;
        double vel = 0.5;
        p.velocity_x = vel*item.x;
        p.velocity_y = vel*item.y;
        p.time_to_live = w/vel;
        p.center_x = 0.5;
        p.center_y = 0.5;
        p.scale = std::min(log(a+1), 0.3);
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

void PipeParticles::update(ReceiveFromDir dir)
{
    auto& a = particles_to_render_interpolated[int(dir)];
    a = Gosu::interpolate(a, particles_to_render[int(dir)], 0.01);
}
