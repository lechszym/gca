cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(gca)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Makes sure that executables go to bin directory (respective to build dir)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
SET(MY_OWN_INSTALL_PREFIX "bin" CACHE PATH "Prefix prepended to install directories")
SET(CMAKE_INSTALL_PREFIX "${MY_OWN_INSTALL_PREFIX}" CACHE INTERNAL "Prefix prepended to install directories" FORCE)

# If Eigen found EIGEN_ENABLED define to compile flags
find_package(Eigen)
if(EIGEN_FOUND)
  include_directories(SYSTEM ${EIGEN_INCLUDE_DIRS})
  add_definitions(-DEIGEN_ENABLED)
endif(EIGEN_FOUND)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -std=c++0x -no-pie")

find_package(OpenMP)
if(OPENMP_FOUND)
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -DOMP_ENABLED")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif(OPENMP_FOUND)


find_package (Python3 COMPONENTS Interpreter Development)

if(Python3_LIBRARIES)
    set(Boost_FIND_QUIETLY 1)
    find_package(Boost COMPONENTS python3)
    if(NOT Boost_FOUND)
       find_package(Boost COMPONENTS python36)
    endif()
    if(NOT Boost_FOUND)
       find_package(Boost COMPONENTS python37)
    endif()
endif() 

#FIND_PACKAGE(Python3Interp)
#FIND_PACKAGE(Python3Libs)
#if(APPLE AND ${PYTHON_VERSION_MAJOR} EQUAL 3)
#    FIND_PACKAGE(Boost COMPONENTS python3)
#else()
#    FIND_PACKAGE(Boost COMPONENTS python)
#endif()

#ENABLE_TESTING()
#INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})
#LINK_LIBRARIES(${Boost_LIBRARIES} ${PYTHON_LIBRARIES}) # Deprecated but so convenient!


# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

# add the endoscopy src for compile - it has its own cmake config
add_subdirectory(tests)

if(Boost_FOUND)
    MESSAGE("-- Found boost-python3")
    add_subdirectory(python)
endif() 