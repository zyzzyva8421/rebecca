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
    virtual void writeValue(QXmlStreamWriter& writer);
    virtual void updateGui(void);
    virtual void updateValue(void);
    virtual void clearValue();
    void deleteConfiguration(const wstring &name);
    void modifyConfiguration(const wstring &name);
    void addConfiguration(const wstring &name);
    MoldConfiguration *getMoldConfiguration(const wstring& stl);
    vector<MoldConfiguration*>& getMolds(void) {return molds;}

private:
    vector<MoldConfiguration*> molds;
};

class MoldConfiguration : public Category {
public:
    enum MoldFunction {
        MoldOn,
        CoverOn,
        MovingOn,
        SprueOn,
        RiserOn,
        CoreOn
    };
    MoldConfiguration(const wstring& name);
    virtual void loadValue(const QDomElement& element);
    virtual void writeValue(QXmlStreamWriter& writer);
    virtual void updateGui(void);
    virtual void updateValue(void);
    virtual void clearValue();

private:
    string moldMaterialId;
    double moldMaterialInitialTemperature;
    double moldHeatExchangeCoefficient;
    bool moldFlipOn;
    double moldAdjustCoordinateX;
    double moldAdjustCoordinateY;
    double moldAdjustCoordinateZ;
    double moldAdjustScale;
    double moldRotateCoordinateX;
    double moldRotateCoordinateY;
    double moldRotateCoordinateZ;
    double moldDomainLength;
    MoldFunction moldFunction;
    wstring moldComment;
};

#endif // MOLD_H
