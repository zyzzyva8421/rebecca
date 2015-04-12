#include "simulation.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

Simulation::Simulation(const wstring& _name) : Category(_name)
{
    clearValue();
}

void Simulation::clearValue()
{
    fluidOn = false;
    fluidOnConf = LaminarOn;
    gravityX = 0.0;
    gravityY = 0.0;
    gravityZ = 0.0;
    fluidMeshLevel = 0;
    adaptMeshLevel = 0;
    solidMeshLevel = 0;
    environmentDensity = 0.0;
    environmentDynamicViscosity = 0.0;
    environmentTemperature = 0.0;
    environmentPressure = 0.0;
    environmentThermalConductivity = 0.0;
    environmentSpecificHeat = 0.0;
    simulationComment = L"";
}

void Simulation::writeValue(QXmlStreamWriter &writer)
{
    updateValue();
    writer.writeStartElement("Simulation");
        writer.writeStartElement("FluidOn");
        writer.writeAttribute("value", QString::number((fluidOn)?1:0));
        writer.writeEndElement();
        writer.writeStartElement("FluidOnConf");
            writer.writeStartElement("LaminarOn");
            writer.writeAttribute("value", QString::number((fluidOnConf==LaminarOn)?1:0));
            writer.writeEndElement();
            writer.writeStartElement("SaTurbulenceOn");
            writer.writeAttribute("value", QString::number((fluidOnConf==SaTurbulenceOn)?1:0));
            writer.writeEndElement();
            writer.writeStartElement("KeTurbulenceOn");
            writer.writeAttribute("value", QString::number((fluidOnConf==KeTurbulenceOn)?1:0));
            writer.writeEndElement();
            writer.writeStartElement("LesTurbulenceOn");
            writer.writeAttribute("value", QString::number((fluidOnConf==LesTurbulenceOn)?1:0));
            writer.writeEndElement();
        writer.writeEndElement();
        writer.writeStartElement("Gravity");
        writer.writeAttribute("x", QString::number(gravityX));
        writer.writeAttribute("y", QString::number(gravityY));
        writer.writeAttribute("z", QString::number(gravityZ));
        writer.writeEndElement();
        writer.writeStartElement("FluidMeshLevel");
        writer.writeAttribute("value", QString::number(fluidMeshLevel));
        writer.writeEndElement();
        writer.writeStartElement("AdaptMeshLevel");
        writer.writeAttribute("value", QString::number(adaptMeshLevel));
        writer.writeEndElement();
        writer.writeStartElement("SolidMeshLevel");
        writer.writeAttribute("value", QString::number(solidMeshLevel));
        writer.writeEndElement();
        writer.writeStartElement("EnvironmentDensity");
        writer.writeAttribute("value", QString::number(environmentDensity));
        writer.writeEndElement();
        writer.writeStartElement("EnvironmentDynamicViscosity");
        writer.writeAttribute("value", QString::number(environmentDynamicViscosity));
        writer.writeEndElement();
        writer.writeStartElement("EnvironmentTemperature");
        writer.writeAttribute("value", QString::number(environmentTemperature));
        writer.writeEndElement();
        writer.writeStartElement("EnvironmentPressure");
        writer.writeAttribute("value", QString::number(environmentPressure));
        writer.writeEndElement();
        writer.writeStartElement("EnvironmentThermalConductivity");
        writer.writeAttribute("value", QString::number(environmentThermalConductivity));
        writer.writeEndElement();
        writer.writeStartElement("EnvironmentSpecificHeat");
        writer.writeAttribute("value", QString::number(environmentSpecificHeat));
        writer.writeEndElement();
        writer.writeStartElement("SimulationComment");
        writer.writeAttribute("value", QString::fromStdWString(simulationComment));
        writer.writeEndElement();
    writer.writeEndElement();
}

void Simulation::loadValue(const QDomElement& element)
{
    QDomNode child = element.firstChild();
    QDomNode child1;
    string tagName;
    string tagName1;
    while (!child.isNull()) {
        tagName = child.toElement().tagName().toStdString();
        if (tagName == "FluidOn") {
            string v = child.toElement().attribute("value").toStdString();
            fluidOn = (v == "1")?true:false;
        } else if (tagName == "FluidOnConf") {
            child1 = child.toElement().firstChild();
            while (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                int value = child1.toElement().attribute("value").toInt();
                if (value != 0) {
                    if (tagName1 == "LaminarOn") {
                        fluidOnConf = LaminarOn;
                        break;
                    } else if (tagName1 == "SaTurbulenceOn") {
                        fluidOnConf = SaTurbulenceOn;
                        break;
                    } else if (tagName1 == "KeTurbulenceOn") {
                        fluidOnConf = KeTurbulenceOn;
                        break;
                    } else if (tagName1 == "LesTurbulenceOn") {
                        fluidOnConf = LesTurbulenceOn;
                        break;
                    }
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "Gravity") {
            gravityX = child.toElement().attribute("x").toDouble();
            gravityY = child.toElement().attribute("y").toDouble();
            gravityZ = child.toElement().attribute("z").toDouble();
        } else if (tagName == "FluidMeshLevel") {
            fluidMeshLevel = child.toElement().attribute("value").toInt();
        } else if (tagName == "AdaptMeshLevel") {
            adaptMeshLevel = child.toElement().attribute("value").toInt();
        } else if (tagName == "SolidMeshLevel") {
            solidMeshLevel = child.toElement().attribute("value").toInt();
        } else if (tagName == "EnvironmentDensity") {
            environmentDensity = child.toElement().attribute("value").toDouble();
        } else if (tagName == "EnvironmentDynamicViscosity") {
            environmentDynamicViscosity = child.toElement().attribute("value").toDouble();
        } else if (tagName == "EnvironmentTemperature") {
            environmentTemperature = child.toElement().attribute("value").toDouble();
        } else if (tagName == "EnvironmentPressure") {
            environmentPressure = child.toElement().attribute("value").toDouble();
        } else if (tagName == "EnvironmentThermalConductivity") {
            environmentThermalConductivity = child.toElement().attribute("value").toDouble();
        } else if (tagName == "EnvironmentSpecificHeat") {
            environmentSpecificHeat = child.toElement().attribute("value").toDouble();
        } else if (tagName == "SimulationComment") {
            simulationComment = child.toElement().attribute("value").toStdWString();
        }
        child = child.nextSibling();
    }
}

void Simulation::updateGui(void)
{
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;

    ui->checkBox_fluidModel->setChecked(fluidOn);

    switch (fluidOnConf) {
    case LaminarOn: {
        ui->radioButton_laminarModel->click();
        break;
    }
    case SaTurbulenceOn: {
        ui->radioButton_SAModel->click();
        break;
    }
    case KeTurbulenceOn: {
        ui->radioButton_KEModel->click();
        break;
    }
    case LesTurbulenceOn: {
        ui->radioButton_LESModel->click();
        break;
    }
    default: break;
    }

    QString text = QString::fromStdWString(simulationComment);
    ui->plainTextEdit_simulationIntroduction->setPlainText(text);

    text = QString::number(gravityX);
    ui->lineEdit_GravityX->setText(text);
    text = QString::number(gravityY);
    ui->lineEdit_GravityY->setText(text);
    text = QString::number(gravityZ);
    ui->lineEdit_GravityZ->setText(text);
    text = QString::number(fluidMeshLevel);
    ui->lineEdit_FluidMeshLevel->setText(text);
    text = QString::number(adaptMeshLevel);
    ui->lineEdit_AdaptMeshLevel->setText(text);
    text = QString::number(solidMeshLevel);
    ui->lineEdit_SolidMeshLevel->setText(text);
    text = QString::number(environmentDensity);
    ui->lineEdit_EnvironmentDensity->setText(text);
    text = QString::number(environmentDynamicViscosity);
    ui->lineEdit_EnvironmentDynamicViscosity->setText(text);
    text = QString::number(environmentTemperature);
    ui->lineEdit_EnvironmentTemperature->setText(text);
    text = QString::number(environmentPressure);
    ui->lineEdit_EnvironmentPressure->setText(text);
    text = QString::number(environmentThermalConductivity);
    ui->lineEdit_EnvironmentThermalConductivity->setText(text);
    text = QString::number(environmentSpecificHeat);
    ui->lineEdit_EnvironmentSpecificHeat->setText(text);
}

void Simulation::updateValue(void)
{
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;

    fluidOn = (ui->checkBox_fluidModel)?true:false;

    QButtonGroup *group = MainWindow::CurrentWindow->get_buttonGroup_fluidModel();
    fluidOnConf = (FluidOnConf)(group->checkedId());

    simulationComment = ui->plainTextEdit_simulationIntroduction->toPlainText().toStdWString();
    gravityX = ui->lineEdit_GravityX->text().toDouble();
    gravityY = ui->lineEdit_GravityY->text().toDouble();
    gravityZ = ui->lineEdit_GravityZ->text().toDouble();
    fluidMeshLevel = ui->lineEdit_FluidMeshLevel->text().toInt();
    adaptMeshLevel = ui->lineEdit_AdaptMeshLevel->text().toInt();
    solidMeshLevel = ui->lineEdit_SolidMeshLevel->text().toInt();
    environmentDensity = ui->lineEdit_EnvironmentDensity->text().toDouble();
    environmentDynamicViscosity = ui->lineEdit_EnvironmentDynamicViscosity->text().toDouble();
    environmentTemperature = ui->lineEdit_EnvironmentTemperature->text().toDouble();
    environmentPressure = ui->lineEdit_EnvironmentPressure->text().toDouble();
    environmentThermalConductivity = ui->lineEdit_EnvironmentThermalConductivity->text().toDouble();
    environmentSpecificHeat = ui->lineEdit_EnvironmentSpecificHeat->text().toDouble();

}
