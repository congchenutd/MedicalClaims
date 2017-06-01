#include "WidgetAttachments.h"
#include "DAO.h"
#include "DlgAttachment.h"
#include <QList>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>

WidgetAttachments::WidgetAttachments(QWidget *parent) :
    QWidget(parent), _attachmentDir("Attachments"), _emptyDir("Attachments/Empty")
{
    ui.setupUi(this);

    QDir::current().mkdir(_attachmentDir);
    QDir::current().mkdir(_emptyDir);

    _model.setRootPath(_attachmentDir);
    _model.setResolveSymlinks(false);
    ui.listView->setModel(&_model);
    ui.listView->setRootIndex(_model.index(_emptyDir));

    ui.btDel->setEnabled(false);

    connect(ui.btAdd,       &QPushButton::clicked,      this, &WidgetAttachments::onAdd);
    connect(ui.btDel,       &QPushButton::clicked,      this, &WidgetAttachments::onDel);
    connect(ui.listView,    &QListView::doubleClicked,  this, &WidgetAttachments::onOpen);
    connect(ui.listView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &WidgetAttachments::onSelectionChanged);
}

void WidgetAttachments::setClaimID(int claimID)
{
    _claimID = claimID;
    update();
}

void WidgetAttachments::onAdd()
{
    DlgAttachment dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        QString dir = getAttachmentDir();
        QString targetFilePath = dir + QDir::separator() + dlg.getTitle() + "." + QFileInfo(dlg.getFilePath()).suffix();
        QFile::copy(dlg.getFilePath(), targetFilePath);
//        QFile::remove(dlg.getFilePath());
    }
}

void WidgetAttachments::onDel()
{
    if(QMessageBox::warning(this, "Warning", "Are you sure to delete?",
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        QModelIndex idx = ui.listView->selectionModel()->currentIndex();
        QString filePath = getAttachmentDir() + QDir::separator() + _model.data(idx).toString();
        QFile::remove(filePath);
        update();
    }
}

void WidgetAttachments::onOpen(const QModelIndex& idx)
{
    QString filePath = getAttachmentDir() + QDir::separator() + _model.data(idx).toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void WidgetAttachments::onSelectionChanged(const QModelIndex& idx) {
    ui.btDel->setEnabled(idx.isValid());
}

QString WidgetAttachments::getAttachmentDir() const {
    return _claimID > 0 ? _attachmentDir + QDir::separator() + tr("%1").arg(_claimID)
                        : QString();
}

void WidgetAttachments::update()
{
    if (_claimID == -1)
    {
        ui.listView->setRootIndex(_model.index(_emptyDir));
        return;
    }

    QString dir = getAttachmentDir();
    QDir::current().mkdir(dir);
    ui.listView->setRootIndex(_model.index(dir));
}
