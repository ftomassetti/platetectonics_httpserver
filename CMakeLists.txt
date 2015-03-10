cmake_minimum_required(VERSION 3.1)

project (PlateTectonicsHttpServer)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

FIND_PACKAGE(OpenSSL REQUIRED)
FIND_PACKAGE(LibUV)
FIND_PACKAGE(LibYAML)
find_package(PNG REQUIRED)

SET(EXTRA_LIBRARIES pthread ${CMAKE_DL_LIBS})

add_executable(ptserver src/server.c src/wrap_pt.cpp src/map_drawing.cpp)

include_directories("deps/h2o/include" "deps/plate-tectonics/src" ${PNG_INCLUDE_DIRS})

target_link_libraries(ptserver ${CMAKE_SOURCE_DIR}/deps/h2o/libh2o.a ${CMAKE_SOURCE_DIR}/deps/plate-tectonics/build/libPlateTectonics.a ${OPENSSL_LIBRARIES} ${LIBUV_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${PNG_LIBRARIES} ${EXTRA_LIBRARIES})
