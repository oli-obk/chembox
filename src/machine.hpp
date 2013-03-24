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
	builtin_wrapper<bool, true> hasUnpoppedParticles;
	Connector(const Connector&) = delete;
	Connector& operator=(const Connector&) = delete;
public:
	Connector() = default;
	~Connector()
	{
		if (connected()) {
			disconnect();
		}
	}
	bool connected() const { return other; }
	void connect(Connector& c)
	{
		assert(!other);
		other.construct(c);
		c.other.construct(*this);
	}
	void disconnect()
	{
		assert(other);
		other->other.destruct();
		other.destruct();
	}
	void push(ParticleState state, ParticleType type, unsigned count)
	{
		assert(other);
		assert(!hasUnpoppedParticles);
		hasUnpoppedParticles = true;
		particles.add(state, type, count);
	}
	void push(ParticleMap parts)
	{
		assert(other);
		assert(!hasUnpoppedParticles);
		hasUnpoppedParticles = true;
		particles += parts;
	}
	ParticleMap pop()
	{
		assert(other);
		assert(hasUnpoppedParticles);
		hasUnpoppedParticles = false;
		return std::move(particles);
	}
	void communicate()
	{
		assert(other);
		assert(hasUnpoppedParticles);
		assert(other->hasUnpoppedParticles);
		std::swap(particles, other->particles);
	}
};

class Machine
{
private:
	bool m_destroyed;
	optional<Connector> connectors[4];
	bool m_initialized;
	void ReInitialize() { m_initialized = false; }
protected:
	Machine(const Machine& rhs);
	ParticleEngine particle_engine;
	void createConnector(ReceiveFromDir dir);
	optional<Connector&> getConnector(ReceiveFromDir dir);
	optional<const Connector&> getConnector(ReceiveFromDir dir) const;
	void destroyConnector(ReceiveFromDir dir);
	void destroyConnectors();
public:
	void Initialize(optional<Machine&> up, optional<Machine&> down, optional<Machine&> left, optional<Machine&> right);
	bool isInitialized() const { return m_initialized; }
	void Destroy() { assert(!m_destroyed); m_destroyed = true; }
	bool isDestroyed() const { return m_destroyed; }
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual std::unique_ptr<Machine> clone() = 0;
	virtual void Action(size_t /* action_id */) {};
	virtual size_t numActions() const { return 0; }
	virtual std::string nameAction(size_t /* action_id */) const { return ""; };

	void communicate();
	void insert(ParticleState, ParticleType, int count);
	Machine(Gosu::Graphics& g);
	virtual ~Machine();
};

#endif // MACHINE_HPP
