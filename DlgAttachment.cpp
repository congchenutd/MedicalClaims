#include "DlgAttachment.h"
#include <QFileDialog>

DlgAttachment::DlgAttachment(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.btFilePath, &QPushButton::clicked, this, &DlgAttachment::onFilePath);
}

QString DlgAttachment::getTitle() const {
    return ui.leTitle->text();
}

QString DlgAttachment::getFilePath() const {
    return ui.leFilePath->text();
}

void DlgAttachment::setTitle(const QString& title) {
    ui.leTitle->setText(title);
}

void DlgAttachment::setFilePath(const QString& filePath) {
    ui.leFilePath->setText(filePath);
}

void DlgAttachment::accept() {
    if (!getTitle().isEmpty() && !getFilePath().isEmpty())
        return QDialog::accept();
}

void DlgAttachment::onFilePath()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                      ".",
                                                      tr("All files (*.*)"));
    if (!filePath.isEmpty())
        ui.leFilePath->setText(filePath);
}
