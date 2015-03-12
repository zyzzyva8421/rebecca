#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
class Project;
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    static Dialog* CurrentDialog;
    Ui::Dialog *getUi(void) {return ui;}

private slots:
    void on_checkBox_BasedProjectOn_stateChanged(int arg1);

    void on_pushButton_BasedProjectPath_clicked();

    void on_pushButton_ProjectLibaryPath_clicked();

    void on_spinBox_CastingNumber_valueChanged(int arg1);

    void on_lineEdit_CastingName_textChanged(const QString &arg1);

    void on_lineEdit_ProjectLibaryPath_textChanged(const QString &arg1);

    void on_lineEdit_BasedProjectPath_textChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Dialog *ui;
    Project *project;
};

#endif // DIALOG_H
