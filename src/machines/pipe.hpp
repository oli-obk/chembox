#ifndef PIPE_HPP
#define PIPE_HPP

#include "rotatable_versioned_machine.hpp" // Base class: RotatableVersionedMachine
#include "Particle.hpp"

class Pipe : public RotatableVersionedMachine
{
private:
	Pipe(const Pipe& rhs);
	std::shared_ptr<Gosu::Font> m_pFont;
	static std::weak_ptr<Gosu::Font> s_pFont;
    static std::mt19937 engine; // Mersenne twister MT19937
public:
	ParticleMap particles;
	Pipe(Gosu::Graphics& g, ReceiveFromDir dir, size_t version = 0);
	virtual ~Pipe();

public:
	virtual void send();
	virtual void receive();
    virtual void draw();
	virtual std::unique_ptr<Machine> clone() { return std::unique_ptr<Pipe>(new Pipe(*this)); }
	size_t numActions() const;
	void Action(size_t id);
    char serialize();
    static constexpr const char* deserializes() { return R"(L7/\-|+><v^)"; }
};

#endif // PIPE_HPP
