#ifndef PUMP_HPP
#define PUMP_HPP

#include "machine.hpp" // Base class: Machine
#include "Particle.hpp"

class Pump : public Machine
{
    static std::weak_ptr<Gosu::Image> s_pImagePump, s_pImageSpinner;
    std::shared_ptr<Gosu::Image> m_pImagePump, m_pImageSpinner;
    double rotation;
    ParticleMap particles;
public:
    Pump(Gosu::Graphics& g);
    Pump(char, Gosu::Graphics& g):Pump(g) {}
    Pump(const Pump& rhs);
    virtual ~Pump();

public:
	virtual void Initialize(optional<Machine&> up, optional<Machine&> down, optional<Machine&> left, optional<Machine&> right);
    virtual std::unique_ptr<Machine> clone() { return std::unique_ptr<Machine>(new Pump(*this)); }
    virtual void draw(double x, double y);
    virtual void receive();
    virtual void send();
    virtual char serialize();
    static const std::string deserializes() { return "P"; }
};

#endif // PUMP_HPP
