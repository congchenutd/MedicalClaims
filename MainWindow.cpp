#include "MainWindow.h"
#include "Page.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    _currentPage = ui.tabClaims;

    ui.actionDel    ->setEnabled(false);
    ui.actionExport ->setEnabled(false);

    connect(ui.actionOptions,   &QAction::triggered, this, &MainWindow::onOptions);
    connect(ui.actionAdd,       &QAction::triggered, this, &MainWindow::onAdd);
    connect(ui.actionDel,       &QAction::triggered, this, &MainWindow::onDel);
    connect(ui.actionSave,      &QAction::triggered, this, &MainWindow::onSave);
    connect(ui.actionExport,    &QAction::triggered, this, &MainWindow::onExport);

    connect(ui.tabWidget, &QTabWidget   ::currentChanged,  this, &MainWindow::onCurrentTabChanged);
    connect(ui.tabClaims, &PageClaims   ::currentRowValid, this, &MainWindow::onRowSelected);
    connect(ui.tabClaims, &PagePatients ::currentRowValid, this, &MainWindow::onRowSelected);
    connect(ui.tabClaims, &PageProviders::currentRowValid, this, &MainWindow::onRowSelected);
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

void MainWindow::onRowSelected(bool selected)
{
    ui.actionDel   ->setEnabled(selected);
    ui.actionExport->setEnabled(selected);
}

void MainWindow::onExport()
{

}
