#ifndef MOLD_H
#define MOLD_H
#include "category.h"

class MoldConfiguration;
class Mold : public Category
{
public:
    Mold(const wstring& name);
    ~Mold();
    virtual void loadValue(const QDomElement& element);
    virtual void updateGui(void);
    virtual void updateValue(void);
    virtual void clearValue();
    void deleteConfiguration(const wstring &name);
    void modifyConfiguration(const wstring &name);
    void addConfiguration(const wstring &name);
    MoldConfiguration *getMoldConfiguration(const wstring& stl);

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
    MoldConfiguration(const wstring& name);
    virtual void loadValue(const QDomElement& element);
    virtual void updateGui(void);
    virtual void updateValue(void);
    virtual void clearValue();

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
