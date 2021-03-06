#include "computing.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

Computing::Computing(const wstring& _name) : Category(_name)
{
    clearValue();
}

void Computing::clearValue() {
    maxAdaptedTimeStep = 0.0;
    iterationMaximumRelativeError = 0.0;
    iterationMaximumStepCount = 0;
    iterationMinimumStepCount = 0;
    terminationCondition = TerminationEndTimeOn;
    terminationEndTimeOnConf = 0.0;
    terminationMaximumStepOnConf = 0;
    parallelMethod = ParallelAdaptedOn;
    parallelFixedOnConf = 0;
    computingComment = L"";
}

void Computing::writeValue(QXmlStreamWriter &writer)
{
    updateValue();
    writer.writeStartElement("Computing");
        writer.writeStartElement("MaxAdaptedTimeStep");
        writer.writeAttribute("value", QString::number(maxAdaptedTimeStep));
        writer.writeEndElement();
        writer.writeStartElement("IterationMaximumRelativeError");
        writer.writeAttribute("value", QString::number(iterationMaximumRelativeError));
        writer.writeEndElement();
        writer.writeStartElement("IterationMaximumStepCount");
        writer.writeAttribute("value", QString::number(iterationMaximumStepCount));
        writer.writeEndElement();
        writer.writeStartElement("IterationMinimumStepCount");
        writer.writeAttribute("value", QString::number(iterationMinimumStepCount));
        writer.writeEndElement();
        writer.writeStartElement("TerminationCondition");
            writer.writeStartElement("TerminationEndTimeOn");
            writer.writeAttribute("value", QString::number((terminationCondition==TerminationEndTimeOn)?1:0));
            writer.writeEndElement();
            writer.writeStartElement("TerminationMaximumStepOn");
            writer.writeAttribute("value", QString::number((terminationCondition==TerminationMaximumStepOn)?1:0));
            writer.writeEndElement();
        writer.writeEndElement();
        writer.writeStartElement("TerminationEndTimeOnConf");
            writer.writeStartElement("Value");
            writer.writeAttribute("value", QString::number(terminationEndTimeOnConf));
            writer.writeEndElement();
        writer.writeEndElement();
        writer.writeStartElement("TerminationMaximumStepOnConf");
            writer.writeStartElement("Value");
            writer.writeAttribute("value", QString::number(terminationMaximumStepOnConf));
            writer.writeEndElement();
        writer.writeEndElement();
        writer.writeStartElement("ParallelMethod");
            writer.writeStartElement("ParallelAdaptedOn");
            writer.writeAttribute("value", QString::number((parallelMethod==ParallelAdaptedOn)?1:0));
            writer.writeEndElement();
            writer.writeStartElement("ParallelFixedOn");
            writer.writeAttribute("value", QString::number((parallelMethod==ParallelFixedOn)?1:0));
            writer.writeEndElement();
        writer.writeEndElement();
        writer.writeStartElement("ParallelFixedOnConf");
            writer.writeStartElement("Value");
            writer.writeAttribute("value", QString::number(parallelFixedOnConf));
            writer.writeEndElement();
        writer.writeEndElement();
        writer.writeStartElement("ComputingComment");
        writer.writeAttribute("value", QString::fromStdWString(computingComment));
        writer.writeEndElement();
    writer.writeEndElement();
}
void Computing::loadValue(const QDomElement &element)
{
    QDomNode child = element.firstChild();
    QDomNode child1;
    string tagName;
    string tagName1;
    while (!child.isNull()) {
        tagName = child.toElement().tagName().toStdString();
        if (tagName == "MaxAdaptedTimeStep") {
            maxAdaptedTimeStep = child.toElement().attribute("value").toDouble();
        } else if (tagName == "IterationMaximumRelativeError") {
            iterationMaximumRelativeError = child.toElement().attribute("value").toDouble();
        } else if (tagName == "IterationMaximumStepCount") {
            iterationMaximumStepCount = child.toElement().attribute("value").toInt();
        } else if (tagName == "IterationMinimumStepCount") {
            iterationMinimumStepCount = child.toElement().attribute("value").toInt();
        } else if (tagName == "TerminationCondition") {
            child1 = child.toElement().firstChild();
            while (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                int value = child1.toElement().attribute("value").toInt();
                if (value != 0) {
                    if (tagName1 == "TerminationEndTimeOn") {
                        terminationCondition = TerminationEndTimeOn;
                        break;
                    } else if (tagName1 == "TerminationMaximumStepOn") {
                        terminationCondition = TerminationMaximumStepOn;
                        break;
                    }
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "TerminationEndTimeOnConf") {
            child1 = child.toElement().firstChild();
            while (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                if (tagName1 == "Value") {
                    terminationEndTimeOnConf = child1.toElement().attribute("value").toDouble();
                    break;
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "TerminationMaximumStepOnConf") {
            child1 = child.toElement().firstChild();
            while (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                if (tagName1 == "Value") {
                    terminationMaximumStepOnConf = child1.toElement().attribute("value").toInt();
                    break;
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "ParallelMethod") {
            child1 = child.toElement().firstChild();
            while (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                int value = child1.toElement().attribute("value").toInt();
                if (value != 0) {
                    if (tagName1 == "ParallelAdaptedOn") {
                        parallelMethod = ParallelAdaptedOn;
                        break;
                    } else if (tagName1 == "ParallelFixedOn") {
                        parallelMethod = ParallelFixedOn;
                        break;
                    }
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "ParallelFixedOnConf") {
            child1 = child.toElement().firstChild();
            while (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                if (tagName1 == "Value") {
                    parallelFixedOnConf = child1.toElement().attribute("value").toInt();
                    break;
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "ComputingComment") {
            computingComment = child.toElement().attribute("value").toStdWString();
        }
        child = child.nextSibling();
    }
}

void Computing::updateGui(void)
{
    QString text;
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;

    text = QString::number(maxAdaptedTimeStep);
    ui->lineEdit_MaxAdaptedTimeStep->setText(text);

    text = QString::number(iterationMaximumRelativeError);
    ui->lineEdit_IterationMaximumRelativeError->setText(text);

    text = QString::number(iterationMaximumStepCount);
    ui->lineEdit_IterationMaximumStepCount->setText(text);

    text = QString::number(iterationMinimumStepCount);
    ui->lineEdit_IterationMinimumStepCount->setText(text);

    switch (terminationCondition) {
    case TerminationEndTimeOn : {
        ui->radioButton_TerminationEndTimeOn->click();
        break;
    }
    case TerminationMaximumStepOn: {
        ui->radioButton_TerminationMaximumStepOn->click();
        break;
    }
    default: break;
    }

    text = QString::number(terminationEndTimeOnConf);
    ui->lineEdit_TerminationEndTimeOnConf->setText(text);

    text = QString::number(terminationMaximumStepOnConf);
    ui->lineEdit_TerminationMaximumStepOnConf->setText(text);

    switch (parallelMethod) {
    case ParallelAdaptedOn: {
        ui->radioButton_ParallelAdaptedOn->click();
        break;
    }
    case ParallelFixedOn: {
        ui->radioButton_ParallelFixedOn->click();
        break;
    }
    default: break;
    }

    text = QString::number(parallelFixedOnConf);
    ui->lineEdit_ParallelFixedOnConf->setText(text);

    text = QString::fromStdWString(computingComment);
    ui->plainTextEdit_computingIntroduction->setPlainText(text);
}

void Computing::updateValue(void)
{
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;

    MainWindow *window = MainWindow::CurrentWindow;
    QButtonGroup *group = NULL;
    maxAdaptedTimeStep = ui->lineEdit_MaxAdaptedTimeStep->text().toDouble();
    iterationMaximumRelativeError = ui->lineEdit_IterationMaximumRelativeError->text().toDouble();
    iterationMaximumStepCount = ui->lineEdit_IterationMaximumStepCount->text().toInt();
    iterationMinimumStepCount = ui->lineEdit_IterationMinimumStepCount->text().toInt();
    group = window->get_buttonGroup_terminationCondition();
    terminationCondition = (TerminationCondition)(group->checkedId());
    terminationEndTimeOnConf = ui->lineEdit_TerminationEndTimeOnConf->text().toDouble();
    terminationMaximumStepOnConf = ui->lineEdit_TerminationMaximumStepOnConf->text().toInt();
    group = window->get_buttonGroup_parallelMethod();
    parallelMethod = (ParallelMethod)(group->checkedId());
    parallelFixedOnConf = ui->lineEdit_ParallelFixedOnConf->text().toInt();
    computingComment = ui->plainTextEdit_computingIntroduction->toPlainText().toStdWString();
}
