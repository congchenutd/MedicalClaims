#ifndef TABLEVIEWCLAIMS_H
#define TABLEVIEWCLAIMS_H

#include <QTableView>

class TableViewClaims : public QTableView
{
    Q_OBJECT

public:
    TableViewClaims(QWidget* parent = Q_NULLPTR);

protected:
    void dragEnterEvent(QDragEnterEvent*    event);
    void dragMoveEvent(QDragMoveEvent*      event);
    void dropEvent     (QDropEvent*         event);

signals:
    void attachmentDropped(const QString& filePath);
};

#endif // TABLEVIEWCLAIMS_H
