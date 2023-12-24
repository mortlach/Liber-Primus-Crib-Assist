QT       += core gui
QT += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


DEFINES += QT_SIG  # temp, should all objects inherit from QT to allow easy use of signals ??

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += source
INCLUDEPATH += source/view
INCLUDEPATH += source/ngram_phrase
INCLUDEPATH += assets

SOURCES += \
    source/definitions.cpp \
    source/main.cpp \
    source/ngram_phrase/basedata.cpp \
    source/ngram_phrase/ngramdata.cpp \
    source/ngram_phrase/ngrammetadata.cpp \
    source/ngram_phrase/ngramphrase.cpp \
    source/ngram_phrase/ngramphraseworker.cpp \
    source/ngram_phrase/phraseuniquewords.cpp \
    source/ngramdatahub.cpp \
    source/raw1grammodel.cpp \
    source/raw1grams.cpp \
    source/rawworddata.cpp \
    source/utilities.cpp \
    source/view/baseform.cpp \
    source/view/choosephrase.cpp \
    source/view/filtermodel.cpp \
    source/view/gng_tableview.cpp \
    source/view/mainwindow.cpp \
    source/view/ngramdataform.cpp \
    source/view/ngramdatamodel.cpp \
    source/view/ngrampuwrationlizeform.cpp \
    source/view/phraseuniquewordsform.cpp \
    source/view/phraseuniquewordsmodel.cpp \
    source/view/spoiler.cpp \
    source/view/wordlistgeneralcontrolsform.cpp \
    source/view/wordpickform.cpp \
    source/view/wordtab.cpp

HEADERS += \
    source/definitions.h \
    source/ngram_phrase/basedata.h \
    source/ngram_phrase/ngramdata.h \
    source/ngram_phrase/ngrammetadata.h \
    source/ngram_phrase/ngramphrase.h \
    source/ngram_phrase/ngramphraseworker.h \
    source/ngram_phrase/phraseuniquewords.h \
    source/ngram_phrase/raw1grams.h \
    source/ngram_phrase/rawworddata.h \
    source/ngramdatahub.h \
    source/raw1grammodel.h \
    source/utilities.h \
    source/view/baseform.h \
    source/view/choosephrase.h \
    source/view/filtermodel.h \
    source/view/gng_tableview.h \
    source/view/mainwindow.h \
    source/view/ngramdataform.h \
    source/view/ngramdatamodel.h \
    source/view/ngrampuwrationlizeform.h \
    source/view/phraseuniquewordsform.h \
    source/view/phraseuniquewordsmodel.h \
    source/view/spoiler.h \
    source/view/wordlistgeneralcontrolsform.h \
    source/view/wordpickform.h \
    source/view/wordtab.h

FORMS += \
    source/view/choosephrase.ui \
    source/view/mainwindow.ui \
    source/view/ngramdataform.ui \
    source/view/ngrampuwrationlizeform.ui \
    source/view/phraseuniquewordsform.ui \
    source/view/wordlistgeneralcontrolsform.ui \
    source/view/wordpickform.ui \
    source/view/wordtab.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    build-LPCribAssist-Desktop_Qt_6_5_2_MinGW_64_bit-Debug/config.ini \
    build-LPCribAssist-Desktop_Qt_6_5_2_MinGW_64_bit-Release/config.ini

win32 {
DESTDIR = $$PWD/bin
RC_ICONS = $$PWD/bin/assets/lpcribassist.ico
}
