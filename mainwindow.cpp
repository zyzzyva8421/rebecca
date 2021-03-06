#include <QFileDialog>
#include <QFile>
#include <QTextCodec>
#include <QModelIndex>
#include <QMessageBox>
#include <QProcess>
#include <QFont>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "project.h"
#include "information.h"
#include "category.h"
#include "casting.h"
#include "mold.h"
#include "simulation.h"
#include "computing.h"
#include "entrance.h"
#include "output.h"
#include "material.h"
#include "dialog.h"
#include "addmaterial.h"
#include "copymaterial.h"
#include "addstl.h"

MainWindow *MainWindow::CurrentWindow = NULL;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    project = NULL;
    dialog = NULL;
    addmaterial = NULL;
    copymaterial = NULL;
    addstl = NULL;
    currentMaterial = NULL;
    simulate = NULL;
    isMaterialGroupChanged = false;
    isKilled = false;
    ui->pushButton_saveMaterialGroup->setDisabled(true);

    materialgroup = new MaterialGroup(L"materialgroup");
    materialgroupModel = new QStandardItemModel(ui->treeView_materials);
    ui->treeView_materials->setModel(materialgroupModel);
    ui->treeView_materials->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->treeView_materials->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(on_treeView_materials_currentRowChanged(QModelIndex,QModelIndex)));
    ui->comboBox_material->setModel(materialgroupModel);
    ui->comboBox_MoldMaterialId->setModel(materialgroupModel);

    ui->pushButton_materialNew->setDisabled(true);
    ui->pushButton_materialUpdate->setDisabled(true);
    ui->pushButton_materialCancel->setDisabled(true);
    ui->pushButton_materialCopy->setDisabled(true);
    ui->pushButton_materialDelete->setDisabled(true);

    tabifyDockWidget(ui->dockWidget_log, ui->dockWidget_material);

    CurrentWindow = this;

    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    if (codec) {
        QTextCodec::setCodecForTr(codec);
        QTextCodec::setCodecForLocale(codec);
        QTextCodec::setCodecForCStrings(codec);
    }
    // Information

    // Casting
    buttonGroup_injectionMethod = new QButtonGroup(this);
    buttonGroup_injectionMethod->addButton(ui->radioButton_constantPressure, Casting::ConstantPressureOn);
    buttonGroup_injectionMethod->addButton(ui->radioButton_constantVelocity, Casting::ConstantVelocityOn);
    buttonGroup_injectionMethod->addButton(ui->radioButton_varyingPressure, Casting::VaryingPressureOn);
    buttonGroup_injectionMethod->addButton(ui->radioButton_varyingVelocity, Casting::VaryingVelocityOn);
    connect(buttonGroup_injectionMethod, SIGNAL(buttonClicked(int)), this, SLOT(on_buttonGroup_injectionMethod_clicked(int)));

    // Mold

    // Entrance
    buttonGroup_BoundaryRight = new QButtonGroup(this);
    buttonGroup_BoundaryRight->addButton(ui->radioButton_BoundaryRight0, Entrance::None);
    buttonGroup_BoundaryRight->addButton(ui->radioButton_BoundaryRight1, Entrance::Inlet);
    buttonGroup_BoundaryRight->addButton(ui->radioButton_BoundaryRight2, Entrance::Outlet);

    buttonGroup_BoundaryLeft = new QButtonGroup(this);
    buttonGroup_BoundaryLeft->addButton(ui->radioButton_BoundaryLeft0, Entrance::None);
    buttonGroup_BoundaryLeft->addButton(ui->radioButton_BoundaryLeft1, Entrance::Inlet);
    buttonGroup_BoundaryLeft->addButton(ui->radioButton_BoundaryLeft2, Entrance::Outlet);

    buttonGroup_BoundaryTop = new QButtonGroup(this);
    buttonGroup_BoundaryTop->addButton(ui->radioButton_BoundaryTop0, Entrance::None);
    buttonGroup_BoundaryTop->addButton(ui->radioButton_BoundaryTop1, Entrance::Inlet);
    buttonGroup_BoundaryTop->addButton(ui->radioButton_BoundaryTop2, Entrance::Outlet);

    buttonGroup_BoundaryBottom = new QButtonGroup(this);
    buttonGroup_BoundaryBottom->addButton(ui->radioButton_BoundaryBottom0, Entrance::None);
    buttonGroup_BoundaryBottom->addButton(ui->radioButton_BoundaryBottom1, Entrance::Inlet);
    buttonGroup_BoundaryBottom->addButton(ui->radioButton_BoundaryBottom2, Entrance::Outlet);

    buttonGroup_BoundaryFront = new QButtonGroup(this);
    buttonGroup_BoundaryFront->addButton(ui->radioButton_BoundaryFront0, Entrance::None);
    buttonGroup_BoundaryFront->addButton(ui->radioButton_BoundaryFront1, Entrance::Inlet);
    buttonGroup_BoundaryFront->addButton(ui->radioButton_BoundaryFront2, Entrance::Outlet);

    buttonGroup_BoundaryBack = new QButtonGroup(this);
    buttonGroup_BoundaryBack->addButton(ui->radioButton_BoundaryBack0, Entrance::None);
    buttonGroup_BoundaryBack->addButton(ui->radioButton_BoundaryBack1, Entrance::Inlet);
    buttonGroup_BoundaryBack->addButton(ui->radioButton_BoundaryBack2, Entrance::Outlet);

    // Simulation
    buttonGroup_fluidModel = new QButtonGroup(this);
    buttonGroup_fluidModel->addButton(ui->radioButton_laminarModel, Simulation::LaminarOn);
    buttonGroup_fluidModel->addButton(ui->radioButton_SAModel, Simulation::SaTurbulenceOn);
    buttonGroup_fluidModel->addButton(ui->radioButton_KEModel, Simulation::KeTurbulenceOn);
    buttonGroup_fluidModel->addButton(ui->radioButton_LESModel, Simulation::LesTurbulenceOn);

    // Computing
    buttonGroup_terminationCondition = new QButtonGroup(this);
    buttonGroup_terminationCondition->addButton(ui->radioButton_TerminationEndTimeOn, Computing::TerminationEndTimeOn);
    buttonGroup_terminationCondition->addButton(ui->radioButton_TerminationMaximumStepOn, Computing::TerminationMaximumStepOn);
    connect(buttonGroup_terminationCondition, SIGNAL(buttonClicked(int)), this, SLOT(on_buttonGroup_terminationCondition_clicked(int)));

    buttonGroup_parallelMethod = new QButtonGroup(this);
    buttonGroup_parallelMethod->addButton(ui->radioButton_ParallelAdaptedOn, Computing::ParallelAdaptedOn);
    buttonGroup_parallelMethod->addButton(ui->radioButton_ParallelFixedOn, Computing::ParallelFixedOn);
    connect(buttonGroup_parallelMethod, SIGNAL(buttonClicked(int)), this, SLOT(on_buttonGroup_parallelMethod_clicked(int)));

    // Output
    buttonGroup_outputMethod = new QButtonGroup(this);
    buttonGroup_outputMethod->addButton(ui->radioButton_OutputIntervalTimeOn, Output::OutputIntervalTimeOn);
    buttonGroup_outputMethod->addButton(ui->radioButton_OutputIntervalStepOn, Output::OutputIntervalStepOn);
    connect(buttonGroup_outputMethod, SIGNAL(buttonClicked(int)), this, SLOT(on_buttonGroup_outputMethod_clicked(int)));
    buttonGroup_loggingMethod = new QButtonGroup(this);
    buttonGroup_loggingMethod->addButton(ui->radioButton_LoggingIntervalTimeOn, Output::LoggingIntervalTimeOn);
    buttonGroup_loggingMethod->addButton(ui->radioButton_LoggingIntervalStepOn, Output::LoggingIntervalStepOn);
    connect(buttonGroup_loggingMethod, SIGNAL(buttonClicked(int)), this, SLOT(on_buttonGroup_loggingMethod_clicked(int)));

    // initilization
    ui->checkBox_basedOnExistingProject->setChecked(false);
    on_checkBox_basedOnExistingProject_stateChanged(Qt::Unchecked);

    ui->checkBox_fluidModel->setChecked(false);
    on_checkBox_fluidModel_stateChanged(Qt::Unchecked);

    ui->radioButton_BoundaryRight0->click();
    ui->radioButton_BoundaryLeft0->click();
    ui->radioButton_BoundaryTop0->click();
    ui->radioButton_BoundaryBottom0->click();
    ui->radioButton_BoundaryFront0->click();
    ui->radioButton_BoundaryBack0->click();

    ui->radioButton_constantVelocity->click();

    ui->radioButton_TerminationEndTimeOn->click();

    ui->radioButton_ParallelAdaptedOn->click();

    ui->radioButton_OutputIntervalTimeOn->click();

    ui->radioButton_LoggingIntervalTimeOn->click();

    ui->action_stop->setDisabled(true);

    ui->textEdit_simuEngine->setStyleSheet("QTextEdit { background: black }");
    ui->textEdit_simuEngine->setTextColor(QColor("white"));
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    ui->textEdit_simuEngine->setFont(font);
    QString materialfile = QDir::homePath()+"/.ifcfd/casting_materials.xml";
    if (materialgroup) {
        materialgroup->loadMaterialFile(materialfile);
        materialgroup->updateGui();
    }
    // TODO: set input mask
    QDoubleValidator *isdouble = new QDoubleValidator(this);
    QIntValidator *isint = new QIntValidator(this);
    ui->lineEdit_constantPressure->setValidator(isdouble);
    ui->lineEdit_constantVelocity->setValidator(isdouble);
    ui->lineEdit_initialTemperature->setValidator(isdouble);
    ui->lineEdit_moldMaterialInitialTemperature->setValidator(isdouble);
    ui->lineEdit_moldHeatExchangeCoefficient->setValidator(isdouble);
    ui->lineEdit_MoldAdjustCoordinateX->setValidator(isdouble);
    ui->lineEdit_MoldAdjustCoordinateY->setValidator(isdouble);
    ui->lineEdit_MoldAdjustCoordinateZ->setValidator(isdouble);
    ui->lineEdit_MoldAdjustScale->setValidator(isdouble);
    ui->lineEdit_MoldRotateCoordinateX->setValidator(isdouble);
    ui->lineEdit_MoldRotateCoordinateY->setValidator(isdouble);
    ui->lineEdit_MoldRotateCoordinateZ->setValidator(isdouble);
    ui->lineEdit_MoldDomainLength->setValidator(isdouble);
    connect(ui->lineEdit_MoldDomainLength, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_double()));
    ui->lineEdit_MoldAdjustScale->setValidator(isdouble);
    connect(ui->lineEdit_MoldAdjustScale, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_double()));
    ui->lineEdit_FluidMeshLevel->setValidator(isint);
    connect(ui->lineEdit_FluidMeshLevel, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_integer()));
    ui->lineEdit_SolidMeshLevel->setValidator(isint);
    connect(ui->lineEdit_SolidMeshLevel, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_integer()));
    ui->lineEdit_AdaptMeshLevel->setValidator(isint);
    connect(ui->lineEdit_AdaptMeshLevel, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_integer()));
    ui->lineEdit_EnvironmentDensity->setValidator(isdouble);
    connect(ui->lineEdit_EnvironmentDensity, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_double()));
    ui->lineEdit_EnvironmentDynamicViscosity->setValidator(isdouble);
    connect(ui->lineEdit_EnvironmentDynamicViscosity, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_double()));
    ui->lineEdit_EnvironmentPressure->setValidator(isdouble);
    ui->lineEdit_EnvironmentTemperature->setValidator(isdouble);
    ui->lineEdit_EnvironmentThermalConductivity->setValidator(isdouble);
    connect(ui->lineEdit_EnvironmentThermalConductivity, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_double()));
    ui->lineEdit_EnvironmentSpecificHeat->setValidator(isdouble);
    connect(ui->lineEdit_EnvironmentSpecificHeat, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_double()));
    ui->lineEdit_GravityX->setValidator(isdouble);
    ui->lineEdit_GravityY->setValidator(isdouble);
    ui->lineEdit_GravityZ->setValidator(isdouble);
    ui->lineEdit_MaxAdaptedTimeStep->setValidator(isdouble);
    connect(ui->lineEdit_MaxAdaptedTimeStep, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_double()));
    ui->lineEdit_IterationMaximumRelativeError->setValidator(isdouble);
    connect(ui->lineEdit_IterationMaximumRelativeError, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_double()));
    ui->lineEdit_IterationMaximumStepCount->setValidator(isint);
    connect(ui->lineEdit_IterationMaximumStepCount, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_integer()));
    ui->lineEdit_IterationMinimumStepCount->setValidator(isint);
    connect(ui->lineEdit_IterationMinimumStepCount, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_integer()));
    ui->lineEdit_TerminationEndTimeOnConf->setValidator(isdouble);
    connect(ui->lineEdit_TerminationEndTimeOnConf, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_double()));
    ui->lineEdit_TerminationMaximumStepOnConf->setValidator(isint);
    connect(ui->lineEdit_TerminationMaximumStepOnConf, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_integer()));
    ui->lineEdit_OutputIntervalStepOnConf->setValidator(isint);
    connect(ui->lineEdit_OutputIntervalStepOnConf, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_integer()));
    ui->lineEdit_OutputIntervalTimeOnConf->setValidator(isdouble);
    connect(ui->lineEdit_OutputIntervalTimeOnConf, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_double()));
    ui->lineEdit_LoggingIntervalTimeOnConf->setValidator(isdouble);
    connect(ui->lineEdit_LoggingIntervalTimeOnConf, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_double()));
    ui->lineEdit_LoggingIntervalStepOnConf->setValidator(isint);
    connect(ui->lineEdit_LoggingIntervalStepOnConf, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_positive_integer()));
    ui->lineEdit_ParallelFixedOnConf->setValidator(isint);
    connect(ui->lineEdit_ParallelFixedOnConf, SIGNAL(editingFinished()), this, SLOT(on_lineEdit_textChanged_ge2_integer()));


    ui->lineEdit_SolidDensity->setValidator(isdouble);
    ui->lineEdit_SolidificationDragCoefficent->setValidator(isdouble);
    ui->lineEdit_SolidLiquidusTemperature->setValidator(isdouble);
    ui->lineEdit_SolidSolidusTemperature->setValidator(isdouble);
    ui->lineEdit_SolidSpecificHeat->setValidator(isdouble);
    ui->lineEdit_SolidThermalConductivity->setValidator(isdouble);
    ui->lineEdit_SolidThermalExpansionCoefficient->setValidator(isdouble);
    ui->lineEdit_FluidCoherencyPoint->setValidator(isdouble);
    ui->lineEdit_FluidContactAngleForWallAdhesion->setValidator(isdouble);
    ui->lineEdit_FluidCriticalPoint->setValidator(isdouble);
    ui->lineEdit_FluidDensity->setValidator(isdouble);
    ui->lineEdit_FluidDynamicViscosity->setValidator(isdouble);
    ui->lineEdit_FluidSpecificHeat->setValidator(isdouble);
    ui->lineEdit_FluidSurfaceTensionCoefficent->setValidator(isdouble);
    ui->lineEdit_FluidThermalConductivity->setValidator(isdouble);
    ui->lineEdit_FluidThermalExpansionCoefficent->setValidator(isdouble);

}

void MainWindow::on_lineEdit_textChanged_positive_double()
{
    QLineEdit *edit = dynamic_cast<QLineEdit*>(sender());
    if (edit) {
        double value = edit->text().toDouble();
        if (value <= 0 ) {
            QMessageBox::critical(this, QString::fromStdWString(L"非法输入"), QString::fromStdWString(L"必须为正浮点数"));
            edit->clear();
            return;
        }
    }
}

void MainWindow::on_lineEdit_textChanged_f0t1_double()
{
    QLineEdit *edit = dynamic_cast<QLineEdit*>(sender());
    if (edit) {
        double value = edit->text().toDouble();
        if (value < 0 || value > 1) {
            QMessageBox::critical(this, QString::fromStdWString(L"非法输入"), QString::fromStdWString(L"必须为0到1之间的浮点数"));
            edit->clear();
            return;
        }
    }
}

void MainWindow::on_lineEdit_textChanged_ge2_integer()
{
    QLineEdit *edit = dynamic_cast<QLineEdit*>(sender());
    if (edit) {
        double value = edit->text().toInt();
        if (value < 2 ) {
            QMessageBox::critical(this, QString::fromStdWString(L"非法输入"), QString::fromStdWString(L"必须为大于等于2的整数"));
            edit->clear();
            return;
        }
    }
}

void MainWindow::on_lineEdit_textChanged_positive_integer()
{
    QLineEdit *edit = dynamic_cast<QLineEdit*>(sender());
    if (edit) {
        double value = edit->text().toInt();
        if (value <=0 ) {
            QMessageBox::critical(this, QString::fromStdWString(L"非法输入"), QString::fromStdWString(L"必须为正整数"));
            edit->clear();
            edit->setFocus();
            return;
        }
    }
}

void MainWindow::setProject(Project *_project) {
    if (project == _project) return;
    if (project) delete project;
    project = _project;
    project->updateGui();
    if (ui->checkBox_basedOnExistingProject->isChecked()) {
        ui->lineEdit_amender->setDisabled(false);
    } else {
        ui->lineEdit_amender->setDisabled(true);
    }
}

void MainWindow::UpdateTable(const vector< vector<double> >&data, QTableWidget *table)
{
    table->setRowCount(0);
    vector< vector<double> >::const_iterator it;
    vector<double> p;
    for (it = data.begin(); it != data.end(); it++) {
        p = (*it);
        table->insertRow(table->rowCount());
        table->setRowHeight(table->rowCount()-1, table->fontMetrics().height());
        vector<double>::iterator it2;
        QString text;
        QTableWidgetItem *item = NULL;
        int col = 0;
        for (it2 = p.begin(); it2 != p.end(); it2++) {
            text = QString::number((*it2));
            item = new QTableWidgetItem(text);
            table->setItem(table->rowCount()-1, col, item);
            col++;
        }
    }
}

void MainWindow::UpdateData(QTableWidget *table, vector< vector<double> >&data)
{
    data.clear();
    QTableWidgetItem *w = NULL;
    for (int i = 0; i < table->rowCount(); i++) {
        vector<double> p;
        bool isValid = true;
        for (int col = 0; col < table->columnCount(); col++) {
            w = table->item(i, col);
            if (!w) {
                isValid = false;
                break;
            }
            p.push_back(w->text().toDouble());
        }
        if (isValid)
            data.push_back(p);
    }
}

void MainWindow::on_action_project_triggered()
{
    if (dialog == NULL)
        dialog = new Dialog(this);
    dialog->show();
}

void MainWindow::on_action_log_triggered() {
    ui->dockWidget_log->setVisible(true);
    ui->dockWidget_log->raise();
}

void MainWindow::on_action_result_triggered() {
    if (!project) return;
    QProcess *process = createProcess(ui->action_result);
    if (!process) return;
    wstring projectpath = project->getInformation()->getProjectPath();
    if (projectpath.empty()) return;
    QString currentpath = QDir::currentPath();
    QDir dir = QDir::current();
    if (!dir.setCurrent(QString::fromStdWString(projectpath))) {
        QMessageBox::critical(this, QString::fromStdWString(L"打开结果目录"), QString::fromStdWString(L"项目路径不合法"));
        dir.setCurrent(currentpath);
        return;
    }
    process->start("ifcfd_casting_shell --browse output");
    process->waitForStarted();
    dir.setCurrent(currentpath);
    return;
}

void MainWindow::on_action_curve_triggered() {
    if (!project) return;
    QProcess *process = createProcess(ui->action_result);
    if (!process) return;
    wstring projectpath = project->getInformation()->getProjectPath();
    if (projectpath.empty()) return;
    QString currentpath = QDir::currentPath();
    QDir dir = QDir::current();
    if (!dir.setCurrent(QString::fromStdWString(projectpath))) {
        QMessageBox::critical(this, QString::fromStdWString(L"查看填充率曲线"), QString::fromStdWString(L"项目路径不合法"));
        dir.setCurrent(currentpath);
        return;
    }
    process->start("ifcfd_casting_shell --view_filling_rate_curve");
    process->waitForStarted();
    dir.setCurrent(currentpath);
    return;
}

void MainWindow::on_action_image_triggered() {
    if (!project) return;
    QProcess *process = createProcess(ui->action_result);
    if (!process) return;
    wstring projectpath = project->getInformation()->getProjectPath();
    if (projectpath.empty()) return;
    QString currentpath = QDir::currentPath();
    QDir dir = QDir::current();
    if (!dir.setCurrent(QString::fromStdWString(projectpath))) {
        QMessageBox::critical(this, QString::fromStdWString(L"查看填充率图像"), QString::fromStdWString(L"项目路径不合法"));
        dir.setCurrent(currentpath);
        return;
    }
    process->start("ifcfd_casting_shell --view_filling_rate_image");
    process->waitForStarted();
    dir.setCurrent(currentpath);
    return;
}

void MainWindow::on_action_video_triggered() {
    if (!project) return;
    QProcess *process = createProcess(ui->action_result);
    if (!process) return;
    wstring projectpath = project->getInformation()->getProjectPath();
    if (projectpath.empty()) return;
    QString currentpath = QDir::currentPath();
    QDir dir = QDir::current();
    if (!dir.setCurrent(QString::fromStdWString(projectpath))) {
        QMessageBox::critical(this, QString::fromStdWString(L"查看填充率视频"), QString::fromStdWString(L"项目路径不合法"));
        dir.setCurrent(currentpath);
        return;
    }
    process->start("ifcfd_casting_shell --view_filling_rate_video");
    process->waitForStarted();
    dir.setCurrent(currentpath);
    return;
}

void MainWindow::on_action_clean_triggered() {
    if (!project) return;
    QProcess *process = createProcess(ui->action_clean);
    if (!process) return;
    wstring projectpath = project->getInformation()->getProjectPath();
    if (projectpath.empty()) return;
    int buttonclicked = QMessageBox::warning(this, QString::fromStdWString(L"清空结果目录"),
                                             QString::fromStdWString(L"是否清空结果目录？"), QMessageBox::Ok | QMessageBox::No);
    if (buttonclicked == QMessageBox::No) {
        return;
    }
    QString currentpath = QDir::currentPath();
    QDir dir = QDir::current();
    if (!dir.setCurrent(QString::fromStdWString(projectpath))) {
        QMessageBox::critical(this, QString::fromStdWString(L"打开结果目录"), QString::fromStdWString(L"项目路径不合法"));
        dir.setCurrent(currentpath);
        return;
    }

    process->start("ifcfd_casting_shell --clean_output");
    process->waitForStarted();
    dir.setCurrent(currentpath);
    return;
}

void MainWindow::on_action_dir_triggered() {
    if (!project) return;
    QProcess *process = createProcess(ui->action_dir);
    if (!process) return;
    wstring projectpath = project->getInformation()->getProjectPath();
    if (projectpath.empty()) return;
    QString currentpath = QDir::currentPath();
    QDir dir = QDir::current();
    if (!dir.setCurrent(QString::fromStdWString(projectpath))) {
        QMessageBox::critical(this, QString::fromStdWString(L"打开项目目录"), QString::fromStdWString(L"项目路径不合法"));
        dir.setCurrent(currentpath);
        return;
    }

    process->start("ifcfd_casting_shell --browse .");
    process->waitForStarted();
    dir.setCurrent(currentpath);
    return;
}

void MainWindow::on_action_close_triggered() {
    if (!project) {
        return;
    }
    int clickedButton = QMessageBox::question(this, QString::fromStdWString(L"关闭项目"), QString::fromStdWString(L"是否关闭当前项目前先保存？"),
                                              QMessageBox::Save | QMessageBox::Discard | QMessageBox::No, QMessageBox::No);
    if (clickedButton == QMessageBox::No) {
        return;
    } else if (clickedButton == QMessageBox::Save) {
        on_action_save_triggered();
    }
    if (project) {
        project->clearValue();
        project->updateGui();
        ui->action_open->setDisabled(false);
        ui->action_project->setDisabled(false);
        setWindowTitle(QString::fromStdWString(L"墨华高科CFD压铸仿真平台"));
        ui->textEdit_simuEngine->clear();
    }
}

QProcess *MainWindow::createProcess(QObject *parent)
{
    QProcess *process = new QProcess(parent);
    process->setEnvironment(QProcess::systemEnvironment());
    process->setProcessChannelMode(QProcess::MergedChannels);
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(on_stdoupt_update()));
    connect(process, SIGNAL(started()), this, SLOT(on_process_started()));
    connect(process, SIGNAL(finished(int)), this, SLOT(on_process_finished(int)));
    return process;
}

void MainWindow::on_action_simulate_triggered() {
    if (!project) return;
    ui->dockWidget_log->setVisible(true);
    ui->dockWidget_log->raise();
    QProcess *process = createProcess(ui->action_simulate);
    if (!process) return;
    wstring projectpath = project->getInformation()->getProjectPath();
    if (projectpath.empty()) return;
    int buttonclicked = QMessageBox::warning(this, QString::fromStdWString(L"运行"),
                                             QString::fromStdWString(L"是否运行仿真，之前结果将被删除？"), QMessageBox::Ok | QMessageBox::No);
    if (buttonclicked == QMessageBox::No) {
        return;
    }
    QString currentpath = QDir::currentPath();
    QDir dir = QDir::current();
    if (!dir.setCurrent(QString::fromStdWString(projectpath))) {
        QMessageBox::critical(this, QString::fromStdWString(L"运行"), QString::fromStdWString(L"项目路径不合法"));
        return;
    }
    process->start("ifcfd_casting_shell --run");
    process->waitForStarted();
    dir.setCurrent(currentpath);
    return;
}

void MainWindow::on_process_started()
{
    QProcess *process = dynamic_cast<QProcess*>(sender());
    if (process) {
        QAction *action = dynamic_cast<QAction*>(process->parent());
        if (action) {
            QTextCursor cursor = ui->textEdit_simuEngine->textCursor();
            cursor.insertText(QString(100, '*')+QString("\n"));
            cursor.movePosition(QTextCursor::End);
            ui->textEdit_simuEngine->setTextCursor(cursor);
            action->setDisabled(true);
            if (action == ui->action_simulate) {
                simulate = process;
                ui->action_stop->setDisabled(false);
            }
        }
    }
}

void MainWindow::on_process_finished(int exitCode)
{
    QProcess *process = dynamic_cast<QProcess*>(sender());
    if (process) {
        QAction *action = dynamic_cast<QAction*>(process->parent());
        if (action) {
            action->setDisabled(false);
            if (action == ui->action_simulate) {
                simulate = NULL;
                ui->action_stop->setDisabled(true);
                if (!isKilled) {
                    if (exitCode == 0) {
                        QMessageBox::information(this, QString::fromStdWString(L"仿真结束"),
                                                 QString::fromStdWString(L"仿真结束，请打开结果目录分析数据"));
                    } else {
                        QMessageBox::information(this, QString::fromStdWString(L"仿真结束"),
                                                 QString::fromStdWString(L"仿真失败，请检查日志或联系墨华高科"));
                    }
                } else {
                    isKilled = false;
                }
            } else {
                if (exitCode != 0) {
                    QString text = action->text()+QString::fromStdWString(L"失败");
                    QMessageBox::information(this, action->text(), text);
                }
            }
        }
        process->deleteLater();
    }
}

void MainWindow::on_stdoupt_update()
{
    QProcess *p = dynamic_cast<QProcess*>(sender());
    if (p) {
        QTextCursor cursor = ui->textEdit_simuEngine->textCursor();
        cursor.insertText(p->readAllStandardOutput());
        cursor.movePosition(QTextCursor::End);
        ui->textEdit_simuEngine->setTextCursor(cursor);
    }
}

void MainWindow::on_action_material_triggered()
{
    ui->dockWidget_material->setVisible(true);
    ui->dockWidget_material->raise();
}

void MainWindow::on_buttonGroup_terminationCondition_clicked(int id) {
    ui->lineEdit_TerminationEndTimeOnConf->setDisabled(true);
    ui->lineEdit_TerminationMaximumStepOnConf->setDisabled(true);
    switch (id) {
    case Computing::TerminationEndTimeOn: {
        ui->lineEdit_TerminationEndTimeOnConf->setDisabled(false);
        break;
    }
    case Computing::TerminationMaximumStepOn: {
        ui->lineEdit_TerminationMaximumStepOnConf->setDisabled(false);
        break;
    }
    default: break;
    }
}

void MainWindow::on_buttonGroup_parallelMethod_clicked(int id)
{
    ui->lineEdit_ParallelFixedOnConf->setDisabled(true);
    switch (id) {
    case Computing::ParallelFixedOn: {
        ui->lineEdit_ParallelFixedOnConf->setDisabled(false);
        break;
    }
    default: break;
    }
}

void MainWindow::on_buttonGroup_injectionMethod_clicked(int id)
{
    ui->lineEdit_constantPressure->setDisabled(true);
    ui->lineEdit_constantVelocity->setDisabled(true);
    ui->tableWidget_varyingPressure->setDisabled(true);
    ui->frame_varyingPressure->setDisabled(true);
    ui->tableWidget_varyingVelocity->setDisabled(true);
    ui->frame_varyingVelocity->setDisabled(true);
    switch (id) {
    case Casting::ConstantPressureOn: {
        ui->lineEdit_constantPressure->setDisabled(false);
        break;
    }
    case Casting::ConstantVelocityOn: {
        ui->lineEdit_constantVelocity->setDisabled(false);
        break;
    }
    case Casting::VaryingPressureOn: {
        ui->tableWidget_varyingPressure->setDisabled(false);
        ui->frame_varyingPressure->setDisabled(false);
        break;
    }
    case Casting::VaryingVelocityOn: {
        ui->tableWidget_varyingVelocity->setDisabled(false);
        ui->frame_varyingVelocity->setDisabled(false);
        break;
    }
    default: break;
    }
    return;
}

void MainWindow::on_buttonGroup_outputMethod_clicked(int id)
{
    ui->lineEdit_OutputIntervalTimeOnConf->setDisabled(true);
    ui->lineEdit_OutputIntervalStepOnConf->setDisabled(true);
    switch (id) {
    case Output::OutputIntervalTimeOn: {
        ui->lineEdit_OutputIntervalTimeOnConf->setDisabled(false);
        break;
    }
    case Output::OutputIntervalStepOn: {
        ui->lineEdit_OutputIntervalStepOnConf->setDisabled(false);
        break;
    }
    default: break;
    }
}

void MainWindow::on_buttonGroup_loggingMethod_clicked(int id)
{
    ui->lineEdit_LoggingIntervalTimeOnConf->setDisabled(true);
    ui->lineEdit_LoggingIntervalStepOnConf->setDisabled(true);
    switch (id) {
    case Output::LoggingIntervalTimeOn: {
        ui->lineEdit_LoggingIntervalTimeOnConf->setDisabled(false);
        break;
    }
    case Output::LoggingIntervalStepOn: {
        ui->lineEdit_LoggingIntervalStepOnConf->setDisabled(false);
        break;
    }
    default: break;
    }
}
MainWindow::~MainWindow()
{
    delete ui;
    CurrentWindow = NULL;
}


void MainWindow::on_checkBox_basedOnExistingProject_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        ui->frame_existingProjectPath->setDisabled(false);
    } else {
        ui->frame_existingProjectPath->setDisabled(true);
    }
}

QString MainWindow::openFileDialog(QWidget *parent,
                                   const QString &caption,
                                   const QString &dir,
                                   const QString &filter)
{
    QString ret = QFileDialog::getOpenFileName(parent, caption, dir, filter);
    return ret;
}

QString MainWindow::openDirDialog(QWidget *parent,
                                  const QString &caption,
                                  const QString &dir)
{
    QString ret = QFileDialog::getExistingDirectory(parent, caption, dir);
    return ret;
}

void MainWindow::on_toolButton_projectLibrary_clicked()
{
    QString dir = openDirDialog(this, QString::fromStdWString(L"项目路径"), "./");
    ui->lineEdit_projectLibrary->setText(dir);
}



void MainWindow::on_toolButton_projectPath_clicked()
{
    QString dir = openDirDialog(this, QString::fromStdWString(L"项目路径"), "./");
    ui->lineEdit_projectPath->setText(dir);
}

void MainWindow::on_toolButton_existingProjectPath_clicked()
{
    QString dir = openDirDialog(this, QString::fromStdWString(L"项目路径"), "./");
    ui->lineEdit_existingProjectPath->setText(dir);
}

void MainWindow::tableInsertRow(QTableWidget *table, int row_num)
{
    if (row_num<1) return;
    int cur_row = table->currentRow();
    if (cur_row == -1) {
        for (int i = 0; i < row_num; i++) {
            table->insertRow(table->rowCount());
            table->setRowHeight(table->rowCount()-1, table->fontMetrics().height());
        }
    } else {
        for (int i = 0; i < row_num; i++) {
            table->insertRow(cur_row+1);
            table->setRowHeight(cur_row+i+1, table->fontMetrics().height());
        }
    }
}

void MainWindow::tableDeleteRow(QTableWidget *table, int row_num)
{
    if (row_num<1) return;
    int cur_row = table->currentRow();
    if (cur_row == -1) {
        for (int i = 0; i < row_num; i++) {
            table->removeRow(table->rowCount()-1);
        }
    } else {
        for (int i = 0; i < row_num; i++) {
            table->removeRow(cur_row);
        }
    }
}

void MainWindow::on_toolButton_varyingVelocityAdd_clicked()
{
    tableInsertRow(ui->tableWidget_varyingVelocity);
}

void MainWindow::on_toolButton_varyingVelocityDelete_clicked()
{
    tableDeleteRow(ui->tableWidget_varyingVelocity);
}

void MainWindow::on_toolButton_varyingPressureAdd_clicked()
{
    tableInsertRow(ui->tableWidget_varyingPressure);
}

void MainWindow::on_toolButton_varyingPressureDelete_clicked()
{
    tableDeleteRow(ui->tableWidget_varyingPressure);
}

void MainWindow::on_listWidget_addedStlMolds_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Mold *mold = dynamic_cast<Mold*>(project->getMold());
    if (mold) {
        if (current) {
            wstring str = current->text().toStdWString();
            MoldConfiguration *conf = mold->getMoldConfiguration(str);
            if (conf) conf->updateGui();
        }
    }
}

void MainWindow::on_treeView_materials_currentRowChanged(QModelIndex current, QModelIndex previous)
{

    QStandardItem *item = materialgroupModel->itemFromIndex(current);
    if (item) {
        ui->pushButton_materialNew->setDisabled(false);
        ui->pushButton_materialDelete->setDisabled(false);
        QVariant v = item->data();
        if (v.isValid()) {
            Category *category = static_cast<Category*>(v.value<void*>());
            if (category) {
                if (category->getName() == L"material") {
                    currentMaterial = (Material*)category;
                    ui->pushButton_materialUpdate->setDisabled(false);
                    ui->pushButton_materialCopy->setDisabled(false);
                    ui->pushButton_materialCancel->setDisabled(false);
                    currentMaterial->updateGui();
                } else {
                    ui->pushButton_materialUpdate->setDisabled(true);
                    ui->pushButton_materialCopy->setDisabled(true);
                    ui->pushButton_materialCancel->setDisabled(true);
                }
            }
        }
    } else {
        ui->pushButton_materialNew->setDisabled(true);
        ui->pushButton_materialUpdate->setDisabled(true);
        ui->pushButton_materialCancel->setDisabled(true);
        ui->pushButton_materialCopy->setDisabled(true);
        ui->pushButton_materialDelete->setDisabled(true);
    }
}

void MainWindow::on_checkBox_fluidModel_stateChanged(int arg1)
{
    if (arg1 == 0) {
        ui->frame_fluidModel->setDisabled(true);
    } else {
        ui->frame_fluidModel->setDisabled(false);
    }
}

void MainWindow::on_action_open_triggered()
{
    wstring title = L"项目路径";
    QString dir = MainWindow::openDirDialog(this, QString::fromStdWString(title), "./");
    if (dir.isEmpty()) return;
    Project *_project = new Project(L"Project");
    QString filename = dir+"/project.xml";
    if (!QFile::exists(filename)) {
        QMessageBox::critical(this, QString::fromStdWString(L"打开项目"), QString::fromStdWString(L"目录不是项目目录"));
        return;
    }
    if (_project->loadConfigFile(filename)) {
        if (project) {
            delete project;
        }
        project = _project;
        project->updateGui();
        ui->dateTimeEdit_amendTime->setDateTime(QDateTime::currentDateTime());
        ui->action_open->setDisabled(true);
        ui->action_project->setDisabled(true);
        QString title = QString::fromStdWString(L"墨华高科CFD压铸仿真平台")+" - "+dir;
        setWindowTitle(title);
    } else {
        delete _project;
        _project = NULL;
    }
}

void MainWindow::on_action_save_triggered()
{
    if (project == NULL) return;
    wstring projectpath = project->getInformation()->getProjectPath();
    if (projectpath.empty()) return;
    QString path = QString::fromStdWString(projectpath);
    QDir dir(path);
    if (!dir.exists()) {
        if (!dir.mkpath(dir.absolutePath())) {
            std::cerr << "Error: Cannnot open dir "
                         << qPrintable(dir.absolutePath()) << std::endl;
            return;
        }
    }
    QString filename = QString::fromStdWString(projectpath)+"/project.xml";
    ui->dateTimeEdit_amendTime->setDateTime(QDateTime::currentDateTime());
    if (!project->writeConfigFile(filename)) {
        // TODO: popup an error message
    }
}

void MainWindow::on_action_stop_triggered()
{
    QProcess *process = simulate;
    if (process) {
        QAction *action = dynamic_cast<QAction*>(process->parent());
        if (action != ui->action_simulate) return;
        int buttonclicked = QMessageBox::warning(this, QString::fromStdWString(L"终止仿真"),
                                                 QString::fromStdWString(L"是否终止仿真？"), QMessageBox::Ok | QMessageBox::No);
        if (buttonclicked == QMessageBox::No) {
            return;
        }
        isKilled = true;
        process->kill();
        process->deleteLater();
        simulate = NULL;
    }
}

void MainWindow::on_toolButton_varyingVelocityOK_clicked()
{
    if (project) {
        ui->tableWidget_varyingVelocity->setCurrentItem(NULL);
        UpdateData(ui->tableWidget_varyingVelocity,
                   project->getCasting()->getInjectMethodVaryingVelocityOnConf());
    }
}

void MainWindow::on_toolButton_varyingVelocityCancel_clicked()
{
    if (project) {
        UpdateTable(project->getCasting()->getInjectMethodVaryingVelocityOnConf(),
                    ui->tableWidget_varyingVelocity);
    }
}

void MainWindow::on_toolButton_varyingPressureOK_clicked()
{
    if (project) {
        ui->tableWidget_varyingPressure->setCurrentItem(NULL);
        UpdateData(ui->tableWidget_varyingPressure,
                   project->getCasting()->getInjectMethodVaryingPressureOnConf());
    }
}

void MainWindow::on_toolButton_varyingPressCancel_clicked()
{
    if (project) {
        UpdateTable(project->getCasting()->getInjectMethodVaryingPressureOnConf(),
                    ui->tableWidget_varyingPressure);
    }
}

void MainWindow::on_pushButton_addMold_clicked()
{
    if (!addstl) {
        addstl = new AddStl(this);
    }
    addstl->show();
}

void MainWindow::on_pushButton_reloadMold_clicked()
{
    QString originalfile = openFileDialog(this, QString::fromStdWString(L"导入前STL文件"), "./", tr("stl file (*.stl)"));
    if (originalfile.isEmpty()) return;
    wstring mold = ui->listWidget_addedStlMolds->currentItem()->text().toStdWString();
    if (project) {
        QString path = QString::fromStdWString(project->getInformation()->getProjectPath()+L"/geometries");
        QDir dir(path);
        if (!dir.exists()) {
            if (!dir.mkpath(dir.absolutePath())) {
                std::cerr << "Error: Cannnot open dir "
                             << qPrintable(dir.absolutePath()) << std::endl;
                return;
            }
        }
        QString newfile = path+"/"+QString::fromStdWString(mold);
        if (QFile::exists(newfile)) {
            QFile::remove(newfile);
        }
        QFile::copy(originalfile, newfile);
    }
}

void MainWindow::on_pushButton_deleteMold_clicked()
{
    QString title = QString::fromStdWString(L"删除当前模具配置");
    QString text = QString::fromStdWString(L"是否删除当前模具配置？");
    int clickedButton = QMessageBox::question(this, title, text,
                                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (clickedButton == QMessageBox::No) return;
    wstring mold = ui->listWidget_addedStlMolds->currentItem()->text().toStdWString();
    if (project) {
        project->getMold()->deleteConfiguration(mold);
    }
}

void MainWindow::on_pushButton_modifyMold_clicked()
{
    wstring mold = ui->listWidget_addedStlMolds->currentItem()->text().toStdWString();
    if (project) {
        project->getMold()->modifyConfiguration(mold);
    }
}

void MainWindow::on_pushButton_materialNew_clicked()
{
    if (addmaterial == NULL)
        addmaterial = new AddMaterial(this);
    addmaterial->updateGui();
    addmaterial->show();
}

void MainWindow::on_pushButton_materialUpdate_clicked()
{
    QModelIndex current = ui->treeView_materials->currentIndex();
    QStandardItem *item = materialgroupModel->itemFromIndex(current);
    if (item) {
        ui->pushButton_materialUpdate->setDisabled(false);
        QVariant v = item->data();
        if (v.isValid()) {
            Category *category = static_cast<Category*>(v.value<void*>());
            if (category) {
                if (category->getName() == L"material") {
                    category->updateValue();
                    isMaterialGroupChanged = true;
                    ui->pushButton_saveMaterialGroup->setDisabled(false);
                }
            }
        }
    }
}

void MainWindow::on_pushButton_materialCancel_clicked()
{
    QModelIndex current = ui->treeView_materials->currentIndex();
    QStandardItem *item = materialgroupModel->itemFromIndex(current);
    if (item) {
        ui->pushButton_materialUpdate->setDisabled(false);
        QVariant v = item->data();
        if (v.isValid()) {
            Category *category = static_cast<Category*>(v.value<void*>());
            if (category) {
                if (category->getName() == L"material") {
                    category->updateGui();
                }
            }
        }
    }
}

void MainWindow::on_pushButton_materialDelete_clicked()
{
    QModelIndex current = ui->treeView_materials->currentIndex();
    QStandardItem *item = materialgroupModel->itemFromIndex(current);
    if (item) {
        ui->pushButton_materialUpdate->setDisabled(false);
        QVariant v = item->data();
        if (v.isValid()) {
            Category *category = static_cast<Category*>(v.value<void*>());
            if (category) {
                if (category->getName() == L"material") {
                    MaterialGroup *group = ((Material*)category)->getGroup();
                    if (group) {
                        group->deleteMaterial(((Material*)category)->getId());
                        item->parent()->removeRow(current.row());
                        isMaterialGroupChanged = true;
                    }
                } else {
                    MaterialGroup *group = ((MaterialGroup*)category)->getGroup();
                    if (group) {
                        group->deleteGroup(((MaterialGroup*)category)->getId());
                        item->parent()->removeRow(current.row());
                        isMaterialGroupChanged = true;
                    }
                }
            }
        }
    }
    if (isMaterialGroupChanged) {
        ui->pushButton_saveMaterialGroup->setDisabled(false);
    }
}

void MainWindow::on_pushButton_materialCopy_clicked()
{
    if (copymaterial == NULL)
        copymaterial = new CopyMaterial(this);
    copymaterial->show();
}

void MainWindow::on_pushButton_saveMaterialGroup_clicked()
{
    QString materialfile = QDir::homePath()+"/.ifcfd/casting_materials.xml";
    if (materialgroup) {
        if (!materialgroup->saveMaterialFile(materialfile)) {
            // TODO:popup an error message
        }
    }
    isMaterialGroupChanged = false;
    ui->pushButton_saveMaterialGroup->setDisabled(true);
}

void MainWindow::on_pushButton_moldCancel_clicked()
{
    wstring mold = ui->listWidget_addedStlMolds->currentItem()->text().toStdWString();
    if (project) {
        MoldConfiguration* config = project->getMold()->getMoldConfiguration(mold);
        if (config) {
            config->updateGui();
        }
    }
}

void MainWindow::on_pushButton_previewMold_clicked()
{
    if (!project) return;
    QProcess *process = createProcess(ui->action_clean);
    if (!process) return;
    wstring projectpath = project->getInformation()->getProjectPath();
    if (projectpath.empty()) return;
    QString currentpath = QDir::currentPath();
    QDir dir = QDir::current();
    if (!dir.setCurrent(QString::fromStdWString(projectpath))) {
        QMessageBox::critical(this, QString::fromStdWString(L"打开结果目录"), QString::fromStdWString(L"项目路径不合法"));
        dir.setCurrent(currentpath);
        return;
    }
    wstring mold = ui->listWidget_addedStlMolds->currentItem()->text().toStdWString();
    QString file = QString::fromStdWString(projectpath)+"/geometries/"+QString::fromStdWString(mold);
    if (!QFile(file).exists()) {
        QMessageBox::critical(this, QString::fromStdWString(L"预览"), QString::fromStdWString(L"Stl文件不存在"));
        dir.setCurrent(currentpath);
        return;
    }
    QString cmd = "ifcfd_casting_shell --view_stl "+QString::fromStdWString(mold);
    process->start(cmd);
    process->waitForStarted();
    dir.setCurrent(currentpath);
    return;
}

void MainWindow::on_pushButton_defaultGroup_clicked()
{
    QString materialfile = QDir::homePath()+"/.ifcfd/casting_default_materials.xml";
    QString originalfile = QDir::homePath()+"/.ifcfd/casting_materials.xml";
    if (QFile::exists(originalfile)) {
        QFile::remove(originalfile);
    }
    QFile::copy(materialfile, originalfile);
    if (materialgroup) {
        materialgroup->clearValue();
        materialgroup->loadMaterialFile(originalfile);
        materialgroup->updateGui();
    }
}
