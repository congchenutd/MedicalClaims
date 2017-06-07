#ifndef FILTERCOMBOBOX_H
#define FILTERCOMBOBOX_H

#include <QLineEdit>

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
