#include "FilterLineEdit.h"

#include <QKeyEvent>

FilterLineEdit::FilterLineEdit(int column, QWidget* parent)
    : QLineEdit (parent),
      _column (column)
{
}

void FilterLineEdit::keyReleaseEvent(QKeyEvent* event)
{
    event->accept();
    emit textEdited(_column, text());
}
