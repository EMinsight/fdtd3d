/*
 * Copyright (C) 2018 Gleb Balykov
 *
 * This file is part of fdtd3d.
 *
 * fdtd3d is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * fdtd3d is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with fdtd3d; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <iostream>

#include "Assert.h"
#include "Settings.h"

#include <ctime>
#include <sys/time.h>

#include <unistd.h>

#ifdef PARALLEL_GRID

#include "ParallelGrid.h"
#include "ParallelYeeGridLayout.h"
#include <mpi.h>

#ifndef CXX11_ENABLED
#include "cstdlib"
#endif /* !CXX11_ENABLED */

int main (int argc, char** argv)
{
  grid_coord gridSizeX = STOI (argv[1]);
  grid_coord gridSizeY = STOI (argv[2]);
  grid_coord gridSizeZ = STOI (argv[3]);
  time_step NUM_TIME_STEPS = STOI (argv[4]);
  time_step SHARE_TIME_STEP = STOI (argv[5]);

  int bufSize = 1;

  int res = MPI_Init (&argc, &argv);
  ALWAYS_ASSERT (res == MPI_SUCCESS);

  int rank, numProcs;

  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &numProcs);

  printf ("X: PID %d of %d, grid size (" C_MOD ", " C_MOD ", " C_MOD ")\n",
    rank, numProcs, gridSizeX, gridSizeY, gridSizeZ);

  printf ("Start process %d of %d\n", rank, numProcs);

  ParallelGridCoordinate zero (0, 0, 0
#ifdef DEBUG_INFO
                               , CoordinateType::X, CoordinateType::Y, CoordinateType::Z
#endif
                               );

  ParallelGridCoordinate overallSize (gridSizeX, gridSizeY, gridSizeZ
#ifdef DEBUG_INFO
                                      , CoordinateType::X, CoordinateType::Y, CoordinateType::Z
#endif
                                      );
  ParallelGridCoordinate pmlSize (2, 2, 2
#ifdef DEBUG_INFO
                                  , CoordinateType::X, CoordinateType::Y, CoordinateType::Z
#endif
                                  );
  ParallelGridCoordinate tfsfSizeLeft (4, 4, 4
#ifdef DEBUG_INFO
                                       , CoordinateType::X, CoordinateType::Y, CoordinateType::Z
#endif
                                       );
  ParallelGridCoordinate tfsfSizeRight (4, 4, 4
#ifdef DEBUG_INFO
                                        , CoordinateType::X, CoordinateType::Y, CoordinateType::Z
#endif
                                        );
  ParallelGridCoordinate bufferSize (bufSize, bufSize, bufSize
#ifdef DEBUG_INFO
                                     , CoordinateType::X, CoordinateType::Y, CoordinateType::Z
#endif
                                     );
  ParallelGridCoordinate topologySize (2, 2, 2
#ifdef DEBUG_INFO
                                       , CoordinateType::X, CoordinateType::Y, CoordinateType::Z
#endif
                                       );

#if defined (GRID_1D)
#define SCHEME_TYPE (static_cast<SchemeType_t> (SchemeType::Dim1_EzHy))
#define ANGLES PhysicsConst::Pi / 2, 0, PhysicsConst::Pi / 2
#endif

#if defined (GRID_2D)
#define SCHEME_TYPE (static_cast<SchemeType_t> (SchemeType::Dim2_TEz))
#define ANGLES PhysicsConst::Pi / 2, 0, 0
#endif

#if defined (GRID_3D)
#define SCHEME_TYPE (static_cast<SchemeType_t> (SchemeType::Dim3))
#define ANGLES 0, 0, 0
#endif

  ParallelGridCore parallelGridCore (rank, numProcs, overallSize, true, topologySize);
  ParallelGrid::initializeParallelCore (&parallelGridCore);

  bool isDoubleMaterialPrecision = false;

  ParallelYeeGridLayout<SCHEME_TYPE, E_CENTERED> yeeLayout (overallSize, pmlSize, tfsfSizeLeft, tfsfSizeRight, ANGLES, isDoubleMaterialPrecision);
  yeeLayout.Initialize (&parallelGridCore);

  ParallelGrid grid (overallSize, bufferSize, 0, yeeLayout.getSizeForCurNode ());

#if defined (GRID_1D) || defined (GRID_2D) || defined (GRID_3D)
  for (grid_coord i = 0; i < grid.getSize ().get1 (); ++i)
#endif
#if defined (GRID_2D) || defined (GRID_3D)
  for (grid_coord j = 0; j < grid.getSize ().get2 (); ++j)
#endif
#if defined (GRID_3D)
  for (grid_coord k = 0; k < grid.getSize ().get3 (); ++k)
#endif
  {
    FieldPointValue val;

#ifdef COMPLEX_FIELD_VALUES
    val.setCurValue (FieldValue (i * PhysicsConst::Eps0, i * PhysicsConst::SpeedOfLight));
#else
    val.setCurValue (FieldValue (i * PhysicsConst::Eps0));
#endif

#ifdef GRID_1D
        GridCoordinate1D pos (i
#ifdef DEBUG_INFO
          , CoordinateType::X
#endif
        );
#endif
#ifdef GRID_2D
        GridCoordinate2D pos (i, j
#ifdef DEBUG_INFO
          , CoordinateType::X, CoordinateType::Y
#endif
        );
#endif
#ifdef GRID_3D
        GridCoordinate3D pos (i, j, k
#ifdef DEBUG_INFO
          , CoordinateType::X, CoordinateType::Y, CoordinateType::Z
#endif
        );
#endif

    grid.setFieldPointValue (val, pos);
  }

  struct timeval tv1, tv2;
  gettimeofday(&tv1, NULL);

  for (time_step t = 0; t < NUM_TIME_STEPS; ++t)
  {
    int state = 1;
#ifdef DYNAMIC_GRID
    state = ParallelGrid::getParallelCore ()->getNodeState ()[ParallelGrid::getParallelCore ()->getProcessId ()];
#endif

    if (state)
    {
#ifdef DYNAMIC_GRID
      ParallelGrid::getParallelCore ()->StartCalcClock ();
#endif

      // if (ParallelGrid::getParallelCore ()->getProcessId () == ParallelGrid::getParallelCore ()->getTotalProcCount () - 1)
      // {
      //   usleep(200000);
      // }

      //printf ("# %d ----> %u %u %u\n", ParallelGrid::getParallelCore ()->getProcessId (), grid.getSize ().get1 (), grid.getSize ().get2 (), grid.getSize ().get3 ());

#if defined (GRID_1D) || defined (GRID_2D) || defined (GRID_3D)
      for (grid_coord i = 0; i < grid.getSize ().get1 (); ++i)
#endif
#if defined (GRID_2D) || defined (GRID_3D)
      for (grid_coord j = 0; j < grid.getSize ().get2 (); ++j)
#endif
#if defined (GRID_3D)
      for (grid_coord k = 0; k < grid.getSize ().get3 (); ++k)
#endif
      {
#ifdef GRID_1D
        GridCoordinate1D pos (i
#ifdef DEBUG_INFO
          , CoordinateType::X
#endif
        );
#endif
#ifdef GRID_2D
        GridCoordinate2D pos (i, j
#ifdef DEBUG_INFO
          , CoordinateType::X, CoordinateType::Y
#endif
        );
#endif
#ifdef GRID_3D
        GridCoordinate3D pos (i, j, k
#ifdef DEBUG_INFO
          , CoordinateType::X, CoordinateType::Y, CoordinateType::Z
#endif
        );
#endif

        FieldPointValue *value = grid.getFieldPointValue (pos);

        FPValue arg = 10000 * t * 0.01 - 2 * PhysicsConst::Pi / 0.2;
#ifdef COMPLEX_FIELD_VALUES
        FieldValue val (sin (arg), cos (arg));
#else
        FieldValue val (sin (arg));
#endif

        value->setCurValue (val);
      }

#ifdef DYNAMIC_GRID
      ParallelGrid::getParallelCore ()->StopCalcClock ();
#endif
    }

#ifdef DYNAMIC_GRID
#ifdef MPI_CLOCK
    //printf ("#### %d : %f\n", ParallelGrid::getParallelCore ()->getProcessId (),
    //  ParallelGrid::getParallelCore ()->getCalcClock (ParallelGrid::getParallelCore ()->getProcessId ()).getFP ());
#else
    //printf ("#### %d : %d %d\n", ParallelGrid::getParallelCore ()->getProcessId (),
    //  ParallelGrid::getParallelCore ()->getCalcClock (ParallelGrid::getParallelCore ()->getProcessId ()).getVal ().tv_sec,
    //  ParallelGrid::getParallelCore ()->getCalcClock (ParallelGrid::getParallelCore ()->getProcessId ()).getVal ().tv_nsec);
#endif
#endif

    grid.shiftInTime (zero, grid->getSize ());
    ParallelGridGroup *group = ParallelGrid::getGroup (0)
    group->nextShareStep ();
    if (group->isShareTime ())
    {
      grid.share ();
      group->zeroShareStep ();
    }

#ifdef DYNAMIC_GRID
    if (t > 0 && t % SHARE_TIME_STEP == 0)
    {
      if (ParallelGrid::getParallelCore ()->getProcessId () == 0)
      {
        printf ("Try rebalance on step " TS_MOD ", steps elapsed after previous " TS_MOD "\n", t, SHARE_TIME_STEP);
      }

      if (yeeLayout.Rebalance (SHARE_TIME_STEP))
      {
        //printf ("Rebalancing for process %d! (size %u)\n", ParallelGrid::getParallelCore ()->getProcessId (), yeeLayout.getEpsSizeForCurNode ().get1 ());

        grid.Resize (yeeLayout.getEpsSizeForCurNode ());
      }
    }
#endif
  }

  gettimeofday(&tv2, NULL);

  MPI_Finalize();

  printf ("Total time = %f seconds\n",
       (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
       (double) (tv2.tv_sec - tv1.tv_sec));

  return 0;
} /* main */

#else /* PARALLEL_GRID */

int main (int argc, char** argv)
{
  ASSERT (0);

  return 0;
} /* main */

#endif /* !PARALLEL_GRID */
