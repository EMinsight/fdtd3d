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

/*
 * Unit test for basic operations with Settings, i.e. command line
 */

#include <iostream>
#include <string>

#include "Assert.h"
#include "Settings.h"

#ifndef DEBUG_INFO
#error Test requires debug info
#endif /* !DEBUG_INFO */

void check (const std::string & arg, SchemeType schemeType, int dim)
{
  Settings settings;

  std::string tmp ("tmp");

  char * argv[2];
  argv[0] = const_cast<char*> (tmp.c_str ());
  argv[1] = const_cast<char*> (arg.c_str ());

  settings.SetupFromCmd (2, argv);

  ASSERT (settings.getDimension () == dim);
  ASSERT (settings.getSchemeType () == schemeType);
}

int main (int argc, char** argv)
{
  check (std::string ("--1d-exhy"), SchemeType::Dim1_ExHy, 1);
  check (std::string ("--1d-exhz"), SchemeType::Dim1_ExHz, 1);
  check (std::string ("--1d-eyhx"), SchemeType::Dim1_EyHx, 1);
  check (std::string ("--1d-eyhz"), SchemeType::Dim1_EyHz, 1);
  check (std::string ("--1d-ezhx"), SchemeType::Dim1_EzHx, 1);
  check (std::string ("--1d-ezhy"), SchemeType::Dim1_EzHy, 1);

  check (std::string ("--2d-tex"), SchemeType::Dim2_TEx, 2);
  check (std::string ("--2d-tey"), SchemeType::Dim2_TEy, 2);
  check (std::string ("--2d-tez"), SchemeType::Dim2_TEz, 2);
  check (std::string ("--2d-tmx"), SchemeType::Dim2_TMx, 2);
  check (std::string ("--2d-tmy"), SchemeType::Dim2_TMy, 2);
  check (std::string ("--2d-tmz"), SchemeType::Dim2_TMz, 2);

  check (std::string ("--3d"), SchemeType::Dim3, 3);

  return 0;
} /* main */
