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

private:
    DAO();
    bool openDB(const QString& name);
    void createTables();

private:
    static DAO* _instance;
};

#endif // DAO_H
