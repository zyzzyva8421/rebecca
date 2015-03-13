#include "entrance.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

Entrance::Entrance(const wstring& _name) : Category(_name)
{
    clearValue();
}

void Entrance::clearValue() {
    outletCoordinates.clear();
    inletCoordinates.clear();
    entranceComment = L"";
}

void Entrance::writeValue(QXmlStreamWriter &writer)
{
    updateValue();
    writer.writeStartElement("Entrance");
        writer.writeStartElement("OutletCoordinates");
        for (vector< vector<double> >::iterator it = outletCoordinates.begin();
             it != outletCoordinates.end(); it++) {
            double x = (*it).at(0);
            double y = (*it).at(1);
            double z = (*it).at(2);
            writer.writeStartElement("Value");
            writer.writeAttribute("x", QString::number(x));
            writer.writeAttribute("y", QString::number(y));
            writer.writeAttribute("z", QString::number(z));
            writer.writeEndElement();
        }
        writer.writeEndElement();
        writer.writeStartElement("InletCoordinates");
        for (vector< vector<double> >::iterator it = inletCoordinates.begin();
             it != inletCoordinates.end(); it++) {
            double x = (*it).at(0);
            double y = (*it).at(1);
            double z = (*it).at(2);
            writer.writeStartElement("Value");
            writer.writeAttribute("x", QString::number(x));
            writer.writeAttribute("y", QString::number(y));
            writer.writeAttribute("z", QString::number(z));
            writer.writeEndElement();
        }
        writer.writeEndElement();
        writer.writeStartElement("EntranceComment");
        writer.writeAttribute("value", QString::fromStdWString(entranceComment));
        writer.writeEndElement();
    writer.writeEndElement();
}

void Entrance::loadValue(const QDomElement& element) {
    QDomNode child = element.firstChild();
    QDomNode child1;
    string tagName;
    string tagName1;
    while (!child.isNull()) {
        tagName = child.toElement().tagName().toStdString();
        if (tagName == "OutletCoordinates" || tagName == "InletCoordinates") {
            child1 = child.toElement().firstChild();
            while (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                if (tagName1 == "Value") {
                    vector<double> p;
                    p.push_back(child1.toElement().attribute("x").toDouble());
                    p.push_back(child1.toElement().attribute("y").toDouble());
                    p.push_back(child1.toElement().attribute("z").toDouble());
                    if (tagName == "OutletCoordinates") {
                        outletCoordinates.push_back(p);
                    } else if (tagName == "InletCoordinates") {
                        inletCoordinates.push_back(p);
                    }
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "EntranceComment") {
            entranceComment = child.toElement().attribute("value").toStdWString();
        }
        child = child.nextSibling();
    }
}

void Entrance::updateGui(void) {
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;
    MainWindow::UpdateTable(outletCoordinates, ui->tableWidget_outletCoordinates);
    MainWindow::UpdateTable(inletCoordinates, ui->tableWidget_inletCoordinates);

    QString text = QString::fromStdWString(entranceComment);
    ui->plainTextEdit_inoutIntroduction->setPlainText(text);
}

void Entrance::updateValue(void) {
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;

    MainWindow::UpdateData(ui->tableWidget_outletCoordinates, outletCoordinates);
    MainWindow::UpdateData(ui->tableWidget_inletCoordinates, inletCoordinates);

    entranceComment = ui->plainTextEdit_inoutIntroduction->toPlainText().toStdWString();
}
