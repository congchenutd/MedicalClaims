#ifndef PAGECLAIMS_H
#define PAGECLAIMS_H

#include "PageDefault.h"

class ClaimsModel;
class AutoFillRule;

class PageClaims : public PageDefault
{
    Q_OBJECT

public:
    explicit PageClaims(QWidget* parent = 0);

    void exportData(const QString& fileName);
    void autoFill();

protected:
    void initRow(int row);
    void copyRow(int sourceRow, int destinationRow);

private slots:
    void onSelectionChanged(const QItemSelection& selected);

private:
    ClaimsModel* _model;
    QMap<int, AutoFillRule*> _autoFillRules;
};

#endif // PAGECLAIMS_H
