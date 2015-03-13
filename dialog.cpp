#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"
#include "project.h"
#include "information.h"
#include <iostream>

Dialog *Dialog::CurrentDialog = NULL;
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    project = new Project(L"Project");
    ui->setupUi(this);
    ui->lineEdit_ProjectPath->setDisabled(true);
    ui->checkBox_BasedProjectOn->setChecked(false);
    on_checkBox_BasedProjectOn_stateChanged(Qt::Unchecked);
    ui->timeEdit_ModifiedTime->setDateTime(QDateTime::currentDateTime());
    CurrentDialog = this;
}

Dialog::~Dialog()
{
    delete ui;
    CurrentDialog = NULL;
}


void Dialog::on_checkBox_BasedProjectOn_stateChanged(int arg1)
{
    if (arg1 != 0) {
        ui->lineEdit_BasedProjectPath->setDisabled(false);
        ui->pushButton_BasedProjectPath->setDisabled(false);
        ui->lineEdit_CreatedAuthor->setDisabled(true);
        ui->timeEdit_CreatedTime->setDisabled(true);
        ui->lineEdit_ModifiedAuthor->setDisabled(false);
        ui->timeEdit_ModifiedTime->setDisabled(false);
    } else {
        ui->lineEdit_BasedProjectPath->setDisabled(true);
        ui->pushButton_BasedProjectPath->setDisabled(true);
        ui->lineEdit_CreatedAuthor->setDisabled(false);
        ui->timeEdit_CreatedTime->setDisabled(false);
        ui->lineEdit_ModifiedAuthor->setDisabled(true);
        ui->timeEdit_ModifiedTime->setDisabled(true);
    }
}

void Dialog::on_pushButton_BasedProjectPath_clicked()
{
    wstring title = L"项目路径";
    QString dir = MainWindow::openDirDialog(this, QString::fromStdWString(title), "./");
    ui->lineEdit_BasedProjectPath->setText(dir);
}

void Dialog::on_pushButton_ProjectLibaryPath_clicked()
{
    wstring title = L"项目路径";
    QString dir = MainWindow::openDirDialog(this, QString::fromStdWString(title), "./");
    ui->lineEdit_ProjectLibaryPath->setText(dir);
}

void Dialog::on_spinBox_CastingNumber_valueChanged(int arg1)
{
    QString castingName = ui->lineEdit_CastingName->text();
    QString projectLibaryPath = ui->lineEdit_ProjectLibaryPath->text();
    if (castingName.isEmpty() || projectLibaryPath.isEmpty()) {
        ui->lineEdit_ProjectPath->clear();
        return;
    }
    QString projectPath = projectLibaryPath+"/"
            +castingName+"/"+QString::number(arg1);
    ui->lineEdit_ProjectPath->setText(projectPath);
}

void Dialog::on_lineEdit_CastingName_textChanged(const QString &arg1)
{
    QString castingNumber = ui->spinBox_CastingNumber->text();
    QString projectLibaryPath = ui->lineEdit_ProjectLibaryPath->text();
    if (arg1.isEmpty() || projectLibaryPath.isEmpty()) {
        ui->lineEdit_ProjectPath->clear();
        return;
    }
    QString projectPath = projectLibaryPath+"/"
            +arg1+"/"+castingNumber;
    ui->lineEdit_ProjectPath->setText(projectPath);
}

void Dialog::on_lineEdit_ProjectLibaryPath_textChanged(const QString &arg1)
{
    QString castingName = ui->lineEdit_CastingName->text();
    QString castingNumber = ui->spinBox_CastingNumber->text();
    if (arg1.isEmpty() || castingName.isEmpty()) {
        ui->lineEdit_ProjectPath->clear();
        return;
    }
    QString projectPath = arg1+"/"
            +castingName+"/"+castingNumber;
    ui->lineEdit_ProjectPath->setText(projectPath);
}

void Dialog::on_lineEdit_BasedProjectPath_textChanged(const QString &arg1)
{
    QString filename = arg1+"/project.xml";
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Can not read file" << qPrintable(filename)
                     << "; " << qPrintable(file.errorString())
                        << std::endl;
        return;
    }
    if (project) {
        project->clearValue();
        project->loadConfigFile(filename);
        project->getInformation()->updateDialogGui();
    }
}

void Dialog::on_buttonBox_accepted()
{
    if (ui->lineEdit_ProjectPath->text().isEmpty()) {
        return;
    }
    MainWindow *main = (MainWindow*)parentWidget();
    if (main) {
        project->getInformation()->updateValueFromDialog();
        main->setProject(project);
    }
    close();
}

void Dialog::on_buttonBox_rejected()
{
    close();
}