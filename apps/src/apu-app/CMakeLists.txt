cmake_minimum_required(VERSION 3.3)
project(apu_app)
list(
        APPEND HEADER_FILES_EXE
        apu_app.h

)

list(
        APPEND SOURCE_FILES_EXE
        apu_app.c

)


set(ALL_SOURCE_FILES_EXE ${HEADER_FILES_EXE} ${SOURCE_FILES_EXE})


add_executable(${PROJECT_NAME}  ${ALL_SOURCE_FILES_EXE})

target_link_libraries(${PROJECT_NAME} rpu_apu_msg flatccrt)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
