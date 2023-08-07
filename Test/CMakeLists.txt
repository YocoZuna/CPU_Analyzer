add_library(Test STATIC
Src/TestFixture.c 

)

target_include_directories(Test PUBLIC
Inc
Src
${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(Test PUBLIC
UnityFrameWork
Includs
)

