set(PROGRAMMER_SRC
    Models/SectorTableModel.cpp
    Models/SerialPortListModel.cpp

    Operations/Operation.cpp
    Operations/Program.cpp

    Widgets/BaudRateSelector.cpp
    Widgets/ConnectionWidget.cpp
    Widgets/LinkButton.cpp
    Widgets/SectorPresetsWidget.cpp
    Widgets/SectorTableWidget.cpp
    Widgets/SerialPortSelector.cpp

    Windows/MainWindow.cpp
    Windows/MainWindowUI.cpp
    Windows/OperationDialog.cpp

    General.cpp
    main.cpp)

set(PROGRAMMER_MOC_TARGETS
    Models/SectorTableModel.h
    Models/SerialPortListModel.h

    Operations/Operation.h
    Operations/Program.h

    Widgets/BaudRateSelector.h
    Widgets/ConnectionWidget.h
    Widgets/LinkButton.h
    Widgets/SectorPresetsWidget.h
    Widgets/SectorTableWidget.h
    Widgets/SerialPortSelector.h

    Windows/MainWindow.h
    Windows/OperationDialog.h
)

add_executable(programmer ${PROGRAMMER_SRC} ${PROGRAMMER_MOC_TARGETS})

target_include_directories(programmer
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(programmer sitl::sitl Qt5::Core Qt5::Widgets Qt5::SerialPort)

target_compile_options(programmer PRIVATE
    -O2
    -pedantic
    -W
    -Wall
    -Wconversion
    -Wctor-dtor-privacy
    -Weffc++
    -Wenum-compare
    -Werror
    -Wextra
    -Wfloat-equal
    -Wlogical-op
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wshadow
    -Wsign-conversion
    -Wstrict-overflow=2
    -Wunreachable-code
)

target_compile_features(programmer PUBLIC
    cxx_std_17
    cxx_user_literals
)

set_target_properties(programmer
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BUILD_OUTPUT_DIR}
)

# deployment

set(WINDEPLOYQT_EXE $ENV{Qt5_DIR}/../../../bin/windeployqt.exe)

if(CMAKE_BUILD_TYPE MATCHES Release)
    set(DEPLOYMENT_FLAGS --no-quick-import --no-translations --release)
elseif()
    set(DEPLOYMENT_FLAGS --no-quick-import --no-translations --debug)
endif()

add_custom_command(TARGET programmer POST_BUILD
    COMMAND ${WINDEPLOYQT_EXE} ${DEPLOYMENT_FLAGS} ${BUILD_OUTPUT_DIR}/programmer.exe
)
