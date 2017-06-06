#include "DlgOptions.h"
#include "Settings.h"
#include <QFontDialog>
#include <QDebug>

DlgOptions::DlgOptions(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    _settings = Settings::getInstance();

    ui.btTableFont  ->setFont(_settings->getTableFont());
    ui.btUIFont     ->setFont(_settings->getUIFont());

    connect(ui.btTableFont, &QPushButton::clicked, this, &DlgOptions::onTableFont);
    connect(ui.btUIFont,    &QPushButton::clicked, this, &DlgOptions::onUIFont);
}

void DlgOptions::accept()
{
    _settings->setTableFont (getTableFont());
    _settings->setUIFont    (getUIFont());
    qApp->setFont(getUIFont());
    QDialog::accept();
}

QFont DlgOptions::getTableFont() const {
    return ui.btTableFont->font();
}

QFont DlgOptions::getUIFont() const {
    return ui.btUIFont->font();
}

void DlgOptions::onTableFont() {
    ui.btTableFont->setFont(QFontDialog::getFont(0, getTableFont()));
}

void DlgOptions::onUIFont() {
    ui.btUIFont->setFont(QFontDialog::getFont(0, getUIFont()));
}
