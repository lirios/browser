macx {
    OBJECTIVE_SOURCES += $$PWD/mac/MacOsEventListener.mm
    HEADERS += $$PWD/mac/MacOsEventListener.h
    LIBS += -framework AppKit -framework Foundation

}

SOURCES += $$PWD/main.cpp \
