add_library(Includs STATIC
 Src/Printer.c
 Src/WatchDog.c
 Src/Reader.c
 Src/MyLogger.c
 Src/Analyzer.c
)
#target_link_libraries(Includs Test)

target_include_directories(Includs PUBLIC
#Test/Src
Inc
Src
${CMAKE_CURRENT_LIST_DIR})
