#ifndef PAGECLAIMS_H
#define PAGECLAIMS_H

#include "PageDefault.h"

class QDataWidgetMapper;

class PageExpenses : public PageDefault
{
public:
    explicit PageExpenses(QWidget* parent = 0);

    void exportData(const QString& fileName);
    void setShowFilter(bool show);

private slots:
    void onSelectionChanged() override;

private:
    QDataWidgetMapper* _mapper;
};

#endif // PAGECLAIMS_H
