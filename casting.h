#ifndef CASTING_H
#define CASTING_H
#include "category.h"
#include <vector>
#include <utility>

class Casting : public Category
{
public:
    enum InjectMethod {
        ConstantVelocityOn,
        VaryingVelocityOn,
        ConstantPressureOn,
        VaryingPressureOn
    };
    Casting(const wstring& name);
    virtual void loadValue(const QDomElement& element);
    virtual void writeValue(QXmlStreamWriter& writer);
    virtual void updateGui(void);
    virtual void updateValue(void);
    virtual void clearValue(void);
    vector< vector<double> >& getInjectMethodVaryingVelocityOnConf(void) {return injectMethodVaryingVelocityOnConf;}
    vector< vector<double> >& getInjectMethodVaryingPressureOnConf(void) {return injectMethodVaryingPressureOnConf;}

private:
    string castingMaterialId;
    double castingMaterialInitialTemperature;
    int castingMaterialFluidMeshLevel;
    int castingMaterialAdaptMeshLevel;
    InjectMethod injectMethod;
    double injectMethodConstantVelocityOnConf;
    vector< vector<double> > injectMethodVaryingVelocityOnConf;
    double injectMethodConstantPressureOnConf;
    vector< vector<double> > injectMethodVaryingPressureOnConf;
    wstring castingComment;
};

#endif // CASTING_H
