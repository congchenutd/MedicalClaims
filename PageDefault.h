#ifndef PAGEDEFAULT_H
#define PAGEDEFAULT_H

#include "Page.h"
#include "ui_PageDefault.h"
#include <QSqlTableModel>

class PageDefault : public Page
{
    Q_OBJECT

public:
    void add();
    void del();
    void save();
    void refresh();

protected:
    explicit PageDefault(QWidget* parent = 0);
    void initModel(QSqlTableModel* model);

private slots:
    void onSelectionChanged(const QItemSelection& selected);

signals:
    void currentRowValid(bool);

protected:
    Ui::PageDefault ui;
    QSqlTableModel* _model;
    int             _currentRow;

    enum {COL_ID};
};

#endif // PAGEDEFAULT_H
