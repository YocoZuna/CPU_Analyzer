cmake_minimum_required(VERSION 3.0.0)
project(CPU_Analyzer VERSION 0.1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 11)

include(CTest)
enable_testing()

add_executable(CPU_Analyzer 
main.c
Reader.c
Analyzer.c
Printer.c
WatchDog.c
MyLogger.c
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
