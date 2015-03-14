#ifndef ADDSTL_H
#define ADDSTL_H

#include <QDialog>

namespace Ui {
class AddStl;
}

class AddStl : public QDialog
{
    Q_OBJECT

public:
    explicit AddStl(QWidget *parent = 0);
    ~AddStl();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_pushButton_OriginalStl_clicked();

private:
    Ui::AddStl *ui;
};

#endif // ADDSTL_H
