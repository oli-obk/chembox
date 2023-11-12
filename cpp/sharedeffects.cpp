#include "sharedeffects.hpp"


std::weak_ptr<Effects> SharedEffects::s_pEffects;

std::shared_ptr<Effects> SharedEffects::effects(Gosu::Graphics& g)
{
    std::shared_ptr<Effects> ret(s_pEffects.lock());
    if (!ret) {
        ret.reset(new Effects(g));
        s_pEffects = ret;
    }
    return ret;
}

SharedEffects::SharedEffects(Gosu::Graphics& g)
:pEffects(effects(g))
{
}
