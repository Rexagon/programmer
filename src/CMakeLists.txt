set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(PROGRAMMER_SRC
    ${INCLUDE_DIR}/MainWindow.h
    MainWindow.cpp
    main.cpp)

add_executable(programmer ${PROGRAMMER_SRC})

target_include_directories(programmer
    PUBLIC
        ${INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(programmer sitl::sitl Qt5::Core Qt5::Widgets)

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
