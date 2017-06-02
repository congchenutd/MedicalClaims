#ifndef PAGECLAIMS_H
#define PAGECLAIMS_H

#include "PageDefault.h"

class ClaimsModel;

class PageClaims : public PageDefault
{
    Q_OBJECT

public:
    explicit PageClaims(QWidget* parent = 0);

    void add();
    void exportData(const QString& fileName);

private slots:
    void onSelectionChanged(const QItemSelection& selected);
    void onDataChanged();

private:
    ClaimsModel* _model;
};

#endif // PAGECLAIMS_H
