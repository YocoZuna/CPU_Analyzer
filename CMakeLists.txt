cmake_minimum_required(VERSION 3.0.0)
project(TestCmake VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()
add_subdirectory(UnityFrameWork)
add_subdirectory(Src)
add_subdirectory(Includs)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)