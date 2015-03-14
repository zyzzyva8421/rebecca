#include "copymaterial.h"
#include "ui_copymaterial.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "material.h"
#include <QMessageBox>

CopyMaterial::CopyMaterial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyMaterial)
{
    ui->setupUi(this);
    MainWindow *main = (MainWindow*)(parentWidget());
    ui->treeView_materialGroup->setModel(main->getMaterialGroupModel());
    ui->treeView_materialGroup->setSelectionMode(QAbstractItemView::SingleSelection);
}

CopyMaterial::~CopyMaterial()
{
    delete ui;
}

void CopyMaterial::on_buttonBox_accepted()
{
    wstring name = ui->lineEdit_materialName->text().toStdWString();
    if (name.empty()) {
        QMessageBox::critical(this, QString::fromStdWString(L"新建材料"), QString::fromStdWString(L"材料名不能为空，请重新命名材料"));
        return;
    }
    MainWindow *main = (MainWindow*)(parentWidget());
    if (main->getMaterialGroup()->checkMaterialId(name)) {
        QMessageBox::critical(this, QString::fromStdWString(L"新建材料"), QString::fromStdWString(L"材料重名，请重新命名材料"));
        return;
    }
    QModelIndex index = ui->treeView_materialGroup->currentIndex();
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
                    main->setIsMaterialGroupChanged(true);
                    main->getUi()->pushButton_saveMaterialGroup->setDisabled(false);
                } else {
                    QMessageBox::critical(this, QString::fromStdWString(L"新建材料"), QString::fromStdWString(L"请选择材料库目录"));
                    return;
                }
            }
        }
    } else {
        QMessageBox::critical(this, QString::fromStdWString(L"新建材料"), QString::fromStdWString(L"请选择材料库目录"));
        return;
    }
    close();
}

void CopyMaterial::on_buttonBox_rejected()
{
    close();
}
