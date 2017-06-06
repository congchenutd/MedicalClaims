#include "FilterableTableView.h"
#include "FilterTableHeader.h"

#include <QDragEnterEvent>
#include <QMimeData>
#include <QLayout>

FilterableTableView::FilterableTableView(QWidget* parent)
    : QTableView (parent)
{
    // Set up filter row
    _tableHeader = new FilterTableHeader(this);
    setHorizontalHeader(_tableHeader);

    setHorizontalScrollMode (QTableView::ScrollPerItem);
    setVerticalScrollMode   (QTableView::ScrollPerItem);
}

FilterTableHeader* FilterableTableView::getTableHeader() const {
    return _tableHeader;
}

void FilterableTableView::setShowFilter(bool show) {
    _tableHeader->setShowFilters(show);
}

void FilterableTableView::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void FilterableTableView::dragMoveEvent(QDragMoveEvent* event)
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

void FilterableTableView::dropEvent(QDropEvent* event)
{
    QUrl url = event->mimeData()->urls().front();
    emit attachmentDropped(url.toLocalFile());
    event->accept();
}
