#ifndef ADDMATERIAL_H
#define ADDMATERIAL_H

#include <QDialog>

namespace Ui {
class AddMaterial;
}

class AddMaterial : public QDialog
{
    Q_OBJECT

public:
    enum Type {
        GroupType,
        MaterialType
    };
    enum Level {
        SameLevel,
        NextLevel
    };
    explicit AddMaterial(QWidget *parent = 0);
    ~AddMaterial();
    void updateGui(void);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_buttonGroup_type_clicked(int);

private:
    Ui::AddMaterial *ui;
};

#endif // ADDMATERIAL_H
