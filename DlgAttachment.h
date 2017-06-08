#ifndef DLGATTACHMENT_H
#define DLGATTACHMENT_H

#include "ui_DlgAttachment.h"

class DlgAttachment : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAttachment(QWidget* parent = 0);

    QString getTitle()      const;
    QString getFilePath()   const;

    void setTitle   (const QString& title);
    void setFilePath(const QString& filePath);

    void accept();

private slots:
    void onFilePath();
    void onEOB();
    void onInvoice();
    void onClaim();

private:
    Ui::DlgAttachment ui;
};

#endif // DLGATTACHMENT_H
