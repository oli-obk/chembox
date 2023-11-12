#ifndef TPIPE_HPP
#define TPIPE_HPP

#include "rotatable_machine.hpp" // Base class: RotatableMachine
#include "imagestore.hpp"

class TPipe
:public ClonableMachine<TPipe, RotatableMachine>
,private ImageStore<TPipe>
{
    ParticleMap particles;
    ParticleMap up, left, right;
    static std::mt19937 engine; // Mersenne twister MT19937
	std::shared_ptr<Gosu::Font> m_pFont;
	static std::weak_ptr<Gosu::Font> s_pFont;
public:
    TPipe(const TPipe&);
    TPipe(Gosu::Graphics& g, ReceiveFromDir dir);
    TPipe(char c, Gosu::Graphics& g);
    void send();
    void receive();
    virtual void draw(double x, double y, double z, double w, double h);
    ~TPipe();
    char serialize() const;
    static std::string deserializes() { return "^<>v"; }

};

#endif // STRAIGHTPIPE_HPP
