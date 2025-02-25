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

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(TOOLCHAIN "aarch64-linux-gnu")
set(TOOLCHAIN_VERSION "7")
set(CROSS_ROOTFS "/mnt/rpi")

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

include_directories(BEFORE SYSTEM ${CROSS_ROOTFS}/usr/include/c++/${TOOLCHAIN_VERSION})
include_directories(BEFORE SYSTEM ${CROSS_ROOTFS}/usr/include/${TOOLCHAIN}/c++/${TOOLCHAIN_VERSION})

set(CMAKE_SYSROOT "${CROSS_ROOTFS}")

#add_link_options("-Wl,--verbose")
add_link_options("-B${CROSS_ROOTFS}/usr/lib/${TOOLCHAIN}")
add_link_options("-L${CROSS_ROOTFS}/lib")
add_link_options("-L${CROSS_ROOTFS}/usr/lib")
add_link_options("-L${CROSS_ROOTFS}/usr/lib/gcc/${TOOLCHAIN}/${TOOLCHAIN_VERSION}")
add_link_options("-L${CROSS_ROOTFS}/usr/lib/${TOOLCHAIN}/")
add_link_options("-L${CROSS_ROOTFS}/lib/${TOOLCHAIN}/")

# --target <triple> option
set(CMAKE_C_COMPILER_TARGET ${TOOLCHAIN})
set(CMAKE_CXX_COMPILER_TARGET ${TOOLCHAIN})

# --gcc-toolchain <path> option
set(CMAKE_C_COMPILER_EXTERNAL_TOOLCHAIN "${CROSS_ROOTFS}/usr")
set(CMAKE_CXX_COMPILER_EXTERNAL_TOOLCHAIN "${CROSS_ROOTFS}/usr")

# Architecture & microarchitecture (mtune has no effect for clang)
add_compile_options(-mcpu=cortex-a72)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
