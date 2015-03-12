#ifndef MATERIAL_H
#define MATERIAL_H
#include "category.h"
#include <QStandardItemModel>

class MaterialGroup;
class Material : public Category
{
public:
    Material(const wstring& name);
    virtual void loadValue(const QDomElement& element);
    virtual void updateGui(void);
    virtual void updateValue(void);
    virtual void clearValue();

    void setGroup(MaterialGroup *_group) {group = _group;}
    MaterialGroup *getGroup(void) {return group;}

    void setId(const wstring& _id) {id = _id;}
    wstring getId(void) {return id;}

    void updateModel(QStandardItem *item);
    void updateModel(QStandardItemModel *model);

private:
    wstring id;
    MaterialGroup *group;
    wstring materialComment;
    double solidDensity;
    double solidSpecificHeat;
    double solidThermalConductivity;
    double solidThermalExpansionCoefficient;
    double fluidDensity;
    double fluidSpecificHeat;
    double fluidThermalConductivity;
    double fluidThermalExpansionCoefficent;
    double fluidDynamicViscosity;
    double fluidSurfaceTensionCoefficent;
    double fluidContactAngleForWallAdhesion;
    double solidLiquidusTemperature;
    double solidSolidusTemperature;
    double fluidCoherencyPoint;
    double fluidCriticalPoint;
    double solidificationDragCoefficent;
};

class MaterialGroup : public Category
{
public:
    MaterialGroup(const wstring& name);
    ~MaterialGroup();
    virtual void loadValue(const QDomElement& element);
    virtual void updateGui(void);
    virtual void updateValue(void);
    virtual void clearValue();
    bool loadMaterialFile(const QString& filename);

    void addMaterial(Material *material) {
        materials.push_back(material);
        material->setGroup(this);
    }

    void addGroup(MaterialGroup *group) {
        groups.push_back(group);
        group->setGroup(this);
    }
    void setGroup(MaterialGroup *_group) {group = _group;}
    MaterialGroup *getGroup(void) {return group;}

    void setId(const wstring& _id) {id = _id;}
    wstring getId(void) {return id;}

    void updateModel(QStandardItem *item);
    void updateModel(QStandardItemModel *model);

private:
    wstring id;
    MaterialGroup *group;
    vector<Material*> materials;
    vector<MaterialGroup*> groups;
};
#endif // MATERIAL_H
