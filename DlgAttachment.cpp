#include "DlgAttachment.h"
#include <QFileDialog>

DlgAttachment::DlgAttachment(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.btFilePath,      &QPushButton::clicked,  this, &DlgAttachment::onFilePath);
    connect(ui.radioEOB,        &QRadioButton::clicked, this, &DlgAttachment::onEOB);
    connect(ui.radioInvoice,    &QRadioButton::clicked, this, &DlgAttachment::onInvoice);
    connect(ui.radioClaim,      &QRadioButton::clicked, this, &DlgAttachment::onClaim);
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

void DlgAttachment::setFilePath(const QString& filePath)
{
    ui.leFilePath->setText(filePath);

    auto fileName = QFileInfo(filePath).fileName();
    if (fileName.contains("EOB", Qt::CaseInsensitive))
        onEOB();
    else if (fileName.contains("Invoice", Qt::CaseInsensitive) || fileName.contains("Bill", Qt::CaseInsensitive))
        onInvoice();
    else if (fileName.contains("Claim", Qt::CaseInsensitive))
        onClaim();
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
        setFilePath(filePath);
}

void DlgAttachment::onEOB() {
    setTitle("EOB");
}

void DlgAttachment::onInvoice() {
    setTitle("Invoice");
}

void DlgAttachment::onClaim() {
    setTitle("Claim");
}
