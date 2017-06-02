#ifndef PAGECLAIMS_H
#define PAGECLAIMS_H

#include "PageDefault.h"

class ClaimsModel;

class PageClaims : public PageDefault
{
    Q_OBJECT

public:
    explicit PageClaims(QWidget* parent = 0);

    void exportData(const QString& fileName);

protected:
    void initRow(int row);
    void copyRow(int sourceRow, int destinationRow);

private slots:
    void onSelectionChanged(const QItemSelection& selected);
    void updateMyResponsibility();
    void updateServiceEnd();

private:
    ClaimsModel* _model;
};

#endif // PAGECLAIMS_H
