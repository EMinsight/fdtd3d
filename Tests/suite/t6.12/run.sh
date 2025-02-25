#!/bin/bash

#
#  Copyright (C) 2017 Gleb Balykov
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

set -e

USED_MODE=$1
BASE_DIR=$2
SOURCE_DIR=$3

MODE=""
RUNNER=""
if [[ "$USED_MODE" -eq "1" ]]; then
  MODE="$MODE --use-cuda --cuda-gpus 0 --num-cuda-threads x:4,y:4,z:4"
fi
if [[ "$USED_MODE" -eq "3" ]]; then
  MODE="$MODE --use-cuda --cuda-gpus 0,0 --cuda-buffer-size 2 --buffer-size 2 --num-cuda-threads x:4,y:4,z:4"
fi
if [[ "$USED_MODE" -eq "2" || "$USED_MODE" -eq "3" ]]; then
  MODE="$MODE"
  RUNNER="mpirun -n 2"
fi

function launch ()
{
  local layout_type="$1"
  output_file=$(mktemp /tmp/fdtd3d.vacuum1D.XXXXXXXX)

  tmp_test_file=$(mktemp /tmp/vacuum1D_EzHx.XXXXXXXX.txt)
  cp ${SOURCE_DIR}/Examples/vacuum1D_EzHx.txt $tmp_test_file
  echo $MODE >> $tmp_test_file
  echo "--layout-type $layout_type" >> $tmp_test_file

  $RUNNER ./fdtd3d --cmd-from-file $tmp_test_file &> $output_file

  local ret=$?

  val_max=$(cat previous-1_[timestep=100]_[pid=0]_[name=Ez]_[mod].txt | awk '{print $1}')
  val_min=$(cat previous-1_[timestep=100]_[pid=0]_[name=Ez]_[mod].txt | awk '{print $2}')
  is_ok=$(echo $val_max $val_min | awk '
            function abs(x){return ((x < 0.0) ? -x : x)}
            {
              percent = (abs($1 - 1.0) / 1.0) * 100.0
              if ($1 <= 1.7 && 0.0 == $2)
              {
                print 1;
              }
              else
              {
                print 0;
              }
            }')
  if [ "$is_ok" != "1" ]; then
    echo "Test result is incorrect for Examples/vacuum1D_EzHx.txt"
    ret=$((2))
  fi

  if [ "$ret" != "0" ]; then
    return $ret
  fi

  cp ${SOURCE_DIR}/Examples/vacuum1D_EzHx_scattered.txt $tmp_test_file
  echo $MODE >> $tmp_test_file
  echo "--layout-type $layout_type" >> $tmp_test_file

  $RUNNER ./fdtd3d --cmd-from-file $tmp_test_file &> $output_file

  ret=$?

  val_max=$(cat previous-1_[timestep=100]_[pid=0]_[name=Ez]_[mod].txt | awk '{print $1}')
  val_min=$(cat previous-1_[timestep=100]_[pid=0]_[name=Ez]_[mod].txt | awk '{print $2}')
  is_ok=$(echo $val_max $val_min | awk '
            {
              if (0.0 <= $1 && $1 <= 1e-14 && 0.0 == $2)
              {
                print 1;
              }
              else
              {
                print 0;
              }
            }')
  if [ "$is_ok" != "1" ]; then
    echo "Test result is incorrect for Examples/vacuum1D_EzHx_scattered.txt"
    ret=$((2))
  fi

  return $ret
}

CUR_DIR=`pwd`
TEST_DIR=$(dirname $(readlink -f $0))
cd $TEST_DIR

retval=$((0))
launch 0
if [ $? -ne 0 ]; then
  retval=$((1))
fi
launch 1
if [ $? -ne 0 ]; then
  retval=$((1))
fi

cd $CUR_DIR

exit $retval
