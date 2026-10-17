cmake_minimum_required(VERSION 3.16)
project(DSA_QuanLyDiem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Thư mục chứa các file header
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Danh sách các file mã nguồn Backend
set(BACKEND_SOURCES
    src/struct.cpp # nếu có
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

# Loại bỏ các file không tồn tại
list(FILTER BACKEND_SOURCES EXCLUDE REGEX "src/struct.cpp")

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

# In thông tin cấu hình
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
