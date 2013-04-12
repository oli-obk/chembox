#ifndef CHEMBOX_CONNECTOR_DIR
#define CHEMBOX_CONNECTOR_DIR

#include <cstdint>
#include <cassert>
class ReceiveFromDir
{
private:
    uint8_t val;
public:
	static const ReceiveFromDir Up;
    static const ReceiveFromDir Right;
    static const ReceiveFromDir Down;
    static const ReceiveFromDir Left;
    explicit ReceiveFromDir(int v):val(v)
    {
        assert(v >=0);
        assert(v < 4);
    }
    ReceiveFromDir rotate(int steps) const
    {
        return ReceiveFromDir(((val+steps)%4+4)%4);
    }
    ReceiveFromDir flip() const
    {
        return rotate(2);
    }
    explicit operator int() const
    {
        return val;
    }
};

#endif //CHEMBOX_CONNECTOR_DIR
