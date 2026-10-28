cmake_minimum_required(VERSION 3.16)

project(DiscoDiag LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS
        Widgets
        LinguistTools
        Bluetooth
        Core
)

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
        src/obd/ObdParser.h
        src/obd/ObdResult.h
        src/ui/RemoteSelector.cpp
        src/ui/RemoteSelector.h
        src/ui/remoteselector.ui
        src/command/impl/common/ObdResetFixCommandImpl.h
        src/command/impl/common/SelectProtocolObdCommandImpl.h
        src/command/impl/common/DisplayHeaderCommandImpl.h
        src/command/impl/common/ExtParamsCommandImpl.h
        src/command/impl/lr322/temperature/CoolantTempCommandImpl.cpp
        src/command/impl/lr322/temperature/CoolantTempCommandImpl.h
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
        ${TS_FILES}
)

qt_add_executable(DiscoDiag ${PROJECT_SOURCES})

target_include_directories(DiscoDiag PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(DiscoDiag PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(DiscoDiag PRIVATE
        Qt6::Widgets
        Qt6::Bluetooth
)

# Embeds the compiled catalogs under :/i18n and adds update_translations /
# release_translations targets.
qt_add_translations(DiscoDiag
        TS_FILES ${TS_FILES}
        RESOURCE_PREFIX /i18n
)

enable_testing()
add_subdirectory(tests)
