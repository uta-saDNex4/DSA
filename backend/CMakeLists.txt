cmake_minimum_required(VERSION 3.20)

project(StudentGradeManagement)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(StudentGradeManagement
    src/main.cpp
    src/database/Database.cpp
    src/repositories/GradeRepository.cpp
)

target_include_directories(
    StudentGradeManagement
    PRIVATE
        include
)

target_link_libraries(
    StudentGradeManagement
    PRIVATE
        odbc32
)