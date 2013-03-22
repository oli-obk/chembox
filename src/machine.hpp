#ifndef MACHINE_HPP
#define MACHINE_HPP

enum class ReceiveFromDir
{
	Up = 0, Down = 1, Left = 2, Right = 3
};

#include "Particle.hpp"
#include "optional.hpp"
#include "particle_engine.hpp"
#include <Gosu/Graphics.hpp>

class Connector
{
private:
	ParticleMap particles;
	ParticleEnergy energy;
	optional<Connector&> other;
public:
	void connect(Connector& c) { assert(!other); other.construct(c); }
	void disconnect() { assert(other); other.destruct(); };
	void push(ParticleState state, ParticleType type, unsigned count)
	{
		particles[state][type] += count;
	}
	ParticleMap pop() { return std::move(particles); }
};

class Machine
{
private:
	ParticleMap particles;
	ParticleEnergy energy;
	bool m_destroyed;
	optional<Connector> connectors[4];
protected:
	ParticleEngine particle_engine;
public:
	Connector& createConnector(ReceiveFromDir dir);
	void Destroy() { assert(!m_destroyed); m_destroyed = true; }
	bool isDestroyed() const { return m_destroyed; }
	virtual void draw() = 0;
	virtual bool accepts(ParticleState, ReceiveFromDir) const { return false; }
	virtual void update() = 0;
	void receive(ParticleState, ParticleType, int count, const Connector&);
	void receive(ParticleEnergy);
	void send(ParticleState, ParticleType, int count, Connector&);
	Machine(Gosu::Graphics& g);
	virtual ~Machine();
};

#endif // MACHINE_HPP
