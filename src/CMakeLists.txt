if(APPLE OR WIN32)
    set(LiriBrowser_OUTPUT_NAME "LiriBrowser")
else()
    set(LiriBrowser_OUTPUT_NAME "liri-browser")
endif()

set(LiriBrowser_SOURCES
    core/global/paths.h
    core/models/downloadsmodel.cpp
    core/models/downloadsmodel.h
    core/models/tab.cpp
    core/models/tab.h
    core/models/tabsmodel.cpp
    core/models/tabsmodel.h
    core/session/session.cpp
    core/session/session.h
    core/session/tabstate.cpp
    core/session/tabstate.h
    core/settings/searchconfig.cpp
    core/settings/searchconfig.h
    core/settings/settings.cpp
    core/settings/settings.h
    core/settings/startconfig.cpp
    core/settings/startconfig.h
    core/settings/themeconfig.cpp
    core/settings/themeconfig.h
    core/utils/darkthemetimer.cpp
    core/utils/darkthemetimer.h
    main/browserapplication.cpp
    main/browserapplication.h
    main/main.cpp
)
if(APPLE)
    set(LiriBrowser_SOURCES
        ${LiriBrowser_SOURCES}
        main/mac/MacOsEventListener.h
        main/mac/MacOsEventListener.mm)
endif()

liri_add_executable(LiriBrowser
    OUTPUT_NAME
        "${LiriBrowser_OUTPUT_NAME}"
    SOURCES
        ${LiriBrowser_SOURCES}
    RESOURCES
        3rdparty/regex-weburl/regex-weburl.qrc
        ui/ui.qrc
    DEFINES
        #QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
        -DPROJECT_VERSION="${PROJECT_VERSION}"
    DESKTOP
        "${CMAKE_CURRENT_SOURCE_DIR}/../res/io.liri.Browser.desktop"
    LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Qml
        Qt5::Quick
        Qt5::QuickControls2
        Qt5::WebEngine
        qtsingleapplication
    GUI
)
if(APPLE)
    target_link_libraries(LiriBrowser PUBLIC "-framework AppKit -framework Foundation")
    set_target_properties(LiriBrowser PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
endif()
set_target_properties(LiriBrowser PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER "io.liri.Browser"
    MACOSX_BUNDLE_ICON_FILE "io.liri.Browser"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
)

liri_finalize_executable(LiriBrowser)
