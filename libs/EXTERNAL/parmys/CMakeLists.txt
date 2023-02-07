cmake_minimum_required(VERSION 3.9)

project("parmys")

file(GLOB_RECURSE LIB_SOURCES *.cpp)
file(GLOB_RECURSE LIB_HEADERS *.h)
files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)

add_library(parmys SHARED
    ${LIB_HEADERS}
    ${LIB_SOURCES}
)
target_include_directories(parmys PUBLIC ${LIB_INCLUDE_DIRS})

target_link_libraries(parmys
    liblog
    libarchfpga
    libvtrutil
    librtlnumber
    libargparse
    ${CMAKE_DL_LIBS}
)

set_target_properties(parmys PROPERTIES PREFIX "") # Avoid extra 'lib' prefix

install(TARGETS parmys DESTINATION bin)
