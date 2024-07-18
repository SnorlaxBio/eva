cmake_minimum_required(VERSION 3.22)
project(SnorlaxEva C)
include(ExternalProject)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DSNORLAX_DEBUG")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSNORLAX_DEBUG")

include_directories(src include)

message(${CMAKE_CURRENT_SOURCE_DIR})

ExternalProject_Add(snorlax-std
    PREFIX          "${PROJECT_SOURCE_DIR}/lib/snorlax/std"
    TMP_DIR         "${PROJECT_SOURCE_DIR}/lib/snorlax/std/tmp"
    STAMP_DIR       "${PROJECT_SOURCE_DIR}/lib/snorlax/std/stamp"
    DOWNLOAD_DIR    "${PROJECT_SOURCE_DIR}/lib/snorlax/std/src"
    SOURCE_DIR      "${PROJECT_SOURCE_DIR}/lib/snorlax/std/src"
    BINARY_DIR      "${PROJECT_SOURCE_DIR}/lib/snorlax/std/build"
    INSTALL_DIR     "${PROJECT_SOURCE_DIR}/lib/snorlax/std/install"
    LOG_DIR         "${PROJECT_SOURCE_DIR}/lib/snorlax/std/log"
    GIT_REPOSITORY  git@github.com:SnorlaxBio/std.git
    GIT_TAG         main
    GIT_REMOTE_NAME origin
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug
                                       -S "${PROJECT_SOURCE_DIR}/lib/snorlax/std/src"
                                       -B "${PROJECT_SOURCE_DIR}/lib/snorlax/std/build"
                                       --install-prefix ${PROJECT_SOURCE_DIR}
    BUILD_COMMAND     ${CMAKE_COMMAND} --build "${PROJECT_SOURCE_DIR}/lib/snorlax/std/build"
                                       --target all)

link_directories(${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR}/build)

set(SNORLAX_EVA_SOURCE      src/snorlax/eva.c
                            src/snorlax/event.c
                            src/snorlax/event/queue.c
                            src/snorlax/event/processor.c
                            src/snorlax/event/generator.c
                            src/snorlax/event/engine.c
                            src/snorlax/event/subscription/event.c
                            src/snorlax/event/subscription/event/queue.c
                            src/snorlax/event/processor/pool.c
                            src/snorlax/event/generator/set.c
                            src/snorlax/command.c
                            src/snorlax/command/event/subscription.c
                            src/snorlax/command/event/generator.c
                            src/snorlax/command/event/subscription/process.c
                            src/snorlax/descriptor.c
                            src/snorlax/descriptor/exception.c
                            src/snorlax/descriptor/event/subscription.c
                            src/snorlax/descriptor/event/subscription/process.c
                            src/snorlax/descriptor/event/generator.c
                            src/snorlax/descriptor/event/generator/epoll.c
                            src/snorlax/descriptor/event/generator/epoll/subscription/process.c
                            src/snorlax/console/out.c
                            src/snorlax/console/in.c
                            src/snorlax/socket.c
                            src/snorlax/socket/session.c
                            src/snorlax/socket/server.c
                            src/snorlax/socket/client.c
                            src/snorlax/socket/session/event/subscription.c
                            src/snorlax/socket/server/event/subscription.c
                            src/snorlax/socket/server/event/subscription/list.c
                            src/snorlax/socket/server/event/subscription/list/node.c
                            src/snorlax/socket/client/event/subscription.c
                            src/snorlax/socket/client/event/subscription/pool.c
                            src/snorlax/socket/client/event/subscription/pool/node.c)

set(SNORLAX_EVA_HEADER      src/snorlax/eva.h
                            src/snorlax/event.h
                            src/snorlax/event/type.h
                            src/snorlax/event/queue.h
                            src/snorlax/event/processor.h
                            src/snorlax/event/generator.h
                            src/snorlax/event/engine.h
                            src/snorlax/event/subscription/event.h
                            src/snorlax/event/subscription/event/queue.h
                            src/snorlax/event/processor/pool.h
                            src/snorlax/event/processor/pool/state.h
                            src/snorlax/event/generator/set.h
                            src/snorlax/event/engine/state.h
                            src/snorlax/command/event.h
                            src/snorlax/command/event/type.h
                            src/snorlax/command/event/subscription.h
                            src/snorlax/command/event/subscription/type.h
                            src/snorlax/command/event/subscription/process.h
                            src/snorlax/descriptor.h
                            src/snorlax/descriptor/event.h
                            src/snorlax/descriptor/event/type.h
                            src/snorlax/descriptor/event/subscription.h
                            src/snorlax/descriptor/event/generator.h
                            src/snorlax/descriptor/event/subscription/type.h
                            src/snorlax/descriptor/event/subscription/process.h
                            src/snorlax/descriptor/event/generator/epoll.h
                            src/snorlax/descriptor/event/generator/epoll/subscription/state.h
                            src/snorlax/descriptor/event/generator/epoll/subscription/process.h
                            src/snorlax/descriptor/event/generator/epoll/control/type.h
                            src/snorlax/console/out.h
                            src/snorlax/console/in.h
                            src/snorlax/socket/session.h
                            src/snorlax/socket/server.h
                            src/snorlax/socket/client.h
                            src/snorlax/socket/client/event/subscription.h
                            src/snorlax/socket/server/event/subscription.h
                            src/snorlax/socket/server/event/subscription/list.h
                            src/snorlax/socket/session/event/subscription.h
                            src/snorlax/socket/client/event/subscription.h
                            src/snorlax/socket/client/event/subscription/pool.h)

add_library(snorlax-eva ${SNORLAX_EVA_SOURCE}
                        ${SNORLAX_EVA_HEADER})

target_compile_options(snorlax-eva PUBLIC "-ffile-prefix-map=${CMAKE_SOURCE_DIR}=.")

add_executable(snorlax-eva-command-event-check src/snorlax/command/event/main.c)
target_link_libraries(snorlax-eva-command-event-check snorlax-eva libsnorlax-std.a)

add_executable(snorlax-eva-console-event-check src/snorlax/console/main.c)
target_link_libraries(snorlax-eva-console-event-check snorlax-eva libsnorlax-std.a)

add_executable(snorlax-eva-client-check src/snorlax/socket/client/main.c)
target_link_libraries(snorlax-eva-client-check snorlax-eva libsnorlax-std.a)

add_executable(snorlax-eva-client-pool-check src/snorlax/socket/client/pool.c)
target_link_libraries(snorlax-eva-client-pool-check snorlax-eva libsnorlax-std.a)

add_executable(snorlax-eva-server-check src/snorlax/socket/server/main.c)
target_link_libraries(snorlax-eva-server-check snorlax-eva libsnorlax-std.a)

add_executable(snorlax-eva-client-benchmark src/snorlax/socket/client/benchmark.c)
target_link_libraries(snorlax-eva-client-benchmark snorlax-eva libsnorlax-std.a)

add_executable(snorlax-eva-server-benchmark src/snorlax/socket/server/benchmark.c)
target_link_libraries(snorlax-eva-server-benchmark snorlax-eva libsnorlax-std.a)

add_executable(snorlax-eva-redis-benchmark src/snorlax/benchmark/redis.c)
target_link_libraries(snorlax-eva-redis-benchmark snorlax-eva libsnorlax-std.a)

install(TARGETS snorlax-eva ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src/ DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
                  FILES_MATCHING PATTERN "*.h")