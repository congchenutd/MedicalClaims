#include "DAO.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

DAO* DAO::getInstance()
{
    if (_instance == 0)
        _instance = new DAO();
    return _instance;
}

DAO* DAO::_instance = 0;

DAO::DAO()
{
    openDB("MedicalClaims.db");
    createTables();
}

bool DAO::openDB(const QString& name)
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(name);
    if(!database.open())
    {
        QMessageBox::critical(0, "Error", "Can not open database " + name);
        return false;
    }
    return true;
}

void DAO::createTables()
{
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON");
    query.exec("create table Patient ( \
               ID          int primary key, \
               Name        varchar unique   \
               )");
    query.exec("create table Provider ( \
               ID          int primary key, \
               Name        varchar unique   \
               )");
    query.exec("create table Claim ( \
               ID                   int primary key, \
               Patient              int references Patient (ID) on delete cascade on update cascade,  \
               Provider             int references Provider(ID) on delete cascade on update cascade,  \
               Service              varchar,    \
               \'Service Start\'    date,       \
               \'Service End\'      date,       \
               Billed               double,     \
               Covered              double,     \
               \'Insurance Paid\'   double,     \
               \'Not Covered\'      double,     \
               Deductible           double,     \
               Coinsurance          double,     \
               \'I Paid\'           double,     \
               \'FSA Claimed\'      double,     \
               \'HSA Claimed\'      double      \
               )");
    query.exec("create table Attachment (   \
               ID               int primary key,    \
               Title            varchar,            \
               \'File Path\'    varchar             \
               )");
    query.exec("create table ClaimAttachmentRelation ( \
               ClaimID        int references Claim        (ID) on delete cascade on update cascade, \
               AttachmentID   int references Attachment   (ID) on delete cascade on update cascade, \
               primary key (ClaimID, AttachmentID) \
               )");
}

int DAO::getNextID(const QString& tableName)
{
    if(tableName.isEmpty())
        return 0;

    QSqlQuery query;
    query.exec("select max(ID) from " + tableName);
    return query.next() ? query.value(0).toInt() + 1 : 0;
}

QList<int> DAO::getIDs(const QString& tableName) const
{
    QList<int> result;
    QSqlQuery query;
    query.exec(tr("select ID from %1 order by ID").arg(tableName));
    while (query.next())
        result << query.value(0).toInt();
    return result;
}

void DAO::addAttachment(int claimID, const QString& title, const QString& filePath)
{
    int attachmentID = getNextID("Attachment");
    QSqlQuery query;
    query.prepare("insert into Attachment values (:id, :title, :filePath)");
    query.bindValue(":id",          attachmentID);
    query.bindValue(":title",       title);
    query.bindValue(":filePath",    filePath);
    query.exec();

    query.exec(tr("insert into ClaimAttachmentRelation values (%1, %2)")
               .arg(claimID).arg(attachmentID));
}

QList<QPair<QString, QString>> DAO::getAttachments(int claimID) const
{
    QList<QPair<QString, QString>> result;
    QSqlQuery query;
    query.exec(tr("select Title, \"File Path\" from Attachment, ClaimAttachmentRelation \
                  where Claim.ID = ClaimAttachmentRelation.ClaimID and \
                  Claim.ID = %1").arg(claimID));
    while (query.next())
        result << QPair<QString, QString>(query.value(0).toString(), query.value(1).toString());
    return result;
}
