#ifndef PAGE_H
#define PAGE_H

#include <QWidget>

class Page: public QWidget
{
public:
    Page(QWidget* parent);
    virtual ~Page();

    virtual void add() = 0;
    virtual void del() = 0;
    virtual void save() = 0;
    virtual void refresh() = 0;
    virtual void exportData(const QString& fileName) = 0;
    virtual void duplicate() = 0;
    virtual void autoFill() = 0;
    virtual void setShowFilter(bool show) = 0;
    virtual double sumUpSelected() const = 0;
    virtual void clearCell() = 0;
};

#endif // PAGE_H
