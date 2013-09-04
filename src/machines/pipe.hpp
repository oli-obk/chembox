#ifndef PIPE_HPP
#define PIPE_HPP

#include "machine.hpp" // Base class: RotatableVersionedMachine
#include "Particle.hpp"
#include "imagestore.hpp"

class Pipe : public ClonableMachine<Pipe>, private ImageStore<Pipe>, private SharedEffects
{
private:
	std::shared_ptr<Gosu::Font> m_pFont;
	static std::weak_ptr<Gosu::Font> s_pFont;
    static std::mt19937 engine; // Mersenne twister MT19937
	ParticleMap particles;
	std::array<ParticleMap, 4> flowing_particles;
    std::array<double, 4> particles_to_render;
    std::array<double, 4> particles_to_render_interpolated;
public:
	Pipe(const Pipe& rhs);
	Pipe(Gosu::Graphics& g);
    Pipe(char c, Gosu::Graphics& g);
	virtual ~Pipe();

public:
	virtual void send();
	virtual void receive();
    virtual void draw(double x, double y, double z, double w, double h);
	size_t numActions() const;
	void Action(size_t id);
    char serialize() const;
    static const std::string deserializes() { return "+"; }
};

#endif // PIPE_HPP
