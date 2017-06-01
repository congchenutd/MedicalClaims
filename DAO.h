#ifndef DAO_H
#define DAO_H

#include <QObject>
#include <QString>

class DAO: public QObject
{
public:
    static DAO* getInstance();
    static int getNextID(const QString& tableName);
    QList<int> getIDs(const QString &tableName) const;
    void addAttachment(int claimID, const QString& title, const QString& filePath);
    QList<QPair<QString, QString>> getAttachments(int claimID) const;

private:
    DAO();
    bool openDB(const QString& name);
    void createTables();

private:
    static DAO* _instance;
};

#endif // DAO_H
