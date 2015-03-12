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
    injectMethod = ConstantVelocityOn;
    injectMethodConstantVelocityOnConf = 0.0;
    injectMethodConstantPressureOnConf = 0.0;
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
    injectMethodConstantPressureOnConf = ui->lineEdit_constantPressure->text().toDouble();
    injectMethodConstantVelocityOnConf = ui->lineEdit_constantVelocity->text().toDouble();

    QButtonGroup *group = MainWindow::CurrentWindow->get_buttonGroup_injectionMethod();
    injectMethod = (InjectMethod)(group->checkedId());

    MainWindow::UpdateData(ui->tableWidget_varyingPressure, injectMethodVaryingPressureOnConf);
    MainWindow::UpdateData(ui->tableWidget_varyingVelocity, injectMethodVaryingVelocityOnConf);

}
