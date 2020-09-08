cmake_minimum_required(VERSION 3.3)
project(msg)

list(
        APPEND HEADER_FILES_RPU
        rpu_apu_msg_generated.h 

)

list(
        APPEND SOURCE_FILES_RPU
        rpu_apu_msg_generated.c

)

list(
        APPEND HEADER_FILES_APU
        apu_rpu_msg_generated.h 

)

list(
        APPEND SOURCE_FILES_APU
        apu_rpu_msg_generated.c
)

set(ALL_SOURCE_FILES_RPU ${HEADER_FILES_RPU} ${SOURCE_FILES_RPU})
add_library(rpu_${PROJECT_NAME}  ${ALL_SOURCE_FILES_RPU} )

set(ALL_SOURCE_FILES_APU ${HEADER_FILES_APU} ${SOURCE_FILES_APU})
add_library(apu_${PROJECT_NAME}  ${ALL_SOURCE_FILES_APU} )

#target_link_libraries(${PROJECT_NAME}   )

