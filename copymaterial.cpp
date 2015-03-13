#include "copymaterial.h"
#include "ui_copymaterial.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "material.h"

CopyMaterial::CopyMaterial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyMaterial)
{
    ui->setupUi(this);
}

CopyMaterial::~CopyMaterial()
{
    delete ui;
}

void CopyMaterial::on_buttonBox_accepted()
{
    wstring name = ui->lineEdit_materialName->text().toStdWString();
    if (name.empty()) {
        return;
    }
    MainWindow *main = (MainWindow*)(parentWidget());
    QModelIndex index = main->getUi()->treeView_materials->currentIndex();
    QStandardItem *item = main->getMaterialGroupModel()->itemFromIndex(index);
    if (item) {
        QVariant v = item->data();
        if (v.isValid()) {
            Category *category = static_cast<Category*>(v.value<void*>());
            if (category) {
                if (category->getName() == L"materialgroup") {
                    Material *newmaterial = new Material(L"material", main->getCurrentMaterial());
                    newmaterial->setId(name);
                    ((MaterialGroup*)category)->addMaterial(newmaterial);
                    newmaterial->updateModel(item);
                }
            }
        }
    }
    close();
}

void CopyMaterial::on_buttonBox_rejected()
{
    close();
}
