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
    bool fluidOn;
    double gravityX;
    double gravityY;
    double gravityZ;
    int fluidMeshLevel;
    int adaptMeshLevel;
    int solidMeshLevel;
    double environmentDensity;
    double environmentDynamicViscosity;
    double environmentTemperature;
    double environmentPressure;
    double environmentThermalConductivity;
    double environmentSpecificHeat;
    FluidOnConf fluidOnConf;
    wstring simulationComment;
};

#endif // SIMULATION_H
