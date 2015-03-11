#ifndef TREEVIEWCOMBOBOX_H
#define TREEVIEWCOMBOBOX_H
#include <QComboBox>
#include <QTreeView>
#include <QStandardItemModel>
#include <QMouseEvent>
class TreeViewComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit TreeViewComboBox(QWidget *parent = 0);
    virtual void showPopup();
    virtual void hidePopup();
    void setModel(QStandardItemModel *model);

public slots:
    void on_treeview_clicked(QModelIndex index);

private:
    QTreeView *treeview;

};

#endif // TREEVIEWCOMBOBOX_H
