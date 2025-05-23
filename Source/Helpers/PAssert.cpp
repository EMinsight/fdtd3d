/*
 * Copyright (C) 2015 Gleb Balykov
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

#include "PAssert.h"

#include <cstdlib>

#ifdef HAVE_EXECINFO_H
#include <execinfo.h>
#endif // HAVE_EXECINFO_H

/**
 * This function is used to exit and for debugging purposes.
 */
void program_fail ()
{
#ifdef HAVE_EXECINFO_H
  const unsigned bufsize = 256;
  int nptrs;
  void *buffer[bufsize];
  char **strings;

  nptrs = backtrace (buffer, bufsize);
  printf ("backtrace () returned %d addresses\n", nptrs);

  strings = backtrace_symbols (buffer, nptrs);

  if (strings == NULL)
  {
    printf ("backtrace_symbols error");
    exit (1);
  }

  for (int j = 0; j < nptrs; j++)
  {
    printf ("%s\n", strings[j]);
  }

  free(strings);
#endif // HAVE_EXECINFO

  exit (1);
} /* program_fail */
