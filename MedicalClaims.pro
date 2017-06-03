#-------------------------------------------------
#
# Project created by QtCreator 2017-05-30T12:16:17
#
#-------------------------------------------------

QT       += core gui sql widgets

TARGET = MedicalClaims
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES +=\
        MainWindow.cpp \
    Main.cpp \
    DAO.cpp \
    Page.cpp \
    PageClaims.cpp \
    PagePatients.cpp \
    PageProviders.cpp \
    PageDefault.cpp \
    DateDelegate.cpp \
    DlgAttachment.cpp \
    WidgetAttachments.cpp \
    ClaimsModel.cpp \
    MyResponsibilityDelegate.cpp \
    TableViewClaims.cpp

HEADERS  += MainWindow.h \
    DAO.h \
    Page.h \
    PageClaims.h \
    PagePatients.h \
    PageProviders.h \
    PageDefault.h \
    DateDelegate.h \
    DlgAttachment.h \
    WidgetAttachments.h \
    ClaimsModel.h \
    MyResponsibilityDelegate.h \
    TableViewClaims.h

FORMS    += MainWindow.ui \
    PageDefault.ui \
    DlgAttachment.ui \
    WidgetAttachments.ui

RESOURCES += \
    Resources.qrc

DISTFILES += \
    TODO.txt

win32 {
	RC_FILE = Resources.rc
}
macx {
	ICON = Images/Medical.icns
}
