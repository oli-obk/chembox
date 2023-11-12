#ifndef MACHINEFACTORY_HPP
#define MACHINEFACTORY_HPP

#include <functional>
#include <memory>
#include "machine.hpp"

class MachineFactory
{
    typedef std::function<std::unique_ptr<Machine>()> MachineCreateFunction;
    std::map<char, MachineCreateFunction> Machines;
public:
    template <typename T, typename... Args>
    void add(Args&... args)
    {
        for (char c:T::deserializes()) {
            if (Machines.count(c) > 0) {
                throw std::runtime_error("duplicate machine deserialization char");
            }
            auto fun = [](char c, Args&... args) { return std::unique_ptr<Machine>(new T(c, args...)); };
            Machines[c] = std::bind(fun, c, std::ref(args...));
        }
    }
    std::unique_ptr<Machine> create(char c) const;
    MachineFactory();
    ~MachineFactory();

};

#endif // MACHINEFACTORY_HPP
