#include "output.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"

Output::Output(const wstring& _name) : Category(_name)
{
    clearValue();
}

void Output::clearValue()
{
    outputMethod = OutputIntervalTimeOn;
    outputIntervalTimeOnConf = 0.0;
    outputIntervalStepOnConf = 0;
    loggingMethod = LoggingIntervalTimeOn;
    loggingIntervalTimeOnConf = 0.0;
    loggingIntervalStepOnConf = 0;
    loggingCurrentStepOn = false;
    loggingCurrentTimeOn = false;
    loggingCurrentFillingRateOn = false;
    loggingCurrentTemperatureOn = false;
    outputComment = L"";
}

void Output::writeValue(QXmlStreamWriter &writer)
{
    updateValue();
    writer.writeStartElement("Output");
    writer.writeStartElement("OutputMethod");
        writer.writeStartElement("OutputIntervalTimeOn");
        writer.writeAttribute("value", QString::number((outputMethod==OutputIntervalTimeOn)?1:0));
        writer.writeEndElement();
        writer.writeStartElement("OutputIntervalStepOn");
        writer.writeAttribute("value", QString::number((outputMethod==OutputIntervalStepOn)?1:0));
        writer.writeEndElement();
    writer.writeEndElement();
    writer.writeStartElement("OutputIntervalTimeOnConf");
        writer.writeStartElement("Value");
        writer.writeAttribute("value", QString::number(outputIntervalTimeOnConf));
        writer.writeEndElement();
    writer.writeEndElement();
    writer.writeStartElement("OutputIntervalStepOnConf");
        writer.writeStartElement("Value");
        writer.writeAttribute("value", QString::number(outputIntervalStepOnConf));
        writer.writeEndElement();
    writer.writeEndElement();
    writer.writeStartElement("LoggingMethod");
        writer.writeStartElement("LoggingIntervalTimeOn");
        writer.writeAttribute("value", QString::number((loggingMethod==LoggingIntervalTimeOn)?1:0));
        writer.writeEndElement();
        writer.writeStartElement("LoggingIntervalStepOn");
        writer.writeAttribute("value", QString::number((loggingMethod==LoggingIntervalStepOn)?1:0));
        writer.writeEndElement();
    writer.writeEndElement();
    writer.writeStartElement("LoggingIntervalTimeOnConf");
        writer.writeStartElement("Value");
        writer.writeAttribute("value", QString::number(loggingIntervalTimeOnConf));
        writer.writeEndElement();
    writer.writeEndElement();
    writer.writeStartElement("LoggingIntervalStepOnConf");
        writer.writeStartElement("Value");
        writer.writeAttribute("value", QString::number(loggingIntervalStepOnConf));
        writer.writeEndElement();
    writer.writeEndElement();
    writer.writeStartElement("LoggingCurrentStepOn");
    writer.writeAttribute("value", QString::number((loggingCurrentStepOn)?1:0));
    writer.writeEndElement();
    writer.writeStartElement("LoggingCurrentTimeOn");
    writer.writeAttribute("value", QString::number((loggingCurrentTimeOn)?1:0));
    writer.writeEndElement();
    writer.writeStartElement("LoggingCurrentFillingRateOn");
    writer.writeAttribute("value", QString::number((loggingCurrentFillingRateOn)?1:0));
    writer.writeEndElement();
    writer.writeStartElement("LoggingCurrentTemperatureOn");
    writer.writeAttribute("value", QString::number((loggingCurrentTemperatureOn)?1:0));
    writer.writeEndElement();
    writer.writeStartElement("OutputComment");
    writer.writeAttribute("value", QString::fromStdWString(outputComment));
    writer.writeEndElement();
    writer.writeEndElement();
}

void Output::loadValue(const QDomElement& element)
{
    QDomNode child = element.firstChild();
    QDomNode child1;
    string tagName;
    string tagName1;
    while (!child.isNull()) {
        tagName = child.toElement().tagName().toStdString();
        if (tagName == "OutputMethod") {
            child1 = child.toElement().firstChild();
            while (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                int value = child1.toElement().attribute("value").toInt();
                if (value != 0) {
                    if (tagName1 == "OutputIntervalTimeOn") {
                        outputMethod = OutputIntervalTimeOn;
                        break;
                    } else if (tagName1 == "OutputIntervalStepOn") {
                        outputMethod = OutputIntervalStepOn;
                        break;
                    }
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "OutputIntervalTimeOnConf") {
            child1 = child.toElement().firstChild();
            while (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                if (tagName1 == "Value") {
                    outputIntervalTimeOnConf = child1.toElement().attribute("value").toDouble();
                    break;
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "OutputIntervalStepOnConf") {
            child1 = child.toElement().firstChild();
            while (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                if (tagName1 == "Value") {
                    outputIntervalStepOnConf = child1.toElement().attribute("value").toInt();
                    break;
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "LoggingMethod") {
                child1 = child.toElement().firstChild();
                while (!child1.isNull()) {
                    tagName1 = child1.toElement().tagName().toStdString();
                    int value = child1.toElement().attribute("value").toInt();
                    if (value != 0) {
                        if (tagName1 == "LoggingIntervalTimeOn") {
                            loggingMethod = LoggingIntervalTimeOn;
                            break;
                        } else if (tagName1 == "LoggingIntervalStepOn") {
                            loggingMethod = LoggingIntervalStepOn;
                            break;
                        }
                    }
                    child1 = child1.nextSibling();
                }
            } else if (tagName == "LoggingIntervalTimeOnConf") {
                child1 = child.toElement().firstChild();
                while (!child1.isNull()) {
                    tagName1 = child1.toElement().tagName().toStdString();
                    if (tagName1 == "Value") {
                        loggingIntervalTimeOnConf = child1.toElement().attribute("value").toDouble();
                        break;
                    }
                    child1 = child1.nextSibling();
                }
            } else if (tagName == "LoggingIntervalStepOnConf") {
                child1 = child.toElement().firstChild();
                while (!child1.isNull()) {
                    tagName1 = child1.toElement().tagName().toStdString();
                    if (tagName1 == "Value") {
                        loggingIntervalStepOnConf = child1.toElement().attribute("value").toInt();
                        break;
                    }
                    child1 = child1.nextSibling();
                }
        } else if (tagName == "LoggingCurrentStepOn") {
            loggingCurrentStepOn = (child.toElement().attribute("value").toStdString()=="1")?true:false;
        } else if (tagName == "LoggingCurrentTimeOn") {
            loggingCurrentTimeOn = (child.toElement().attribute("value").toStdString()=="1")?true:false;
        } else if (tagName == "LoggingCurrentFillingRateOn") {
            loggingCurrentFillingRateOn = (child.toElement().attribute("value").toStdString()=="1")?true:false;
        } else if (tagName == "LoggingCurrentTemperatureOn") {
            loggingCurrentTemperatureOn = (child.toElement().attribute("value").toStdString()=="1")?true:false;
        } else if (tagName == "OutputComment") {
            outputComment = child.toElement().attribute("value").toStdWString();
        }
        child = child.nextSibling();
    }
}

void Output::updateGui(void)
{
    QString text;
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;

    switch (outputMethod) {
    case OutputIntervalTimeOn: {
        ui->radioButton_OutputIntervalTimeOn->click();
        break;
    }
    case OutputIntervalStepOn: {
        ui->radioButton_OutputIntervalStepOn->click();
        break;
    }
    default: break;
    }

    text = QString::number(outputIntervalTimeOnConf);
    ui->lineEdit_OutputIntervalTimeOnConf->setText(text);

    text = QString::number(outputIntervalStepOnConf);
    ui->lineEdit_OutputIntervalStepOnConf->setText(text);

    switch (loggingMethod) {
    case LoggingIntervalTimeOn: {
        ui->radioButton_LoggingIntervalTimeOn->click();
        break;
    }
    case LoggingIntervalStepOn: {
        ui->radioButton_LoggingIntervalStepOn->click();
        break;
    }
    default: break;
    }

    text = QString::number(loggingIntervalTimeOnConf);
    ui->lineEdit_LoggingIntervalTimeOnConf->setText(text);

    text = QString::number(loggingIntervalStepOnConf);
    ui->lineEdit_LoggingIntervalStepOnConf->setText(text);

    ui->checkBox_LoggingCurrentStepOn->setChecked(loggingCurrentStepOn);

    ui->checkBox_LoggingCurrentTimeOn->setChecked(loggingCurrentTimeOn);

    ui->checkBox_LoggingCurrentFillingRateOn->setChecked(loggingCurrentFillingRateOn);

    ui->checkBox_LoggingCurrentTemperatureOn->setChecked(loggingCurrentTemperatureOn);

    text = QString::fromStdWString(outputComment);
    ui->plainTextEdit_OutputComment->setPlainText(text);
}

void Output::updateValue(void)
{
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;

    MainWindow *window = MainWindow::CurrentWindow;
    QButtonGroup *group = window->get_buttonGroup_outputMethod();
    outputMethod = (OutputMethod)(group->checkedId());
    outputIntervalTimeOnConf = ui->lineEdit_OutputIntervalTimeOnConf->text().toDouble();
    outputIntervalStepOnConf = ui->lineEdit_OutputIntervalStepOnConf->text().toInt();
    group = window->get_buttonGroup_loggingMethod();
    loggingMethod = (LoggingMethod)(group->checkedId());
    loggingIntervalTimeOnConf = ui->lineEdit_LoggingIntervalTimeOnConf->text().toDouble();
    loggingIntervalStepOnConf = ui->lineEdit_LoggingIntervalStepOnConf->text().toInt();
    loggingCurrentStepOn = ui->checkBox_LoggingCurrentStepOn->isChecked();
    loggingCurrentTimeOn = ui->checkBox_LoggingCurrentTimeOn->isChecked();
    loggingCurrentFillingRateOn = ui->checkBox_LoggingCurrentFillingRateOn->isChecked();
    loggingCurrentTemperatureOn = ui->checkBox_LoggingCurrentTemperatureOn->isChecked();
    outputComment = ui->plainTextEdit_OutputComment->toPlainText().toStdWString();
}
