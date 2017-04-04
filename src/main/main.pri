macx {
    OBJECTIVE_SOURCES += $$PWD/MacOsEventListener.mm
    HEADERS += $$PWD/MacOsEventListener.h
    LIBS += -framework AppKit -framework Foundation

}

SOURCES += $$PWD/main.cpp \
