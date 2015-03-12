#include "category.h"
#include "mainwindow.h"
#include <map>
Category::Category(const wstring& _name) : name(_name)
{
}

Category::~Category() {
}

void* Category::getUi()
{
    if (MainWindow::CurrentWindow) return MainWindow::CurrentWindow->getUi();
    else return NULL;
}
