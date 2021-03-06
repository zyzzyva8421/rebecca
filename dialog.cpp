#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "project.h"
#include "information.h"
#include "mold.h"
#include <QMessageBox>
#include <iostream>
#include <QDir>

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
    ui->timeEdit_CreatedTime->setDisabled(true);
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
        ui->lineEdit_ModifiedAuthor->setDisabled(false);
        ui->timeEdit_ModifiedTime->setDisabled(false);
    } else {
        ui->lineEdit_BasedProjectPath->setDisabled(true);
        ui->pushButton_BasedProjectPath->setDisabled(true);
        ui->lineEdit_CreatedAuthor->setDisabled(false);
        ui->lineEdit_ModifiedAuthor->setDisabled(true);
        ui->timeEdit_ModifiedTime->setDisabled(true);
        ui->timeEdit_CreatedTime->setDateTime(QDateTime::currentDateTime());
    }
}

void Dialog::on_pushButton_BasedProjectPath_clicked()
{
    wstring title = L"项目路径";
    QString dir = MainWindow::openDirDialog(this, QString::fromStdWString(title), "./");
    if (!QFile::exists(dir+"/project.xml")) {
        QMessageBox::critical(this, QString::fromStdWString(L"基于已有项目"), QString::fromStdWString(L"目录不是项目目录"));
        return;
    }
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
        if (!ui->checkBox_BasedProjectOn->isChecked()) {
            // when create a new project from scratch, load a default $HOME/.ifcfd/casting_default_project.xml
            // for setting default value
            QString filename = QDir::homePath()+"/.ifcfd/casting_default_project.xml";
            QFile file(filename);
            if (!file.open(QFile::ReadOnly | QFile::Text)) {
                std::cout << "Error: Can not read file" << qPrintable(filename)
                             << "; " << qPrintable(file.errorString())
                                << std::endl;
            } else {
                project->clearValue();
                project->loadConfigFile(filename);
            }
            ui->timeEdit_CreatedTime->setDateTime(QDateTime::currentDateTime());
        } else {
            ui->timeEdit_ModifiedTime->setDateTime(QDateTime::currentDateTime());
        }
        project->getInformation()->updateValueFromDialog();
        main->setProject(project);
        wstring projectpath = project->getInformation()->getProjectPath();
        QString path = QString::fromStdWString(projectpath);
        QDir dir(path);
        if (!dir.exists()) {
            if (!dir.mkpath(dir.absolutePath())) {
                std::cerr << "Error: Cannnot open dir "
                             << qPrintable(dir.absolutePath()) << std::endl;
                return;
            }
        }
        if (ui->checkBox_BasedProjectOn->isChecked()) {
            // copy stl
            QString originpath = ui->lineEdit_BasedProjectPath->text()+"/geometries";
            Mold *mold = project->getMold();
            QString newpath = QString::fromStdWString(project->getInformation()->getProjectPath()+L"/geometries");
            QDir dir2(newpath);
            if (!dir2.exists()) {
                if (!dir2.mkpath(dir2.absolutePath())) {
                    std::cerr << "Error: Cannnot open dir "
                                 << qPrintable(dir2.absolutePath()) << std::endl;
                }
            }
            for (vector<MoldConfiguration*>::iterator it = mold->getMolds().begin();
                 it != mold->getMolds().end(); it++) {
                MoldConfiguration *config = (*it);
                QString originfile = originpath+"/"+QString::fromStdWString(config->getName());
                QString newfile = newpath+"/"+QString::fromStdWString(config->getName());
                if (QFile::exists(newfile)) {
                    QFile::remove(newfile);
                }
                QFile::copy(originfile, newfile);
            }
        }
        main->getUi()->action_open->setDisabled(true);
        main->getUi()->action_project->setDisabled(true);
        QString title = QString::fromStdWString(L"墨华高科CFD压铸仿真平台")+" - "
                        +QString::fromStdWString(project->getInformation()->getProjectPath());
        main->setWindowTitle(title);
    }
    close();
}

void Dialog::on_buttonBox_rejected()
{
    close();
}
