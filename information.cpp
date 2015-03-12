#include "information.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "dialog.h"
#include "ui_dialog.h"

Information::Information(const wstring& _name) : Category(_name)
{
    clearValue();
}

void Information::clearValue()
{
    castingName = L"";
    castingNumber = 0;
    createdAuthor = L"";
    createdTime = QDateTime::currentDateTime().toString().toStdString();
    modifiedAuthor = L"";
    modifiedTime = QDateTime::currentDateTime().toString().toStdString();
    basedProjectOn = false;
    basedProjectPath = L"";
    projectLibaryPath = L"";
    projectPath = L"";
    projectComment = L"";

}

void Information::loadValue(const QDomElement& element)
{
    QDomNode child = element.firstChild();
    while (!child.isNull()) {
        string tagName = child.toElement().tagName().toStdString();
        if (tagName == "CastingName") {
            castingName = child.toElement().attribute("value").toStdWString();
        } else if (tagName == "CastingNumber") {
            castingNumber = child.toElement().attribute("value").toInt();
        } else if (tagName == "CreatedAuthor") {
            createdAuthor = child.toElement().attribute("value").toStdWString();
        } else if (tagName == "CreatedTime") {
            createdTime = child.toElement().attribute("value").toStdString();
        } else if (tagName == "ModifiedAuthor") {
            modifiedAuthor = child.toElement().attribute("value").toStdWString();
        } else if (tagName == "ModifiedTime") {
            modifiedTime = child.toElement().attribute("value").toStdString();
        } else if (tagName == "BasedProjectOn") {
            string v = child.toElement().attribute("value").toStdString();
            basedProjectOn = (v == "1")?true:false;
        } else if (tagName == "BasedProjectPath") {
            basedProjectPath = child.toElement().attribute("value").toStdWString();
        } else if (tagName == "ProjectLibaryPath") {
            projectLibaryPath = child.toElement().attribute("value").toStdWString();
        } else if (tagName == "ProjectPath") {
            projectPath = child.toElement().attribute("value").toStdWString();
        } else if (tagName == "ProjectComment") {
            projectComment = child.toElement().attribute("value").toStdWString();
        }
        child = child.nextSibling();
    }
}

void* Information::getDialogUi()
{
    if (Dialog::CurrentDialog) return Dialog::CurrentDialog->getUi();
    else return NULL;
}

void Information::updateDialogGui(void) {
    Ui::Dialog *ui = (Ui::Dialog*)getDialogUi();
    if (ui == NULL) return;
    QString text;
    QDateTime time;

    text = QString::fromStdWString(castingName);
    ui->lineEdit_CastingName->setText(text);

    ui->spinBox_CastingNumber->setValue(castingNumber);

    text = QString::fromStdWString(createdAuthor);
    ui->lineEdit_CreatedAuthor->setText(text);

    text = QString::fromStdString(createdTime);
    time = QDateTime::fromString(text, Qt::ISODate);
    ui->timeEdit_CreatedTime->setDateTime(time);

    text = QString::fromStdWString(modifiedAuthor);
    ui->lineEdit_ModifiedAuthor->setText(text);

    text = QString::fromStdString(modifiedTime);
    time = QDateTime::fromString(text, Qt::ISODate);
    ui->timeEdit_ModifiedTime->setDateTime(time);

    text = QString::fromStdWString(projectComment);
    ui->plainTextEdit_ProjectComment->setPlainText(text);
}

void Information::updateValueFromDialog(void) {
    QDateTime time;
    Ui::Dialog *ui = (Ui::Dialog*)getDialogUi();
    if (ui == NULL) return;

    castingName = ui->lineEdit_CastingName->text().toStdWString();
    castingNumber = ui->spinBox_CastingNumber->value();
    createdAuthor = ui->lineEdit_CreatedAuthor->text().toStdWString();
    time = ui->timeEdit_CreatedTime->dateTime();
    createdTime = time.toString(Qt::ISODate).toStdString();
    modifiedAuthor = ui->lineEdit_ModifiedAuthor->text().toStdWString();
    time = ui->timeEdit_ModifiedTime->dateTime();
    modifiedTime = time.toString(Qt::ISODate).toStdString();
    basedProjectOn = ui->checkBox_BasedProjectOn->checkState();
    basedProjectPath =  ui->lineEdit_BasedProjectPath->text().toStdWString();
    projectLibaryPath = ui->lineEdit_ProjectLibaryPath->text().toStdWString();
    projectPath = ui->lineEdit_ProjectPath->text().toStdWString();
    projectComment = ui->plainTextEdit_ProjectComment->toPlainText().toStdWString();
}

void Information::updateGui(void)
{
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;
    QString text;
    QDateTime time;

    text = QString::fromStdWString(castingName);
    ui->lineEdit_castName->setText(text);

    ui->spinBox_castNumber->setValue(castingNumber);

    text = QString::fromStdWString(createdAuthor);
    ui->lineEdit_creator->setText(text);

    text = QString::fromStdString(createdTime);
    time = QDateTime::fromString(text, Qt::ISODate);
    ui->dateTimeEdit_createTime->setDateTime(time);

    text = QString::fromStdWString(modifiedAuthor);
    ui->lineEdit_amender->setText(text);

    text = QString::fromStdString(modifiedTime);
    time = QDateTime::fromString(text, Qt::ISODate);
    ui->dateTimeEdit_amendTime->setDateTime(time);

    ui->checkBox_basedOnExistingProject->setChecked(basedProjectOn);

    text = QString::fromStdWString(basedProjectPath);
    ui->lineEdit_existingProjectPath->setText(text);

    text = QString::fromStdWString(projectLibaryPath);
    ui->lineEdit_projectLibrary->setText(text);

    text = QString::fromStdWString(projectPath);
    ui->lineEdit_projectPath->setText(text);

    text = QString::fromStdWString(projectComment);
    ui->plainTextEdit_projectDescription->setPlainText(text);
}

void Information::updateValue(void)
{
    QDateTime time;
    Ui::MainWindow *ui = (Ui::MainWindow*)getUi();
    if (ui == NULL) return;

    castingName = ui->lineEdit_castName->text().toStdWString();
    castingNumber = ui->spinBox_castNumber->value();
    createdAuthor = ui->lineEdit_creator->text().toStdWString();
    time = ui->dateTimeEdit_createTime->dateTime();
    createdTime = time.toString(Qt::ISODate).toStdString();
    modifiedAuthor = ui->lineEdit_amender->text().toStdWString();
    time = ui->dateTimeEdit_amendTime->dateTime();
    modifiedTime = time.toString(Qt::ISODate).toStdString();
    basedProjectOn = ui->checkBox_basedOnExistingProject->checkState();
    basedProjectPath =  ui->lineEdit_existingProjectPath->text().toStdWString();
    projectLibaryPath = ui->lineEdit_projectLibrary->text().toStdWString();
    projectPath = ui->lineEdit_projectPath->text().toStdWString();
    projectComment = ui->plainTextEdit_projectDescription->toPlainText().toStdWString();
}
