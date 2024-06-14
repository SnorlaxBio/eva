cmake_minimum_required(VERSION 3.22)
project(SnorlaxSTD C)
include(ExternalProject)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSNORLAX_DEBUG")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSNORLAX_DEBUG")

include_directories(src include)

message(${CMAKE_CURRENT_SOURCE_DIR})

ExternalProject_Add(snorlax-std
    PREFIX          "${PROJECT_SOURCE_DIR}/lib"
    TMP_DIR         "${PROJECT_SOURCE_DIR}/lib/tmp/snorlax/std"
    STAMP_DIR       "${PROJECT_SOURCE_DIR}/lib/stamp/snorlax/std"
    DOWNLOAD_DIR    "${PROJECT_SOURCE_DIR}/lib/snorlax/std"
    SOURCE_DIR      "${PROJECT_SOURCE_DIR}/lib/snorlax/std"
    BINARY_DIR      "${PROJECT_SOURCE_DIR}/lib/snorlax/std/build"
    INSTALL_DIR     "${PROJECT_SOURCE_DIR}/lib"
    LOG_DIR         "${PROJECT_SOURCE_DIR}/lib/log"
    GIT_REPOSITORY  git@github.com:SnorlaxBio/std.git
    GIT_TAG         main
    GIT_REMOTE_NAME origin
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug
                                       -S "${PROJECT_SOURCE_DIR}/lib/snorlax/std"
                                       -B "${PROJECT_SOURCE_DIR}/lib/snorlax/std/build"
                                       --install-prefix ${PROJECT_SOURCE_DIR}
    BUILD_COMMAND   ${CMAKE_COMMAND} --build "${PROJECT_SOURCE_DIR}/lib/snorlax/std/build"
                                     --target all)

set(SNORLAX_EVA_SOURCE      src/snorlax/event.c)
set(SNORLAX_EVA_HEADER      src/snorlax/event.h)

add_library(snorlax-eva ${SNORLAX_EVA_SOURCE}
                        ${SNORLAX_EVA_HEADER})

target_compile_options(snorlax-eva PUBLIC "-ffile-prefix-map=${CMAKE_SOURCE_DIR}=.")

add_executable(snorlax-eva-check src/main.c)
target_link_libraries(snorlax-eva-check snorlax-eva)
