add_executable(CPUA
#1main.c
#Reader_AnalyzerTest.c
#CircuralTest.c
#LoggerTest.c
App.c

)

target_link_libraries(CPUA
Includs
UnityFrameWork

)
target_include_directories(CPUA PUBLIC
Includs
)

target_compile_definitions(CPUA PUBLIC NDEBUG)