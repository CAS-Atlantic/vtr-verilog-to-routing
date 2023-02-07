cmake_minimum_required(VERSION 3.9)

project("parmys")

# file(GLOB_RECURSE LIB_SOURCES *.cpp)
# file(GLOB_RECURSE LIB_HEADERS *.h)
file(GLOB_RECURSE LIB_SOURCES parmys-plugin/*.cc)
file(GLOB_RECURSE LIB_HEADERS parmys-plugin/*.hpp)
file(GLOB_RECURSE LIB_SOURCES_O parmys-plugin/src/*.cc)
file(GLOB_RECURSE LIB_HEADERS_O parmys-plugin/src/include/*.h)

files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)
files_to_dirs(LIB_HEADERS_O LIB_INCLUDE_DIRS)

add_library(parmys SHARED
    ${LIB_HEADERS}
    ${LIB_SOURCES}
    ${LIB_HEADERS_O}
    ${LIB_SOURCES_O}
)
target_include_directories(parmys PUBLIC ${LIB_INCLUDE_DIRS})
target_include_directories(parmys PUBLIC ${VTR_SOURCE_DIR}/Yosys/share/yosys/include)

add_definitions("-D_YOSYS_")
add_dependencies(parmys libyosys)
target_link_libraries(parmys
    liblog
    libarchfpga
    libvtrutil
    librtlnumber
    libpugixml
	libpugiutil
    pthread
    ${CMAKE_DL_LIBS}
)

set_target_properties(parmys PROPERTIES PREFIX "") # Avoid extra 'lib' prefix

install(TARGETS parmys DESTINATION bin)
