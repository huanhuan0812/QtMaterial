# examples/CMakeLists.txt

qt_add_executable(QtMaterialExample main.cpp)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

target_link_libraries(QtMaterialExample PRIVATE QtMaterial Qt6::Widgets)

# 启用 MOC
set_target_properties(QtMaterialExample PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

if(APPLE)
    set_target_properties(QtMaterialExample PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()

if(WIN32)
    set_target_properties(QtMaterialExample PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()