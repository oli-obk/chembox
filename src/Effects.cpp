#include "Effects.hpp"
#include "defines.hpp"

Effects::Effects(Gosu::Graphics& g)
:graphics(g)
{
    transform_x = 0;
    transform_y = 0;
    scale_x = 1.0;
    scale_y = 1.0;
}

ParticleEmitter& Effects::emitter(std::wstring imagename)
{
    auto it = emitters.find(imagename);
    if (it != emitters.end()) {
        return *(it -> second);
    }
    emitters[imagename] = std::unique_ptr<ParticleEmitter>(new ParticleEmitter(graphics, imagename, RenderLayer::Particles, 50000));
    return *emitters[imagename];
}

void Effects::draw()
{
    for (auto& it:emitters) {
        it.second->draw();
    }
}

void Effects::update()
{
    for (auto& it:emitters) {
        it.second->update();
    }
}

size_t Effects::getTotalParticleCount() const
{
    size_t sum = 0;
    for (auto& it:emitters) {
        sum += it.second->getCount();
    }
    return sum;
}

void Effects::emit(std::wstring imagename, Particle p)
{
    p.x *= scale_x;
    p.y *= scale_y;
    p.x += transform_x;
    p.y += transform_y;
    p.velocity_x *= scale_x;
    p.velocity_y *= scale_y;
    emitter(imagename).emit(p);
}

void Effects::setScale(double x, double y)
{
    scale_x = x;
    scale_y = y;
}

void Effects::setTransform(double x, double y)
{
    transform_x = x;
    transform_y = y;
}
