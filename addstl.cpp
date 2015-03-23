#include "addstl.h"
#include "ui_addstl.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringListModel>
#include <QMessageBox>
#include <QDir>
#include <iostream>
#include "project.h"
#include "information.h"
#include "mold.h"
AddStl::AddStl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStl)
{
    ui->setupUi(this);
}

AddStl::~AddStl()
{
    delete ui;
}

void AddStl::on_buttonBox_accepted()
{
    QString newfile = ui->lineEdit_NewStl->text();
    QString originfile = ui->lineEdit_OriginalStl->text();
    if (originfile.isEmpty()) {
        QMessageBox::critical(this, QString::fromStdWString(L"新建STL文件"), QString::fromStdWString(L"请指定前STL文件名"));
        return;
    }
    if (newfile.isEmpty()) {
        QMessageBox::critical(this, QString::fromStdWString(L"新建STL文件"), QString::fromStdWString(L"请指定STL文件名"));
        return;
    }
    MainWindow *main = (MainWindow*)(parentWidget());
    if (((QStringListModel*)(main->getUi()->listWidget_addedStlMolds->model()))->stringList().contains(newfile)) {
        QMessageBox::critical(this, QString::fromStdWString(L"新建STL文件"), QString::fromStdWString(L"STL文件重名"));
        return;
    }
    if (main->getProject()) {
        QString path = QString::fromStdWString(main->getProject()->getInformation()->getProjectPath()+L"/geometries");
        QDir dir(path);
        if (!dir.exists()) {
            if (!dir.mkpath(dir.absolutePath())) {
                std::cerr << "Error: Cannnot open dir "
                             << qPrintable(dir.absolutePath()) << std::endl;
                return;
            }
        }
        QString newstl = path+"/"+newfile;
        if (QFile::exists(newstl)) {
            QFile::remove(newstl);
        }
        QFile::copy(originfile, newstl);
        main->getProject()->getMold()->addConfiguration(newfile.toStdWString());
    }
}

void AddStl::on_buttonBox_rejected()
{
    close();
}

void AddStl::on_pushButton_OriginalStl_clicked()
{
    QString originalfile = MainWindow::openFileDialog(this, QString::fromStdWString(L"导入前STL文件"), "./", tr("stl file (*.stl)"));
    if (originalfile.isEmpty()) return;
    ui->lineEdit_OriginalStl->setText(originalfile);
}
