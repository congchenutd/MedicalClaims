#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

class Page;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    void onOptions();
    void onCurrentTabChanged(int index);
    void onAdd();
    void onDel();
    void onSave();
    void onSelectionChanged(const QModelIndexList& selected);
    void onExport();
    void onAbout();
    void onDuplicate();
    void onAutoFill();
    void onFilter(bool show);

private:
    Ui::MainWindow ui;
    Page*   _currentPage;
};

#endif // MAINWINDOW_H
