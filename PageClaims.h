#ifndef PAGECLAIMS_H
#define PAGECLAIMS_H

#include "PageDefault.h"

class PageClaims : public PageDefault
{
    Q_OBJECT

public:
    explicit PageClaims(QWidget* parent = 0);

    void add();
    void exportData(const QString& fileName);

private slots:
    void onSelectionChanged(const QItemSelection& selected);

public:
    enum HeaderData {
        COL_ID, COL_PATIENT, COL_PROVIDER, COL_SERVICE_START, COL_SERVICE_END,
        COL_BILLED, COL_COVERED, COL_INSURANCE_PAID, COL_NOT_COVERED, COL_DEDUCTIBLE,
        COL_COINSURANCE, COL_I_PAID, COL_FSA_CLAIMED, COL_HSA_CLAIMED};
    Q_ENUM(HeaderData)
};

#endif // PAGECLAIMS_H
