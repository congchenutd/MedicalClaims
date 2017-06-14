#include "DlgOptions.h"
#include "MainWindow.h"
#include "Page.h"
#include "Settings.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    _currentPage = ui.tabExpenses;

    ui.actionDel        ->setEnabled(false);
    ui.actionExport     ->setEnabled(false);
    ui.actionDuplicate  ->setEnabled(false);
    ui.actionAutoFill   ->setEnabled(false);
    ui.actionClear      ->setEnabled(false);

    setFont(Settings::getInstance()->getUIFont());
    ui.tabWidget->setFont(Settings::getInstance()->getTableFont());

    connect(ui.actionOptions,   &QAction::triggered,    this, &MainWindow::onOptions);
    connect(ui.actionAdd,       &QAction::triggered,    this, &MainWindow::onAdd);
    connect(ui.actionDel,       &QAction::triggered,    this, &MainWindow::onDel);
    connect(ui.actionSave,      &QAction::triggered,    this, &MainWindow::onSave);
    connect(ui.actionExport,    &QAction::triggered,    this, &MainWindow::onExport);
    connect(ui.actionAbout,     &QAction::triggered,    this, &MainWindow::onAbout);
    connect(ui.actionDuplicate, &QAction::triggered,    this, &MainWindow::onDuplicate);
    connect(ui.actionAutoFill,  &QAction::triggered,    this, &MainWindow::onAutoFill);
    connect(ui.actionClear,     &QAction::triggered,    this, &MainWindow::onClearCell);
    connect(ui.actionFilter,    &QAction::toggled,      this, &MainWindow::onFilter);
    connect(ui.btEsc,           &QPushButton::clicked,  this, &MainWindow::onESC);

    connect(ui.tabWidget,       &QTabWidget   ::currentChanged,   this, &MainWindow::onCurrentTabChanged);
    connect(ui.tabExpenses,     &PageExpenses ::selectionChanged, this, &MainWindow::onSelectionChanged);
    connect(ui.tabPatients,     &PagePatients ::selectionChanged, this, &MainWindow::onSelectionChanged);
    connect(ui.tabProviders,    &PageProviders::selectionChanged, this, &MainWindow::onSelectionChanged);

    connect(ui.tabExpenses,     &PageExpenses::modelDirty,  this, &MainWindow::onModelDirty);
    connect(ui.tabPatients,     &PagePatients::modelDirty,  this, &MainWindow::onModelDirty);
    connect(ui.tabProviders,    &PageProviders::modelDirty, this, &MainWindow::onModelDirty);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    backup();
    event->accept();
}

void MainWindow::backup()
{
    // get all the .db files
    const QString backupDir = "Backup";
    QDir::current().mkdir(backupDir);
    QFileInfoList fileInfos =
        QDir(backupDir).entryInfoList(QStringList() << "*.db", QDir::Files);

    // for each .db file, remove it if it's older than daysToKeep
    int daysToKeep = Settings::getInstance()->getBackupDays();
    foreach(QFileInfo fileInfo, fileInfos)
    {
        int age = static_cast<int>(fileInfo.lastModified().date().daysTo(QDate::currentDate()));
        if(age >= daysToKeep)
            QFile::remove(fileInfo.filePath());
    }

    // backup current db
    if (daysToKeep > 0 && _modelIsDirty)
    {
        extern QString dbFileName;
        QString backupFileName = backupDir + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss") + ".db";
        QFile(dbFileName).copy(backupFileName);
    }
}

void MainWindow::onOptions()
{
    DlgOptions dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        ui.tabWidget->setFont(dlg.getTableFont());
    }
}

void MainWindow::onCurrentTabChanged(int index)
{
    _currentPage = static_cast<Page*>(ui.tabWidget->widget(index));
    _currentPage->refresh();
}

void MainWindow::onAdd() {
    _currentPage->add();
}

void MainWindow::onDel() {
    _currentPage->del();
}

void MainWindow::onSave() {
    _currentPage->save();
}

void MainWindow::onSelectionChanged(const QModelIndexList& selected)
{
    ui.actionDel        ->setEnabled(!selected.isEmpty());
    ui.actionExport     ->setEnabled(!selected.isEmpty());
    ui.actionDuplicate  ->setEnabled(!selected.isEmpty());
    ui.actionAutoFill   ->setEnabled(!selected.isEmpty());
    ui.actionClear      ->setEnabled(!selected.isEmpty());

    statusBar()->showMessage(tr("    Sum = %1").arg(_currentPage->sumUpSelected()));
}

void MainWindow::onExport()
{
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
                       .arg("06/14/2017"));
}

void MainWindow::onDuplicate() {
    _currentPage->duplicate();
}

void MainWindow::onAutoFill() {
    _currentPage->autoFill();
}

void MainWindow::onFilter(bool show) {
    _currentPage->setShowFilter(show);
}

void MainWindow::onClearCell() {
    _currentPage->clearCell();
}

void MainWindow::onESC() {
    if (ui.actionFilter->isChecked())
        ui.actionFilter->toggle();
}

void MainWindow::onModelDirty(bool isDirty) {
    _modelIsDirty = isDirty;
}
