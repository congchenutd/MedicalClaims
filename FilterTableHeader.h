#ifndef FILTERTABLEHEADER_H
#define FILTERTABLEHEADER_H

#include <QHeaderView>
#include <QList>

class QTableView;
class FilterLineEdit;

class FilterTableHeader : public QHeaderView
{
    Q_OBJECT

public:
    explicit FilterTableHeader(QTableView* parent = 0);
    void generateFilters();
    virtual QSize sizeHint() const;
    void setShowFilters(bool show);
    
public slots:
    void adjustPositions();

signals:
    void filterChanged(int column, const QString& text);

protected:
    virtual void updateGeometries();

private:
    QList<FilterLineEdit*> _filterLineEdits;
};

#endif
