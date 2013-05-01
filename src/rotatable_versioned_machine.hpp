#ifndef ROTATABLEVERSIONEDMACHINE_HPP
#define ROTATABLEVERSIONEDMACHINE_HPP

#include "rotatable_machine.hpp" // Base class: RotatableMachine

class RotatableVersionedMachine : public RotatableMachine
{
    size_t version;
	static std::array<std::weak_ptr<Gosu::Image>, 5> s_pImage;
	std::array<std::shared_ptr<Gosu::Image>, 5> m_pImage;
protected:
    RotatableVersionedMachine(Gosu::Graphics& g, size_t rot, size_t version, std::wstring basename);
    RotatableVersionedMachine(const RotatableVersionedMachine& rhs);
    virtual ~RotatableVersionedMachine() {}
    virtual void draw();
public:
    void set_version(size_t version);
    size_t get_version() const;

};

#endif // ROTATABLEVERSIONEDMACHINE_HPP
