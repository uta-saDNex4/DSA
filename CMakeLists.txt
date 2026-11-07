cmake_minimum_required(VERSION 3.16)
project(DSA_QuanLyDiem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Thư mục chứa các file header
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Danh sách các file mã nguồn Backend
set(BACKEND_SOURCES
    src/utils.cpp
    src/rang_buoc.cpp
    src/monhoc.cpp
    src/sinhvien.cpp
    src/lop.cpp
    src/dangky.cpp
    src/loptc.cpp
    src/file_io.cpp
    src/diem.cpp
)

# Target 1: Console App (Chạy kiểm thử và hoàn thiện toàn bộ Backend)
add_executable(DSA_Console
    src/main.cpp
    ${BACKEND_SOURCES}
)

# Cấu hình compiler flags
if (MSVC)
    target_compile_options(DSA_Console PRIVATE /W3 /utf-8)
else()
    target_compile_options(DSA_Console PRIVATE -Wall -Wextra)
endif()

# ====================================================================
# Target 2: Qt GUI App (Giao diện đồ họa sinh viên hệ tín chỉ)
# ====================================================================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets Core Gui QUIET)
if (Qt6_FOUND)
    message(STATUS "Qt6 found! Configuring Qt GUI target: DSA_QtApp")
    file(GLOB GUI_SOURCES "src/gui/*.cpp" "src/gui/*.h")
    add_executable(DSA_QtApp
        ${GUI_SOURCES}
        ${BACKEND_SOURCES}
    )
    target_include_directories(DSA_QtApp PRIVATE src/gui include)
    target_link_libraries(DSA_QtApp PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)
else()
    message(WARNING "Qt6 not found, skipping DSA_QtApp target")
endif()

# In thông tin cấu hình
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
