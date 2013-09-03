#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "ConnectorDir.hpp"
#include "Particle.hpp"
#include "optional.hpp"
#include <Gosu/Graphics.hpp>
#include "Effects.hpp"

class Connector
{
private:
	ParticleMap particles;
	ParticleEnergy energy;
	optional<Connector&> other;
#ifndef NDEBUG
    enum class ConnectorState{
        ReadyToPop, ReadyToPush, Pushing
    };
	builtin_wrapper<ConnectorState, ConnectorState::ReadyToPush> __state;
#endif //NDEBUG
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
#ifndef NDEBUG
		assert(__state == ConnectorState::Pushing || __state == ConnectorState::ReadyToPush);
		__state = ConnectorState::Pushing;
#endif //NDEBUG
		particles.add(state, type, count);
	}

    void push()
    {
		assert(other);
#ifndef NDEBUG
		assert(__state == ConnectorState::Pushing || __state == ConnectorState::ReadyToPush);
		__state = ConnectorState::Pushing;
#endif //NDEBUG
    }

	void push(ParticleMap&& parts)
	{
		assert(other);
#ifndef NDEBUG
		assert(__state == ConnectorState::Pushing || __state == ConnectorState::ReadyToPush);
		__state = ConnectorState::Pushing;
#endif //NDEBUG
		particles += std::move(parts);
	}

	ParticleMap&& pop()
	{
		assert(other);
#ifndef NDEBUG
		assert(__state == ConnectorState::ReadyToPop);
		__state = ConnectorState::ReadyToPush;
#endif //NDEBUG
		return std::move(particles);
	}

	void communicate()
	{
		assert(other);
#ifndef NDEBUG
		assert(__state == ConnectorState::Pushing);
		__state = ConnectorState::ReadyToPop;
		assert(other->__state == ConnectorState::Pushing);
		other->__state = ConnectorState::ReadyToPop;
#endif //NDEBUG
		std::swap(particles, other->particles);
	}
};

class Machine
{
private:
	bool m_destroyed;
	std::array<optional<Connector>, 4> connectors;
	bool m_initialized;
	void ReInitialize() { m_initialized = false; }
protected:
	Machine(const Machine& rhs);
	Machine();
	void createConnector(ReceiveFromDir dir);
	void destroyConnector(ReceiveFromDir dir);
	void destroyConnectors();
public:
	optional<Connector&> getConnector(ReceiveFromDir dir);
	optional<const Connector&> getConnector(ReceiveFromDir dir) const;
	virtual void Initialize(optional<Machine&> up, optional<Machine&> down, optional<Machine&> left, optional<Machine&> right);
	bool isInitialized() const { return m_initialized; }
	void Destroy() { assert(!m_destroyed); m_destroyed = true; }
	bool isDestroyed() const { return m_destroyed; }
	virtual void draw(double x, double y) = 0;
    // push your particles to the connectors in this function
	virtual void send() = 0;
    // pop your particles from the connectors in this function
	virtual void receive() = 0;
	virtual std::unique_ptr<Machine> clone() const = 0;
	virtual void Action(size_t /* action_id */) {};
	virtual size_t numActions() const { return 0; }
	virtual std::string nameAction(size_t /* action_id */) const { return ""; };
    // char this object currently represents
    virtual char serialize() const = 0;
    static constexpr const char* deserializes();

	void communicate();
public:
	virtual ~Machine();
};

class SharedEffects
{
private:
    static std::weak_ptr<Effects> s_pEffects;
    std::shared_ptr<Effects> pEffects;
protected:
	SharedEffects(Gosu::Graphics& g);
    Effects& effects() { return *pEffects; };
public:
    static std::shared_ptr<Effects> effects(Gosu::Graphics& g);
};

template<typename Derived, typename Base = Machine>
class ClonableMachine : public Base
{
    using Base::Base;
public:
    constexpr std::unique_ptr<Machine> clone() const
    {
        return std::unique_ptr<Machine>(new Derived(static_cast<const Derived&>(*this)));
    }
};

#endif // MACHINE_HPP
