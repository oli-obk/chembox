#ifndef ROTATABLEVERSIONEDMACHINE_HPP
#define ROTATABLEVERSIONEDMACHINE_HPP

#include "rotatable_machine.hpp" // Base class: RotatableMachine

class RotatableVersionedMachine : public RotatableMachine
{
    size_t version;
	static std::map<const wchar_t*, std::array<std::weak_ptr<Gosu::Image>, 6>> s_pImage;
	std::array<std::shared_ptr<Gosu::Image>, 6> m_pImage;
protected:
    RotatableVersionedMachine(Gosu::Graphics& g, ReceiveFromDir rot, size_t version, const wchar_t* basename);
    RotatableVersionedMachine(const RotatableVersionedMachine& rhs);
    virtual ~RotatableVersionedMachine() {}
    virtual void draw(double x, double y);
public:
    void set_version(size_t version);
    size_t get_version() const;

};

#endif // ROTATABLEVERSIONEDMACHINE_HPP
