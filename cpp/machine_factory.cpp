#include "machine_factory.hpp"

MachineFactory::MachineFactory()
{
}

MachineFactory::~MachineFactory()
{
}

std::unique_ptr<Machine> MachineFactory::create(char c) const
{
    auto it = Machines.find(c);
    if (it == Machines.end()) {
        throw std::runtime_error("tried to deserialize an unknown machine");
    }
    return it->second();
}
