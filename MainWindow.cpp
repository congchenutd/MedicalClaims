#include "MainWindow.h"
#include "Page.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    _currentPage = ui.tabClaims;

    connect(ui.actionOptions,   &QAction::triggered,  this, &MainWindow::onOptions);
    connect(ui.actionAdd,       &QAction::triggered,  this, &MainWindow::onAdd);
    connect(ui.actionDel,       &QAction::triggered,  this, &MainWindow::onDel);
    connect(ui.actionSave,      &QAction::triggered,  this, &MainWindow::onSave);
    connect(ui.tabWidget,       &QTabWidget::currentChanged, this, &MainWindow::onCurrentTabChanged);
}

void MainWindow::onOptions()
{

}

void MainWindow::onCurrentTabChanged(int index)
{
    _currentPage = static_cast<Page*>(ui.tabWidget->widget(index));
    _currentPage->refresh();
}

void MainWindow::onAdd()
{
    if (_currentPage != 0)
        _currentPage->add();
}

void MainWindow::onDel()
{
    if (_currentPage != 0)
        _currentPage->del();
}

void MainWindow::onSave()
{
    if (_currentPage != 0)
        _currentPage->save();
}
