#ifndef PIPEPARTICLES_H
#define PIPEPARTICLES_H

#include "sharedeffects.hpp"
#include "ConnectorDir.hpp"

class PipeParticles : private SharedEffects
{
public:
    using SharedEffects::SharedEffects;
protected:
    void draw(double x, double y, double z, double w, double h);
    void update(ReceiveFromDir dir);
    double& operator[](ReceiveFromDir dir) { return particles_to_render[int(dir)]; }
    double& at(ReceiveFromDir dir) { return particles_to_render[int(dir)]; }
private:
    std::array<double, 4> particles_to_render = {{0.0, 0.0, 0.0, 0.0}};
    std::array<double, 4> particles_to_render_interpolated = {{0.0, 0.0, 0.0, 0.0}};
};

#endif // PIPEPARTICLES_H
