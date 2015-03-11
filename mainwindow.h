#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QTableWidget>
#include <QDomDocument>
#include <QListWidgetItem>
#include <QStandardItemModel>
#include <QVariant>
#include <vector>
#include <utility>

class Project;
class MaterialGroup;

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

    static void UpdateTable(const vector< vector<double> >&data, QTableWidget *table);
    static void UpdateData(QTableWidget *table, vector< vector<double> >&data);

    QButtonGroup *get_buttonGroup_injectionMethod(void) {return buttonGroup_injectionMethod;}
    QButtonGroup *get_buttonGroup_moldSurfaceRoughness(void) {return buttonGroup_moldSurfaceRoughness;}
    QButtonGroup *get_buttonGroup_fluidModel(void) {return buttonGroup_fluidModel;}
    QButtonGroup *get_buttonGroup_timeStepMethod(void) {return buttonGroup_timeStepMethod;}
    QButtonGroup *get_buttonGroup_terminationCondition(void) {return buttonGroup_terminationCondition;}
    QButtonGroup *get_buttonGroup_parallelMethod(void) {return buttonGroup_parallelMethod;}
    QButtonGroup *get_buttonGroup_outputMethod(void) {return buttonGroup_outputMethod;}
    QStandardItemModel *getMaterialGroupModel(void) {return materialgroupModel;}

    static MainWindow *CurrentWindow;

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

    void on_actionOpen_Project_triggered();
    void on_actionMaterial_Group_triggered();

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

private:

    QString xmlPath;
    QString openFileDialog(QWidget *parent, const QString &caption, const QString &dir, const QString &filter);
    QString openDirDialog(QWidget *parent, const QString &caption, const QString &dir);
    void tableInsertRow(QTableWidget *table, int row_num = 1);
    void tableDeleteRow(QTableWidget *table, int row_num = 1);
    bool loadConfigFile(const QString& filename);
    bool loadMaterialFile(const QString& filename);

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

};

#endif // MAINWINDOW_H
