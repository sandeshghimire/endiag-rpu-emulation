cmake_minimum_required(VERSION 3.3)
project(msg)

list(
        APPEND HEADER_FILES_LIBS
        rpu_apu_msg_generated.h 

)

list(
        APPEND SOURCE_FILES_LIBS
        rpu_apu_msg_generated.c

)

set(ALL_SOURCE_FILES_LIBS ${HEADER_FILES_LIBS} ${SOURCE_FILES_LIBS})
add_library(${PROJECT_NAME}  ${ALL_SOURCE_FILES_LIBS})

#target_link_libraries(${PROJECT_NAME}   flatccrt)

