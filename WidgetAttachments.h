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

public slots:
    bool onDropAttachment(const QString& filePath);

protected:
    void dragEnterEvent(QDragEnterEvent*    event);
    void dropEvent     (QDropEvent*         event);

private slots:
    void onAdd();
    void onDel();
    void onOpen(const QModelIndex& idx);
    void onSelectionChanged(const QModelIndex& idx);

private:
    QString getAttachmentDir() const;
    void update();
    void addFile(const QString& title, const QString& sourceFilePath);
    bool fileExists(const QString& dirPath, const QString& targetFilePath) const;
    QString findDuplicatedFile(const QString& dirPath, const QString& sourceFilePath) const;

private:
    Ui::WidgetAttachments ui;
    int                 _claimID;
    QFileSystemModel    _model;
    const QString       _attachmentDir;
    const QString       _emptyDir;
    enum {COL_TITLE, COL_FILE_PATH};
};

#endif // WIDGETATTACHMENTS_H
