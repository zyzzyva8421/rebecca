#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QTableWidget>
#include <QDomDocument>
#include <QListWidgetItem>
#include <QStandardItemModel>
#include <QProcess>
#include <QVariant>
#include <vector>
#include <utility>
#include <iostream>

class Project;
class MaterialGroup;
class Dialog;
class AddMaterial;
class Material;
class CopyMaterial;
class AddStl;

using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *getUi(void) {return ui;}
    Material *getCurrentMaterial(void) {return currentMaterial;}

    static void UpdateTable(const vector< vector<double> >&data, QTableWidget *table);
    static void UpdateData(QTableWidget *table, vector< vector<double> >&data);
    static QString openFileDialog(QWidget *parent, const QString &caption, const QString &dir, const QString &filter);
    static QString openDirDialog(QWidget *parent, const QString &caption, const QString &dir);

    QButtonGroup *get_buttonGroup_injectionMethod(void) {return buttonGroup_injectionMethod;}
    QButtonGroup *get_buttonGroup_moldSurfaceRoughness(void) {return buttonGroup_moldSurfaceRoughness;}
    QButtonGroup *get_buttonGroup_fluidModel(void) {return buttonGroup_fluidModel;}
    QButtonGroup *get_buttonGroup_timeStepMethod(void) {return buttonGroup_timeStepMethod;}
    QButtonGroup *get_buttonGroup_terminationCondition(void) {return buttonGroup_terminationCondition;}
    QButtonGroup *get_buttonGroup_parallelMethod(void) {return buttonGroup_parallelMethod;}
    QButtonGroup *get_buttonGroup_outputMethod(void) {return buttonGroup_outputMethod;}
    QStandardItemModel *getMaterialGroupModel(void) {return materialgroupModel;}
    MaterialGroup *getMaterialGroup(void) {return materialgroup;}

    static MainWindow *CurrentWindow;

    void setProject(Project *_project);
    void setIsMaterialGroupChanged(bool isChanged) {isMaterialGroupChanged = isChanged;}
    bool getIsMaterialGroupChanged(void) {return isMaterialGroupChanged;}
    Project *getProject(void) {return project;}

private slots:

    void on_checkBox_basedOnExistingProject_stateChanged(int arg1);

    void on_toolButton_projectLibrary_clicked();

    void on_toolButton_projectPath_clicked();

    void on_toolButton_existingProjectPath_clicked();

    void on_buttonGroup_injectionMethod_clicked(int id);

    void on_toolButton_varyingVelocityAdd_clicked();

    void on_toolButton_varyingVelocityDelete_clicked();

    void on_toolButton_varyingPressureAdd_clicked();

    void on_toolButton_varyingPressureDelete_clicked();

    void on_action_project_triggered();
    void on_action_material_triggered();
    void on_action_simulate_triggered();
    void on_action_open_triggered();
    void on_action_save_triggered();
    void on_action_stop_triggered();
    void on_action_log_triggered();
    void on_action_result_triggered();
    void on_action_clean_triggered();
    void on_action_dir_triggered();
    void on_action_close_triggered();

    void on_listWidget_addedStlMolds_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_treeView_materials_currentRowChanged(QModelIndex current, QModelIndex previous);

    void on_toolButton_outletAdd_clicked();

    void on_toolButton_outletDelete_clicked();

    void on_toolButton_inletAdd_clicked();

    void on_toolButton_inletDelete_clicked();

    void on_checkBox_fluidModel_stateChanged(int arg1);

    void on_buttonGroup_timeStepMethod_clicked(int id);

    void on_buttonGroup_terminationCondition_clicked(int id);

    void on_buttonGroup_parallelMethod_clicked(int id);

    void on_buttonGroup_outputMethod_clicked(int id);

    void on_toolButton_varyingVelocityOK_clicked();

    void on_toolButton_varyingVelocityCancel_clicked();

    void on_toolButton_varyingPressureOK_clicked();

    void on_toolButton_varyingPressCancel_clicked();

    void on_toolButton_outletOK_clicked();

    void on_toolButton_outletCancel_clicked();

    void on_toolButton_inletOK_clicked();

    void on_toolButton_inletCancel_clicked();

    void on_pushButton_addMold_clicked();

    void on_pushButton_reloadMold_clicked();

    void on_pushButton_deleteMold_clicked();

    void on_pushButton_modifyMold_clicked();

    void on_pushButton_materialNew_clicked();

    void on_pushButton_materialUpdate_clicked();

    void on_pushButton_materialCancel_clicked();

    void on_pushButton_materialDelete_clicked();

    void on_pushButton_materialCopy_clicked();

    void on_stdoupt_update();

    void on_pushButton_saveMaterialGroup_clicked();

    void on_pushButton_moldCancel_clicked();

private:

    void tableInsertRow(QTableWidget *table, int row_num = 1);
    void tableDeleteRow(QTableWidget *table, int row_num = 1);

    Ui::MainWindow *ui;
    QButtonGroup *buttonGroup_injectionMethod;
    QButtonGroup *buttonGroup_moldSurfaceRoughness;
    QButtonGroup *buttonGroup_fluidModel;
    QButtonGroup *buttonGroup_timeStepMethod;
    QButtonGroup *buttonGroup_terminationCondition;
    QButtonGroup *buttonGroup_parallelMethod;
    QButtonGroup *buttonGroup_outputMethod;
    Project *project;
    MaterialGroup *materialgroup;
    QStandardItemModel *materialgroupModel;
    Dialog *dialog;
    AddMaterial *addmaterial;
    Material *currentMaterial;
    CopyMaterial *copymaterial;
    AddStl *addstl;
    QProcess *process;
    bool isMaterialGroupChanged;

};

#endif // MAINWINDOW_H
