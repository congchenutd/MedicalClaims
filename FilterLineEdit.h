#ifndef FILTERCOMBOBOX_H
#define FILTERCOMBOBOX_H

#include <QLineEdit>

class FilterLineEdit : public QLineEdit
{
public:
    FilterLineEdit(int column, QWidget* parent = Q_NULLPTR);

private:
    int _column;
};

#endif // FILTERCOMBOBOX_H
