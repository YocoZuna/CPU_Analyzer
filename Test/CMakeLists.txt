add_library(Test STATIC
Src/Dummy.c
)

target_include_directories(Test PUBLIC
Inc
Includs
${CMAKE_CURRENT_LIST_DIR})


