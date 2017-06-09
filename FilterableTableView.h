#ifndef FILTERABLETABLEVIEW_H
#define FILTERABLETABLEVIEW_H

#include <QTableView>

class FilterTableHeader;

/**
 * A table view that supports filtering
 */
class FilterableTableView : public QTableView
{
    Q_OBJECT

public:
    FilterableTableView(QWidget* parent = Q_NULLPTR);
    FilterTableHeader* getTableHeader() const;
    void setShowFilter(bool show);

protected:
    void dragEnterEvent (QDragEnterEvent*   event);
    void dragMoveEvent  (QDragMoveEvent*    event);
    void dropEvent      (QDropEvent*        event);

signals:
    void attachmentDropped(const QString& filePath);

private:
    FilterTableHeader*  _tableHeader;
};

#endif // FILTERABLETABLEVIEW_H
