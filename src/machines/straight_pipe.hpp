#ifndef STRAIGHTPIPE_HPP
#define STRAIGHTPIPE_HPP

#include "rotatable_machine.hpp" // Base class: RotatableMachine
#include "imagestore.hpp"

class StraightPipe
:public ClonableMachine<StraightPipe ,RotatableMachine>
,private ImageStore<StraightPipe>
{
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
    char serialize() const;
    static std::string deserializes() { return "-|"; }

};

#endif // STRAIGHTPIPE_HPP
