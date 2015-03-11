#include "mold.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

Mold::Mold(const string &_name) : Category(_name)
{

}

Mold::~Mold() {
    vector<MoldConfiguration*>::iterator it;
    for (it = molds.begin(); it != molds.end(); it++) {
        MoldConfiguration *conf = (*it);
        delete conf;
        conf = NULL;
    }
    molds.clear();
}

void Mold::loadValue(const QDomElement& element)
{
    QDomNode child = element.firstChild();
    string tagName;
    MoldConfiguration *conf;
    while (!child.isNull()) {
        tagName = child.toElement().tagName().toStdString();
        if (tagName == "Stl") {
            conf = new MoldConfiguration("Stl");
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
    ui->listWidget_addedStlMolds->clear();
    vector<MoldConfiguration*>::iterator it;
    MoldConfiguration *conf = NULL;
    for (it = molds.begin(); it != molds.end(); it++) {
        conf = (*it);
        QString str = QString::fromStdString(conf->getName());
        ui->listWidget_addedStlMolds->addItem(str);
    }
    ui->listWidget_addedStlMolds->setCurrentItem(ui->listWidget_addedStlMolds->item(0));

}

void Mold::updateValue(void)
{
}

MoldConfiguration *Mold::getMoldConfiguration(const string& stl)
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

MoldConfiguration::MoldConfiguration(const string& _name) : Category(_name)
{
    moldMaterialInitialTemperature = 0.0;
    moldHeatExchangeCoefficient = 0.0;
    moldSurfaceRoughness = NoSlipOn;
    moldFunction = CoverOn;
}

void MoldConfiguration::loadValue(const QDomElement& element)
{
    string name = element.attribute("id").toStdString();
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

    text = QString::fromStdString(getName());
    ui->lineEdit_inProjectName->setText(text);

    text = QString::fromStdWString(originalStlPath);
    ui->lineEdit_originalStlPath->setText(text);

    text = QString::fromStdString(moldMaterialId);
    ui->lineEdit_moldMaterialId->setText(text);

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

    string name = ui->lineEdit_inProjectName->text().toStdString();
    setName(name);

    originalStlPath = ui->lineEdit_originalStlPath->text().toStdWString();

    moldMaterialId = ui->lineEdit_moldMaterialId->text().toStdString();

    moldMaterialInitialTemperature = ui->lineEdit_moldMaterialInitialTemperature->text().toDouble();

    moldHeatExchangeCoefficient = ui->lineEdit_moldHeatExchangeCoefficient->text().toDouble();

    QButtonGroup *group = MainWindow::CurrentWindow->get_buttonGroup_moldSurfaceRoughness();
    moldSurfaceRoughness = (MoldSurfaceRoughness)(group->checkedId());

    moldFunction = (MoldFunction)(ui->comboBox_moldFunction->currentIndex());

    moldComment = ui->plainTextEdit_moldComment->toPlainText().toStdWString();
}
