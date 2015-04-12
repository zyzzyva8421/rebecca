#include "entrance.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

Entrance::Entrance(const wstring& _name) : Category(_name)
{
    clearValue();
}

void Entrance::clearValue() {
    boundaryRight = None;
    boundaryLeft = None;
    boundaryTop = None;
    boundaryBottom = None;
    boundaryFront = None;
    boundaryBack = None;
    entranceComment = L"";
}

void Entrance::writeValue(QXmlStreamWriter &writer)
{
    updateValue();
    writer.writeStartElement("Entrance");
        writer.writeStartElement("BoundaryRight");
        writer.writeAttribute("value", QString::number(boundaryRight));
        writer.writeEndElement();
        writer.writeStartElement("BoundaryLeft");
        writer.writeAttribute("value", QString::number(boundaryLeft));
        writer.writeEndElement();
        writer.writeStartElement("BoundaryTop");
        writer.writeAttribute("value", QString::number(boundaryTop));
        writer.writeEndElement();
        writer.writeStartElement("BoundaryBottom");
        writer.writeAttribute("value", QString::number(boundaryBottom));
        writer.writeEndElement();
        writer.writeStartElement("BoundaryFront");
        writer.writeAttribute("value", QString::number(boundaryFront));
        writer.writeEndElement();
        writer.writeStartElement("BoundaryBack");
        writer.writeAttribute("value", QString::number(boundaryBack));
        writer.writeEndElement();
        writer.writeStartElement("EntranceComment");
        writer.writeAttribute("value", QString::fromStdWString(entranceComment));
        writer.writeEndElement();
    writer.writeEndElement();
}

void Entrance::loadValue(const QDomElement& element) {
    QDomNode child = element.firstChild();
    string tagName;
    while (!child.isNull()) {
        tagName = child.toElement().tagName().toStdString();
        if (tagName == "BoundaryRight") {
            boundaryRight = (Type)(child.toElement().attribute("value").toInt());
        } else if (tagName == "BoundaryLeft") {
            boundaryLeft = (Type)(child.toElement().attribute("value").toInt());
        } else if (tagName == "BoundaryTop") {
            boundaryTop = (Type)(child.toElement().attribute("value").toInt());
        } else if (tagName == "BoundaryBottom") {
            boundaryBottom = (Type)(child.toElement().attribute("value").toInt());
        } else if (tagName == "BoundaryFront") {
            boundaryFront = (Type)(child.toElement().attribute("value").toInt());
        } else if (tagName == "BoundaryBack") {
            boundaryBack = (Type)(child.toElement().attribute("value").toInt());
        } else if (tagName == "EntranceComment") {
            entranceComment = child.toElement().attribute("value").toStdWString();
        }
        child = child.nextSibling();
    }
}

void Entrance::updateGui(void) {
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;
    switch (boundaryRight) {
    case None: {
        ui->radioButton_BoundaryRight0->click();
        break;
    }
    case Inlet: {
        ui->radioButton_BoundaryRight1->click();
        break;
    }
    case Outlet: {
        ui->radioButton_BoundaryRight2->click();
        break;
    }
    default: break;
    }
    switch (boundaryLeft) {
    case None: {
        ui->radioButton_BoundaryLeft0->click();
        break;
    }
    case Inlet: {
        ui->radioButton_BoundaryLeft1->click();
        break;
    }
    case Outlet: {
        ui->radioButton_BoundaryLeft2->click();
        break;
    }
    default: break;
    }
    switch (boundaryTop) {
    case None: {
        ui->radioButton_BoundaryTop0->click();
        break;
    }
    case Inlet: {
        ui->radioButton_BoundaryTop1->click();
        break;
    }
    case Outlet: {
        ui->radioButton_BoundaryTop2->click();
        break;
    }
    default: break;
    }
    switch (boundaryBottom) {
    case None: {
        ui->radioButton_BoundaryBottom0->click();
        break;
    }
    case Inlet: {
        ui->radioButton_BoundaryBottom1->click();
        break;
    }
    case Outlet: {
        ui->radioButton_BoundaryBottom2->click();
        break;
    }
    default: break;
    }
    switch (boundaryFront) {
    case None: {
        ui->radioButton_BoundaryFront0->click();
        break;
    }
    case Inlet: {
        ui->radioButton_BoundaryFront1->click();
        break;
    }
    case Outlet: {
        ui->radioButton_BoundaryFront2->click();
        break;
    }
    default: break;
    }
    switch (boundaryBack) {
    case None: {
        ui->radioButton_BoundaryBack0->click();
        break;
    }
    case Inlet: {
        ui->radioButton_BoundaryBack1->click();
        break;
    }
    case Outlet: {
        ui->radioButton_BoundaryBack2->click();
        break;
    }
    default: break;
    }

    QString text = QString::fromStdWString(entranceComment);
    ui->plainTextEdit_inoutIntroduction->setPlainText(text);
}

void Entrance::updateValue(void) {
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;

    QButtonGroup *group = MainWindow::CurrentWindow->get_buttonGroup_BoundaryRight();
    boundaryRight = (Type)(group->checkedId());
    group = MainWindow::CurrentWindow->get_buttonGroup_BoundaryLeft();
    boundaryLeft = (Type)(group->checkedId());
    group = MainWindow::CurrentWindow->get_buttonGroup_BoundaryTop();
    boundaryTop = (Type)(group->checkedId());
    group = MainWindow::CurrentWindow->get_buttonGroup_BoundaryBottom();
    boundaryBottom = (Type)(group->checkedId());
    group = MainWindow::CurrentWindow->get_buttonGroup_BoundaryFront();
    boundaryFront = (Type)(group->checkedId());
    group = MainWindow::CurrentWindow->get_buttonGroup_BoundaryBack();
    boundaryBack = (Type)(group->checkedId());

    entranceComment = ui->plainTextEdit_inoutIntroduction->toPlainText().toStdWString();
}
