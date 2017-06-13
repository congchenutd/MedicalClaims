#include "FilterTableHeader.h"
#include "FilterLineEdit.h"

#include <QTableView>
#include <QScrollBar>

FilterTableHeader::FilterTableHeader(QTableView* parent) :
    QHeaderView(Qt::Horizontal, parent)
{
    // Activate the click signals to allow sorting
    setSectionsClickable(true);
    setSortIndicatorShown(true);

    // Keep filter positions up to date
    connect(this, &FilterTableHeader::sectionResized, this, &FilterTableHeader::adjustPositions);
    connect(parent->horizontalScrollBar(), &QScrollBar::valueChanged, this, &FilterTableHeader::adjustPositions);
}

void FilterTableHeader::generateFilters()
{
    for(int i = 0;i < count(); ++i)
    {
        FilterLineEdit* filter = new FilterLineEdit(i, this);
        connect(filter, &FilterLineEdit::textEdited, this, &FilterTableHeader::onFilterChanged);
        _filterLineEdits << filter;
    }
}

QSize FilterTableHeader::sizeHint() const
{
    QSize s = QHeaderView::sizeHint();
    if(!_filterLineEdits.isEmpty() && _filterLineEdits.front()->isVisible())
        s.setHeight(s.height() + _filterLineEdits.front()->sizeHint().height() + 4); // The 4 adds some extra space
    return s;
}

void FilterTableHeader::setShowFilters(bool show)
{
    foreach (auto filter, _filterLineEdits)
        filter->setVisible(show);
    updateGeometries();
}

void FilterTableHeader::updateGeometries()
{
    // Extra space for the shown filters
    if(_filterLineEdits.size() && _filterLineEdits.front()->isVisible())
        setViewportMargins(0, 0, 0, _filterLineEdits.front()->sizeHint().height());
    else
        setViewportMargins(0, 0, 0, 0);

    QHeaderView::updateGeometries();
    adjustPositions();
}

void FilterTableHeader::adjustPositions()
{
    if (_filterLineEdits.isEmpty() || !_filterLineEdits.front()->isVisible())
        return;

    // Loop through all widgets
    for(int i = 0; i < _filterLineEdits.size(); ++i)
    {
        // Get the current widget, move it and resize it
        QWidget* w = _filterLineEdits.at(i);
        w->move(sectionPosition(i) - offset(), w->sizeHint().height() + 3);
        w->resize(sectionSize(i), w->sizeHint().height());
    }
}

void FilterTableHeader::onFilterChanged()
{
    QList<QPair<int, QString>> filters;
    for(int i = 0; i < _filterLineEdits.size(); ++i)
    {
        auto lineEdit = _filterLineEdits.at(i);
        if (!lineEdit->text().isEmpty())
            filters << QPair<int, QString>(i, lineEdit->text());
    }

    emit filtersChanged(filters);
}
