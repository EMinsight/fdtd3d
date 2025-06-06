#
#  Copyright (C) 2015 Gleb Balykov
#
#  This file is part of fdtd3d.
#
#  fdtd3d is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  fdtd3d is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
#  with fdtd3d; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

cmake_minimum_required(VERSION 3.8)

project (fdtd3d LANGUAGES CXX)

option(PRINT_MESSAGE "Print all messages" OFF)
option(PARALLEL_GRID "Use parallel grid" OFF)
option(DYNAMIC_GRID "Use dynamic grid" OFF)
option(CUDA_ENABLED "Cuda support enabled" OFF)
option(COMPLEX_FIELD_VALUES "Complex field values" OFF)
option(LARGE_COORDINATES "Use int64 for grid coordinates" OFF)
option(COMBINED_SENDRECV "Use combined sendrecv" ON)
option(MPI_CLOCK "Use mpi clock (MPI_Wtime) for clocks" OFF)
option(LINK_NUMA "Link with NUMA library" OFF)

set(SOLVER_DIM_MODES "ALL" CACHE STRING "Defines FDTD solver dimension modes, which are compiled")

set(VALUE_TYPE "d" CACHE STRING "Defines type of values")
set(PARALLEL_GRID_DIMENSION "2" CACHE STRING "Defines number dimension in grid")
set(PARALLEL_BUFFER_DIMENSION "x" CACHE STRING "Defines dimension of parallel grid")
set(CUDA_ARCH_SM_TYPE "sm_20" CACHE STRING "Defines cuda gpu arch")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORCE_INLINES")

add_definitions (-DSOLVER_VERSION=1.2)

# All the supported solver modes
list (APPEND SUPPORTED_DIM_MODES_1D "DIM1" "EX_HY" "EX_HZ" "EY_HX" "EY_HZ" "EZ_HX" "EZ_HY")
list (APPEND SUPPORTED_DIM_MODES_2D "DIM2" "TEX" "TEY" "TEZ" "TMX" "TMY" "TMZ")
list (APPEND SUPPORTED_DIM_MODES_3D "DIM3")
list (APPEND SUPPORTED_DIM_MODES "${SUPPORTED_DIM_MODES_1D}" "${SUPPORTED_DIM_MODES_2D}" "${SUPPORTED_DIM_MODES_3D}")

string (REPLACE "," ";" SOLVER_DIM_MODES "${SOLVER_DIM_MODES}")

if ("${SOLVER_DIM_MODES}" STREQUAL "ALL")
  message ("Compiling all modes")

  foreach (mode IN LISTS SUPPORTED_DIM_MODES)
    add_definitions (-DMODE_${mode})
  endforeach (mode)

  add_definitions (-DMODE_DIM1)
  add_definitions (-DMODE_DIM2)
  add_definitions (-DMODE_DIM3)
else ()
  foreach (mode IN LISTS SOLVER_DIM_MODES)
    if ("${mode}" IN_LIST SUPPORTED_DIM_MODES)
      message ("Compiling with support of ${mode} mode")
    else ()
      message (FATAL_ERROR "${mode} mode is not supported. Supported modes: ALL, ${SUPPORTED_DIM_MODES}")
    endif ()

    if ("${mode}" STREQUAL "DIM1")
      foreach (mode IN LISTS SUPPORTED_DIM_MODES_1D)
        add_definitions (-DMODE_${mode})
      endforeach (mode)
    elseif ("${mode}" STREQUAL "DIM2")
      foreach (mode IN LISTS SUPPORTED_DIM_MODES_2D)
        add_definitions (-DMODE_${mode})
      endforeach (mode)
    else ()
      add_definitions (-DMODE_${mode})

      if ("${mode}" IN_LIST SUPPORTED_DIM_MODES_1D)
        add_definitions (-DMODE_DIM1)
      endif ()
      if ("${mode}" IN_LIST SUPPORTED_DIM_MODES_2D)
        add_definitions (-DMODE_DIM2)
      endif ()
    endif ()
  endforeach (mode)
endif ()

if ("${CUDA_ENABLED}")
  enable_language (CUDA)
  set(CUDA_SEPARABLE_COMPILATION ON)
  set(CUDA_PROPAGATE_HOST_FLAGS ON)
endif ()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set (CMAKE_BUILD_TYPE "RelWithDebInfo")
endif ()

if ("${CMAKE_CXX_COMPILER}" STREQUAL "")
  set (CMAKE_CXX_COMPILER "g++")
endif ()

if ("${CMAKE_C_COMPILER}" STREQUAL "")
  set (CMAKE_C_COMPILER "gcc")
endif ()

if ("${CUDA_ENABLED}")
  if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    if ("${SOLVER_DIM_MODES}" STREQUAL "ALL")
      message (FATAL_ERROR "Debug CUDA builds with all dimension modes (ALL) are too heavy in terms of compilation time. Use specific dimension.")
    endif ()

    set (CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -O0 -G")
  endif ()

  set (CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -arch ${CUDA_ARCH_SM_TYPE}")
  #set (CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -arch ${CUDA_ARCH_SM_TYPE} -coda ${CUDA_ARCH_SM_TYPE} -x cu -ccbin ${CMAKE_CXX_COMPILER} -Xcompiler -m64 -keep -v")
endif ()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  message ("Build type: ${CMAKE_BUILD_TYPE}.")
  add_definitions (-DENABLE_ASSERTS)
  add_definitions (-DDEBUG_INFO)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  message ("Build type: ${CMAKE_BUILD_TYPE}.")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
  message ("Build type: ${CMAKE_BUILD_TYPE}.")
  add_definitions (-DENABLE_ASSERTS)
  add_definitions (-DDEBUG_INFO)
endif ()

message ("C++ compiler: ${CMAKE_CXX_COMPILER}.")
message ("C compiler: ${CMAKE_C_COMPILER}.")

if ("${VALUE_TYPE}" STREQUAL "f")
  message ("Floating point values: float.")
  add_definitions (-DFLOAT_VALUES)
elseif ("${VALUE_TYPE}" STREQUAL "d")
  message ("Floating point values: double.")
  add_definitions (-DDOUBLE_VALUES)
elseif ("${VALUE_TYPE}" STREQUAL "ld")
  if ("${CUDA_ENABLED}")
    message(FATAL_ERROR "Long double values are not supported with Cuda")
  endif ()
  if (NOT ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" OR "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64" OR "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "riscv64"))
    message(FATAL_ERROR "Long double values are supported only with x86_64, aarch64, riscv64.")
  endif ()

  message ("Floating point values: long double.")
  add_definitions (-DLONG_DOUBLE_VALUES)
else ()
  message(FATAL_ERROR "Unknown values type")
endif ()

if ("${COMPLEX_FIELD_VALUES}")
  message ("Complex field values.")
  add_definitions (-DCOMPLEX_FIELD_VALUES)
else ()
  message ("Real field values.")
endif ()

if ("${LARGE_COORDINATES}")
  add_definitions (-DLARGE_COORDINATES)
endif ()

if ("${PRINT_MESSAGE}")
  message ("Print messages.")
  add_definitions (-DPRINT_MESSAGE=1)
else ()
  message ("Do not print messages.")
  add_definitions (-DPRINT_MESSAGE=0)
endif ()

if ("${PARALLEL_GRID_DIMENSION}" STREQUAL "1")
  add_definitions (-DGRID_1D)
elseif ("${PARALLEL_GRID_DIMENSION}" STREQUAL "2")
  add_definitions (-DGRID_2D)
elseif ("${PARALLEL_GRID_DIMENSION}" STREQUAL "3")
  add_definitions (-DGRID_3D)
else ()
  #message(FATAL_ERROR "Unknown PARALLEL_GRID_DIMENSION")
  add_definitions (-DGRID_2D)
endif ()

message ("Grid dimension: ${PARALLEL_GRID_DIMENSION}.")

if ("${PARALLEL_GRID}")
  message ("Parallel grid: ON.")
  add_definitions (-DPARALLEL_GRID)

  if ("${COMBINED_SENDRECV}")
    add_definitions (-DCOMBINED_SENDRECV)
  endif ()

  if ("${MPI_CLOCK}")
    add_definitions (-DMPI_CLOCK)
  endif ()

  if ("${DYNAMIC_GRID}")
    add_definitions (-DDYNAMIC_GRID)

    if ("${COMBINED_SENDRECV}")
      message(FATAL_ERROR "Unsupported: combined sendrecv is not yet implemented for dynamic grid")
    endif ()
  endif ()

  if ("${PARALLEL_GRID_DIMENSION}" STREQUAL "1")

    if ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "x")
      add_definitions (-DPARALLEL_BUFFER_DIMENSION_1D_X)
      message ("Parallel dimension: ${PARALLEL_BUFFER_DIMENSION}.")
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "y")
      message(FATAL_ERROR "Unsupported PARALLEL_BUFFER_DIMENSION for this PARALLEL_GRID_DIMENSION")
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "z")
      message(FATAL_ERROR "Unsupported PARALLEL_BUFFER_DIMENSION for this PARALLEL_GRID_DIMENSION")
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "xy")
      message(FATAL_ERROR "Unsupported PARALLEL_BUFFER_DIMENSION for this PARALLEL_GRID_DIMENSION")
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "yz")
      message(FATAL_ERROR "Unsupported PARALLEL_BUFFER_DIMENSION for this PARALLEL_GRID_DIMENSION")
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "xy")
      message(FATAL_ERROR "Unsupported PARALLEL_BUFFER_DIMENSION for this PARALLEL_GRID_DIMENSION")
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "xyz")
      message(FATAL_ERROR "Unsupported PARALLEL_BUFFER_DIMENSION for this PARALLEL_GRID_DIMENSION")
    else ()
      message(FATAL_ERROR "Unknown PARALLEL_BUFFER_DIMENSION")
    endif ()

  elseif ("${PARALLEL_GRID_DIMENSION}" STREQUAL "2")

    if ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "x")
      message ("Parallel dimension: ${PARALLEL_BUFFER_DIMENSION}.")
      add_definitions (-DPARALLEL_BUFFER_DIMENSION_1D_X)
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "y")
      message ("Parallel dimension: ${PARALLEL_BUFFER_DIMENSION}.")
      add_definitions (-DPARALLEL_BUFFER_DIMENSION_1D_Y)
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "z")
      message(FATAL_ERROR "Unsupported PARALLEL_BUFFER_DIMENSION for this PARALLEL_GRID_DIMENSION")
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "xy")
      message ("Parallel dimension: ${PARALLEL_BUFFER_DIMENSION}.")
      add_definitions (-DPARALLEL_BUFFER_DIMENSION_2D_XY)
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "yz")
      message(FATAL_ERROR "Unsupported PARALLEL_BUFFER_DIMENSION for this PARALLEL_GRID_DIMENSION")
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "xz")
      message(FATAL_ERROR "Unsupported PARALLEL_BUFFER_DIMENSION for this PARALLEL_GRID_DIMENSION")
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "xyz")
      message(FATAL_ERROR "Unsupported PARALLEL_BUFFER_DIMENSION for this PARALLEL_GRID_DIMENSION")
    else ()
      message(FATAL_ERROR "Unknown PARALLEL_BUFFER_DIMENSION")
    endif ()

  elseif ("${PARALLEL_GRID_DIMENSION}" STREQUAL "3")

    if ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "x")
      message ("Parallel dimension: ${PARALLEL_BUFFER_DIMENSION}.")
      add_definitions (-DPARALLEL_BUFFER_DIMENSION_1D_X)
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "y")
      message ("Parallel dimension: ${PARALLEL_BUFFER_DIMENSION}.")
      add_definitions (-DPARALLEL_BUFFER_DIMENSION_1D_Y)
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "z")
      message ("Parallel dimension: ${PARALLEL_BUFFER_DIMENSION}.")
      add_definitions (-DPARALLEL_BUFFER_DIMENSION_1D_Z)
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "xy")
      message ("Parallel dimension: ${PARALLEL_BUFFER_DIMENSION}.")
      add_definitions (-DPARALLEL_BUFFER_DIMENSION_2D_XY)
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "yz")
      message ("Parallel dimension: ${PARALLEL_BUFFER_DIMENSION}.")
      add_definitions (-DPARALLEL_BUFFER_DIMENSION_2D_YZ)
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "xz")
      message ("Parallel dimension: ${PARALLEL_BUFFER_DIMENSION}.")
      add_definitions (-DPARALLEL_BUFFER_DIMENSION_2D_XZ)
    elseif ("${PARALLEL_BUFFER_DIMENSION}" STREQUAL "xyz")
      message ("Parallel dimension: ${PARALLEL_BUFFER_DIMENSION}.")
      add_definitions (-DPARALLEL_BUFFER_DIMENSION_3D_XYZ)
    else ()
      message(FATAL_ERROR "Unknown PARALLEL_BUFFER_DIMENSION")
    endif ()

  else ()
    message(FATAL_ERROR "Unknown PARALLEL_GRID_DIMENSION")
  endif ()
else ()
  message ("Parallel grid: OFF.")
endif ()

if ("${CUDA_ENABLED}")
  message ("Cuda: ON.")
  add_definitions (-DCUDA_ENABLED)
endif ()

set (BUILD_FLAGS "")

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-std=c++11 COMPILER_SUPPORTS_CXX11)
if (COMPILER_SUPPORTS_CXX11)
  if ("${CXX11_ENABLED}" STREQUAL "")
    set(CXX11_ENABLED ON)
  endif ()
else ()
  set(CXX11_ENABLED OFF)
endif ()
if ("${CXX11_ENABLED}")
  message ("C++11: ON.")
  add_definitions (-DCXX11_ENABLED)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set (CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -std=c++11")
endif ()

if ("${LINK_NUMA}")
  set (BUILD_FLAGS "${BUILD_FLAGS} -lnuma")
endif ()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  #set (BUILD_FLAGS "${BUILD_FLAGS} -flto -fno-fat-lto-objects")
  #set (BUILD_FLAGS "${BUILD_FLAGS} -fno-stack-protector")
  #set (BUILD_FLAGS "${BUILD_FLAGS} -fno-exceptions -fno-rtti")
endif ()

#-pedantic -pedantic-errors

set (WARNING_FLAGS "")
#set (WARNING_FLAGS "${WARNING_FLAGS} -Wfatal-errors")
#set (WARNING_FLAGS "${WARNING_FLAGS} -Wall -Wextra")
set (WARNING_FLAGS "${WARNING_FLAGS} -Wformat=2 -Wformat-security -Wuninitialized")
set (WARNING_FLAGS "${WARNING_FLAGS} -Winit-self -Wno-stack-protector")
#set (WARNING_FLAGS "${WARNING_FLAGS} -Wconversion -Wsign-conversion")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set (BUILD_FLAGS "${BUILD_FLAGS} -O0")
endif ()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BUILD_FLAGS} ${WARNING_FLAGS}")

# Include MPI
if ("${PARALLEL_GRID}")
  find_package (MPI REQUIRED)
  message ("MPI_CXX_INCLUDE_DIRS: ${MPI_CXX_INCLUDE_DIRS}")
  message ("MPI_CXX_LINK_FLAGS: ${MPI_CXX_LINK_FLAGS}")
  message ("MPI_CXX_LIBRARIES: ${MPI_CXX_LIBRARIES}")

  include_directories (${MPI_CXX_INCLUDE_DIRS})
  set (LINK_FLAGS "${LINK_FLAGS} ${MPI_CXX_LINK_FLAGS}")
endif()

message ("Compile flags: ${CMAKE_CXX_FLAGS}")
message ("Link flags: ${LINK_FLAGS}")


if (NOT EXISTS "${PROJECT_SOURCE_DIR}/Third-party/EasyBMP/source")
  execute_process (COMMAND wget https://github.com/zer011b/EasyBMP/archive/v1.6.zip -O ${PROJECT_SOURCE_DIR}/Third-party/EasyBMP/EasyBMP.zip)
  execute_process (COMMAND unzip ${PROJECT_SOURCE_DIR}/Third-party/EasyBMP/EasyBMP.zip -d ${PROJECT_SOURCE_DIR}/Third-party/EasyBMP)
  execute_process (COMMAND mv ${PROJECT_SOURCE_DIR}/Third-party/EasyBMP/EasyBMP-1.6/source ${PROJECT_SOURCE_DIR}/Third-party/EasyBMP/)
  execute_process (COMMAND rm -rf ${PROJECT_SOURCE_DIR}/Third-party/EasyBMP/EasyBMP.zip ${PROJECT_SOURCE_DIR}/Third-party/EasyBMP/EasyBMP-1.6/)
endif ()

include_directories ("${PROJECT_SOURCE_DIR}/Third-party/EasyBMP/source")
add_subdirectory (${PROJECT_SOURCE_DIR}/Third-party/EasyBMP)

add_subdirectory (Source)
