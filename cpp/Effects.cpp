#include "Effects.hpp"
#include "defines.hpp"

Effects::Effects(Gosu::Graphics& g)
:graphics(g)
{
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
    emitter(imagename).emit(p);
}
