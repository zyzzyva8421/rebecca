#include "addmaterial.h"
#include "ui_addmaterial.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "material.h"
#include <QStandardItemModel>
#include <QMessageBox>
#include <QTreeView>
#include <iostream>

AddMaterial::AddMaterial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMaterial)
{
    ui->setupUi(this);
    QButtonGroup *buttonGroup_type = new QButtonGroup(this);
    buttonGroup_type->addButton(ui->radioButton_group, GroupType);
    buttonGroup_type->addButton(ui->radioButton_material, MaterialType);
    connect(buttonGroup_type, SIGNAL(buttonClicked(int)), this, SLOT(on_buttonGroup_type_clicked(int)));

    QButtonGroup *buttonGroup_level = new QButtonGroup(this);
    buttonGroup_level->addButton(ui->radioButton_sameLevel, SameLevel);
    buttonGroup_level->addButton(ui->radioButton_nextLevel, NextLevel);

}

void AddMaterial::updateGui() {
    ui->radioButton_group->click();
    ui->radioButton_sameLevel->click();
    ui->radioButton_sameLevel->setDisabled(false);
    ui->radioButton_nextLevel->setDisabled(false);

    MainWindow *main = (MainWindow*)(parentWidget());
    QModelIndex index = main->getUi()->treeView_materials->currentIndex();
    QStandardItem *item = main->getMaterialGroupModel()->itemFromIndex(index);
    if (item) {
        QVariant v = item->data();
        if (v.isValid()) {
            Category *category = static_cast<Category*>(v.value<void*>());
            if (category) {
                if (category->getName() == L"materialgroup" && ((MaterialGroup*)category)->getGroup() == NULL) {
                    ui->radioButton_nextLevel->click();
                    ui->radioButton_sameLevel->setDisabled(true);
                } else if (category->getName() == L"material") {
                    ui->radioButton_sameLevel->click();
                    ui->radioButton_nextLevel->setDisabled(true);
                }
            }
        }
    }
}

AddMaterial::~AddMaterial()
{
    delete ui;
}

void AddMaterial::on_buttonBox_accepted()
{
    wstring name;
    if (ui->radioButton_group->isChecked()) {
        name = ui->lineEdit_group->text().toStdWString();
    } else {
        name = ui->lineEdit_material->text().toStdWString();
    }
    if (name.empty()) {
        return;
    }

    MainWindow *main = (MainWindow*)(parentWidget());
    if (ui->radioButton_material->isChecked() && main->getMaterialGroup()->checkMaterialId(name)) {
        QMessageBox::critical(this, QString::fromStdWString(L"新建材料"), QString::fromStdWString(L"材料重名，请重新命名材料"));
        return;
    }
    QModelIndex index = main->getUi()->treeView_materials->currentIndex();
    QStandardItem *item = main->getMaterialGroupModel()->itemFromIndex(index);
    if (item) {
        QVariant v = item->data();
        if (v.isValid()) {
            Category *category = static_cast<Category*>(v.value<void*>());
            if (category) {
                if (category->getName() == L"materialgroup") {
                    MaterialGroup *group = (MaterialGroup*)category;
                    if (ui->radioButton_group->isChecked()) {
                        // add group in group
                        MaterialGroup *newgroup = new MaterialGroup(L"materialgroup");
                        newgroup->setId(name);
                        if (ui->radioButton_nextLevel->isChecked()) {
                            // in next level
                            group->addGroup(newgroup);
                            newgroup->updateModel(item);
                        } else {
                            // in current level
                            MaterialGroup *parent = group->getGroup();
                            if (parent) {
                                parent->addGroup(newgroup);
                                newgroup->updateModel(item->parent());
                            }
                        }
                    } else {
                        // add material in group
                        Material *newmaterial = new Material(L"material");
                        newmaterial->setId(name);
                        if (ui->radioButton_nextLevel->isChecked()) {
                            // in next level
                            group->addMaterial(newmaterial);
                            newmaterial->updateModel(item);
                        } else {
                            // in current level
                            MaterialGroup *parent = group->getGroup();
                            if (parent) {
                                parent->addMaterial(newmaterial);
                                newmaterial->updateModel(item->parent());
                            }
                        }
                    }
                } else if (category->getName() == L"material") {
                    Material *material = (Material*)material;
                    if (ui->radioButton_group->isChecked()) {
                        // add group in material
                        MaterialGroup *newgroup = new MaterialGroup(L"materialgroup");
                        newgroup->setId(name);
                        if (ui->radioButton_sameLevel->isChecked()) {
                            // in current level
                            MaterialGroup *parent = material->getGroup();
                            if (parent) {
                                parent->addGroup(newgroup);
                                newgroup->updateModel(item->parent());
                            }
                        }
                    } else {
                        // add material in material
                        Material *newmaterial = new Material(L"material");
                        newmaterial->setId(name);
                        if (ui->radioButton_sameLevel->isChecked()) {
                            // in current level
                            MaterialGroup *parent = material->getGroup();
                            if (parent) {
                                parent->addMaterial(newmaterial);
                                newmaterial->updateModel(item->parent());
                            }
                        }
                    }
                }
            }
        }
    }
    close();
}

void AddMaterial::on_buttonBox_rejected()
{
    close();
}

void AddMaterial::on_buttonGroup_type_clicked(int id)
{
    ui->lineEdit_group->setDisabled(true);
    ui->lineEdit_material->setDisabled(true);
    if (id == GroupType) {
        ui->lineEdit_group->setDisabled(false);
    } else {
        ui->lineEdit_material->setDisabled(false);
    }
}

