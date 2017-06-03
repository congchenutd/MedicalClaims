#include "WidgetAttachments.h"
#include "DAO.h"
#include "DlgAttachment.h"
#include <QList>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>
#include <QMimeData>

WidgetAttachments::WidgetAttachments(QWidget *parent) :
    QWidget(parent), _attachmentDir("Attachments"), _emptyDir("Attachments/Empty")
{
    ui.setupUi(this);
    setAcceptDrops(true);

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
    setEnabled(_claimID > -1);
}

void WidgetAttachments::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasUrls() && _claimID > 0)
        event->acceptProposedAction();
}

void WidgetAttachments::dropEvent(QDropEvent* event)
{
    QUrl url = event->mimeData()->urls().front();
    if (onDropAttachment(url.toLocalFile()))
        event->accept();
}

void WidgetAttachments::onAdd()
{
    DlgAttachment dlg(this);
    if (dlg.exec() == QDialog::Accepted)
        addFile(dlg.getTitle(), dlg.getFilePath());
}

void WidgetAttachments::addFile(const QString& title, const QString& filePath)
{
    QString dir = getAttachmentDir();
    QDir::current().mkdir(dir);
    QString targetFilePath = dir + QDir::separator() + title + "." + QFileInfo(filePath).suffix();
    QFile::copy(filePath, targetFilePath);
//        QFile::remove(dlg.getFilePath());
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

bool WidgetAttachments::onDropAttachment(const QString& filePath)
{
    DlgAttachment dlg(this);
    dlg.setFilePath(filePath);
    if (dlg.exec() == QDialog::Accepted)
    {
        addFile(dlg.getTitle(), dlg.getFilePath());
        return true;
    }
    return false;
}

QString WidgetAttachments::getAttachmentDir() const {
    return _claimID > 0 ? _attachmentDir + QDir::separator() + tr("%1").arg(_claimID)
                        : QString();
}

void WidgetAttachments::update()
{
    QString dir = getAttachmentDir();
    if (_claimID == -1 || !QDir(dir).exists())
    {
        ui.listView->setRootIndex(_model.index(_emptyDir));
        return;
    }

    ui.listView->setRootIndex(_model.index(dir));
}
