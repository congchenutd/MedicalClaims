#ifndef WIDGETATTACHMENTS_H
#define WIDGETATTACHMENTS_H

#include "ui_WidgetAttachments.h"

#include <QFileSystemModel>

class WidgetAttachments : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetAttachments(QWidget* parent = 0);

    void setClaimID(int claimID);

private slots:
    void onAdd();
    void onDel();
    void onOpen(const QModelIndex& idx);
    void onSelectionChanged(const QModelIndex& idx);

private:
    QString getAttachmentDir() const;
    void update();

private:
    Ui::WidgetAttachments ui;
    int                 _claimID;
    QFileSystemModel    _model;
    const QString       _attachmentDir;
    const QString       _emptyDir;
    enum {COL_TITLE, COL_FILE_PATH};
};

#endif // WIDGETATTACHMENTS_H
