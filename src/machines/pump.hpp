#ifndef PUMP_HPP
#define PUMP_HPP

#include "machine.hpp" // Base class: Machine
#include "Particle.hpp"
#include "imagestore.hpp"

class PumpSpinner;

class Pump : public ClonableMachine<Pump>, private ImageStore<Pump>, private ImageStore<PumpSpinner>
{
    double rotation;
    ParticleMap particles;
public:
    Pump(Gosu::Graphics& g);
    Pump(char, Gosu::Graphics& g):Pump(g) {}
    Pump(const Pump& rhs);
    virtual ~Pump();

public:
    virtual void draw(double x, double y);
    virtual void receive();
    virtual void send();
    virtual char serialize() const;
    static const std::string deserializes() { return "P"; }
};

#endif // PUMP_HPP
