add_library(Includs STATIC
 Src/Reader.c
 Src/Printer.c
 Src/Analayzer.c
 Src/main.c
 Src/WatchDog.c
 Src/Loger.c
 Src/CircuralBuffer.c
)

target_include_directories(Includs PUBLIC
Inc
Src
${CMAKE_CURRENT_LIST_DIR})
