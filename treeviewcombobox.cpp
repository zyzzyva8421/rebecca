#include "treeviewcombobox.h"
#include "material.h"
#include <QTreeView>
#include <QEvent>
#include <QModelIndex>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <iostream>

TreeViewComboBox::TreeViewComboBox(QWidget *parent) :
    QComboBox(parent)
{
    treeview = new QTreeView(this);
    treeview->setWindowFlags(Qt::Popup);
    treeview->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(treeview, SIGNAL(clicked(QModelIndex)), this, SLOT(on_treeview_clicked(QModelIndex)));
    treeview->setAttribute(Qt::WA_WindowPropagation);
    treeview->setAttribute(Qt::WA_X11NetWmWindowTypeCombo);
}

void TreeViewComboBox::setModel(QStandardItemModel *model)
{
    if (treeview)
        treeview->setModel(model);
}

void TreeViewComboBox::on_treeview_clicked(QModelIndex index)
{
    QStandardItem *item = static_cast<QStandardItemModel*>(treeview->model())->itemFromIndex(index);
    if (item) {
        QVariant v = item->data();
        if (v.isValid()) {
            Material *material = (Material*)(v.value<void*>());
            if (material) {
                QString text = QString::fromStdWString(material->getId());
                clear();
                addItem(text);
                setCurrentIndex(0);
            }
        }
    }
    hidePopup();
}

void TreeViewComboBox::showPopup()
{
    if (treeview) {
        QRect rect = geometry();
        treeview->move(mapToGlobal(QPoint(0, rect.height())));
        treeview->show();
    } else {
        QComboBox::showPopup();
    }
}

void TreeViewComboBox::hidePopup()
{
    if (treeview) {
        treeview->hide();
    } else {
        QComboBox::hidePopup();
    }
}
