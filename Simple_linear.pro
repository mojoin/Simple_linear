QT       += core gui charts printsupport
#QT += axcontainer  # 添加这行可以调用window上的应用(Excel)

# 包含QXlsx库
include($$PWD/QXlsx/QXlsx.pri)

# 相对路径（Eigen 放在项目根目录）
INCLUDEPATH += $$PWD/eigen-5.0.0

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = ChartExporter
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    graphicsview.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    graphicsview.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rec.qrc

#设置exe图标
win32:{
    RC_ICONS=icon/linear.ico
}
