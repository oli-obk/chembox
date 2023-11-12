#ifndef CHEMBOX_EFFECTS_HPP
#define CHEMBOX_EFFECTS_HPP

#include <Gosu/Fwd.hpp>
#include "ParticleEmitter.hpp"
#include <string>
#include <map>
#include <memory>

class Effects
{
private:
    Gosu::Graphics& graphics;
    std::map<std::wstring, std::unique_ptr<ParticleEmitter> > emitters;
    ParticleEmitter& emitter(std::wstring imagename);
public:
    Effects(Gosu::Graphics& g);
    void emit(std::wstring imagename, Particle p);
    void update();
    void draw();
    size_t getTotalParticleCount() const;
};

#endif
