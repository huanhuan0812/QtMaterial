cmake_minimum_required(VERSION 3.16)

project(QtMaterial VERSION 0.1.0 LANGUAGES CXX)

# ==================== 构建选项 ====================
option(BUILD_EXAMPLES "Build example applications" OFF)  # 改为 OFF
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_STATIC_LIBS "Build static libraries" ON)

# ==================== C++ 标准配置 ====================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==================== 输出目录 ====================
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ==================== 查找 Qt ====================
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

# ==================== 源文件列表 ====================
# 按钮组件
set(BUTTON_SOURCES
    src/MButtons/MButtonBase.cpp
    src/MButtons/MFilledButton.cpp
    src/MButtons/MOutlinedButton.cpp
    src/MButtons/MTextButton.cpp
    src/MButtons/MElevatedButton.cpp
    src/MButtons/MToggleButton.cpp
)

set(BUTTON_HEADERS
    src/MButtons/MButtonBase.h
    src/MButtons/MFilledButton.h
    src/MButtons/MOutlinedButton.h
    src/MButtons/MTextButton.h
    src/MButtons/MElevatedButton.h
    src/MButtons/MToggleButton.h
)

# Toast 组件
set(TOAST_SOURCES
    src/MToast/QToast.cpp
)

set(TOAST_HEADERS
    src/MToast/QToast.h
)

# 合并所有源文件
set(QTMATERIAL_SOURCES
    ${BUTTON_SOURCES}
    ${TOAST_SOURCES}
)

set(QTMATERIAL_HEADERS
    ${BUTTON_HEADERS}
    ${TOAST_HEADERS}
)

# ==================== 公共配置宏 ====================
macro(add_qtmaterial_library TYPE LIB_NAME SUFFIX)
    add_library(${LIB_NAME} ${TYPE} ${QTMATERIAL_SOURCES} ${QTMATERIAL_HEADERS})
    
    # 启用 Qt 元对象编译
    set_target_properties(${LIB_NAME} PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
        AUTOMOC_COMPILER_PREDEFINES ON
    )
    
    # 设置版本信息
    set_target_properties(${LIB_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        CLEAN_DIRECT_OUTPUT 1
    )
    
    # 编译选项
    target_compile_features(${LIB_NAME} PUBLIC cxx_std_17)
    
    if(MSVC)
        target_compile_options(${LIB_NAME} PRIVATE /W4)
        # 设置运行时库
        set_target_properties(${LIB_NAME} PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        )
    else()
        target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    
    # 导出宏定义
    if(${TYPE} STREQUAL "SHARED")
        target_compile_definitions(${LIB_NAME} 
            PRIVATE QTMATERIAL_LIBRARY
            PUBLIC QTMATERIAL_SHARED_LIB
        )
    else()
        target_compile_definitions(${LIB_NAME} 
            PUBLIC QTMATERIAL_STATIC_LIB
        )
    endif()
    
    # 链接 Qt
    target_link_libraries(${LIB_NAME} 
        PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    )
    
    # Android 特定配置
    if(ANDROID)
        target_link_libraries(${LIB_NAME} PRIVATE log)
        set_target_properties(${LIB_NAME} PROPERTIES
            ANDROID_ABI ${ANDROID_ABI}
            ANDROID_PLATFORM ${ANDROID_PLATFORM}
        )
    endif()
    
    # macOS 动态库配置
    if(APPLE AND ${TYPE} STREQUAL "SHARED")
        set_target_properties(${LIB_NAME} PROPERTIES
            MACOSX_RPATH ON
            INSTALL_NAME_DIR "@rpath"
        )
    endif()
    
    # Unix/Linux 动态库配置
    if(UNIX AND NOT APPLE AND ${TYPE} STREQUAL "SHARED")
        set_target_properties(${LIB_NAME} PROPERTIES
            INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
        )
    endif()
    
    # 包含目录
    target_include_directories(${LIB_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/QtMaterial>
        PRIVATE
            ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    # 设置输出名称
    if(${TYPE} STREQUAL "STATIC")
        set_target_properties(${LIB_NAME} PROPERTIES
            OUTPUT_NAME "QtMaterial${SUFFIX}"
        )
    else()
        set_target_properties(${LIB_NAME} PROPERTIES
            OUTPUT_NAME "QtMaterial"
        )
    endif()
    
    # 启用 C++17 特性
    target_compile_features(${LIB_NAME} PUBLIC cxx_std_17)
endmacro()

# ==================== 构建库 ====================
# 共享库
if(BUILD_SHARED_LIBS)
    add_qtmaterial_library(SHARED QtMaterial_SHARED "")
    set(QTMATERIAL_SHARED_TARGET QtMaterial_SHARED)
endif()

# 静态库
if(BUILD_STATIC_LIBS)
    add_qtmaterial_library(STATIC QtMaterial_STATIC "-static")
    set(QTMATERIAL_STATIC_TARGET QtMaterial_STATIC)
endif()

# ==================== 接口库（方便用户选择） ====================
add_library(QtMaterial INTERFACE)

target_link_libraries(QtMaterial INTERFACE
    $<$<BOOL:${BUILD_SHARED_LIBS}>:QtMaterial_SHARED>
    $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:QtMaterial_STATIC>
)

target_include_directories(QtMaterial INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/QtMaterial>
)

# ==================== 示例程序 ====================
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ==================== 测试 ====================
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ==================== 安装配置 ====================
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# 安装头文件
install(DIRECTORY src/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QtMaterial
    FILES_MATCHING PATTERN "*.h"
    PATTERN "*.cpp" EXCLUDE
)

# 安装库
if(QTMATERIAL_SHARED_TARGET)
    install(TARGETS ${QTMATERIAL_SHARED_TARGET}
        EXPORT QtMaterialTargets
        RUNTIME
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT Runtime
        LIBRARY
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Runtime
            NAMELINK_SKIP
        ARCHIVE
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Development
    )
endif()

if(QTMATERIAL_STATIC_TARGET)
    install(TARGETS ${QTMATERIAL_STATIC_TARGET}
        EXPORT QtMaterialTargets
        ARCHIVE
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT Development
    )
endif()

# 安装接口库
install(TARGETS QtMaterial
    EXPORT QtMaterialTargets
)

# ==================== CMake 包配置 ====================
# 生成配置文件
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/QtMaterialConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtMaterial
    PATH_VARS
        CMAKE_INSTALL_INCLUDEDIR
        CMAKE_INSTALL_LIBDIR
)

# 生成版本文件
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/QtMaterialConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# 生成配置头文件
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/QtMaterialConfig.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/QtMaterialConfig.h
    @ONLY
)

# 安装配置头文件
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/QtMaterialConfig.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QtMaterial
)

# 导出 Targets
install(EXPORT QtMaterialTargets
    FILE QtMaterialTargets.cmake
    NAMESPACE QtMaterial::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtMaterial
)

# 安装 Config 文件
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/QtMaterialConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/QtMaterialConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtMaterial
)

# ==================== 打印配置信息 ====================
message(STATUS "========================================")
message(STATUS "QtMaterial Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Qt Version: ${Qt6_VERSION}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
if(QTMATERIAL_SHARED_TARGET)
    message(STATUS "  Shared Library: QtMaterial_SHARED")
endif()
if(QTMATERIAL_STATIC_TARGET)
    message(STATUS "  Static Library: QtMaterial_STATIC")
endif()
if(ANDROID)
    message(STATUS "  Android ABI: ${ANDROID_ABI}")
    message(STATUS "  Android Platform: ${ANDROID_PLATFORM}")
endif()
message(STATUS "  Examples: ${BUILD_EXAMPLES}")
message(STATUS "  Tests: ${BUILD_TESTS}")
message(STATUS "========================================")