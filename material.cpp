#include "material.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QFile>
#include <iostream>

Material::Material(const wstring& _name) : Category(_name)
{
    clearValue();
}

Material::Material(const wstring &_name, Material *material) : Category(_name)
{
    if (material) {
        group = material->getGroup();
        solidDensity = material->getSolidDensity();
        solidSpecificHeat = material->getSolidSpecificHeat();
        solidThermalConductivity = material->getSolidThermalConductivity();
        solidThermalExpansionCoefficient = material->getSolidThermalExpansionCoefficient();
        fluidDensity = material->getFluidDensity();
        fluidSpecificHeat = material->getFluidSpecificHeat();
        fluidThermalConductivity = material->getFluidThermalConductivity();
        fluidThermalExpansionCoefficent = material->getFluidThermalExpansionCoefficent();
        fluidDynamicViscosity = material->getFluidDynamicViscosity();
        fluidSurfaceTensionCoefficent = material->getFluidSurfaceTensionCoefficent();
        fluidContactAngleForWallAdhesion = material->getFluidContactAngleForWallAdhesion();
        solidLiquidusTemperature = material->getSolidLiquidusTemperature();
        solidSolidusTemperature = material->getSolidSolidusTemperature();
        fluidCoherencyPoint = material->getFluidCoherencyPoint();
        fluidCriticalPoint = material->getFluidCriticalPoint();
        solidificationDragCoefficent = material->getSolidificationDragCoefficent();
        materialComment = material->getMaterialComment();
    } else {
        clearValue();
    }
}

void Material::clearValue()
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
    materialComment = L"";
}

void Material::writeValue(QXmlStreamWriter &writer)
{
    writer.writeStartElement("material");
    writer.writeAttribute("id", QString::fromStdWString(getId()));
        writer.writeStartElement("MaterialComment");
        writer.writeAttribute("value", QString::fromStdWString(materialComment));
        writer.writeEndElement();
        writer.writeStartElement("SolidDensity");
        writer.writeAttribute("value", QString::number(solidDensity));
        writer.writeEndElement();
        writer.writeStartElement("SolidSpecificHeat");
        writer.writeAttribute("value", QString::number(solidSpecificHeat));
        writer.writeEndElement();
        writer.writeStartElement("SolidThermalConductivity");
        writer.writeAttribute("value", QString::number(solidThermalConductivity));
        writer.writeEndElement();
        writer.writeStartElement("SolidThermalExpansionCoefficient");
        writer.writeAttribute("value", QString::number(solidThermalExpansionCoefficient));
        writer.writeEndElement();
        writer.writeStartElement("FluidDensity");
        writer.writeAttribute("value", QString::number(fluidDensity));
        writer.writeEndElement();
        writer.writeStartElement("FluidSpecificHeat");
        writer.writeAttribute("value", QString::number(fluidSpecificHeat));
        writer.writeEndElement();
        writer.writeStartElement("FluidThermalConductivity");
        writer.writeAttribute("value", QString::number(fluidThermalConductivity));
        writer.writeEndElement();
        writer.writeStartElement("FluidThermalExpansionCoefficent");
        writer.writeAttribute("value", QString::number(fluidThermalExpansionCoefficent));
        writer.writeEndElement();
        writer.writeStartElement("FluidDynamicViscosity");
        writer.writeAttribute("value", QString::number(fluidDynamicViscosity));
        writer.writeEndElement();
        writer.writeStartElement("FluidSurfaceTensionCoefficent");
        writer.writeAttribute("value", QString::number(fluidSurfaceTensionCoefficent));
        writer.writeEndElement();
        writer.writeStartElement("FluidContactAngleForWallAdhesion");
        writer.writeAttribute("value", QString::number(fluidContactAngleForWallAdhesion));
        writer.writeEndElement();
        writer.writeStartElement("SolidLiquidusTemperature");
        writer.writeAttribute("value", QString::number(solidLiquidusTemperature));
        writer.writeEndElement();
        writer.writeStartElement("SolidSolidusTemperature");
        writer.writeAttribute("value", QString::number(solidSolidusTemperature));
        writer.writeEndElement();
        writer.writeStartElement("FluidCoherencyPoint");
        writer.writeAttribute("value", QString::number(fluidCoherencyPoint));
        writer.writeEndElement();
        writer.writeStartElement("FluidCriticalPoint");
        writer.writeAttribute("value", QString::number(fluidCriticalPoint));
        writer.writeEndElement();
        writer.writeStartElement("SolidificationDragCoefficent");
        writer.writeAttribute("value", QString::number(solidificationDragCoefficent));
        writer.writeEndElement();
    writer.writeEndElement();
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

MaterialGroup::MaterialGroup(const wstring &_name) : Category(_name){
    clearValue();
}

MaterialGroup::~MaterialGroup() {
    clearValue();
}

void MaterialGroup::clearValue()
{
    id = L"";
    group = NULL;
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

bool MaterialGroup::loadMaterialFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Can not read file" << qPrintable(filename)
                     << "; " << qPrintable(file.errorString())
                        << std::endl;

    }
    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
        std::cerr << "Error: Parse error at line " << errorLine << ", "
                     << "column " << errorColumn << ": "
                        << qPrintable(errorStr) << std::endl;
        return false;
    }
    QDomElement root = doc.documentElement();
    if (root.tagName() != "materialgroup") {
        std::cerr << "Error: Not a material file" << std::endl;
        return false;
    }

    loadValue(root);
    file.close();
    return true;
}

void MaterialGroup::deleteGroup(const wstring &id)
{
    vector<MaterialGroup*>::iterator it;
    MaterialGroup *group = NULL;
    for (it = groups.begin(); it != groups.end(); it++) {
        group = (*it);
        if (group && group->getId() == id) {
            break;
        }
    }
    if (it != groups.end()) {
        groups.erase(it);
        delete group;
        group = NULL;
    }
}

void MaterialGroup::deleteMaterial(const wstring &id)
{
    vector<Material*>::iterator it;
    Material*material = NULL;
    for (it = materials.begin(); it != materials.end(); it++) {
        material = (*it);
        if (material && material->getId() == id) {
            break;
        }
    }
    if (it != materials.end()) {
        materials.erase(it);
        delete material;
        material = NULL;
    }
}

void MaterialGroup::writeValue(QXmlStreamWriter &writer)
{
    writer.writeStartElement("materialgroup");
    writer.writeAttribute("id", QString::fromStdWString(getId()));
    for (vector<MaterialGroup*>::iterator it = groups.begin(); it != groups.end(); it++) {
        (*it)->writeValue(writer);
    }
    for (vector<Material*>::iterator it = materials.begin(); it != materials.end(); it++) {
        (*it)->writeValue(writer);
    }
    writer.writeEndElement();
}

bool MaterialGroup::saveMaterialFile(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        std::cerr << "Error: Cannot write file "
                     << qPrintable(filename) << ": "
                        << qPrintable(file.errorString()) << std::endl;
        return false;
    }
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    writeValue(xmlWriter);
    xmlWriter.writeEndDocument();
    file.close();
    if (file.error()) {
        std::cerr << "Error: Cannot write file "
                     << qPrintable(filename) << ": "
                        << qPrintable(file.errorString()) << std::endl;
        return false;
    }
    return true;
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
            _group = new MaterialGroup(L"materialgroup");
            _group->loadValue(child.toElement());
            addGroup(_group);
        } else if (tagName == "material") {
            _material = new Material(L"material");
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
    model->clear();
    QString id = QString::fromStdWString(getId());
    model->setHorizontalHeaderLabels(QStringList()<<id);
    QIcon icon(":/file.png");
    QStandardItem *parent = new QStandardItem(icon, id);
    QVariant v = QVariant::fromValue((void *) this);
    parent->setData(v);
    model->appendRow(parent);
}

void MaterialGroup::updateModel(QStandardItem *item)
{
    QString id = QString::fromStdWString(getId());
    QIcon icon(":/folder.png");
    QStandardItem *parent = new QStandardItem(icon, id);
    QVariant v = QVariant::fromValue((void *) this);
    parent->setData(v);
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
    model->clear();
    QString id = QString::fromStdWString(getId());
    model->setHorizontalHeaderLabels(QStringList()<<id);
    QIcon icon(":/folder.png");
    QStandardItem *parent = new QStandardItem(icon, id);
    QVariant v = QVariant::fromValue((void *) this);
    parent->setData(v);
    model->appendRow(parent);
    parent->setSelectable(false);
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
    updateModel(model);
}

void MaterialGroup::updateValue(void)
{
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;
}

bool MaterialGroup::checkMaterialId(const wstring &_id) {
    for (vector<MaterialGroup*>::iterator it = groups.begin(); it != groups.end(); it++) {
        if ((*it)->checkMaterialId(_id)) {
            return true;
        }
    }
    for (vector<Material*>::iterator it = materials.begin(); it != materials.end(); it++) {
        if ((*it)->getId() == _id) {
            return true;
        }
    }
    return false;
}


