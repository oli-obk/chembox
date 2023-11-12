#ifndef TURNPIPE_HPP
#define TURNPIPE_HPP

#include "rotatable_machine.hpp" // Base class: RotatableMachine
#include "imagestore.hpp"

class TurnPipe
:public ClonableMachine<TurnPipe, RotatableMachine>
,private ImageStore<TurnPipe>
{
    ParticleMap particles;
    ParticleMap up, right;
	std::shared_ptr<Gosu::Font> m_pFont;
	static std::weak_ptr<Gosu::Font> s_pFont;
public:
    TurnPipe(const TurnPipe&);
    TurnPipe(Gosu::Graphics& g, ReceiveFromDir dir);
    TurnPipe(char c, Gosu::Graphics& g);
    void send();
    void receive();
    virtual void draw(double x, double y, double z, double w, double h);
    ~TurnPipe();
    char serialize() const;
    static std::string deserializes() { return "/\\7L"; }

};

#endif // STRAIGHTPIPE_HPP
