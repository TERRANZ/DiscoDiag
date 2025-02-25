cmake_minimum_required(VERSION 3.5)

project(DiscoDiag LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS
        Widgets
        LinguistTools
        Bluetooth
        SerialPort
        Network
        Core
        REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
        Widgets
        LinguistTools
        Bluetooth
        SerialPort
        Network
        Core
        REQUIRED)

set(TS_FILES DiscoDiag_ru_RU.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/ui/mainwindow.cpp
        src/ui/mainwindow.h
        src/ui/mainwindow.ui
        src/bt/btbackend.cpp
        src/bt/btbackend.h
        src/command/abstractcommand.cpp
        src/command/abstractcommand.h
        src/command/commandIds.h
        src/obd/obdservice.cpp
        src/obd/obdservice.h
        src/obd/obdresult.cpp
        src/obd/obdresult.h
        src/ui/remoteselector.cpp
        src/ui/remoteselector.h
        src/ui/remoteselector.ui
        src/command/impl/obdresetfixcommandimpl.cpp
        src/command/impl/obdresetfixcommandimpl.h
        ${TS_FILES}
        src/command/impl/selectprotocolobdcommandimpl.cpp
        src/command/impl/selectprotocolobdcommandimpl.h
        src/command/ObdParser.cpp
        src/command/ObdParser.h
        src/command/impl/displayheadercommandimpl.cpp
        src/command/impl/displayheadercommandimpl.h
        src/command/impl/coolanttempcommandimpl.cpp
        src/command/impl/coolanttempcommandimpl.h
        src/command/impl/extparamscommandimpl.cpp
        src/command/impl/extparamscommandimpl.h
)
if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DiscoDiag
            ${PROJECT_SOURCES}
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else ()
    if (ANDROID)
        add_library(DiscoDiag SHARED
                ${PROJECT_SOURCES}
        )
    else ()
        add_executable(DiscoDiag
                ${PROJECT_SOURCES}
        )
    endif ()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif ()

target_link_libraries(DiscoDiag PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Bluetooth)
