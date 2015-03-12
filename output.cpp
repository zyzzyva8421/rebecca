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
    loggingCurrentStepOn = false;
    loggingCurrentTimeOn = false;
    loggingCurrentFillingRateOn = false;
    loggingCurrentTemperatureOn = false;
    loggingFrequency = 0;
    outputComment = L"";
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
                    } else if (tagName1 == "OutputIntervalTimeOn") {
                        outputMethod = OutputIntervalTimeOn;
                        break;
                    }
                }
                child1 = child1.nextSibling();
            }
        } else if (tagName == "OutputIntervalTimeOnConf") {
            child1 = child.toElement().firstChild();
            if (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                if (tagName1 == "Value") {
                    outputIntervalTimeOnConf = child1.toElement().attribute("value").toDouble();
                }
            }
        } else if (tagName == "OutputIntervalStepOnConf") {
            child1 = child.toElement().firstChild();
            if (!child1.isNull()) {
                tagName1 = child1.toElement().tagName().toStdString();
                if (tagName1 == "Value") {
                    outputIntervalStepOnConf = child1.toElement().attribute("value").toInt();
                }
            }
        } else if (tagName == "LoggingCurrentStepOn") {
            loggingCurrentStepOn = (child.toElement().attribute("value").toStdString()=="1")?true:false;
        } else if (tagName == "LoggingCurrentTimeOn") {
            loggingCurrentTimeOn = (child.toElement().attribute("value").toStdString()=="1")?true:false;
        } else if (tagName == "LoggingCurrentFillingRateOn") {
            loggingCurrentFillingRateOn = (child.toElement().attribute("value").toStdString()=="1")?true:false;
        } else if (tagName == "LoggingCurrentTemperatureOn") {
            loggingCurrentTemperatureOn = (child.toElement().attribute("value").toStdString()=="1")?true:false;
        } else if (tagName == "LoggingFrequency") {
            loggingFrequency = child.toElement().attribute("value").toInt();
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

    ui->checkBox_LoggingCurrentStepOn->setChecked(loggingCurrentStepOn);

    ui->checkBox_LoggingCurrentTimeOn->setChecked(loggingCurrentTimeOn);

    ui->checkBox_LoggingCurrentFillingRateOn->setChecked(loggingCurrentFillingRateOn);

    ui->checkBox_LoggingCurrentTemperatureOn->setChecked(loggingCurrentTemperatureOn);

    text = QString::number(loggingFrequency);
    ui->lineEdit_LoggingFrequency->setText(text);

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
    loggingCurrentStepOn = ui->checkBox_LoggingCurrentStepOn->isChecked();
    loggingCurrentTimeOn = ui->checkBox_LoggingCurrentTimeOn->isChecked();
    loggingCurrentFillingRateOn = ui->checkBox_LoggingCurrentFillingRateOn->isChecked();
    loggingCurrentTemperatureOn = ui->checkBox_LoggingCurrentTemperatureOn->isChecked();
    loggingFrequency = ui->lineEdit_LoggingFrequency->text().toInt();
    outputComment = ui->plainTextEdit_OutputComment->toPlainText().toStdWString();
}
