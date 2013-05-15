#include "pump.hpp"
#include "defines.hpp"
#include <Gosu/Math.hpp>
#include <Gosu/Image.hpp>


std::weak_ptr<Gosu::Image> Pump::s_pImagePump, Pump::s_pImageSpinner;

Pump::Pump(Gosu::Graphics& g)
:m_pImagePump(s_pImagePump.lock())
,m_pImageSpinner(s_pImageSpinner.lock())
{
    if (!m_pImagePump) {
        assert(!m_pImageSpinner);
        m_pImagePump.reset(new Gosu::Image(g, L"pump.png", true));
        m_pImageSpinner.reset(new Gosu::Image(g, L"pump_spinner.png", false));
        s_pImagePump = m_pImagePump;
        s_pImageSpinner = m_pImageSpinner;
    }
    assert(m_pImagePump);
    assert(m_pImageSpinner);
    createConnector(ReceiveFromDir::Left);
    createConnector(ReceiveFromDir::Up);
}

Pump::~Pump()
{
}

void Pump::draw(double x, double y)
{
    m_pImagePump->draw( x, y, RenderLayer::Machines,
                        1.0/double(m_pImagePump->width()),
                        1.0/double(m_pImagePump->height())
                        );
    m_pImageSpinner->drawRot(   x+ 0.5, y + 0.5, RenderLayer::Machines+1,
                                rotation,
                                0.5, 0.5,
                                0.5/double(m_pImageSpinner->width()),
                                0.5/double(m_pImageSpinner->height())
                                );
}

void Pump::receive()
{
    rotation = Gosu::wrap(rotation+5, -180.0, 180.0);
    particles += getConnector(ReceiveFromDir::Left)->pop();
    particles += getConnector(ReceiveFromDir::Up)->pop();
}

void Pump::send()
{
    getConnector(ReceiveFromDir::Up)->push(particles);
    particles.clear();
    getConnector(ReceiveFromDir::Left)->push();
}

char Pump::serialize()
{
    return 'P';
}

void Pump::Initialize(optional<Machine&> up, optional<Machine&> down, optional<Machine&> left, optional<Machine&> right)
{
    Machine::Initialize(up, down, left, right);
}

Pump::Pump(const Pump& rhs)
:Machine(rhs)
,m_pImagePump(rhs.m_pImagePump)
,m_pImageSpinner(rhs.m_pImageSpinner)
{
    createConnector(ReceiveFromDir::Left);
    createConnector(ReceiveFromDir::Up);
}
