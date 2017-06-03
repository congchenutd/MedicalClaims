#include "MainWindow.h"
#include "Page.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    _currentPage = ui.tabClaims;

    ui.actionDel        ->setEnabled(false);
    ui.actionExport     ->setEnabled(false);
    ui.actionDuplicate  ->setEnabled(false);

    connect(ui.actionOptions,   &QAction::triggered, this, &MainWindow::onOptions);
    connect(ui.actionAdd,       &QAction::triggered, this, &MainWindow::onAdd);
    connect(ui.actionDel,       &QAction::triggered, this, &MainWindow::onDel);
    connect(ui.actionSave,      &QAction::triggered, this, &MainWindow::onSave);
    connect(ui.actionExport,    &QAction::triggered, this, &MainWindow::onExport);
    connect(ui.actionAbout,     &QAction::triggered, this, &MainWindow::onAbout);
    connect(ui.actionDuplicate, &QAction::triggered, this, &MainWindow::onDuplicate);

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
    ui.actionDel        ->setEnabled(selected);
    ui.actionExport     ->setEnabled(selected);
    ui.actionDuplicate  ->setEnabled(selected);
}

void MainWindow::onExport()
{
    if (_currentPage == 0)
        return;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Export to File"),
                                 ".",
                                 tr("Comma-Separated Values (*.csv)"));
    if (fileName.isEmpty())
        return;

    _currentPage->exportData(fileName);
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "About",
        tr("<h3><b>Medical Claims: A Medical Claims Manager</b></h3>"
           "<p>Built on %1</p>"
           "<p>Cong Chen &lt;<a href=mailto:CongChenUTD@Gmail.com>CongChenUTD@Gmail.com</a>&gt;</p>")
                       .arg("06/02/2017"));
}

void MainWindow::onDuplicate()
{
    if (_currentPage != 0)
        _currentPage->duplicate();
}
