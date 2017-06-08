#include "WidgetAttachments.h"
#include "DAO.h"
#include "DlgAttachment.h"
#include <QList>
#include <QDir>
#include <QDesktopServices>
#include <QMessageBox>
#include <QMimeData>
#include <QDirIterator>
#include <QDebug>

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

void WidgetAttachments::setModelID(int claimID)
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

/**
 * @brief Add a given file to the attachments, using a given title.
 * E.g., if the source file path is xxxx/asdf.pdf and title is EOB, the attachment file will be named EOB.pdf
 * @param title             - target file name, e.g., EOB
 * @param sourceFilePath    - source file path, e.g., "asdf.pdf"
 */
void WidgetAttachments::addFile(const QString& title, const QString& sourceFilePath)
{
    QString dir = getAttachmentDir();   // attachments dir
    QDir::current().mkdir(dir);         // ensure the dir exists
    QString targetFilePath = dir + QDir::separator() + title + "." + QFileInfo(sourceFilePath).suffix();  // title.suffix

    // check duplicated title
    if (fileExists(dir, targetFilePath))
    {
        if (QMessageBox::warning(this, tr("File exits"),
                                 tr("Another attachment with the same name already exists, replace it?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            QFile::remove(targetFilePath);
        else
            return;
    }

    // check duplicated content
    QString sameContentFile = findDuplicatedFile(dir, sourceFilePath);
    if (!sameContentFile.isEmpty())
    {
        if (QMessageBox::warning(this, tr("Content exits"),
                                 tr("An existing attachment %1 has the same content, replace it?").arg(sameContentFile),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            QFile::remove(sameContentFile);
        else
            return;
    }

    QFile::copy(sourceFilePath, targetFilePath);
    update();
}

/**
 * @brief Check if there is a same name file in the given dir
 * @param dir               - path of the dir
 * @param targetFilePath    - file path to be checked against
 * @return                  - true if exists
 */
bool WidgetAttachments::fileExists(const QString& dirPath, const QString& targetFilePath) const
{
    QDirIterator it(dirPath, QDir::Files | QDir::NoDotAndDotDot);
    while (it.hasNext())
    {
        QString filePath = it.next();                           // an existing file
        if (QFileInfo(filePath) == QFileInfo(targetFilePath))   // it's path is the same as the given file path
            return true;
    }
    return false;
}

/**
 * @brief Check if there is a file in the given dir that has the same content as the given file
 * @param dirPath           - path of the dir
 * @param sourceFilePath    - file path to be checked against
 * @return                  - the path of the duplicated file
 */
QString WidgetAttachments::findDuplicatedFile(const QString& dirPath, const QString& sourceFilePath) const
{
    // Load the content of the source file
    QFile sourceFile(sourceFilePath);
    if (!sourceFile.open(QFile::ReadOnly))
        return QString();

    QByteArray sourceContent = sourceFile.readAll();

    // Scan all files in the dir
    QDirIterator it(dirPath, QDir::Files | QDir::NoDotAndDotDot);
    while (it.hasNext())
    {
        QString filePath = it.next();
        QFile file(filePath);
        if (file.open(QFile::ReadOnly))
        {
            if (file.readAll() == sourceContent)    // compare content
                return filePath;
        }
    }
    return QString();
}

void WidgetAttachments::onDel()
{
    if(QMessageBox::warning(this, "Warning", "Are you sure to delete?",
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        auto dirPath = getAttachmentDir();
        QModelIndex idx = ui.listView->selectionModel()->currentIndex();
        QString filePath = getAttachmentDir() + QDir::separator() + _model.data(idx).toString();
        QFile::remove(filePath);

        // remove empty dir
        if (QDir(dirPath).entryInfoList(QDir::Files | QDir::NoDotAndDotDot).isEmpty())
            QDir::current().rmdir(dirPath);
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
