#include "DlgOptions.h"
#include "MainWindow.h"
#include "Page.h"
#include "Settings.h"
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
    ui.actionAutoFill   ->setEnabled(false);

    setFont(Settings::getInstance()->getUIFont());
    ui.tabWidget->setFont(Settings::getInstance()->getTableFont());

    connect(ui.actionOptions,   &QAction::triggered, this, &MainWindow::onOptions);
    connect(ui.actionAdd,       &QAction::triggered, this, &MainWindow::onAdd);
    connect(ui.actionDel,       &QAction::triggered, this, &MainWindow::onDel);
    connect(ui.actionSave,      &QAction::triggered, this, &MainWindow::onSave);
    connect(ui.actionExport,    &QAction::triggered, this, &MainWindow::onExport);
    connect(ui.actionAbout,     &QAction::triggered, this, &MainWindow::onAbout);
    connect(ui.actionDuplicate, &QAction::triggered, this, &MainWindow::onDuplicate);
    connect(ui.actionAutoFill,  &QAction::triggered, this, &MainWindow::onAutoFill);
    connect(ui.actionFilter,    &QAction::triggered, this, &MainWindow::onFilter);

    connect(ui.tabWidget, &QTabWidget   ::currentChanged,  this, &MainWindow::onCurrentTabChanged);
    connect(ui.tabClaims, &PageClaims   ::selectionChanged, this, &MainWindow::onSelectionChanged);
    connect(ui.tabClaims, &PagePatients ::selectionChanged, this, &MainWindow::onSelectionChanged);
    connect(ui.tabClaims, &PageProviders::selectionChanged, this, &MainWindow::onSelectionChanged);
}

void MainWindow::onOptions()
{
    DlgOptions dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        ui.tabWidget->setFont(dlg.getTableFont());
    }
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

void MainWindow::onSelectionChanged(const QModelIndexList& selected)
{
    ui.actionDel        ->setEnabled(!selected.isEmpty());
    ui.actionExport     ->setEnabled(!selected.isEmpty());
    ui.actionDuplicate  ->setEnabled(!selected.isEmpty());
    ui.actionAutoFill   ->setEnabled(!selected.isEmpty());

    statusBar()->showMessage(tr("Sum = %1").arg(_currentPage->sumUp()));
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
                       .arg("06/08/2017"));
}

void MainWindow::onDuplicate()
{
    if (_currentPage != 0)
        _currentPage->duplicate();
}

void MainWindow::onAutoFill()
{
    if (_currentPage != 0)
        _currentPage->autoFill();
}

void MainWindow::onFilter(bool show)
{
    if (_currentPage != 0)
        _currentPage->setShowFilter(show);
}
