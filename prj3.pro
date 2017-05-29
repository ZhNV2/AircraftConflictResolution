#-------------------------------------------------
#
# Project created by QtCreator 2017-05-28T19:01:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = prj3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    DomainData/Aircraft/aircraft.cpp \
    DomainData/Collection/domain_objects_collection.cpp \
    DomainData/ConflictManager/conflict_manager.cpp \
    DomainData/ConflictSolver/conflict_solver.cpp \
    Geometry/Point/point.cpp \
    Geometry/Track/track.cpp \
    Geometry/Vector/vector.cpp \
    Main/Runner/runner.cpp \
    Main/main.cpp

HEADERS  += mainwindow.h \
    DomainData/Aircraft/aircraft.h \
    DomainData/Collection/domain_objects_collection.h \
    DomainData/ConflictManager/conflict_manager.h \
    DomainData/ConflictSolver/conflict_solver.h \
    DomainData/domain_object.h \
    Geometry/Point/point.h \
    Geometry/Track/track.h \
    Geometry/Vector/vector.h \
    Main/Runner/runner.h

FORMS    += mainwindow.ui
