#include "casting.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"

Casting::Casting(const wstring&_name) : Category(_name)
{
    clearValue();
}

void Casting::clearValue() {
    castingMaterialId = "";
    injectMethodVaryingVelocityOnConf.clear();
    injectMethodVaryingPressureOnConf.clear();
    castingComment = L"";
    castingMaterialInitialTemperature = 0.0;
    castingMaterialFluidMeshLevel = 0;
    castingMaterialAdaptMeshLevel = 0;
    injectMethod = ConstantVelocityOn;
    injectMethodConstantVelocityOnConf = 0.0;
    injectMethodConstantPressureOnConf = 0.0;
}

void Casting::writeValue(QXmlStreamWriter &writer)
{
    updateValue();
    writer.writeStartElement("Casting");
        writer.writeStartElement("CastingMaterialId");
        writer.writeAttribute("value", QString::fromStdString(castingMaterialId));
        writer.writeEndElement();
        writer.writeStartElement("CastingMaterialInitialTemperature");
        writer.writeAttribute("value", QString::number(castingMaterialInitialTemperature));
        writer.writeEndElement();
        writer.writeStartElement("CastingMaterialFluidMeshLevel");
        writer.writeAttribute("value", QString::number(castingMaterialFluidMeshLevel));
        writer.writeEndElement();
        writer.writeStartElement("CastingMaterialAdaptMeshLevel");
        writer.writeAttribute("value", QString::number(castingMaterialAdaptMeshLevel));
        writer.writeEndElement();
        writer.writeStartElement("InjectMethod");
            writer.writeStartElement("ConstantVelocityOn");
            writer.writeAttribute("value", QString::number((injectMethod==ConstantVelocityOn)?1:0));
            writer.writeEndElement();
            writer.writeStartElement("VaryingVelocityOn");
            writer.writeAttribute("value", QString::number((injectMethod==VaryingVelocityOn)?1:0));
            writer.writeEndElement();
            writer.writeStartElement("ConstantPressureOn");
            writer.writeAttribute("value", QString::number((injectMethod==ConstantPressureOn)?1:0));
            writer.writeEndElement();
            writer.writeStartElement("VaryingPressureOn");
            writer.writeAttribute("value", QString::number((injectMethod==VaryingPressureOn)?1:0));
            writer.writeEndElement();
        writer.writeEndElement();
        writer.writeStartElement("InjectMethodConstantVelocityOnConf");
            writer.writeStartElement("Value");
            writer.writeAttribute("value", QString::number(injectMethodConstantVelocityOnConf));
            writer.writeEndElement();
        writer.writeEndElement();
        writer.writeStartElement("InjectMethodVaryingVelocityOnConf");
            for (vector< vector<double> >::iterator it = injectMethodVaryingVelocityOnConf.begin();
                 it != injectMethodVaryingVelocityOnConf.end(); it++) {
                double time = (*it).at(0);
                double speed = (*it).at(1);
                writer.writeStartElement("Value");
                writer.writeAttribute("time", QString::number(time));
                writer.writeAttribute("speed", QString::number(speed));
                writer.writeEndElement();
            }
        writer.writeEndElement();
        writer.writeStartElement("InjectMethodConstantPressureOnConf");
            writer.writeStartElement("Value");
            writer.writeAttribute("value", QString::number(injectMethodConstantPressureOnConf));
            writer.writeEndElement();
        writer.writeEndElement();
        writer.writeStartElement("InjectMethodVaryingPressureOnConf");
            for (vector< vector<double> >::iterator it = injectMethodVaryingPressureOnConf.begin();
                 it != injectMethodVaryingPressureOnConf.end(); it++) {
                double time = (*it).at(0);
                double pressure = (*it).at(1);
                writer.writeStartElement("Value");
                writer.writeAttribute("time", QString::number(time));
                writer.writeAttribute("pressure", QString::number(pressure));
                writer.writeEndElement();
            }
        writer.writeEndElement();
        writer.writeStartElement("CastingComment");
        writer.writeAttribute("value", QString::fromStdWString(castingComment));
        writer.writeEndElement();
    writer.writeEndElement();
}

void Casting::loadValue(const QDomElement& element) {
    QDomNode child = element.firstChild();
    QDomNode child1;
    string tagName;
    string tagName1;
    while (!child.isNull()) {
        tagName = child.toElement().tagName().toStdString();
        if (tagName == "CastingMaterialId") {
            castingMaterialId = child.toElement().attribute("value").toStdString();
        } else if (tagName == "CastingMaterialInitialTemperature") {
            castingMaterialInitialTemperature = child.toElement().attribute("value").toDouble();
        } else if (tagName == "CastingMaterialFluidMeshLevel") {
            castingMaterialFluidMeshLevel = child.toElement().attribute("value").toInt();
        } else if (tagName == "CastingMaterialAdaptMeshLevel") {
            castingMaterialAdaptMeshLevel = child.toElement().attribute("value").toInt();
        } else if (tagName == "InjectMethod") {
            child1 = child.toElement().firstChild();
            while (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                int value = child1.toElement().attribute("value").toInt();
                if (value != 0) {
                    if (tagName1 == "ConstantVelocityOn") {
                        injectMethod = ConstantVelocityOn;
                        break;
                    } else if (tagName1 == "VaryingVelocityOn") {
                        injectMethod = VaryingVelocityOn;
                        break;
                    } else if (tagName1 == "ConstantPressureOn") {
                        injectMethod = ConstantPressureOn;
                        break;
                    } else if (tagName1 == "VaryingPressureOn") {
                        injectMethod = VaryingPressureOn;
                        break;
                    }
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "InjectMethodConstantVelocityOnConf") {
            child1 = child.toElement().firstChild();
            if (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                if (tagName1 == "Value") {
                    injectMethodConstantVelocityOnConf = child1.toElement().attribute("value").toDouble();
                }
            }
        } else if (tagName == "InjectMethodVaryingVelocityOnConf") {
            child1 = child.toElement().firstChild();
            while (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                if (tagName1 == "Value") {
                    vector<double> p;
                    p.push_back(child1.toElement().attribute("time").toDouble());
                    p.push_back(child1.toElement().attribute("speed").toDouble());
                    injectMethodVaryingVelocityOnConf.push_back(p);
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "InjectMethodConstantPressureOnConf") {
            child1 = child.toElement().firstChild();
            if (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                if (tagName1 == "Value") {
                    injectMethodConstantPressureOnConf = child1.toElement().attribute("value").toDouble();
                }
            }
        } else if (tagName == "InjectMethodVaryingPressureOnConf") {
            child1 = child.toElement().firstChild();
            while (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                if (tagName1 == "Value") {
                    vector<double> p;
                    p.push_back(child1.toElement().attribute("time").toDouble());
                    p.push_back(child1.toElement().attribute("pressure").toDouble());
                    injectMethodVaryingPressureOnConf.push_back(p);
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "CastingComment") {
            castingComment = child.toElement().attribute("value").toStdWString();
        }
        child = child.nextSibling();
    }
}

void Casting::updateGui(void) {
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;
    QString text;

    text = QString::fromStdString(castingMaterialId);
    ui->comboBox_material->clear();
    ui->comboBox_material->addItem(text);
    ui->comboBox_material->setCurrentIndex(0);

    text = QString::fromStdWString(castingComment);
    ui->plainTextEdit_projectDescription->setPlainText(text);

    text = QString::number(castingMaterialInitialTemperature);
    ui->lineEdit_initialTemperature->setText(text);

    text = QString::number(castingMaterialFluidMeshLevel);
    ui->lineEdit_CastingMaterialFluidMeshLevel->setText(text);

    text = QString::number(castingMaterialAdaptMeshLevel);
    ui->lineEdit_CastingMaterialAdaptMeshLevel->setText(text);

    text = QString::number(injectMethodConstantPressureOnConf);
    ui->lineEdit_constantPressure->setText(text);

    text = QString::number(injectMethodConstantVelocityOnConf);
    ui->lineEdit_constantVelocity->setText(text);

    switch (injectMethod) {
    case ConstantPressureOn: {
        ui->radioButton_constantPressure->click();
        break;
    }
    case ConstantVelocityOn: {
        ui->radioButton_constantVelocity->click();
        break;
    }
    case VaryingPressureOn: {
        ui->radioButton_varyingPressure->click();
        break;
    }
    case VaryingVelocityOn: {
        ui->radioButton_varyingVelocity->click();
        break;
    }
    default:break;
    }

    MainWindow::UpdateTable(injectMethodVaryingPressureOnConf, ui->tableWidget_varyingPressure);
    MainWindow::UpdateTable(injectMethodVaryingVelocityOnConf, ui->tableWidget_varyingVelocity);
}

void Casting::updateValue(void) {
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;

    castingMaterialId = ui->comboBox_material->currentText().toStdString();
    castingComment = ui->plainTextEdit_introduction->toPlainText().toStdWString();
    castingMaterialInitialTemperature = ui->lineEdit_initialTemperature->text().toDouble();
    castingMaterialFluidMeshLevel = ui->lineEdit_CastingMaterialFluidMeshLevel->text().toInt();
    castingMaterialAdaptMeshLevel = ui->lineEdit_CastingMaterialAdaptMeshLevel->text().toInt();
    injectMethodConstantPressureOnConf = ui->lineEdit_constantPressure->text().toDouble();
    injectMethodConstantVelocityOnConf = ui->lineEdit_constantVelocity->text().toDouble();

    QButtonGroup *group = MainWindow::CurrentWindow->get_buttonGroup_injectionMethod();
    injectMethod = (InjectMethod)(group->checkedId());

    MainWindow::UpdateData(ui->tableWidget_varyingPressure, injectMethodVaryingPressureOnConf);
    MainWindow::UpdateData(ui->tableWidget_varyingVelocity, injectMethodVaryingVelocityOnConf);

}
