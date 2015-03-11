#include "material.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"

Material::Material(const string& _name) : Category(_name)
{
    group = NULL;
    solidDensity = 0.0;
    solidSpecificHeat = 0.0;
    solidThermalConductivity = 0.0;
    solidThermalExpansionCoefficient = 0.0;
    fluidDensity = 0.0;
    fluidSpecificHeat = 0.0;
    fluidThermalConductivity = 0.0;
    fluidThermalExpansionCoefficent = 0.0;
    fluidDynamicViscosity = 0.0;
    fluidSurfaceTensionCoefficent = 0.0;
    fluidContactAngleForWallAdhesion = 0.0;
    solidLiquidusTemperature = 0.0;
    solidSolidusTemperature = 0.0;
    fluidCoherencyPoint = 0.0;
    fluidCriticalPoint = 0.0;
    solidificationDragCoefficent = 0.0;
}

void Material::loadValue(const QDomElement &element)
{
    id = element.attribute("id").toStdWString();
    QDomNode child = element.firstChild();
    while (!child.isNull()) {
        string tagName = child.toElement().tagName().toStdString();
        if (tagName == "MaterialComment") {
            materialComment = child.toElement().attribute("value").toStdWString();
        } else {
            double value = child.toElement().attribute("value").toDouble();
            if (tagName == "SolidDensity") {
                solidDensity = value;
            } else if (tagName == "SolidSpecificHeat") {
                solidSpecificHeat = value;
            } else if (tagName == "SolidThermalConductivity") {
                solidThermalConductivity = value;
            } else if (tagName == "SolidThermalExpansionCoefficient") {
                solidThermalExpansionCoefficient = value;
            } else if (tagName == "FluidDensity") {
                fluidDensity = value;
            } else if (tagName == "FluidSpecificHeat") {
                fluidSpecificHeat = value;
            } else if (tagName == "FluidThermalConductivity") {
                fluidThermalConductivity = value;
            } else if (tagName == "FluidThermalExpansionCoefficent") {
                fluidThermalExpansionCoefficent = value;
            } else if (tagName == "FluidDynamicViscosity") {
                fluidDynamicViscosity = value;
            } else if (tagName == "FluidSurfaceTensionCoefficent") {
                fluidSurfaceTensionCoefficent = value;
            } else if (tagName == "FluidContactAngleForWallAdhesion") {
                fluidContactAngleForWallAdhesion = value;
            } else if (tagName == "SolidLiquidusTemperature") {
                solidLiquidusTemperature = value;
            } else if (tagName == "SolidSolidusTemperature") {
                solidSolidusTemperature = value;
            } else if (tagName == "FluidCoherencyPoint") {
                fluidCoherencyPoint = value;
            } else if (tagName == "FluidCriticalPoint") {
                fluidCriticalPoint = value;
            } else if (tagName == "SolidificationDragCoefficent") {
                solidificationDragCoefficent = value;
            }
        }
        child = child.nextSibling();
    }
}

void Material::updateGui(void)
{
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;
    QString text;

    text = QString::number(solidDensity);
    ui->lineEdit_SolidDensity->setText(text);

    text = QString::number(solidSpecificHeat);
    ui->lineEdit_SolidSpecificHeat->setText(text);

    text = QString::number(solidThermalConductivity);
    ui->lineEdit_SolidThermalConductivity->setText(text);

    text = QString::number(solidThermalExpansionCoefficient);
    ui->lineEdit_SolidThermalExpansionCoefficient->setText(text);

    text = QString::number(fluidDensity);
    ui->lineEdit_FluidDensity->setText(text);

    text = QString::number(fluidSpecificHeat);
    ui->lineEdit_FluidSpecificHeat->setText(text);

    text = QString::number(fluidThermalConductivity);
    ui->lineEdit_FluidThermalConductivity->setText(text);

    text = QString::number(fluidThermalExpansionCoefficent);
    ui->lineEdit_FluidThermalExpansionCoefficent->setText(text);

    text = QString::number(fluidDynamicViscosity);
    ui->lineEdit_FluidDynamicViscosity->setText(text);

    text = QString::number(fluidSurfaceTensionCoefficent);
    ui->lineEdit_FluidSurfaceTensionCoefficent->setText(text);

    text = QString::number(fluidContactAngleForWallAdhesion);
    ui->lineEdit_FluidContactAngleForWallAdhesion->setText(text);

    text = QString::number(solidLiquidusTemperature);
    ui->lineEdit_SolidLiquidusTemperature->setText(text);

    text = QString::number(solidSolidusTemperature);
    ui->lineEdit_SolidSolidusTemperature->setText(text);

    text = QString::number(fluidCoherencyPoint);
    ui->lineEdit_FluidCoherencyPoint->setText(text);

    text = QString::number(fluidCriticalPoint);
    ui->lineEdit_FluidCriticalPoint->setText(text);

    text = QString::number(solidificationDragCoefficent);
    ui->lineEdit_SolidificationDragCoefficent->setText(text);

    text = QString::fromStdWString(materialComment);
    ui->plainTextEdit_MaterialComment->setPlainText(text);
}

void Material::updateValue(void)
{
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;

    solidDensity = ui->lineEdit_SolidDensity->text().toDouble();
    solidSpecificHeat = ui->lineEdit_SolidSpecificHeat->text().toDouble();
    solidThermalConductivity = ui->lineEdit_SolidThermalConductivity->text().toDouble();
    solidThermalExpansionCoefficient = ui->lineEdit_SolidThermalExpansionCoefficient->text().toDouble();
    fluidDensity = ui->lineEdit_FluidDensity->text().toDouble();
    fluidSpecificHeat = ui->lineEdit_FluidSpecificHeat->text().toDouble();
    fluidThermalConductivity = ui->lineEdit_FluidThermalConductivity->text().toDouble();
    fluidThermalExpansionCoefficent = ui->lineEdit_FluidSurfaceTensionCoefficent->text().toDouble();
    fluidDynamicViscosity = ui->lineEdit_FluidDynamicViscosity->text().toDouble();
    fluidSurfaceTensionCoefficent = ui->lineEdit_FluidSurfaceTensionCoefficent->text().toDouble();
    fluidContactAngleForWallAdhesion = ui->lineEdit_FluidContactAngleForWallAdhesion->text().toDouble();
    solidLiquidusTemperature = ui->lineEdit_SolidLiquidusTemperature->text().toDouble();
    solidSolidusTemperature = ui->lineEdit_SolidSolidusTemperature->text().toDouble();
    fluidCoherencyPoint = ui->lineEdit_FluidCoherencyPoint->text().toDouble();
    fluidCriticalPoint = ui->lineEdit_FluidCriticalPoint->text().toDouble();
    solidificationDragCoefficent = ui->lineEdit_SolidificationDragCoefficent->text().toDouble();
    materialComment = ui->plainTextEdit_MaterialComment->toPlainText().toStdWString();
}

MaterialGroup::MaterialGroup(const string &_name) : Category(_name){
    group = NULL;
}

MaterialGroup::~MaterialGroup() {
    for (vector<Material*>::iterator it = materials.begin(); it != materials.end(); it++) {
        Material *material = (*it);
        delete material;
        material = NULL;
    }
    materials.clear();
    for (vector<MaterialGroup*>::iterator it = groups.begin(); it != groups.end(); it++) {
        MaterialGroup *group = (*it);
        delete group;
        group = NULL;
    }
    groups.clear();
}

void MaterialGroup::loadValue(const QDomElement& element)
{
    id = element.attribute("id").toStdWString();
    QDomNode child = element.firstChild();
    while (!child.isNull()) {
        MaterialGroup *_group = NULL;
        Material* _material = NULL;
        string tagName = child.toElement().tagName().toStdString();
        if (tagName == "materialgroup") {
            _group = new MaterialGroup("materialgroup");
            _group->loadValue(child.toElement());
            addGroup(_group);
        } else if (tagName == "material") {
            _material = new Material("material");
            _material->loadValue(child.toElement());
            addMaterial(_material);
        }
        child = child.nextSibling();
    }
}

void Material::updateModel(QStandardItem *item)
{
    QString id = QString::fromStdWString(getId());
    QIcon icon(":/file.png");
    QStandardItem *parent = new QStandardItem(icon, id);
    QVariant v = QVariant::fromValue((void *) this);
    parent->setData(v);
    item->appendRow(parent);
}

void Material::updateModel(QStandardItemModel *model)
{
    QString id = QString::fromStdWString(getId());
    QIcon icon(":/file.png");
    QStandardItem *parent = new QStandardItem(icon, id);

    model->appendRow(parent);
}

void MaterialGroup::updateModel(QStandardItem *item)
{
    QString id = QString::fromStdWString(getId());
    QIcon icon(":/folder.png");
    QStandardItem *parent = new QStandardItem(icon, id);
    item->appendRow(parent);
    for (vector<MaterialGroup*>::iterator it = groups.begin(); it != groups.end(); it++) {
        MaterialGroup *child = (*it);
        if (child) {
            child->updateModel(parent);
        }
    }
    for (vector<Material*>::iterator it = materials.begin(); it != materials.end(); it++) {
        Material *child = (*it);
        if (child) {
            child->updateModel(parent);
        }
    }
}

void MaterialGroup::updateModel(QStandardItemModel *model)
{
    QString id = QString::fromStdWString(getId());
    QIcon icon(":/folder.png");
    QStandardItem *parent = new QStandardItem(icon, id);
    model->appendRow(parent);
    for (vector<MaterialGroup*>::iterator it = groups.begin(); it != groups.end(); it++) {
        MaterialGroup *child = (*it);
        if (child) {
            child->updateModel(parent);
        }
    }
    for (vector<Material*>::iterator it = materials.begin(); it != materials.end(); it++) {
        Material *child = (*it);
        if (child) {
            child->updateModel(parent);
        }
    }
}

void MaterialGroup::updateGui(void)
{
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;
    QStandardItemModel *model = MainWindow::CurrentWindow->getMaterialGroupModel();
    model->clear();
    QString id = QString::fromStdWString(getId());
    model->setHorizontalHeaderLabels(QStringList()<<id);
    updateModel(model);
}

void MaterialGroup::updateValue(void)
{
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;
}

