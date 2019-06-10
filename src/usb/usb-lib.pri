INCLUDEPATH += $$PWD

PUBLIC_HEADERS += \
    $$PWD/qusbglobal.h \
    $$PWD/qusbdevice.h \
    $$PWD/qusbinfo.h \
    $$PWD/qusbtransfer.h

PRIVATE_HEADERS += \
    $$PWD/qusbdevice_p.h \
    $$PWD/qusbinfo_p.h \
    $$PWD/qusbtransfer_p.h

SOURCES += \
    $$PWD/qusbinfo.cpp \
    $$PWD/qusbdevice.cpp \
    $$PWD/qusbtransfer.cpp

win32 {
    LIBS_PRIVATE += -L$$PWD/../ -L$$PWD/../../ Advapi32.lib
    CONFIG(debug, debug|release) {
        LIBS_PRIVATE += libusb-1.0d.lib
    }
    CONFIG(release, debug|release) {
        LIBS_PRIVATE += libusb-1.0.lib
    }
    INCLUDEPATH += $$PWD/libusb $$PWD/../libusb $$PWD/../../libusb
}

unix {
#    CONFIG += link_pkgconfig
#    PKGCONFIG += libusb-1.0
    LIBS += -L$$/home/xuan/libusb-1.0.22/android/libs/armeabi-v7a/ -lusb1.0
    INCLUDEPATH += $$/home/xuan/libusb-1.0.22/libusb/
#DEPENDPATH += $$PWD/../../libusb-1.0.22/android/libs/armeabi-v7a
}


#unix {
#    !packagesExist(libusb-1.0):error("Could not find libusb-1.0 using PKGCONFIG")
#    CONFIG += link_pkgconfig
#    PKGCONFIG += libusb-1.0
#}

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS
