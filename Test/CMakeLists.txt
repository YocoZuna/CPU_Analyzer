add_library(Test STATIC
Src/Dummy.c
)

target_include_directories(Test PUBLIC
Includs
UnityFrameWork
${CMAKE_CURRENT_LIST_DIR})


