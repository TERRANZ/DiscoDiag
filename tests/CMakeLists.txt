find_package(Qt6 REQUIRED COMPONENTS Test Core)

set(COMMAND_SOURCES
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/temperature/CoolantTempCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/temperature/OilTempCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/temperature/GbTempCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/temperature/IntakeAirTempCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/temperature/AmbientAirTempCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/temperature/ComprTempCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/temperature/TCTempCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/temperature/RDTempCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/FuelPressCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/BrakePressCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/FuelLevelCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/OilLevelCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/OilVolumeCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/BoostCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/IntPressCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/SpeedCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/AbsFLCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/AbsFRCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/AbsRLCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/AbsRRCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/HeightFLCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/HeightFRCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/HeightRLCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/HeightRRCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/MainPressCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/UTrCaseCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/UAbsCommandImpl.cpp
        ${CMAKE_SOURCE_DIR}/src/command/impl/lr322/MafCommandImpl.cpp
)

add_executable(test_obdparser test_obdparser.cpp)
target_link_libraries(test_obdparser PRIVATE Qt6::Test Qt6::Core)
target_include_directories(test_obdparser PRIVATE ${CMAKE_SOURCE_DIR})
add_test(NAME obdparser COMMAND test_obdparser)

add_executable(test_commands test_commands.cpp ${COMMAND_SOURCES})
target_link_libraries(test_commands PRIVATE Qt6::Test Qt6::Core)
target_include_directories(test_commands PRIVATE ${CMAKE_SOURCE_DIR})
add_test(NAME commands COMMAND test_commands)

add_executable(test_abstractcommand test_abstractcommand.cpp)
target_link_libraries(test_abstractcommand PRIVATE Qt6::Test Qt6::Core)
target_include_directories(test_abstractcommand PRIVATE ${CMAKE_SOURCE_DIR})
add_test(NAME abstractcommand COMMAND test_abstractcommand)

set_tests_properties(obdparser commands abstractcommand
        PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
