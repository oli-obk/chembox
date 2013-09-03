#include "pump.hpp"
#include "defines.hpp"
#include <Gosu/Math.hpp>
#include <Gosu/Image.hpp>

Pump::Pump(Gosu::Graphics& g)
:ImageStore<Pump>(g, L"pump.png", true)
,ImageStore<PumpSpinner>(g, L"pump_spinner.png", false)
{
    createConnector(ReceiveFromDir::Left);
    createConnector(ReceiveFromDir::Up);
}

Pump::~Pump()
{
}

void Pump::draw(double x, double y)
{
    ImageStore<Pump>::draw(x, y, RenderLayer::Machines);
    ImageStore<PumpSpinner>::Image().drawRot(x+ 0.5, y + 0.5, RenderLayer::Machines+1,
                                rotation,
                                0.5, 0.5,
                                0.5/double(ImageStore<PumpSpinner>::Image().width()),
                                0.5/double(ImageStore<PumpSpinner>::Image().height())
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
    getConnector(ReceiveFromDir::Up)->push(std::move(particles));
    getConnector(ReceiveFromDir::Left)->push();
}

char Pump::serialize() const
{
    return 'P';
}

Pump::Pump(const Pump& rhs)
:ClonableMachine(rhs)
,ImageStore<Pump>(rhs)
,ImageStore<PumpSpinner>(rhs)
{
}
