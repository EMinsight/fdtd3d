#!/bin/bash

#
#  Copyright (C) 2020 Gleb Balykov
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

set -ex

# Home directory of project where root CMakeLists.txt is placed
HOME_DIR=$1

# Build directory of unit test
BUILD_DIR=$2

# CXX compiler
CXX_COMPILER=$3

# C compiler
C_COMPILER=$4

# Build type
BUILD_TYPE=$5

# Whether build with cxx11 or not
CXX11_ENABLED=$6

mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

function build
{
  PARALLEL_GRID_DIM=$1
  LIST_OF_BUFFERS="$2"

  for VALUE_TYPE in f d ld; do
  for PARALLEL_BUFFER in `echo $LIST_OF_BUFFERS`; do
  for COMBINED_SENDRECV in ON OFF; do
  for COMPLEX_FIELD_VALUES in ON OFF; do

    cmake ${HOME_DIR} \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DVALUE_TYPE=${VALUE_TYPE} \
      -DCOMPLEX_FIELD_VALUES=${COMPLEX_FIELD_VALUES} \
      -DPARALLEL_GRID_DIMENSION=${PARALLEL_GRID_DIM} \
      -DPRINT_MESSAGE=ON \
      -DPARALLEL_GRID=ON \
      -DPARALLEL_BUFFER_DIMENSION=${PARALLEL_BUFFER} \
      -DCXX11_ENABLED=${CXX11_ENABLED} \
      -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
      -DCMAKE_C_COMPILER=${C_COMPILER} \
      -DCOMBINED_SENDRECV=${COMBINED_SENDRECV}

    res=$(echo $?)

    if [[ res -ne 0 ]]; then
      exit 1
    fi

    make unit-test-parallel-grid

    res=$(echo $?)

    if [[ res -ne 0 ]]; then
      exit 1
    fi

    if [[ "$PARALLEL_BUFFER" = "x" ]]; then
      mpirun -n 2 ./Source/UnitTests/unit-test-parallel-grid
    elif [[ "$PARALLEL_BUFFER" = "y" ]]; then
      mpirun -n 2 ./Source/UnitTests/unit-test-parallel-grid
    elif [[ "$PARALLEL_BUFFER" = "z" ]]; then
      mpirun -n 2 ./Source/UnitTests/unit-test-parallel-grid
    elif [[ "$PARALLEL_BUFFER" = "xy" ]]; then
      mpirun -n 4 ./Source/UnitTests/unit-test-parallel-grid
    elif [[ "$PARALLEL_BUFFER" = "yz" ]]; then
      mpirun -n 4 ./Source/UnitTests/unit-test-parallel-grid
    elif [[ "$PARALLEL_BUFFER" = "xz" ]]; then
      mpirun -n 4 ./Source/UnitTests/unit-test-parallel-grid
    fi

    res=$(echo $?)

    if [[ res -ne 0 ]]; then
      exit 1
    fi

    if [[ "$PARALLEL_BUFFER" = "x" ]]; then
      mpirun -n 4 ./Source/UnitTests/unit-test-parallel-grid
    elif [[ "$PARALLEL_BUFFER" = "y" ]]; then
      mpirun -n 4 ./Source/UnitTests/unit-test-parallel-grid
    elif [[ "$PARALLEL_BUFFER" = "z" ]]; then
      mpirun -n 4 ./Source/UnitTests/unit-test-parallel-grid
    elif [[ "$PARALLEL_BUFFER" = "xy" ]]; then
      mpirun -n 16 ./Source/UnitTests/unit-test-parallel-grid
    elif [[ "$PARALLEL_BUFFER" = "yz" ]]; then
      mpirun -n 16 ./Source/UnitTests/unit-test-parallel-grid
    elif [[ "$PARALLEL_BUFFER" = "xz" ]]; then
      mpirun -n 16 ./Source/UnitTests/unit-test-parallel-grid
    elif [[ "$PARALLEL_BUFFER" = "xyz" ]]; then
      mpirun -n 8 ./Source/UnitTests/unit-test-parallel-grid
    fi

    res=$(echo $?)

    if [[ res -ne 0 ]]; then
      exit 1
    fi

  done
  done
  done
  done
}

array1D="x"
build 1 "$array1D"

array2D="x y xy"
build 2 "$array2D"

array3D="x y z xy yz xz xyz"
build 3 "$array3D"

exit 0
