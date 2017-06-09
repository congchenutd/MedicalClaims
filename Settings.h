#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QSettings
{
public:
    static Settings* getInstance();

    QFont getTableFont() const;
    void setTableFont(const QFont& font);

    QFont getUIFont() const;
    void setUIFont(const QFont& font);

    int getBackupDays() const;
    void setBackupDays(int nDays);

private:
    void loadDefaults();
    Settings();

    static Settings* _instance;
};

#endif // SETTINGS_H
