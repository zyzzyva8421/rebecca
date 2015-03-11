#ifndef MOLD_H
#define MOLD_H
#include "category.h"

class MoldConfiguration;
class Mold : public Category
{
public:
    Mold(const string& name);
    ~Mold();
    virtual void loadValue(const QDomElement& element);
    virtual void updateGui(void);
    virtual void updateValue(void);

    MoldConfiguration *getMoldConfiguration(const string& stl);

private:
    vector<MoldConfiguration*> molds;
};

class MoldConfiguration : public Category {
public:
    enum MoldSurfaceRoughness {
        NoSlipOn,
        FreeSlipOn
    };
    enum MoldFunction {
        CoverOn,
        MovingOn,
        SprueOn,
        RiserOn,
        CoreOn
    };
    MoldConfiguration(const string& name);
    virtual void loadValue(const QDomElement& element);
    virtual void updateGui(void);
    virtual void updateValue(void);

private:
    wstring originalStlPath;
    string moldMaterialId;
    double moldMaterialInitialTemperature;
    double moldHeatExchangeCoefficient;
    MoldSurfaceRoughness moldSurfaceRoughness;
    MoldFunction moldFunction;
    wstring moldComment;
};

#endif // MOLD_H
