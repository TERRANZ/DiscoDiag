cmake_minimum_required(VERSION 3.10)

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
        src/ui/MainWindow.cpp
        src/ui/MainWindow.h
        src/ui/mainwindow.ui
        src/bt/BtBackend.cpp
        src/bt/BtBackend.h
        src/command/AbstractCommand.h
        src/command/commandIds.h
        src/obd/ObdService.cpp
        src/obd/ObdService.h
        src/obd/ObdResult.h
        src/ui/RemoteSelector.cpp
        src/ui/RemoteSelector.h
        src/ui/remoteselector.ui
        src/command/impl/common/ObdResetFixCommandImpl.h
        ${TS_FILES}
        src/command/impl/common/SelectProtocolObdCommandImpl.h
        src/obd/ObdParser.h
        src/command/impl/common/DisplayHeaderCommandImpl.h
        src/command/impl/lr322/temperature/CoolantTempCommandImpl.cpp
        src/command/impl/lr322/temperature/CoolantTempCommandImpl.h
        src/command/impl/common/ExtParamsCommandImpl.h
        src/command/impl/lr322/temperature/OilTempCommandImpl.cpp
        src/command/impl/lr322/temperature/OilTempCommandImpl.h
        src/command/impl/lr322/temperature/GbTempCommandImpl.cpp
        src/command/impl/lr322/temperature/GbTempCommandImpl.h
        src/command/impl/lr322/temperature/TCTempCommandImpl.cpp
        src/command/impl/lr322/temperature/TCTempCommandImpl.h
        src/command/impl/lr322/temperature/RDTempCommandImpl.cpp
        src/command/impl/lr322/temperature/RDTempCommandImpl.h
        src/command/impl/lr322/temperature/ComprTempCommandImpl.cpp
        src/command/impl/lr322/temperature/ComprTempCommandImpl.h
        src/command/impl/lr322/temperature/IntakeAirTempCommandImpl.cpp
        src/command/impl/lr322/temperature/IntakeAirTempCommandImpl.h
        src/command/impl/lr322/temperature/AmbientAirTempCommandImpl.cpp
        src/command/impl/lr322/temperature/AmbientAirTempCommandImpl.h
        src/command/impl/lr322/FuelPressCommandImpl.cpp
        src/command/impl/lr322/FuelPressCommandImpl.h
        src/command/impl/lr322/BrakePressCommandImpl.cpp
        src/command/impl/lr322/BrakePressCommandImpl.h
        src/command/impl/lr322/OilLevelCommandImpl.cpp
        src/command/impl/lr322/OilLevelCommandImpl.h
        src/command/impl/lr322/OilVolumeCommandImpl.cpp
        src/command/impl/lr322/OilVolumeCommandImpl.h
        src/command/impl/lr322/FuelLevelCommandImpl.cpp
        src/command/impl/lr322/FuelLevelCommandImpl.h
        src/obd/ObdThread.cpp
        src/obd/ObdThread.h
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
