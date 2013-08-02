#ifndef STRAIGHTPIPE_HPP
#define STRAIGHTPIPE_HPP

#include "rotatable_machine.hpp" // Base class: RotatableMachine

class StraightPipe : public RotatableMachine
{
    static std::weak_ptr<Gosu::Image> s_pImg;
    std::shared_ptr<Gosu::Image> m_pImg;
    ParticleMap particles;
    ParticleMap up, down;
public:
    StraightPipe(const StraightPipe&);
    StraightPipe(Gosu::Graphics& g, ReceiveFromDir dir);
    StraightPipe(char c, Gosu::Graphics& g);
    void send();
    void receive();
    void draw(double x, double y);
    ~StraightPipe();
    std::unique_ptr<Machine> clone() const;
    char serialize() const;
    static std::string deserializes() { return "-|"; }

};

#endif // STRAIGHTPIPE_HPP
