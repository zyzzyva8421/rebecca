#include "information.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"

Information::Information(const string& _name) : Category(_name)
{
    castingNumber = 0;
    basedProjectOn = false;
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
