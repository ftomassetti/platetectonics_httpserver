cmake_minimum_required(VERSION 3.1)

project (PlateTectonicsHttpServer)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

FIND_PACKAGE(OpenSSL REQUIRED)
FIND_PACKAGE(LibUV)
FIND_PACKAGE(LibYAML)

SET(EXTRA_LIBRARIES pthread ${CMAKE_DL_LIBS})

add_executable(ptserver src/server.c)

include_directories("deps/h2o/include")

target_link_libraries(ptserver ${CMAKE_SOURCE_DIR}/deps/h2o/libh2o.a ${OPENSSL_LIBRARIES} ${LIBUV_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${EXTRA_LIBRARIES})
