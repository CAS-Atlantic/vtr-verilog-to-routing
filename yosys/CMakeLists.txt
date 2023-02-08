cmake_minimum_required(VERSION 3.9)

project("yosys")

# Create a target out of the library compilation result
SET(BINARY_LIB_FILE1 ${CMAKE_CURRENT_BINARY_DIR}/yosys1)
SET(BINARY_LIB_FILE2 ${CMAKE_CURRENT_BINARY_DIR}/yosys2)
SET(YOSYS_EXEC ${CMAKE_CURRENT_SOURCE_DIR}/yosys)
SET(BINARY_EXEC_FILE ${CMAKE_CURRENT_BINARY_DIR}/yosys)
SET(YOSYS_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

# handle cppflags to suppress yosys warning with both ninja and unix style
set(CURRENT_CPPFLAGS "$(CPPFLAGS)-w")
if(${CMAKE_GENERATOR} STREQUAL "Ninja")
    set(CURRENT_CPPFLAGS "-w")
endif()

# how to build the result of the library
add_custom_command(OUTPUT  ${BINARY_LIB_FILE1}
        COMMAND ${MAKE_PROGRAM} "ENABLE_ABC=0"
            PREFIX=${CMAKE_BINARY_DIR}
            > /dev/null

        COMMAND ${MAKE_PROGRAM} install
            PREFIX=${CMAKE_BINARY_DIR}
            > /dev/null

        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# create a target out of the library compilation result
add_custom_target(yosys-qwerty ALL DEPENDS ${BINARY_LIB_FILE1})

# create an library target out of the library compilation result
#add_library(libyosys SHARED IMPORTED GLOBAL)

# adding dependency to libarchfpga, as it is required
# by VTR scripts for Yosys standalone frontend
#add_dependencies(libyosys yosys libarchfpga)

# specify where the library is and where to find the headers
set_target_properties(yosys-qwerty
        PROPERTIES PREFIX "" #Avoid extra 'lib' prefix
        IMPORTED_LOCATION ${BINARY_LIB_FILE1}
        INTERFACE_INCLUDE_DIRECTORIES ${YOSYS_INCLUDE_DIRS})


#install(FILES ${BINARY_LIB_FILE1} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})