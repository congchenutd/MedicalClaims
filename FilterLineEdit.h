#ifndef FILTERCOMBOBOX_H
#define FILTERCOMBOBOX_H

#include <QLineEdit>

/**
 * A line edit for filting data in a column of an item model
 */
class FilterLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    FilterLineEdit(int column, QWidget* parent = Q_NULLPTR);

protected:
    void keyReleaseEvent(QKeyEvent* event);

signals:
    void textEdited(int column, const QString& text);

private:
    int _column;
};

#endif // FILTERCOMBOBOX_H
