#ifndef FILTERTABLEHEADER_H
#define FILTERTABLEHEADER_H

#include <QHeaderView>
#include <QList>

class QLineEdit;
class QTableView;
class FilterLineEdit;

class FilterTableHeader : public QHeaderView
{
    Q_OBJECT

public:
    explicit FilterTableHeader(QTableView* parent = 0);
    virtual QSize sizeHint() const;
    void setShowFilters(bool show);
    
public slots:
    void generateFilters();
    void adjustPositions();
    void clearFilters();
    void setFilter(int column, const QString& value);

signals:
    void filterChanged(int column, const QString& text);

protected:
    virtual void updateGeometries();

private:
    QList<FilterLineEdit*> _filterLineEdits;
};

#endif
