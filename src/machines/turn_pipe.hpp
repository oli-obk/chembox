#ifndef TURNPIPE_HPP
#define TURNPIPE_HPP

#include "rotatable_machine.hpp" // Base class: RotatableMachine
#include "imagestore.hpp"

class TurnPipe : public RotatableMachine, private ImageStore<TurnPipe>
{
    ParticleMap particles;
    ParticleMap up, right;
public:
    TurnPipe(const TurnPipe&);
    TurnPipe(Gosu::Graphics& g, ReceiveFromDir dir);
    TurnPipe(char c, Gosu::Graphics& g);
    void send();
    void receive();
    void draw(double x, double y);
    ~TurnPipe();
    std::unique_ptr<Machine> clone() const;
    char serialize() const;
    static std::string deserializes() { return "/\\7L"; }

};

#endif // STRAIGHTPIPE_HPP
