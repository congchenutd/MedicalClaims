#ifndef PAGECLAIMS_H
#define PAGECLAIMS_H

#include "PageDefault.h"

class ExpensesModel;
class AutoFillRule;

class PageExpenses : public PageDefault
{
    Q_OBJECT

public:
    explicit PageExpenses(QWidget* parent = 0);

    void exportData(const QString& fileName);
    void autoFill();
    void setShowFilter(bool show);

protected:
    void initRow(int row);
    void copyRow(int sourceRow, int destinationRow);

private slots:
    void onFilterChanged(int column, const QString& filterValue);

private:
    ExpensesModel* _model;
    QMap<int, AutoFillRule*> _autoFillRules;
};

#endif // PAGECLAIMS_H
