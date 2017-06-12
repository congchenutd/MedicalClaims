#ifndef PAGEDEFAULT_H
#define PAGEDEFAULT_H

#include "Page.h"
#include "ui_PageDefault.h"

class MyModel;

class PageDefault : public Page
{
    Q_OBJECT

public:
    void add();
    void del();
    void save();
    void refresh();
    void exportData(const QString& fileName);
    void duplicate();
    void autoFill();
    void setShowFilter(bool show);
    double sumUpSelected() const;
    void clearCell();

protected:
    explicit PageDefault(QWidget* parent = 0);
    void setModel(MyModel* model);
    void initRow(int row);
    void copyRow(int sourceRow, int destinationRow);

    QList<int> getSelectedRows() const;
    QModelIndexList getSelectedIndexes() const;

    void keepSorted();

private slots:
    void onSelectionChanged();

signals:
    void selectionChanged(const QModelIndexList selected);

protected:
    Ui::PageDefault ui;
    MyModel* _model;
    int      _currentRow;

    enum {COL_ID};
};

#endif // PAGEDEFAULT_H
