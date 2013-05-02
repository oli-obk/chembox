#ifndef CHEMBOX_CONNECTOR_DIR
#define CHEMBOX_CONNECTOR_DIR

#include <cstdint>
#include <cassert>

enum class ReceiveFromDir : uint8_t
{
    Up, Right, Down, Left
};

inline ReceiveFromDir operator+(ReceiveFromDir lhs, int rhs)
{
    return static_cast<ReceiveFromDir>(((static_cast<int>(lhs)+rhs)%4+4)%4);
}

inline ReceiveFromDir operator+(ReceiveFromDir lhs, ReceiveFromDir rhs)
{
    return static_cast<ReceiveFromDir>(((static_cast<int>(lhs)+static_cast<int>(rhs)))%4);
}

inline ReceiveFromDir flip(ReceiveFromDir dir)
{
    return dir + 2;
}

#endif //CHEMBOX_CONNECTOR_DIR
