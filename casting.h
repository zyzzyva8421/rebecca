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
    Casting(const string& name);
    virtual void loadValue(const QDomElement& element);
    virtual void updateGui(void);
    virtual void updateValue(void);
private:
    string castingMaterialId;
    double castingMaterialInitialTemperature;
    InjectMethod injectMethod;
    double injectMethodConstantVelocityOnConf;
    vector< vector<double> > injectMethodVaryingVelocityOnConf;
    double injectMethodConstantPressureOnConf;
    vector< vector<double> > injectMethodVaryingPressureOnConf;
    wstring castingComment;
};

#endif // CASTING_H
