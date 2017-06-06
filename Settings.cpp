#include "Settings.h"

#include <QFile>
#include <QFont>

Settings* Settings::_instance = 0;

Settings* Settings::getInstance()
{
    if (_instance == 0)
        _instance = new Settings;
    return _instance;
}

Settings::Settings()
    : QSettings ("Settings.ini", QSettings::IniFormat)
{
    if(QFile("Settings.ini").size() == 0)   // no setting
        loadDefaults();
}

QFont Settings::getTableFont() const {
    QFont font;
    font.fromString(value("TableFont").toString());
    return font;
}

void Settings::setTableFont(const QFont& font) {
    setValue("TableFont", font.toString());
}

QFont Settings::getUIFont() const
{
    QFont font;
    font.fromString(value("UIFont").toString());
    return font;
}

void Settings::setUIFont(const QFont& font) {
    setValue("UIFont", font.toString());
}

void Settings::loadDefaults()
{

}
