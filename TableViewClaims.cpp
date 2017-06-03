#include "DlgAttachment.h"
#include "TableViewClaims.h"

#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>

TableViewClaims::TableViewClaims(QWidget* parent)
    : QTableView(parent)
{
    setAcceptDrops(true);
}

void TableViewClaims::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void TableViewClaims::dragMoveEvent(QDragMoveEvent* event)
{
    if (event->mimeData()->hasUrls())
    {
        // highlight hovered row
        int row = rowAt(event->pos().y());
        if (-1 < row && row < model()->rowCount())
        {
            selectionModel()->select(model()->index(row, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            event->accept();
            return;
        }
    }
    event->ignore();
}

void TableViewClaims::dropEvent(QDropEvent* event)
{
    QUrl url = event->mimeData()->urls().front();
    emit attachmentDropped(url.toLocalFile());
    event->accept();
}
