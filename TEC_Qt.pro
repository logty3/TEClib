TEMPLATE = app
CONFIG += console c++11 c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += C:\local\boost_1_74_0\boost_mingw_530\include\boost-1_74
LIBS += -LC:\local\boost_1_74_0\boost_mingw_530\lib\
        -llibboost_date_time-mgw7-mt-x64-1_74

SOURCES += \
    GFileParser.cpp \
    IFileParser.cpp \
    NFileParser.cpp \
    OFileParser.cpp \
    dateTime.cpp \
        main.cpp \
    models.cpp \
    utils.cpp


DISTFILES += \
    .gitignore \
    TEC_Qt.pro.user

HEADERS += \
    GFileParser.h \
    IFileParser.h \
    NFileParser.h \
    OFileParser.h \
    Tec.h \
    dateTime.h \
    models.h \
    utils.h
