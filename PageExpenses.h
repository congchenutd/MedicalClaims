#ifndef PAGECLAIMS_H
#define PAGECLAIMS_H

#include "PageDefault.h"

class PageExpenses : public PageDefault
{
public:
    explicit PageExpenses(QWidget* parent = 0);

    void exportData(const QString& fileName);
    void setShowFilter(bool show);
};

#endif // PAGECLAIMS_H
