add_library( UnityFrameWork STATIC
unity.c
unity_fixture.c
unity_memory.c

)
target_include_directories(UnityFrameWork PUBLIC

${CMAKE_CURRENT_LIST_DIR}

)