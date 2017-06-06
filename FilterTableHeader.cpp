#include "FilterTableHeader.h"
#include "FilterLineEdit.h"

#include <QApplication>
#include <QTableView>
#include <QScrollBar>
#include <QLayout>

FilterTableHeader::FilterTableHeader(QTableView* parent) :
    QHeaderView(Qt::Horizontal, parent)
{
    // Activate the click signals to allow sorting
    setSectionsClickable(true);
    setSortIndicatorShown(true);

    // Do some connects: Basically just resize and reposition the input widgets whenever anything changes
    connect(this, SIGNAL(sectionResized(int,int,int)), this, SLOT(adjustPositions()));
    connect(parent->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjustPositions()));
    connect(parent->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjustPositions()));
}

void FilterTableHeader::generateFilters()
{
    for(int i = 0;i < count(); ++i)
    {
        FilterLineEdit* filter = new FilterLineEdit(i, this);
        filter->show();
//        connect(l, SIGNAL(delayedTextChanged(QString)), this, SLOT(inputChanged(QString)));
        _filterLineEdits << filter;
    }

    // Position them correctly
    adjustPositions();
}

QSize FilterTableHeader::sizeHint() const
{
    QSize s = QHeaderView::sizeHint();
    if(!_filterLineEdits.isEmpty() && _filterLineEdits.front()->isVisible())
        s.setHeight(s.height() + _filterLineEdits.at(0)->sizeHint().height() + 5); // The 5 adds some extra space
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
    // If there are any input widgets add a viewport margin to the header to generate some empty space for them which is not affected by scrolling
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
        w->move(sectionPosition(i) - offset(), w->sizeHint().height() + 2);
        w->resize(sectionSize(i), w->sizeHint().height());
    }
}

void FilterTableHeader::inputChanged(const QString& new_value)
{
    // Just get the column number and the new value and send them to anybody interested in filter changes
    emit filterChanged(sender()->property("column").toInt(), new_value);
}

void FilterTableHeader::clearFilters()
{
    foreach (auto filter, _filterLineEdits)
        filter->clear();
}

void FilterTableHeader::setFilter(int column, const QString& value)
{
//    if(column < _filterComboBoxs.size())
//        _filterComboBoxs.at(column)->setText(value);
}
