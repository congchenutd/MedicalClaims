#ifndef DLGOPTIONS_H
#define DLGOPTIONS_H

#include "ui_DlgOptions.h"

class Settings;

class DlgOptions : public QDialog
{
    Q_OBJECT

public:
    explicit DlgOptions(QWidget* parent = 0);
    void accept() override;

    QFont getTableFont()    const;
    QFont getUIFont()       const;

private slots:
    void onTableFont();
    void onUIFont();

private:
    Ui::DlgOptions ui;
    Settings* _settings;
};

#endif // DLGOPTIONS_H
