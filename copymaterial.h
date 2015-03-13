#ifndef COPYMATERIAL_H
#define COPYMATERIAL_H

#include <QDialog>

namespace Ui {
class CopyMaterial;
}

class CopyMaterial : public QDialog
{
    Q_OBJECT

public:
    explicit CopyMaterial(QWidget *parent = 0);
    ~CopyMaterial();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::CopyMaterial *ui;
};

#endif // COPYMATERIAL_H
