#include "simulation.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

Simulation::Simulation(const wstring& _name) : Category(_name)
{
    clearValue();
}

void Simulation::clearValue()
{
    heatTransferOn = false;
    phaseChangeOn = false;
    fluidOn = false;
    fluidOnConf = LaminarOn;
    simulationComment = L"";
}

void Simulation::writeValue(QXmlStreamWriter &writer)
{
    updateValue();
    writer.writeStartElement("Simulation");
        writer.writeStartElement("HeatTransferOn");
        writer.writeAttribute("value", QString::number((heatTransferOn)?1:0));
        writer.writeEndElement();
        writer.writeStartElement("PhaseChangeOn");
        writer.writeAttribute("value", QString::number((phaseChangeOn)?1:0));
        writer.writeEndElement();
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
        if (tagName == "HeatTransferOn") {
            string v = child.toElement().attribute("value").toStdString();
            heatTransferOn = (v == "1")?true:false;
        } else if (tagName == "PhaseChangeOn") {
            string v = child.toElement().attribute("value").toStdString();
            phaseChangeOn = (v == "1")?true:false;
        } else if (tagName == "FluidOn") {
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

    ui->checkBox_heatTransferModel->setChecked(heatTransferOn);
    ui->checkBox_phaseChangeModel->setChecked(phaseChangeOn);
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
}

void Simulation::updateValue(void)
{
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;

    heatTransferOn = (ui->checkBox_heatTransferModel->isChecked())?true:false;
    phaseChangeOn = (ui->checkBox_phaseChangeModel->isChecked())?true:false;
    fluidOn = (ui->checkBox_fluidModel)?true:false;

    QButtonGroup *group = MainWindow::CurrentWindow->get_buttonGroup_fluidModel();
    fluidOnConf = (FluidOnConf)(group->checkedId());

    simulationComment = ui->plainTextEdit_simulationIntroduction->toPlainText().toStdWString();
}
