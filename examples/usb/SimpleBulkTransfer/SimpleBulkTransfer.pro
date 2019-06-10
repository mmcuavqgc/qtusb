#-------------------------------------------------
#
# Project created by QtCreator 2015-07-28T13:55:22
#
#-------------------------------------------------

QT += core usb
QT -= gui

TARGET = SimpleBulkTransfer
TEMPLATE = app

SOURCES += main.cpp \
    usbexample.cpp

HEADERS += \
    usbexample.h

target.path = $$[QT_INSTALL_EXAMPLES]/usb/SimpleBulkTransfer
INSTALLS += target

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/../../../../../libusb-1.0.22/android/libs/armeabi-v7a/libusb1.0.so
}
