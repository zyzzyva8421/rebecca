#include <QFileDialog>
#include <QFile>
#include <QTextCodec>
#include <QModelIndex>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "project.h"
#include "category.h"
#include "casting.h"
#include "mold.h"
#include "simulation.h"
#include "computing.h"
#include "output.h"
#include "material.h"
MainWindow *MainWindow::CurrentWindow = NULL;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    project = NULL;
    materialgroup = NULL;
    materialgroupModel = new QStandardItemModel(ui->treeView_materials);
    ui->treeView_materials->setModel(materialgroupModel);
    ui->treeView_materials->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->treeView_materials->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(on_treeView_materials_currentRowChanged(QModelIndex,QModelIndex)));
    ui->comboBox_material->setModel(materialgroupModel);
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
    connect(ui->actionOpen_Project, SIGNAL(triggered()), this, SLOT(on_actionOpen_Project_triggered()));
    connect(ui->actionMaterial_Group, SIGNAL(triggered()), this, SLOT(on_actionMaterial_Group_triggered()));

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

}

void MainWindow::UpdateTable(const vector< vector<double> >&data, QTableWidget *table)
{
    table->setRowCount(0);
    vector< vector<double> >::const_iterator it;
    vector<double> p;
    for (it = data.begin(); it != data.end(); it++) {
        p = (*it);
        table->insertRow(table->rowCount());
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
        for (int col = 0; col < table->colorCount(); i++) {
            w = table->item(i, col);
            p.push_back(w->text().toDouble());
            data.push_back(p);
        }
    }
}

void MainWindow::on_actionOpen_Project_triggered()
{
    QString filename = openFileDialog(this, "Open Project", xmlPath, "XML files (*.xml)");
    if (!filename.isEmpty()) {
        loadConfigFile(filename);
    }
    if (project) project->updateGui();
}

void MainWindow::on_actionMaterial_Group_triggered()
{
    QString filename = openFileDialog(this, "Material Group", xmlPath, "XML files (*.xml)");
    if (!filename.isEmpty()) {
        loadMaterialFile(filename);
    }
    if (materialgroup) materialgroup->updateGui();
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
    ui->toolButton_varyingPressureAdd->setDisabled(true);
    ui->toolButton_varyingPressureDelete->setDisabled(true);
    ui->tableWidget_varyingVelocity->setDisabled(true);
    ui->toolButton_varyingVelocityAdd->setDisabled(true);
    ui->toolButton_varyingVelocityDelete->setDisabled(true);
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
        ui->toolButton_varyingPressureAdd->setDisabled(false);
        ui->toolButton_varyingPressureDelete->setDisabled(false);
        break;
    }
    case Casting::VaryingVelocityOn: {
        ui->tableWidget_varyingVelocity->setDisabled(false);
        ui->toolButton_varyingVelocityAdd->setDisabled(false);
        ui->toolButton_varyingVelocityDelete->setDisabled(false);
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
        }
    } else {
        for (int i = 0; i < row_num; i++) {
            table->insertRow(cur_row);
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

bool MainWindow::loadMaterialFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Can not read file" << qPrintable(filename)
                     << "; " << qPrintable(file.errorString())
                        << std::endl;

    }
    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
        std::cerr << "Error: Parse error at line " << errorLine << ", "
                     << "column " << errorColumn << ": "
                        << qPrintable(errorStr) << std::endl;
        return false;
    }
    QDomElement root = doc.documentElement();
    if (root.tagName() != "materialgroup") {
        std::cerr << "Error: Not a material file" << std::endl;
        return false;
    }

    materialgroup = new MaterialGroup("materialgroup");
    materialgroup->loadValue(root);
    materialgroup->updateGui();
    file.close();
    return true;
}

bool MainWindow::loadConfigFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Can not read file" << qPrintable(filename)
                     << "; " << qPrintable(file.errorString())
                        << std::endl;

    }
    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) {
        std::cerr << "Error: Parse error at line " << errorLine << ", "
                     << "column " << errorColumn << ": "
                        << qPrintable(errorStr) << std::endl;
        return false;
    }
    xmlPath = filename;
    QDomElement root = doc.documentElement();
    if (root.tagName() != "Project") {
        std::cerr << "Error: Not a project file" << std::endl;
        return false;
    }
    string name = filename.toStdString();
    project = new Project(name);
    project->loadValue(root);
    file.close();
    return true;
}

void MainWindow::on_listWidget_addedStlMolds_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Mold *mold = dynamic_cast<Mold*>(project->getCategory("Mold"));
    if (mold) {
        if (current) {
            string str = current->text().toStdString();
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
