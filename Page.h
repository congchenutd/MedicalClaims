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
};

#endif // PAGE_H
