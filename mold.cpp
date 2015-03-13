#include "mold.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

Mold::Mold(const wstring &_name) : Category(_name)
{
    clearValue();
}

Mold::~Mold() {
    clearValue();
}

void Mold::clearValue()
{
    vector<MoldConfiguration*>::iterator it;
    for (it = molds.begin(); it != molds.end(); it++) {
        MoldConfiguration *conf = (*it);
        delete conf;
        conf = NULL;
    }
    molds.clear();
}

void Mold::writeValue(QXmlStreamWriter &writer)
{
    updateValue();
    writer.writeStartElement("Mold");
    for (vector<MoldConfiguration*>::iterator it = molds.begin(); it != molds.end(); it++) {
        (*it)->writeValue(writer);
    }
    writer.writeEndElement();
}

void Mold::loadValue(const QDomElement& element)
{
    QDomNode child = element.firstChild();
    string tagName;
    MoldConfiguration *conf;
    while (!child.isNull()) {
        tagName = child.toElement().tagName().toStdString();
        if (tagName == "Stl") {
            conf = new MoldConfiguration(L"Stl");
            molds.push_back(conf);
            conf->loadValue(child.toElement());
        }
        child = child.nextSibling();
    }
}

void Mold::updateGui(void)
{
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;
    int index= ui->listWidget_addedStlMolds->currentRow();
    ui->listWidget_addedStlMolds->clear();
    vector<MoldConfiguration*>::iterator it;
    MoldConfiguration *conf = NULL;
    for (it = molds.begin(); it != molds.end(); it++) {
        conf = (*it);
        QString str = QString::fromStdWString(conf->getName());
        ui->listWidget_addedStlMolds->addItem(str);
    }
    if (index < 0 || index >= (int)molds.size()) index = 0;
    ui->listWidget_addedStlMolds->setCurrentRow(index);

}

void Mold::updateValue(void)
{
}

void Mold::deleteConfiguration(const wstring &name)
{
    vector<MoldConfiguration*>::iterator it;
    MoldConfiguration *conf = NULL;
    for (it = molds.begin(); it != molds.end(); it++) {
        conf = (*it);
        if (conf && conf->getName() == name) {
            break;
        }
    }
    if (it != molds.end()) {
        molds.erase(it);
        delete conf;
        conf = NULL;
        updateGui();
    }
}

void Mold::modifyConfiguration(const wstring &name)
{
    MoldConfiguration *config = getMoldConfiguration(name);
    config->updateValue();
    updateGui();
}

void Mold::addConfiguration(const wstring &name)
{
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;
    MoldConfiguration *config = new MoldConfiguration(name);
    molds.push_back(config);
    updateGui();
    ui->listWidget_addedStlMolds->setCurrentRow((int)molds.size()-1);
    config->updateGui();
}

MoldConfiguration *Mold::getMoldConfiguration(const wstring& stl)
{
    vector<MoldConfiguration*>::iterator it;
    MoldConfiguration *conf = NULL;
    for (it = molds.begin(); it != molds.end(); it++) {
        conf = (*it);
        if (conf && conf->getName() == stl) {
            return conf;
        }
    }
    return NULL;
}

MoldConfiguration::MoldConfiguration(const wstring& _name) : Category(_name)
{
    clearValue();
}

void MoldConfiguration::clearValue()
{
    originalStlPath = L"";
    moldMaterialId = "";
    moldMaterialInitialTemperature = 0.0;
    moldHeatExchangeCoefficient = 0.0;
    moldSurfaceRoughness = NoSlipOn;
    moldFunction = CoverOn;
    moldComment = L"";
}

void MoldConfiguration::writeValue(QXmlStreamWriter &writer)
{
    writer.writeStartElement("Stl");
    writer.writeAttribute("id", QString::fromStdWString(getName()));
        writer.writeStartElement("OriginalStlPath");
        writer.writeAttribute("value", QString::fromStdWString(originalStlPath));
        writer.writeEndElement();
        writer.writeStartElement("MoldMaterialId");
        writer.writeAttribute("value", QString::fromStdString(moldMaterialId));
        writer.writeEndElement();
        writer.writeStartElement("MoldMaterialInitialTemperature");
        writer.writeAttribute("value", QString::number(moldMaterialInitialTemperature));
        writer.writeEndElement();
        writer.writeStartElement("MoldHeatExchangeCoefficient");
        writer.writeAttribute("value", QString::number(moldHeatExchangeCoefficient));
        writer.writeEndElement();
        writer.writeStartElement("MoldSurfaceRoughness");
            writer.writeStartElement("NoSlipOn");
            writer.writeAttribute("value", QString::number((moldSurfaceRoughness==NoSlipOn)?1:0));
            writer.writeEndElement();
            writer.writeStartElement("FreeSlipOn");
            writer.writeAttribute("value", QString::number((moldSurfaceRoughness==FreeSlipOn)?1:0));
            writer.writeEndElement();
        writer.writeEndElement();
        writer.writeStartElement("MoldFunction");
            writer.writeStartElement("CoverOn");
            writer.writeAttribute("value", QString::number((moldFunction==CoverOn)?1:0));
            writer.writeEndElement();
            writer.writeStartElement("MovingOn");
            writer.writeAttribute("value", QString::number((moldFunction==MovingOn)?1:0));
            writer.writeEndElement();
            writer.writeStartElement("SprueOn");
            writer.writeAttribute("value", QString::number((moldFunction==SprueOn)?1:0));
            writer.writeEndElement();
            writer.writeStartElement("RiserOn");
            writer.writeAttribute("value", QString::number((moldFunction==RiserOn)?1:0));
            writer.writeEndElement();
            writer.writeStartElement("CoreOn");
            writer.writeAttribute("value", QString::number((moldFunction==CoreOn)?1:0));
            writer.writeEndElement();
        writer.writeEndElement();
        writer.writeStartElement("MoldComment");
        writer.writeAttribute("value", QString::fromStdWString(moldComment));
        writer.writeEndElement();
    writer.writeEndElement();
}

void MoldConfiguration::loadValue(const QDomElement& element)
{
    wstring name = element.attribute("id").toStdWString();
    setName(name);
    QDomNode child = element.firstChild();
    QDomNode child1;
    string tagName;
    string tagName1;
    while (!child.isNull()) {
        tagName = child.toElement().tagName().toStdString();
        if (tagName == "OriginalStlPath") {
            originalStlPath = child.toElement().attribute("value").toStdWString();
        } else if (tagName == "MoldMaterialId") {
            moldMaterialId = child.toElement().attribute("value").toStdString();
        } else if (tagName == "MoldMaterialInitialTemperature") {
            moldMaterialInitialTemperature = child.toElement().attribute("value").toDouble();
        } else if (tagName == "MoldHeatExchangeCoefficient") {
            moldHeatExchangeCoefficient = child.toElement().attribute("value").toDouble();
        } else if (tagName == "MoldSurfaceRoughness") {
            child1 = child.toElement().firstChild();
            while (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                int value = child1.toElement().attribute("value").toInt();
                if (value != 0) {
                    if (tagName1 == "NoSlipOn") {
                        moldSurfaceRoughness = NoSlipOn;
                        break;
                    } else if (tagName1 == "FreeSlipOn") {
                        moldSurfaceRoughness = FreeSlipOn;
                        break;
                    }
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "MoldFunction") {
            child1 = child.toElement().firstChild();
            while (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                int value = child1.toElement().attribute("value").toInt();
                if (value != 0) {
                    if (tagName1 == "CoverOn") {
                        moldFunction = CoverOn;
                        break;
                    } else if (tagName1 == "MovingOn") {
                        moldFunction = MovingOn;
                        break;
                    } else if (tagName1 == "SprueOn") {
                        moldFunction = SprueOn;
                        break;
                    } else if (tagName1 == "RiserOn") {
                        moldFunction = RiserOn;
                    } else if (tagName1 == "CoreOn") {
                        moldFunction = CoreOn;
                    }
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "MoldComment") {
            moldComment = child.toElement().attribute("value").toStdWString();
        }
        child = child.nextSibling();
    }
}

void MoldConfiguration::updateGui(void)
{
    QString text;
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;

    text = QString::fromStdWString(getName());
    ui->lineEdit_inProjectName->setText(text);

    text = QString::fromStdWString(originalStlPath);
    ui->lineEdit_originalStlPath->setText(text);

    text = QString::fromStdString(moldMaterialId);
    ui->comboBox_MoldMaterialId->clear();
    ui->comboBox_MoldMaterialId->addItem(text);
    ui->comboBox_MoldMaterialId->setCurrentIndex(0);

    text = QString::number(moldMaterialInitialTemperature);
    ui->lineEdit_moldMaterialInitialTemperature->setText(text);

    text = QString::number(moldHeatExchangeCoefficient);
    ui->lineEdit_moldHeatExchangeCoefficient->setText(text);

    switch (moldSurfaceRoughness) {
    case NoSlipOn: {
        ui->radioButton_noSlipOn->click();
        break;
    }
    case FreeSlipOn: {
        ui->radioButton_freeSlipOn->click();
        break;
    }
    default: break;
    }

    ui->comboBox_moldFunction->setCurrentIndex(moldFunction);

    text = QString::fromStdWString(moldComment);
    ui->plainTextEdit_moldComment->setPlainText(text);
}

void MoldConfiguration::updateValue(void)
{
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;

    wstring name = ui->lineEdit_inProjectName->text().toStdWString();
    setName(name);

    originalStlPath = ui->lineEdit_originalStlPath->text().toStdWString();

    moldMaterialId = ui->comboBox_MoldMaterialId->currentText().toStdString();

    moldMaterialInitialTemperature = ui->lineEdit_moldMaterialInitialTemperature->text().toDouble();

    moldHeatExchangeCoefficient = ui->lineEdit_moldHeatExchangeCoefficient->text().toDouble();

    QButtonGroup *group = MainWindow::CurrentWindow->get_buttonGroup_moldSurfaceRoughness();
    moldSurfaceRoughness = (MoldSurfaceRoughness)(group->checkedId());

    moldFunction = (MoldFunction)(ui->comboBox_moldFunction->currentIndex());

    moldComment = ui->plainTextEdit_moldComment->toPlainText().toStdWString();
}
