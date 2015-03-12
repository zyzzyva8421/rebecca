#include <QFileDialog>
#include <QFile>
#include <QTextCodec>
#include <QModelIndex>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "project.h"
#include "category.h"
#include "casting.h"
#include "mold.h"
#include "simulation.h"
#include "computing.h"
#include "entrance.h"
#include "output.h"
#include "material.h"
#include "dialog.h"
MainWindow *MainWindow::CurrentWindow = NULL;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    project = NULL;
    materialgroup = new MaterialGroup(L"MaterialGroup");
    materialgroupModel = new QStandardItemModel(ui->treeView_materials);
    ui->treeView_materials->setModel(materialgroupModel);
    ui->treeView_materials->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->treeView_materials->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(on_treeView_materials_currentRowChanged(QModelIndex,QModelIndex)));
    ui->comboBox_material->setModel(materialgroupModel);
    ui->comboBox_MoldMaterialId->setModel(materialgroupModel);
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
    buttonGroup_moldSurfaceRoughness = new QButtonGroup(this);
    buttonGroup_moldSurfaceRoughness->addButton(ui->radioButton_noSlipOn, MoldConfiguration::NoSlipOn);
    buttonGroup_moldSurfaceRoughness->addButton(ui->radioButton_freeSlipOn, MoldConfiguration::FreeSlipOn);

    // Simulation
    buttonGroup_fluidModel = new QButtonGroup(this);
    buttonGroup_fluidModel->addButton(ui->radioButton_laminarModel, Simulation::LaminarOn);
    buttonGroup_fluidModel->addButton(ui->radioButton_SAModel, Simulation::SaTurbulenceOn);
    buttonGroup_fluidModel->addButton(ui->radioButton_KEModel, Simulation::KeTurbulenceOn);
    buttonGroup_fluidModel->addButton(ui->radioButton_LESModel, Simulation::LesTurbulenceOn);

    // Computing
    buttonGroup_timeStepMethod = new QButtonGroup(this);
    buttonGroup_timeStepMethod->addButton(ui->radioButton_timeStepAdaptedOn, Computing::TimeStepAdaptedOn);
    buttonGroup_timeStepMethod->addButton(ui->radioButton_timeStepFixedOn, Computing::TimeStepFixedOn);
    connect(buttonGroup_timeStepMethod, SIGNAL(buttonClicked(int)), this, SLOT(on_buttonGroup_timeStepMethod_clicked(int)));

    buttonGroup_terminationCondition = new QButtonGroup(this);
    buttonGroup_terminationCondition->addButton(ui->radioButton_TerminationEndTimeOn, Computing::TerminationEndTimeOn);
    buttonGroup_terminationCondition->addButton(ui->radioButton_TerminationFillingRateOn, Computing::TerminationFillingRateOn);
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

    // action
    connect(ui->action_project, SIGNAL(triggered()), this, SLOT(on_action_project_triggered()));
    connect(ui->action_material, SIGNAL(triggered()), this, SLOT(on_action_material_triggered()));
    connect(ui->action_simulate, SIGNAL(triggered()), this, SLOT(on_action_log_triggered()));
    connect(ui->action_open, SIGNAL(triggered()), this, SLOT(on_action_open_triggered()));
    connect(ui->action_save, SIGNAL(triggered()), this, SLOT(on_action_save_triggered()));

    // initilization
    ui->checkBox_basedOnExistingProject->setChecked(false);
    on_checkBox_basedOnExistingProject_stateChanged(Qt::Unchecked);

    ui->checkBox_fluidModel->setChecked(false);
    on_checkBox_fluidModel_stateChanged(Qt::Unchecked);

    ui->radioButton_constantVelocity->click();

    ui->radioButton_timeStepAdaptedOn->click();

    ui->radioButton_TerminationEndTimeOn->click();

    ui->radioButton_ParallelAdaptedOn->click();

    ui->radioButton_OutputIntervalTimeOn->click();

    wstring filename = L"/Developer/Applications/Qt/GFS_Simulator/casting_materials.xml";
    QString materialfile = QString::fromStdWString(filename);
    if (materialgroup) {
        materialgroup->loadMaterialFile(materialfile);
        materialgroup->updateGui();
    }
}

void MainWindow::setProject(Project *_project) {
    if (project == _project) return;
    if (project) delete project;
    project = _project;
    project->updateGui();
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
        for (int col = 0; col < table->columnCount(); col++) {
            w = table->item(i, col);
            p.push_back(w->text().toDouble());
        }
        data.push_back(p);
    }
}

void MainWindow::on_action_project_triggered()
{
    Dialog *dialog = new Dialog(this);
    dialog->show();
}

void MainWindow::on_action_log_triggered() {
    ui->dockWidget_log->setVisible(true);
    ui->dockWidget_log->raise();
}

void MainWindow::on_action_material_triggered()
{
    ui->dockWidget_material->setVisible(true);
    ui->dockWidget_material->raise();
}

void MainWindow::on_buttonGroup_timeStepMethod_clicked(int id)
{
    ui->lineEdit_timeStepFixedOn->setDisabled(true);
    switch (id) {
    case Computing::TimeStepFixedOn: {
        ui->lineEdit_timeStepFixedOn->setDisabled(false);
        break;
    }
    default: break;
    }
}

void MainWindow::on_buttonGroup_terminationCondition_clicked(int id) {
    ui->lineEdit_TerminationEndTimeOnConf->setDisabled(true);
    ui->lineEdit_TerminationFillingRateOnConf->setDisabled(true);
    ui->lineEdit_TerminationMaximumStepOnConf->setDisabled(true);
    switch (id) {
    case Computing::TerminationEndTimeOn: {
        ui->lineEdit_TerminationEndTimeOnConf->setDisabled(false);
        break;
    }
    case Computing::TerminationFillingRateOn: {
        ui->lineEdit_TerminationFillingRateOnConf->setDisabled(false);
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
    QString dir = openDirDialog(this, "Project Library", "./");
    ui->lineEdit_projectLibrary->setText(dir);
}



void MainWindow::on_toolButton_projectPath_clicked()
{
    QString dir = openDirDialog(this, "Project Path", "./");
    ui->lineEdit_projectPath->setText(dir);
}

void MainWindow::on_toolButton_existingProjectPath_clicked()
{
    QString dir = openDirDialog(this, "Project Path", "./");
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
            table->insertRow(cur_row);
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
        QVariant v = item->data();
        if (v.isValid()) {
            Material *material = (Material*)(v.value<void*>());
            if (material) material->updateGui();
        }
    }
}

void MainWindow::on_toolButton_outletAdd_clicked()
{
    tableInsertRow(ui->tableWidget_outletCoordinates);
}

void MainWindow::on_toolButton_outletDelete_clicked()
{
    tableDeleteRow(ui->tableWidget_outletCoordinates);
}

void MainWindow::on_toolButton_inletAdd_clicked()
{
    tableInsertRow(ui->tableWidget_inletCoordinates);
}

void MainWindow::on_toolButton_inletDelete_clicked()
{
    tableDeleteRow(ui->tableWidget_inletCoordinates);
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
    Project *_project = new Project(L"Project");
    QString filename = dir+"/project.xml";
    if (_project->loadConfigFile(filename)) {
        if (project) {
            delete project;
        }
        project = _project;
        project->updateGui();
    } else {
        delete _project;
        _project = NULL;
    }
}

void MainWindow::on_action_save_triggered()
{

}

void MainWindow::on_toolButton_varyingVelocityOK_clicked()
{
    if (project) {
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

void MainWindow::on_toolButton_outletOK_clicked()
{
    if (project) {
        UpdateData(ui->tableWidget_outletCoordinates,
                   project->getEntrance()->getOutletCoordinates());
    }
}

void MainWindow::on_toolButton_outletCancel_clicked()
{
    if (project) {
        UpdateTable(project->getEntrance()->getOutletCoordinates(),
                    ui->tableWidget_outletCoordinates);
    }
}

void MainWindow::on_toolButton_inletOK_clicked()
{
    if (project) {
        UpdateData(ui->tableWidget_inletCoordinates,
                   project->getEntrance()->getInletCoordinates());
    }
}

void MainWindow::on_toolButton_inletCancel_clicked()
{
    if (project) {
        UpdateTable(project->getEntrance()->getInletCoordinates(),
                    ui->tableWidget_inletCoordinates);
    }
}

void MainWindow::on_pushButton_addMold_clicked()
{
    if (project) {
        project->getMold()->addConfiguration(L"");
    }
}

void MainWindow::on_pushButton_reloadMold_clicked()
{

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
