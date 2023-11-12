#ifndef SHARED_EFFECTS_HPP
#define SHARED_EFFECTS_HPP

#include <Gosu/Graphics.hpp>
#include "Effects.hpp"

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

#endif //SHARED_EFFECTS_HPP
