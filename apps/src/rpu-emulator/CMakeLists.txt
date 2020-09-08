cmake_minimum_required(VERSION 3.3)
project(rpu_emulator)
list(
        APPEND HEADER_FILES_EXE
        rpu_emulator.h
        task_10ms.h
        task_20ms.h
        task_40ms.h
)

list(
        APPEND SOURCE_FILES_EXE
        rpu_emulator.c
        task_10ms.c
        task_20ms.c
        task_40ms.c

)

list(
        APPEND HEADER_FILES_LIBS
        timer_lib.h
        mmap_lib.h

)

list(
        APPEND SOURCE_FILES_LIBS
        timer_lib.c
        mmap_lib.c

)

set(ALL_SOURCE_FILES_EXE ${HEADER_FILES_EXE} ${SOURCE_FILES_EXE})
set(ALL_SOURCE_FILES_LIBS ${HEADER_FILES_LIBS} ${SOURCE_FILES_LIBS})

add_executable(${PROJECT_NAME}  ${ALL_SOURCE_FILES_EXE})
add_library(${PROJECT_NAME}_libs  ${ALL_SOURCE_FILES_LIBS})

target_link_libraries(${PROJECT_NAME}   ${PROJECT_NAME}_libs pthread  apu_msg rpu_msg  flatccrt)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
