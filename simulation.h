#ifndef SIMULATION_H
#define SIMULATION_H
#include "category.h"

class Simulation : public Category
{
public:
    enum FluidOnConf {
        LaminarOn,
        SaTurbulenceOn,
        KeTurbulenceOn,
        LesTurbulenceOn
    };
    Simulation(const wstring& name);
    virtual void loadValue(const QDomElement& element);
    virtual void writeValue(QXmlStreamWriter& writer);
    virtual void updateGui(void);
    virtual void updateValue(void);
    virtual void clearValue();
private:
    bool heatTransferOn;
    bool phaseChangeOn;
    bool fluidOn;
    FluidOnConf fluidOnConf;
    wstring simulationComment;
};

#endif // SIMULATION_H
