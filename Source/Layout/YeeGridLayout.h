/*
 * Copyright (C) 2016 Gleb Balykov
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

#ifndef YEE_GRID_LAYOUT_H
#define YEE_GRID_LAYOUT_H

#include "Approximation.h"
#include "Assert.h"
#include "GridLayout.h"
#include "PhysicsConst.h"

#include <cmath>

/**
 * Yee grid layout which specifies how field components are placed in space
 *
 * TODO: add link to docs with description of yee layout
 *
 * TODO: inline getter/setter
 */
template <SchemeType_t Type, template <typename, bool> class TCoord, LayoutType layout_type>
class YeeGridLayout
{
public:

  static const bool isParallel;

private:

  typedef TCoord<grid_coord, true> TC;
  typedef TCoord<grid_coord, false> TCS;
  typedef TCoord<FPValue, true> TCFP;
  typedef TCoord<FPValue, false> TCSFP;

protected:

  CoordinateType ct1;
  CoordinateType ct2;
  CoordinateType ct3;

  const TCS circuitExDownDiff;
  const TCS circuitExUpDiff;
  const TCS circuitExBackDiff;
  const TCS circuitExFrontDiff;

  const TCS circuitEyLeftDiff;
  const TCS circuitEyRightDiff;
  const TCS circuitEyBackDiff;
  const TCS circuitEyFrontDiff;

  const TCS circuitEzLeftDiff;
  const TCS circuitEzRightDiff;
  const TCS circuitEzDownDiff;
  const TCS circuitEzUpDiff;

  const TCS circuitHxDownDiff;
  const TCS circuitHxUpDiff;
  const TCS circuitHxBackDiff;
  const TCS circuitHxFrontDiff;

  const TCS circuitHyLeftDiff;
  const TCS circuitHyRightDiff;
  const TCS circuitHyBackDiff;
  const TCS circuitHyFrontDiff;

  const TCS circuitHzLeftDiff;
  const TCS circuitHzRightDiff;
  const TCS circuitHzDownDiff;
  const TCS circuitHzUpDiff;

  const TCFP zeroCoordFP; /**< Real zero coordinate of grid */
  const TCFP minEpsCoordFP; /**< Minimum real epsilon coordinate */
  const TCFP minMuCoordFP; /**< Minimum real mu coordinate */
  const TCFP minExCoordFP; /**< Minimum real Ex field coordinate */
  const TCFP minEyCoordFP; /**< Minimum real Ey field coordinate */
  const TCFP minEzCoordFP; /**< Minimum real Ez field coordinate */
  const TCFP minHxCoordFP; /**< Minimum real Hx field coordinate */
  const TCFP minHyCoordFP; /**< Minimum real Hy field coordinate */
  const TCFP minHzCoordFP; /**< Minimum real Hz field coordinate */

  const TC size; /**< Size of grids */
  const TC sizeEps; /**< Size of epsilon grid */
  const TC sizeMu; /**< Size of mu grid */
  const TC sizeEx; /**< size of Ex field */
  const TC sizeEy; /**< size of Ey field */
  const TC sizeEz; /**< size of Ez field */
  const TC sizeHx; /**< size of Hx field */
  const TC sizeHy; /**< size of Hy field */
  const TC sizeHz; /**< size of Hz field */

  const TC leftBorderPML; /**< Coordinate of left border of PML */
  const TC rightBorderPML; /**< Coordinate of right border of PML */

  const TC leftBorderTotalField; /**< Coordinate of left border of TF/SF */
  const TC rightBorderTotalField; /**< Coordinate of right border of TF/SF */
  const TCFP leftBorderTotalFieldFP; /**< Floating-point coordinate of left border of TF/SF */
  const TCFP rightBorderTotalFieldFP; /**< Floating-point coordinate of right border of TF/SF */

  const TCFP zeroIncCoordFP; /**< Real coordinate corresponding to zero coordinate of auxiliary grid
                              *   for incident wave */

  const FPValue incidentWaveAngle1; /**< Teta incident wave angle */
  const FPValue incidentWaveAngle2; /**< Phi incident wave angle */
  const FPValue incidentWaveAngle3; /**< Psi incident wave angle */

  const bool isDoubleMaterialPrecision; /**< Flag whether to use double material precision grids */

public:

  /*
   * Get coordinate of circut field component
   */
  CUDA_DEVICE CUDA_HOST TCS getExCircuitElementDiff (LayoutDirection) const;
  CUDA_DEVICE CUDA_HOST TCS getEyCircuitElementDiff (LayoutDirection) const;
  CUDA_DEVICE CUDA_HOST TCS getEzCircuitElementDiff (LayoutDirection) const;
  CUDA_DEVICE CUDA_HOST TCS getHxCircuitElementDiff (LayoutDirection) const;
  CUDA_DEVICE CUDA_HOST TCS getHyCircuitElementDiff (LayoutDirection) const;
  CUDA_DEVICE CUDA_HOST TCS getHzCircuitElementDiff (LayoutDirection) const;

  /*
   * Get size of field component grid
   */
  CUDA_DEVICE CUDA_HOST TC getSize () const
  {
    return size;
  }
  CUDA_DEVICE CUDA_HOST TC getEpsSize () const
  {
    return sizeEps;
  }
  CUDA_DEVICE CUDA_HOST TC getMuSize () const
  {
    return sizeEps;
  }
  CUDA_DEVICE CUDA_HOST TC getSigmaXSize () const
  {
    return sizeEps;
  }
  CUDA_DEVICE CUDA_HOST TC getSigmaYSize () const
  {
    return sizeEps;
  }
  CUDA_DEVICE CUDA_HOST TC getSigmaZSize () const
  {
    return sizeEps;
  }
  CUDA_DEVICE CUDA_HOST TC getExSize () const
  {
    return sizeEx;
  }
  CUDA_DEVICE CUDA_HOST TC getEySize () const
  {
    return sizeEy;
  }
  CUDA_DEVICE CUDA_HOST TC getEzSize () const
  {
    return sizeEz;
  }
  CUDA_DEVICE CUDA_HOST TC getHxSize () const
  {
    return sizeHx;
  }
  CUDA_DEVICE CUDA_HOST TC getHySize () const
  {
    return sizeHy;
  }
  CUDA_DEVICE CUDA_HOST TC getHzSize () const
  {
    return sizeHz;
  }

  CUDA_DEVICE CUDA_HOST TC getSizePML () const
  {
    return leftBorderPML;
  }

  /*
   * Get start coordinate of field component
   */
  CUDA_DEVICE CUDA_HOST TC getExStartDiff () const
  {
    return TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);
  }
  CUDA_DEVICE CUDA_HOST TC getEyStartDiff () const
  {
    return TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);
  }
  CUDA_DEVICE CUDA_HOST TC getEzStartDiff () const
  {
    return TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);
  }
  CUDA_DEVICE CUDA_HOST TC getHxStartDiff () const
  {
    return TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);
  }
  CUDA_DEVICE CUDA_HOST TC getHyStartDiff () const
  {
    return TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);
  }
  CUDA_DEVICE CUDA_HOST TC getHzStartDiff () const
  {
    return TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);
  }

  /*
   * Get end coordinate of field component
   */
  CUDA_DEVICE CUDA_HOST TC getExEndDiff () const
  {
    return TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);
  }
  CUDA_DEVICE CUDA_HOST TC getEyEndDiff () const
  {
    return TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);
  }
  CUDA_DEVICE CUDA_HOST TC getEzEndDiff () const
  {
    return TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);
  }
  CUDA_DEVICE CUDA_HOST TC getHxEndDiff () const
  {
    return TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);
  }
  CUDA_DEVICE CUDA_HOST TC getHyEndDiff () const
  {
    return TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);
  }
  CUDA_DEVICE CUDA_HOST TC getHzEndDiff () const
  {
    return TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);
  }

  /*
   * Get minimum real coordinate of field component
   */
  CUDA_DEVICE CUDA_HOST TCFP getZeroCoordFP () const
  {
    return zeroCoordFP;
  }
  CUDA_DEVICE CUDA_HOST TCFP getMinEpsCoordFP () const
  {
    return minEpsCoordFP;
  }
  CUDA_DEVICE CUDA_HOST TCFP getMinMuCoordFP () const
  {
    return minMuCoordFP;
  }
  CUDA_DEVICE CUDA_HOST TCFP getMinExCoordFP () const
  {
    return minExCoordFP;
  }
  CUDA_DEVICE CUDA_HOST TCFP getMinEyCoordFP () const
  {
    return minEyCoordFP;
  }
  CUDA_DEVICE CUDA_HOST TCFP getMinEzCoordFP () const
  {
    return minEzCoordFP;
  }
  CUDA_DEVICE CUDA_HOST TCFP getMinHxCoordFP () const
  {
    return minHxCoordFP;
  }
  CUDA_DEVICE CUDA_HOST TCFP getMinHyCoordFP () const
  {
    return minHyCoordFP;
  }
  CUDA_DEVICE CUDA_HOST TCFP getMinHzCoordFP () const
  {
    return minHzCoordFP;
  }

  /*
   * Get real coordinate of field component by its coordinate
   */
  CUDA_DEVICE CUDA_HOST TCFP getEpsCoordFP (TC coord) const
  {
    return convertCoord (coord) + minEpsCoordFP;
  }
  CUDA_DEVICE CUDA_HOST TCFP getMuCoordFP (TC coord) const
  {
    return convertCoord (coord) + minMuCoordFP;
  }
  CUDA_DEVICE CUDA_HOST TCFP getExCoordFP (TC coord) const
  {
    return convertCoord (coord) + minExCoordFP;
  }
  CUDA_DEVICE CUDA_HOST TCFP getEyCoordFP (TC coord) const
  {
    return convertCoord (coord) + minEyCoordFP;
  }
  CUDA_DEVICE CUDA_HOST TCFP getEzCoordFP (TC coord) const
  {
    return convertCoord (coord) + minEzCoordFP;
  }
  CUDA_DEVICE CUDA_HOST TCFP getHxCoordFP (TC coord) const
  {
    return convertCoord (coord) + minHxCoordFP;
  }
  CUDA_DEVICE CUDA_HOST TCFP getHyCoordFP (TC coord) const
  {
    return convertCoord (coord) + minHyCoordFP;
  }
  CUDA_DEVICE CUDA_HOST TCFP getHzCoordFP (TC coord) const
  {
    return convertCoord (coord) + minHzCoordFP;
  }

  /*
   * Get coordinate of field component by its real coordinate
   */
  CUDA_DEVICE CUDA_HOST TC getEpsCoord (TCFP coord) const
  {
    return convertCoord (coord - minEpsCoordFP);
  }
  CUDA_DEVICE CUDA_HOST TC getMuCoord (TCFP coord) const
  {
    return convertCoord (coord - minMuCoordFP);
  }
  CUDA_DEVICE CUDA_HOST TC getExCoord (TCFP coord) const
  {
    return convertCoord (coord - minExCoordFP);
  }
  CUDA_DEVICE CUDA_HOST TC getEyCoord (TCFP coord) const
  {
    return convertCoord (coord - minEyCoordFP);
  }
  CUDA_DEVICE CUDA_HOST TC getEzCoord (TCFP coord) const
  {
    return convertCoord (coord - minEzCoordFP);
  }
  CUDA_DEVICE CUDA_HOST TC getHxCoord (TCFP coord) const
  {
    return convertCoord (coord - minHxCoordFP);
  }
  CUDA_DEVICE CUDA_HOST TC getHyCoord (TCFP coord) const
  {
    return convertCoord (coord - minHyCoordFP);
  }
  CUDA_DEVICE CUDA_HOST TC getHzCoord (TCFP coord) const
  {
    return convertCoord (coord - minHzCoordFP);
  }

  /*
   * PML
   */
  CUDA_DEVICE CUDA_HOST TC getLeftBorderPML () const
  {
    return leftBorderPML;
  }
  CUDA_DEVICE CUDA_HOST TC getRightBorderPML () const
  {
    return rightBorderPML;
  }

  CUDA_DEVICE CUDA_HOST bool isExInPML (TC coord) const
  {
    return isInPML (getExCoordFP (coord));
  }
  CUDA_DEVICE CUDA_HOST bool isEyInPML (TC coord) const
  {
    return isInPML (getEyCoordFP (coord));
  }
  CUDA_DEVICE CUDA_HOST bool isEzInPML (TC coord) const
  {
    return isInPML (getEzCoordFP (coord));
  }
  CUDA_DEVICE CUDA_HOST bool isHxInPML (TC coord) const
  {
    return isInPML (getHxCoordFP (coord));
  }
  CUDA_DEVICE CUDA_HOST bool isHyInPML (TC coord) const
  {
    return isInPML (getHyCoordFP (coord));
  }
  CUDA_DEVICE CUDA_HOST bool isHzInPML (TC coord) const
  {
    return isInPML (getHzCoordFP (coord));
  }

  /*
   * Total field / scattered field
   */
  CUDA_DEVICE CUDA_HOST TC getLeftBorderTFSF () const
  {
    return leftBorderTotalField;
  }
  CUDA_DEVICE CUDA_HOST TC getRightBorderTFSF () const
  {
    return rightBorderTotalField;
  }
  CUDA_DEVICE CUDA_HOST TCFP getZeroIncCoordFP () const
  {
    return zeroIncCoordFP;
  }
  CUDA_DEVICE CUDA_HOST bool doNeedTFSFUpdateExBorder (TC, LayoutDirection) const;
  CUDA_DEVICE CUDA_HOST bool doNeedTFSFUpdateEyBorder (TC, LayoutDirection) const;
  CUDA_DEVICE CUDA_HOST bool doNeedTFSFUpdateEzBorder (TC, LayoutDirection) const;
  CUDA_DEVICE CUDA_HOST bool doNeedTFSFUpdateHxBorder (TC, LayoutDirection) const;
  CUDA_DEVICE CUDA_HOST bool doNeedTFSFUpdateHyBorder (TC, LayoutDirection) const;
  CUDA_DEVICE CUDA_HOST bool doNeedTFSFUpdateHzBorder (TC, LayoutDirection) const;

  CUDA_DEVICE CUDA_HOST FPValue getIncidentWaveAngle1 () const
  {
    return incidentWaveAngle1;
  }
  CUDA_DEVICE CUDA_HOST FPValue getIncidentWaveAngle2 () const
  {
    return incidentWaveAngle2;
  }
  CUDA_DEVICE CUDA_HOST FPValue getIncidentWaveAngle3 () const
  {
    return incidentWaveAngle3;
  }

  CUDA_DEVICE CUDA_HOST bool getIsDoubleMaterialPrecision () const
  {
    return isDoubleMaterialPrecision;
  }

  CUDA_DEVICE CUDA_HOST FieldValue getExFromIncidentE (FieldValue valE) const;
  CUDA_DEVICE CUDA_HOST FieldValue getEyFromIncidentE (FieldValue valE) const;
  CUDA_DEVICE CUDA_HOST FieldValue getEzFromIncidentE (FieldValue valE) const;
  CUDA_DEVICE CUDA_HOST FieldValue getHxFromIncidentH (FieldValue valH) const;
  CUDA_DEVICE CUDA_HOST FieldValue getHyFromIncidentH (FieldValue valH) const;
  CUDA_DEVICE CUDA_HOST FieldValue getHzFromIncidentH (FieldValue valH) const;

  /**
   * Constructor of Yee grid
   */
  CUDA_DEVICE CUDA_HOST YeeGridLayout (TC coordSize,
                 TC sizePML,
                 TC sizeScatteredZoneLeft,
                 TC sizeScatteredZoneRight,
                 FPValue incWaveAngle1, /**< teta */
                 FPValue incWaveAngle2, /**< phi */
                 FPValue incWaveAngle3, /**< psi */
                 bool doubleMaterialPrecision);

  CUDA_DEVICE CUDA_HOST ~YeeGridLayout ()
  {
  } /* ~YeeGridLayout */

  CUDA_DEVICE CUDA_HOST FieldValue getApproximateMaterial (const FieldValue &, const FieldValue &);
  CUDA_DEVICE CUDA_HOST FieldValue getApproximateMaterial (const FieldValue &, const FieldValue &, const FieldValue &, const FieldValue &);
  CUDA_DEVICE CUDA_HOST FieldValue getApproximateMaterial (const FieldValue &, const FieldValue &, const FieldValue &, const FieldValue &, const FieldValue &, const FieldValue &, const FieldValue &, const FieldValue &);
  CUDA_DEVICE CUDA_HOST FieldValue getApproximateMetaMaterial (const FieldValue &, const FieldValue &, const FieldValue &,
                                                               const FieldValue &, const FieldValue &, const FieldValue &,
                                                               FPValue &, FPValue &);
  CUDA_DEVICE CUDA_HOST FieldValue getApproximateMetaMaterial (const FieldValue &, const FieldValue &, const FieldValue &,
                                                               const FieldValue &, const FieldValue &, const FieldValue &,
                                                               const FieldValue &, const FieldValue &, const FieldValue &,
                                                               const FieldValue &, const FieldValue &, const FieldValue &,
                                                               FPValue &, FPValue &);
  CUDA_DEVICE CUDA_HOST FieldValue getApproximateMetaMaterial (const FieldValue &, const FieldValue &, const FieldValue &,
                                                               const FieldValue &, const FieldValue &, const FieldValue &,
                                                               const FieldValue &, const FieldValue &, const FieldValue &,
                                                               const FieldValue &, const FieldValue &, const FieldValue &,
                                                               const FieldValue &, const FieldValue &, const FieldValue &,
                                                               const FieldValue &, const FieldValue &, const FieldValue &,
                                                               const FieldValue &, const FieldValue &, const FieldValue &,
                                                               const FieldValue &, const FieldValue &, const FieldValue &,
                                                               FPValue &, FPValue &);

  template <bool isMetaMaterial>
  CUDA_DEVICE CUDA_HOST void initMaterialCoordinates (const TC &, GridType, TC &, TC &, TC &, TC &, TC &, TC &, TC &, TC &);

private:

  CUDA_DEVICE CUDA_HOST bool isInPML (TCFP realCoordFP) const;
}; /* YeeGridLayout */

class YeeGridLayoutHelper
{
  template <SchemeType_t Type, template <typename, bool> class TCoord, LayoutType layout_type>
  friend class YeeGridLayout;

private:

  template<int8_t Offset>
  CUDA_DEVICE CUDA_HOST static bool tfsfBorder (FPValue coordFP, FPValue borderFP)
  {
    ASSERT (Offset > -10 && Offset < 10);
    return IS_FP_EXACT (coordFP, borderFP + Offset * FPValue (1) / FPValue (10));
  }

  template<int8_t Offset1, int8_t Offset2>
  CUDA_DEVICE CUDA_HOST static bool tfsfBorder (FPValue coordFP, FPValue firstBorderFP, FPValue secondBorderFP)
  {
    ASSERT (Offset1 > -10 && Offset1 < 10);
    ASSERT (Offset2 > -10 && Offset2 < 10);
    return coordFP > firstBorderFP + Offset1 * FPValue (1) / FPValue (10)
           && coordFP < secondBorderFP + Offset2 * FPValue (1) / FPValue (10);
  }

  template<int8_t T>
  CUDA_DEVICE CUDA_HOST static bool tfsfBorder1DFirst__1 (GridCoordinateFP1D coord, GridCoordinateFP1D leftBorderTotalFieldFP)
  {
    return YeeGridLayoutHelper::tfsfBorder<T> (coord.get1 (), leftBorderTotalFieldFP.get1 ());
  }
  template<int8_t T>
  CUDA_DEVICE CUDA_HOST static bool tfsfBorder1DSecond__1 (GridCoordinateFP1D coord, GridCoordinateFP1D rightBorderTotalFieldFP)
  {
    return YeeGridLayoutHelper::tfsfBorder<T> (coord.get1 (), rightBorderTotalFieldFP.get1 ());
  }
  template<int8_t T1, int8_t T2>
  CUDA_DEVICE CUDA_HOST static bool tfsfBorder2DFirst__1 (GridCoordinateFP2D coord, GridCoordinateFP2D leftBorderTotalFieldFP, GridCoordinateFP2D rightBorderTotalFieldFP)
  {
    return YeeGridLayoutHelper::tfsfBorder<T1, -T1> (coord.get1 (), leftBorderTotalFieldFP.get1 (), rightBorderTotalFieldFP.get1 ())
           && YeeGridLayoutHelper::tfsfBorder<T2> (coord.get2 (), leftBorderTotalFieldFP.get2 ());
  }
  template<int8_t T1, int8_t T2>
  CUDA_DEVICE CUDA_HOST static bool tfsfBorder2DSecond__1 (GridCoordinateFP2D coord, GridCoordinateFP2D leftBorderTotalFieldFP, GridCoordinateFP2D rightBorderTotalFieldFP)
  {
    return YeeGridLayoutHelper::tfsfBorder<T1, -T1> (coord.get1 (), leftBorderTotalFieldFP.get1 (), rightBorderTotalFieldFP.get1 ())
           && YeeGridLayoutHelper::tfsfBorder<T2> (coord.get2 (), rightBorderTotalFieldFP.get2 ());
  }
  template<int8_t T1, int8_t T2>
  CUDA_DEVICE CUDA_HOST static bool tfsfBorder2DFirst__2 (GridCoordinateFP2D coord, GridCoordinateFP2D leftBorderTotalFieldFP, GridCoordinateFP2D rightBorderTotalFieldFP)
  {
    return YeeGridLayoutHelper::tfsfBorder<T2> (coord.get1 (), leftBorderTotalFieldFP.get1 ())
           && YeeGridLayoutHelper::tfsfBorder<T1, -T1> (coord.get2 (), leftBorderTotalFieldFP.get2 (), rightBorderTotalFieldFP.get2 ());
  }
  template<int8_t T1, int8_t T2>
  CUDA_DEVICE CUDA_HOST static bool tfsfBorder2DSecond__2 (GridCoordinateFP2D coord, GridCoordinateFP2D leftBorderTotalFieldFP, GridCoordinateFP2D rightBorderTotalFieldFP)
  {
    return YeeGridLayoutHelper::tfsfBorder<T2> (coord.get1 (), rightBorderTotalFieldFP.get1 ())
           && YeeGridLayoutHelper::tfsfBorder<T1, -T1> (coord.get2 (), leftBorderTotalFieldFP.get2 (), rightBorderTotalFieldFP.get2 ());
  }

  CUDA_DEVICE CUDA_HOST static bool doNeedTFSFUpdateBorder (LayoutDirection dir,
                                      bool condL,
                                      bool condR,
                                      bool condD,
                                      bool condU,
                                      bool condB,
                                      bool condF)
  {
    bool match = false;

    switch (dir)
    {
      case LayoutDirection::LEFT:
      {
        match = condL;
        break;
      }
      case LayoutDirection::RIGHT:
      {
        match = condR;
        break;
      }
      case LayoutDirection::DOWN:
      {
        match = condD;
        break;
      }
      case LayoutDirection::UP:
      {
        match = condU;
        break;
      }
      case LayoutDirection::BACK:
      {
        match = condB;
        break;
      }
      case LayoutDirection::FRONT:
      {
        match = condF;
        break;
      }
      default:
      {
        UNREACHABLE;
      }
    }

    return match;
  }

  CUDA_DEVICE CUDA_HOST static bool isInPML1D (GridCoordinateFP1D realCoordFP, GridCoordinateFP1D zeroCoordFP,
                         GridCoordinate1D leftBorderPML, GridCoordinate1D rightBorderPML)
  {
    GridCoordinateFP1D coordLeftBorderPMLFP = convertCoord (leftBorderPML) + zeroCoordFP;
    GridCoordinateFP1D coordRightBorderPMLFP = convertCoord (rightBorderPML) + zeroCoordFP;

    /*
     * TODO: remove floating point equality comparison
     */
    ASSERT (coordLeftBorderPMLFP < coordRightBorderPMLFP);
    bool isInXPML = realCoordFP.get1 () < coordLeftBorderPMLFP.get1 ()
                    || realCoordFP.get1 () >= coordRightBorderPMLFP.get1 ();

    return isInXPML;
  }

  CUDA_DEVICE CUDA_HOST static bool isInPML2D (GridCoordinateFP2D realCoordFP, GridCoordinateFP2D zeroCoordFP,
                         GridCoordinate2D leftBorderPML, GridCoordinate2D rightBorderPML)
  {
    GridCoordinateFP2D coordLeftBorderPMLFP = convertCoord (leftBorderPML) + zeroCoordFP;
    GridCoordinateFP2D coordRightBorderPMLFP = convertCoord (rightBorderPML) + zeroCoordFP;

    /*
     * TODO: remove floating point equality comparison
     */
    ASSERT (coordLeftBorderPMLFP < coordRightBorderPMLFP);
    bool isInXPML = realCoordFP.get1 () < coordLeftBorderPMLFP.get1 ()
                    || realCoordFP.get1 () >= coordRightBorderPMLFP.get1 ();
    bool isInYPML = realCoordFP.get2 () < coordLeftBorderPMLFP.get2 ()
                    || realCoordFP.get2 () >= coordRightBorderPMLFP.get2 ();

    return isInXPML || isInYPML;
  }

  CUDA_DEVICE CUDA_HOST static bool isInPML3D (GridCoordinateFP3D realCoordFP, GridCoordinateFP3D zeroCoordFP,
                         GridCoordinate3D leftBorderPML, GridCoordinate3D rightBorderPML)
  {
    GridCoordinateFP3D coordLeftBorderPMLFP = convertCoord (leftBorderPML) + zeroCoordFP;
    GridCoordinateFP3D coordRightBorderPMLFP = convertCoord (rightBorderPML) + zeroCoordFP;

    /*
     * TODO: remove floating point equality comparison
     */
    ASSERT (coordLeftBorderPMLFP < coordRightBorderPMLFP);
    bool isInXPML = realCoordFP.get1 () < coordLeftBorderPMLFP.get1 ()
                    || realCoordFP.get1 () >= coordRightBorderPMLFP.get1 ();
    bool isInYPML = realCoordFP.get2 () < coordLeftBorderPMLFP.get2 ()
                    || realCoordFP.get2 () >= coordRightBorderPMLFP.get2 ();
    bool isInZPML = realCoordFP.get3 () < coordLeftBorderPMLFP.get3 ()
                    || realCoordFP.get3 () >= coordRightBorderPMLFP.get3 ();

    return isInXPML || isInYPML || isInZPML;
  }
};

template <SchemeType_t Type, template <typename, bool> class TCoord, LayoutType layout_type>
CUDA_DEVICE CUDA_HOST FieldValue
YeeGridLayout<Type, TCoord, layout_type>::getApproximateMaterial (const FieldValue & val1, const FieldValue & val2)
{
  return FIELDVALUE (Approximation::approximateMaterial (Approximation::getMaterial (val1),
                                                         Approximation::getMaterial (val2)), 0);
}

template <SchemeType_t Type, template <typename, bool> class TCoord, LayoutType layout_type>
CUDA_DEVICE CUDA_HOST FieldValue
YeeGridLayout<Type, TCoord, layout_type>::getApproximateMaterial (const FieldValue & val1, const FieldValue & val2, const FieldValue & val3, const FieldValue & val4)
{
  return FIELDVALUE (Approximation::approximateMaterial (Approximation::getMaterial (val1),
                                                         Approximation::getMaterial (val2),
                                                         Approximation::getMaterial (val3),
                                                         Approximation::getMaterial (val4)), 0);
}

template <SchemeType_t Type, template <typename, bool> class TCoord, LayoutType layout_type>
CUDA_DEVICE CUDA_HOST FieldValue
YeeGridLayout<Type, TCoord, layout_type>::getApproximateMaterial (const FieldValue & val1, const FieldValue & val2, const FieldValue & val3, const FieldValue & val4,
                                                                  const FieldValue & val5, const FieldValue & val6, const FieldValue & val7, const FieldValue & val8)
{
  return FIELDVALUE (Approximation::approximateMaterial (Approximation::getMaterial (val1),
                                                         Approximation::getMaterial (val2),
                                                         Approximation::getMaterial (val3),
                                                         Approximation::getMaterial (val4),
                                                         Approximation::getMaterial (val5),
                                                         Approximation::getMaterial (val6),
                                                         Approximation::getMaterial (val7),
                                                         Approximation::getMaterial (val8)), 0);
}

template <SchemeType_t Type, template <typename, bool> class TCoord, LayoutType layout_type>
CUDA_DEVICE CUDA_HOST FieldValue
YeeGridLayout<Type, TCoord, layout_type>::getApproximateMetaMaterial (const FieldValue & val1, const FieldValue & val2,
                                           const FieldValue & val3, const FieldValue & val4,
                                           const FieldValue & val5, const FieldValue & val6,
                                           FPValue &omega,
                                           FPValue &gamma)
{
  FPValue material = Approximation::approximateMaterial (Approximation::getMaterial (val1),
                                                         Approximation::getMaterial (val2));

  Approximation::approximateDrudeModel (omega,
                                        gamma,
                                        Approximation::getMaterial (val1),
                                        Approximation::getMaterial (val2),
                                        Approximation::getMaterial (val3),
                                        Approximation::getMaterial (val4),
                                        Approximation::getMaterial (val5),
                                        Approximation::getMaterial (val6));

  return FIELDVALUE (material, 0);
}

template <SchemeType_t Type, template <typename, bool> class TCoord, LayoutType layout_type>
CUDA_DEVICE CUDA_HOST FieldValue
YeeGridLayout<Type, TCoord, layout_type>::getApproximateMetaMaterial (const FieldValue & val1, const FieldValue & val2,
                                                                      const FieldValue & val3, const FieldValue & val4,
                                                                      const FieldValue & val5, const FieldValue & val6,
                                                                      const FieldValue & val7, const FieldValue & val8,
                                                                      const FieldValue & val9, const FieldValue & val10,
                                                                      const FieldValue & val11, const FieldValue & val12,
                                                                      FPValue &omega,
                                                                      FPValue &gamma)
{
  FPValue material = Approximation::approximateMaterial (Approximation::getMaterial (val1),
                                                         Approximation::getMaterial (val2),
                                                         Approximation::getMaterial (val3),
                                                         Approximation::getMaterial (val4));

  Approximation::approximateDrudeModel (omega,
                                        gamma,
                                        Approximation::getMaterial (val1),
                                        Approximation::getMaterial (val2),
                                        Approximation::getMaterial (val3),
                                        Approximation::getMaterial (val4),
                                        Approximation::getMaterial (val5),
                                        Approximation::getMaterial (val6),
                                        Approximation::getMaterial (val7),
                                        Approximation::getMaterial (val8),
                                        Approximation::getMaterial (val9),
                                        Approximation::getMaterial (val10),
                                        Approximation::getMaterial (val11),
                                        Approximation::getMaterial (val12));

  return FIELDVALUE (material, 0);
}

template <SchemeType_t Type, template <typename, bool> class TCoord, LayoutType layout_type>
CUDA_DEVICE CUDA_HOST FieldValue
YeeGridLayout<Type, TCoord, layout_type>::getApproximateMetaMaterial (const FieldValue & val1, const FieldValue & val2,
                                                                      const FieldValue & val3, const FieldValue & val4,
                                                                      const FieldValue & val5, const FieldValue & val6,
                                                                      const FieldValue & val7, const FieldValue & val8,
                                                                      const FieldValue & val9, const FieldValue & val10,
                                                                      const FieldValue & val11, const FieldValue & val12,
                                                                      const FieldValue & val13, const FieldValue & val14,
                                                                      const FieldValue & val15, const FieldValue & val16,
                                                                      const FieldValue & val17, const FieldValue & val18,
                                                                      const FieldValue & val19, const FieldValue & val20,
                                                                      const FieldValue & val21, const FieldValue & val22,
                                                                      const FieldValue & val23, const FieldValue & val24,
                                                                      FPValue &omega,
                                                                      FPValue &gamma)
{
  FPValue material = Approximation::approximateMaterial (Approximation::getMaterial (val1),
                                                         Approximation::getMaterial (val2),
                                                         Approximation::getMaterial (val3),
                                                         Approximation::getMaterial (val4),
                                                         Approximation::getMaterial (val5),
                                                         Approximation::getMaterial (val6),
                                                         Approximation::getMaterial (val7),
                                                         Approximation::getMaterial (val8));

  Approximation::approximateDrudeModel (omega,
                                        gamma,
                                        Approximation::getMaterial (val1),
                                        Approximation::getMaterial (val2),
                                        Approximation::getMaterial (val3),
                                        Approximation::getMaterial (val4),
                                        Approximation::getMaterial (val5),
                                        Approximation::getMaterial (val6),
                                        Approximation::getMaterial (val7),
                                        Approximation::getMaterial (val8),
                                        Approximation::getMaterial (val9),
                                        Approximation::getMaterial (val10),
                                        Approximation::getMaterial (val11),
                                        Approximation::getMaterial (val12),
                                        Approximation::getMaterial (val13),
                                        Approximation::getMaterial (val14),
                                        Approximation::getMaterial (val15),
                                        Approximation::getMaterial (val16),
                                        Approximation::getMaterial (val17),
                                        Approximation::getMaterial (val18),
                                        Approximation::getMaterial (val19),
                                        Approximation::getMaterial (val20),
                                        Approximation::getMaterial (val21),
                                        Approximation::getMaterial (val22),
                                        Approximation::getMaterial (val23),
                                        Approximation::getMaterial (val24));

  return FIELDVALUE (material, 0);
}

/*
 * TODO: add correct approximation for metamaterials
 *
 * TODO: add separate for all fields
 */
template <SchemeType_t Type, template <typename, bool> class TCoord, LayoutType layout_type>
template <bool isMetaMaterial>
CUDA_DEVICE CUDA_HOST void
YeeGridLayout<Type, TCoord, layout_type>::initMaterialCoordinates (const TC &posAbs,
                                     GridType typeOfField,
                                     TC &absPos11,
                                     TC &absPos12,
                                     TC &absPos21,
                                     TC &absPos22,
                                     TC &absPos31,
                                     TC &absPos32,
                                     TC &absPos41,
                                     TC &absPos42)
{
  TCFP realCoord;

  switch (typeOfField)
  {
    case GridType::EX:
    case GridType::DX:
    {
      realCoord = getExCoordFP (posAbs);
      break;
    }
    case GridType::EY:
    case GridType::DY:
    {
      realCoord = getEyCoordFP (posAbs);
      break;
    }
    case GridType::EZ:
    case GridType::DZ:
    {
      realCoord = getEzCoordFP (posAbs);
      break;
    }
    case GridType::HX:
    case GridType::BX:
    {
      realCoord = getHxCoordFP (posAbs);
      break;
    }
    case GridType::HY:
    case GridType::BY:
    {
      realCoord = getHyCoordFP (posAbs);
      break;
    }
    case GridType::HZ:
    case GridType::BZ:
    {
      realCoord = getHzCoordFP (posAbs);
      break;
    }
    default:
    {
      UNREACHABLE;
    }
  }

  if ((typeOfField == GridType::EX || typeOfField == GridType::DX) && layout_type == E_CENTERED
      || (typeOfField == GridType::HX || typeOfField == GridType::BX) && layout_type == H_CENTERED)
  {
    if (isDoubleMaterialPrecision)
    {
      absPos11 = getEpsCoord (realCoord - TCSFP::initAxesCoordinate (0.5, 0, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (1, 0, 0, ct1, ct2, ct3);
      absPos12 = getEpsCoord (realCoord - TCSFP::initAxesCoordinate (0.5, 0, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (1, 1, 0, ct1, ct2, ct3);

      absPos21 = getEpsCoord (realCoord + TCSFP::initAxesCoordinate (0.5, 0, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (0, 0, 0, ct1, ct2, ct3);
      absPos22 = getEpsCoord (realCoord + TCSFP::initAxesCoordinate (0.5, 0, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (0, 1, 0, ct1, ct2, ct3);

      absPos31 = getEpsCoord (realCoord - TCSFP::initAxesCoordinate (0.5, 0, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (1, 0, 1, ct1, ct2, ct3);
      absPos32 = getEpsCoord (realCoord - TCSFP::initAxesCoordinate (0.5, 0, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);

      absPos41 = getEpsCoord (realCoord + TCSFP::initAxesCoordinate (0.5, 0, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (0, 0, 1, ct1, ct2, ct3);
      absPos42 = getEpsCoord (realCoord + TCSFP::initAxesCoordinate (0.5, 0, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (0, 1, 1, ct1, ct2, ct3);
    }
    else
    {
      absPos11 = getEpsCoord (realCoord - TCSFP::initAxesCoordinate (0.5, 0, 0, ct1, ct2, ct3));
      absPos12 = getEpsCoord (realCoord + TCSFP::initAxesCoordinate (0.5, 0, 0, ct1, ct2, ct3));
    }
  }
  else if ((typeOfField == GridType::EY || typeOfField == GridType::DY) && layout_type == E_CENTERED
           || (typeOfField == GridType::HY || typeOfField == GridType::BY) && layout_type == H_CENTERED)
  {
    if (isDoubleMaterialPrecision)
    {
      absPos11 = getEpsCoord (realCoord - TCSFP::initAxesCoordinate (0, 0.5, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (0, 1, 0, ct1, ct2, ct3);
      absPos12 = getEpsCoord (realCoord - TCSFP::initAxesCoordinate (0, 0.5, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (1, 1, 0, ct1, ct2, ct3);

      absPos21 = getEpsCoord (realCoord + TCSFP::initAxesCoordinate (0, 0.5, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (0, 0, 0, ct1, ct2, ct3);
      absPos22 = getEpsCoord (realCoord + TCSFP::initAxesCoordinate (0, 0.5, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (1, 0, 0, ct1, ct2, ct3);

      absPos31 = getEpsCoord (realCoord - TCSFP::initAxesCoordinate (0, 0.5, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (0, 1, 1, ct1, ct2, ct3);
      absPos32 = getEpsCoord (realCoord - TCSFP::initAxesCoordinate (0, 0.5, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);

      absPos41 = getEpsCoord (realCoord + TCSFP::initAxesCoordinate (0, 0.5, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (0, 0, 1, ct1, ct2, ct3);
      absPos42 = getEpsCoord (realCoord + TCSFP::initAxesCoordinate (0, 0.5, 0, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (1, 0, 1, ct1, ct2, ct3);
    }
    else
    {
      absPos11 = getEpsCoord (realCoord - TCSFP::initAxesCoordinate (0, 0.5, 0, ct1, ct2, ct3));
      absPos12 = getEpsCoord (realCoord + TCSFP::initAxesCoordinate (0, 0.5, 0, ct1, ct2, ct3));
    }
  }
  else if ((typeOfField == GridType::EZ || typeOfField == GridType::DZ) && layout_type == E_CENTERED
           || (typeOfField == GridType::HZ || typeOfField == GridType::BZ) && layout_type == H_CENTERED)
  {
    if (isDoubleMaterialPrecision)
    {
      absPos11 = getEpsCoord (realCoord - TCSFP::initAxesCoordinate (0, 0, 0.5, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (0, 0, 1, ct1, ct2, ct3);
      absPos12 = getEpsCoord (realCoord - TCSFP::initAxesCoordinate (0, 0, 0.5, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (0, 1, 1, ct1, ct2, ct3);

      absPos21 = getEpsCoord (realCoord - TCSFP::initAxesCoordinate (0, 0, 0.5, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (1, 0, 1, ct1, ct2, ct3);
      absPos22 = getEpsCoord (realCoord - TCSFP::initAxesCoordinate (0, 0, 0.5, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);

      absPos31 = getEpsCoord (realCoord + TCSFP::initAxesCoordinate (0, 0, 0.5, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (0, 0, 0, ct1, ct2, ct3);
      absPos32 = getEpsCoord (realCoord + TCSFP::initAxesCoordinate (0, 0, 0.5, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (0, 1, 0, ct1, ct2, ct3);

      absPos41 = getEpsCoord (realCoord + TCSFP::initAxesCoordinate (0, 0, 0.5, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (1, 0, 0, ct1, ct2, ct3);
      absPos42 = getEpsCoord (realCoord + TCSFP::initAxesCoordinate (0, 0, 0.5, ct1, ct2, ct3)) * grid_coord (2) + TC::initAxesCoordinate (1, 1, 0, ct1, ct2, ct3);
    }
    else
    {
      absPos11 = getEpsCoord (realCoord - TCSFP::initAxesCoordinate (0, 0, 0.5, ct1, ct2, ct3));
      absPos12 = getEpsCoord (realCoord + TCSFP::initAxesCoordinate (0, 0, 0.5, ct1, ct2, ct3));
    }
  }
  else if ((typeOfField == GridType::HX || typeOfField == GridType::BX) && layout_type == E_CENTERED
           || (typeOfField == GridType::EX || typeOfField == GridType::DX) && layout_type == H_CENTERED)
  {
    TCFP coord;
    if (isDoubleMaterialPrecision)
    {
      coord = realCoord + TCSFP::initAxesCoordinate (0.0, -0.5, -0.5, ct1, ct2, ct3);
      absPos11 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (0, 1, 1, ct1, ct2, ct3);
      absPos12 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);

      coord = realCoord + TCSFP::initAxesCoordinate (0.0, -0.5, 0.5, ct1, ct2, ct3);
      absPos21 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (0, 1, 0, ct1, ct2, ct3);
      absPos22 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (1, 1, 0, ct1, ct2, ct3);

      coord = realCoord + TCSFP::initAxesCoordinate (0.0, 0.5, -0.5, ct1, ct2, ct3);
      absPos31 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (0, 0, 1, ct1, ct2, ct3);
      absPos32 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (1, 0, 1, ct1, ct2, ct3);

      coord = realCoord + TCSFP::initAxesCoordinate (0.0, 0.5, 0.5, ct1, ct2, ct3);
      absPos41 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (0, 0, 0, ct1, ct2, ct3);
      absPos42 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (1, 0, 0, ct1, ct2, ct3);
    }
    else
    {
      coord = realCoord + TCSFP::initAxesCoordinate (0.0, -0.5, -0.5, ct1, ct2, ct3);
      absPos11 = getEpsCoord (coord);
      coord = realCoord + TCSFP::initAxesCoordinate (0.0, -0.5, 0.5, ct1, ct2, ct3);
      absPos12 = getEpsCoord (coord);
      coord = realCoord + TCSFP::initAxesCoordinate (0.0, 0.5, -0.5, ct1, ct2, ct3);
      absPos21 = getEpsCoord (coord);
      coord = realCoord + TCSFP::initAxesCoordinate (0.0, 0.5, 0.5, ct1, ct2, ct3);
      absPos22 = getEpsCoord (coord);
    }
  }
  else if ((typeOfField == GridType::HY || typeOfField == GridType::BY) && layout_type == E_CENTERED
           || (typeOfField == GridType::EY || typeOfField == GridType::DY) && layout_type == H_CENTERED)
  {
    TCFP coord;
    if (isDoubleMaterialPrecision)
    {
      coord = realCoord + TCSFP::initAxesCoordinate (-0.5, 0.0, -0.5, ct1, ct2, ct3);
      absPos11 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (1, 0, 1, ct1, ct2, ct3);
      absPos12 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);

      coord = realCoord + TCSFP::initAxesCoordinate (-0.5, 0.0, 0.5, ct1, ct2, ct3);
      absPos21 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (1, 0, 0, ct1, ct2, ct3);
      absPos22 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (1, 1, 0, ct1, ct2, ct3);

      coord = realCoord + TCSFP::initAxesCoordinate (0.5, 0.0, -0.5, ct1, ct2, ct3);
      absPos31 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (0, 0, 1, ct1, ct2, ct3);
      absPos32 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (0, 1, 1, ct1, ct2, ct3);

      coord = realCoord + TCSFP::initAxesCoordinate (0.5, 0.0, 0.5, ct1, ct2, ct3);
      absPos41 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (0, 0, 0, ct1, ct2, ct3);
      absPos42 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (0, 1, 0, ct1, ct2, ct3);
    }
    else
    {
      coord = realCoord + TCSFP::initAxesCoordinate (-0.5, 0.0, -0.5, ct1, ct2, ct3);
      absPos11 = getEpsCoord (coord);
      coord = realCoord + TCSFP::initAxesCoordinate (-0.5, 0.0, 0.5, ct1, ct2, ct3);
      absPos12 = getEpsCoord (coord);
      coord = realCoord + TCSFP::initAxesCoordinate (0.5, 0.0, -0.5, ct1, ct2, ct3);
      absPos21 = getEpsCoord (coord);
      coord = realCoord + TCSFP::initAxesCoordinate (0.5, 0.0, 0.5, ct1, ct2, ct3);
      absPos22 = getEpsCoord (coord);
    }
  }
  else if ((typeOfField == GridType::HZ || typeOfField == GridType::BZ) && layout_type == E_CENTERED
           || (typeOfField == GridType::EZ || typeOfField == GridType::DZ) && layout_type == H_CENTERED)
  {
    TCFP coord;
    if (isDoubleMaterialPrecision)
    {
      coord = realCoord + TCSFP::initAxesCoordinate (-0.5, -0.5, 0.0, ct1, ct2, ct3);
      absPos11 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (1, 1, 0, ct1, ct2, ct3);
      absPos12 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (1, 1, 1, ct1, ct2, ct3);

      coord = realCoord + TCSFP::initAxesCoordinate (-0.5, 0.5, 0.0, ct1, ct2, ct3);
      absPos21 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (1, 0, 0, ct1, ct2, ct3);
      absPos22 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (1, 0, 1, ct1, ct2, ct3);

      coord = realCoord + TCSFP::initAxesCoordinate (0.5, -0.5, 0.0, ct1, ct2, ct3);
      absPos31 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (0, 1, 0, ct1, ct2, ct3);
      absPos32 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (0, 1, 1, ct1, ct2, ct3);

      coord = realCoord + TCSFP::initAxesCoordinate (0.5, 0.5, 0.0, ct1, ct2, ct3);
      absPos41 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (0, 0, 0, ct1, ct2, ct3);
      absPos42 = getEpsCoord (coord) * grid_coord (2) + TC::initAxesCoordinate (0, 0, 1, ct1, ct2, ct3);
    }
    else
    {
      coord = realCoord + TCSFP::initAxesCoordinate (-0.5, -0.5, 0.0, ct1, ct2, ct3);
      absPos11 = getEpsCoord (coord);
      coord = realCoord + TCSFP::initAxesCoordinate (-0.5, 0.5, 0.0, ct1, ct2, ct3);
      absPos12 = getEpsCoord (coord);
      coord = realCoord + TCSFP::initAxesCoordinate (0.5, -0.5, 0.0, ct1, ct2, ct3);
      absPos21 = getEpsCoord (coord);
      coord = realCoord + TCSFP::initAxesCoordinate (0.5, 0.5, 0.0, ct1, ct2, ct3);
      absPos22 = getEpsCoord (coord);
    }
  }
  else
  {
    UNREACHABLE;
  }
}

template <SchemeType_t Type, template <typename, bool> class TCoord, LayoutType layout_type>
CUDA_DEVICE CUDA_HOST typename YeeGridLayout<Type, TCoord, layout_type>::TCS
YeeGridLayout<Type, TCoord, layout_type>::getExCircuitElementDiff (LayoutDirection dir) const
{
  switch (dir)
  {
    case LayoutDirection::DOWN:
    {
      return circuitExDownDiff;
    }
    case LayoutDirection::UP:
    {
      return circuitExUpDiff;
    }
    case LayoutDirection::BACK:
    {
      return circuitExBackDiff;
    }
    case LayoutDirection::FRONT:
    {
      return circuitExFrontDiff;
    }
    case LayoutDirection::LEFT:
    case LayoutDirection::RIGHT:
    default:
    {
      UNREACHABLE;
    }
  }

  UNREACHABLE;
  return circuitExDownDiff;
}

template <SchemeType_t Type, template <typename, bool> class TCoord, LayoutType layout_type>
CUDA_DEVICE CUDA_HOST typename YeeGridLayout<Type, TCoord, layout_type>::TCS
YeeGridLayout<Type, TCoord, layout_type>::getEyCircuitElementDiff (LayoutDirection dir) const
{
  switch (dir)
  {
    case LayoutDirection::LEFT:
    {
      return circuitEyLeftDiff;
    }
    case LayoutDirection::RIGHT:
    {
      return circuitEyRightDiff;
    }
    case LayoutDirection::BACK:
    {
      return circuitEyBackDiff;
    }
    case LayoutDirection::FRONT:
    {
      return circuitEyFrontDiff;
    }
    case LayoutDirection::DOWN:
    case LayoutDirection::UP:
    default:
    {
      UNREACHABLE;
    }
  }

  UNREACHABLE;
  return circuitEyLeftDiff;
}

template <SchemeType_t Type, template <typename, bool> class TCoord, LayoutType layout_type>
CUDA_DEVICE CUDA_HOST typename YeeGridLayout<Type, TCoord, layout_type>::TCS
YeeGridLayout<Type, TCoord, layout_type>::getEzCircuitElementDiff (LayoutDirection dir) const
{
  switch (dir)
  {
    case LayoutDirection::LEFT:
    {
      return circuitEzLeftDiff;
    }
    case LayoutDirection::RIGHT:
    {
      return circuitEzRightDiff;
    }
    case LayoutDirection::DOWN:
    {
      return circuitEzDownDiff;
    }
    case LayoutDirection::UP:
    {
      return circuitEzUpDiff;
    }
    case LayoutDirection::BACK:
    case LayoutDirection::FRONT:
    default:
    {
      UNREACHABLE;
    }
  }

  UNREACHABLE;
  return circuitEzLeftDiff;
}

template <SchemeType_t Type, template <typename, bool> class TCoord, LayoutType layout_type>
CUDA_DEVICE CUDA_HOST typename YeeGridLayout<Type, TCoord, layout_type>::TCS
YeeGridLayout<Type, TCoord, layout_type>::getHxCircuitElementDiff (LayoutDirection dir) const
{
  switch (dir)
  {
    case LayoutDirection::DOWN:
    {
      return circuitHxDownDiff;
    }
    case LayoutDirection::UP:
    {
      return circuitHxUpDiff;
    }
    case LayoutDirection::BACK:
    {
      return circuitHxBackDiff;
    }
    case LayoutDirection::FRONT:
    {
      return circuitHxFrontDiff;
    }
    case LayoutDirection::LEFT:
    case LayoutDirection::RIGHT:
    default:
    {
      UNREACHABLE;
    }
  }

  UNREACHABLE;
  return circuitHxDownDiff;
}

template <SchemeType_t Type, template <typename, bool> class TCoord, LayoutType layout_type>
CUDA_DEVICE CUDA_HOST typename YeeGridLayout<Type, TCoord, layout_type>::TCS
YeeGridLayout<Type, TCoord, layout_type>::getHyCircuitElementDiff (LayoutDirection dir) const
{
  switch (dir)
  {
    case LayoutDirection::LEFT:
    {
      return circuitHyLeftDiff;
    }
    case LayoutDirection::RIGHT:
    {
      return circuitHyRightDiff;
    }
    case LayoutDirection::BACK:
    {
      return circuitHyBackDiff;
    }
    case LayoutDirection::FRONT:
    {
      return circuitHyFrontDiff;
    }
    case LayoutDirection::DOWN:
    case LayoutDirection::UP:
    default:
    {
      UNREACHABLE;
    }
  }

  UNREACHABLE;
  return circuitHyLeftDiff;
}

template <SchemeType_t Type, template <typename, bool> class TCoord, LayoutType layout_type>
CUDA_DEVICE CUDA_HOST typename YeeGridLayout<Type, TCoord, layout_type>::TCS
YeeGridLayout<Type, TCoord, layout_type>::getHzCircuitElementDiff (LayoutDirection dir) const
{
  switch (dir)
  {
    case LayoutDirection::LEFT:
    {
      return circuitHzLeftDiff;
    }
    case LayoutDirection::RIGHT:
    {
      return circuitHzRightDiff;
    }
    case LayoutDirection::DOWN:
    {
      return circuitHzDownDiff;
    }
    case LayoutDirection::UP:
    {
      return circuitHzUpDiff;
    }
    case LayoutDirection::BACK:
    case LayoutDirection::FRONT:
    default:
    {
      UNREACHABLE;
    }
  }

  UNREACHABLE;
  return circuitHzLeftDiff;
}

/* Dim1_ExHy */
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHy), GridCoordinate1DTemplate, E_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  return - valE;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHy), GridCoordinate1DTemplate, H_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  return - valE;
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHy), GridCoordinate1DTemplate, E_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHy), GridCoordinate1DTemplate, H_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHy), GridCoordinate1DTemplate, E_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHy), GridCoordinate1DTemplate, H_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHy), GridCoordinate1DTemplate, E_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHy), GridCoordinate1DTemplate, H_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHy), GridCoordinate1DTemplate, E_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  return - valH;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHy), GridCoordinate1DTemplate, H_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  return - valH;
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHy), GridCoordinate1DTemplate, E_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHy), GridCoordinate1DTemplate, H_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

/* Dim1_ExHz */
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHz), GridCoordinate1DTemplate, E_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  return valE;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHz), GridCoordinate1DTemplate, H_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  return valE;
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHz), GridCoordinate1DTemplate, E_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHz), GridCoordinate1DTemplate, H_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHz), GridCoordinate1DTemplate, E_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHz), GridCoordinate1DTemplate, H_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHz), GridCoordinate1DTemplate, E_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHz), GridCoordinate1DTemplate, H_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHz), GridCoordinate1DTemplate, E_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHz), GridCoordinate1DTemplate, H_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHz), GridCoordinate1DTemplate, E_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  return - valH;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_ExHz), GridCoordinate1DTemplate, H_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  return - valH;
}

/* Dim1_EyHx */
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHx), GridCoordinate1DTemplate, E_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHx), GridCoordinate1DTemplate, H_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHx), GridCoordinate1DTemplate, E_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  return - valE;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHx), GridCoordinate1DTemplate, H_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  return - valE;
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHx), GridCoordinate1DTemplate, E_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHx), GridCoordinate1DTemplate, H_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHx), GridCoordinate1DTemplate, E_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  return valH;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHx), GridCoordinate1DTemplate, H_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  return valH;
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHx), GridCoordinate1DTemplate, E_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHx), GridCoordinate1DTemplate, H_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHx), GridCoordinate1DTemplate, E_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHx), GridCoordinate1DTemplate, H_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

/* Dim1_EyHz */
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHz), GridCoordinate1DTemplate, E_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHz), GridCoordinate1DTemplate, H_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHz), GridCoordinate1DTemplate, E_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  return - valE;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHz), GridCoordinate1DTemplate, H_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  return - valE;
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHz), GridCoordinate1DTemplate, E_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHz), GridCoordinate1DTemplate, H_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHz), GridCoordinate1DTemplate, E_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHz), GridCoordinate1DTemplate, H_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHz), GridCoordinate1DTemplate, E_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHz), GridCoordinate1DTemplate, H_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHz), GridCoordinate1DTemplate, E_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  return - valH;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EyHz), GridCoordinate1DTemplate, H_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  return - valH;
}

/* Dim1_EzHx */
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHx), GridCoordinate1DTemplate, E_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHx), GridCoordinate1DTemplate, H_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHx), GridCoordinate1DTemplate, E_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHx), GridCoordinate1DTemplate, H_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHx), GridCoordinate1DTemplate, E_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  return valE;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHx), GridCoordinate1DTemplate, H_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  return valE;
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHx), GridCoordinate1DTemplate, E_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  return valH;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHx), GridCoordinate1DTemplate, H_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  return valH;
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHx), GridCoordinate1DTemplate, E_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHx), GridCoordinate1DTemplate, H_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHx), GridCoordinate1DTemplate, E_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHx), GridCoordinate1DTemplate, H_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

/* Dim1_EzHy */
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHy), GridCoordinate1DTemplate, E_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHy), GridCoordinate1DTemplate, H_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHy), GridCoordinate1DTemplate, E_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHy), GridCoordinate1DTemplate, H_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHy), GridCoordinate1DTemplate, E_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  return valE;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHy), GridCoordinate1DTemplate, H_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  return valE;
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHy), GridCoordinate1DTemplate, E_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHy), GridCoordinate1DTemplate, H_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHy), GridCoordinate1DTemplate, E_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  return - valH;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHy), GridCoordinate1DTemplate, H_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  return - valH;
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHy), GridCoordinate1DTemplate, E_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim1_EzHy), GridCoordinate1DTemplate, H_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

/* Dim2_TEx */
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEx), GridCoordinate2DTemplate, E_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEx), GridCoordinate2DTemplate, H_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEx), GridCoordinate2DTemplate, E_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) ( - cos (incidentWaveAngle1));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEx), GridCoordinate2DTemplate, H_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) ( - cos (incidentWaveAngle1));
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEx), GridCoordinate2DTemplate, E_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) (sin (incidentWaveAngle1));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEx), GridCoordinate2DTemplate, H_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) (sin (incidentWaveAngle1));
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEx), GridCoordinate2DTemplate, E_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  return valH;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEx), GridCoordinate2DTemplate, H_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  return valH;
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEx), GridCoordinate2DTemplate, E_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEx), GridCoordinate2DTemplate, H_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEx), GridCoordinate2DTemplate, E_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEx), GridCoordinate2DTemplate, H_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

/* Dim2_TEy */
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEy), GridCoordinate2DTemplate, E_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) (- cos (incidentWaveAngle1));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEy), GridCoordinate2DTemplate, H_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) (- cos (incidentWaveAngle1));
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEy), GridCoordinate2DTemplate, E_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEy), GridCoordinate2DTemplate, H_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEy), GridCoordinate2DTemplate, E_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) (sin (incidentWaveAngle1));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEy), GridCoordinate2DTemplate, H_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) (sin (incidentWaveAngle1));
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEy), GridCoordinate2DTemplate, E_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEy), GridCoordinate2DTemplate, H_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEy), GridCoordinate2DTemplate, E_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  return - valH;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEy), GridCoordinate2DTemplate, H_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  return - valH;
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEy), GridCoordinate2DTemplate, E_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEy), GridCoordinate2DTemplate, H_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

/* Dim2_TEz */
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEz), GridCoordinate2DTemplate, E_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) (sin (incidentWaveAngle2));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEz), GridCoordinate2DTemplate, H_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) (sin (incidentWaveAngle2));
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEz), GridCoordinate2DTemplate, E_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) ( - cos (incidentWaveAngle2));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEz), GridCoordinate2DTemplate, H_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) ( - cos (incidentWaveAngle2));
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEz), GridCoordinate2DTemplate, E_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEz), GridCoordinate2DTemplate, H_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEz), GridCoordinate2DTemplate, E_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEz), GridCoordinate2DTemplate, H_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEz), GridCoordinate2DTemplate, E_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEz), GridCoordinate2DTemplate, H_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEz), GridCoordinate2DTemplate, E_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  return - valH;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TEz), GridCoordinate2DTemplate, H_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  return - valH;
}

/* Dim2_TMx */
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMx), GridCoordinate2DTemplate, E_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  return valE;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMx), GridCoordinate2DTemplate, H_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  return valE;
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMx), GridCoordinate2DTemplate, E_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMx), GridCoordinate2DTemplate, H_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMx), GridCoordinate2DTemplate, E_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMx), GridCoordinate2DTemplate, H_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMx), GridCoordinate2DTemplate, E_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMx), GridCoordinate2DTemplate, H_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMx), GridCoordinate2DTemplate, E_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  return valH * (FPValue) (cos (incidentWaveAngle1));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMx), GridCoordinate2DTemplate, H_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  return valH * (FPValue) (cos (incidentWaveAngle1));
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMx), GridCoordinate2DTemplate, E_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  return - valH * (FPValue) (sin (incidentWaveAngle1));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMx), GridCoordinate2DTemplate, H_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  return - valH * (FPValue) (sin (incidentWaveAngle1));
}

/* Dim2_TMy */
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMy), GridCoordinate2DTemplate, E_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMy), GridCoordinate2DTemplate, H_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMy), GridCoordinate2DTemplate, E_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  return - valE;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMy), GridCoordinate2DTemplate, H_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  return - valE;
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMy), GridCoordinate2DTemplate, E_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMy), GridCoordinate2DTemplate, H_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMy), GridCoordinate2DTemplate, E_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  return valH * (FPValue) (cos (incidentWaveAngle1));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMy), GridCoordinate2DTemplate, H_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  return valH * (FPValue) (cos (incidentWaveAngle1));
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMy), GridCoordinate2DTemplate, E_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMy), GridCoordinate2DTemplate, H_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMy), GridCoordinate2DTemplate, E_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  return - valH * (FPValue) (sin (incidentWaveAngle1));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMy), GridCoordinate2DTemplate, H_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  return - valH * (FPValue) (sin (incidentWaveAngle1));
}

/* Dim2_TMz */
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMz), GridCoordinate2DTemplate, E_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMz), GridCoordinate2DTemplate, H_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMz), GridCoordinate2DTemplate, E_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMz), GridCoordinate2DTemplate, H_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  UNREACHABLE;
  return FPValue (0);
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMz), GridCoordinate2DTemplate, E_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  return valE;
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMz), GridCoordinate2DTemplate, H_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  return valE;
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMz), GridCoordinate2DTemplate, E_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  return valH * (FPValue) (sin (incidentWaveAngle2));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMz), GridCoordinate2DTemplate, H_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  return valH * (FPValue) (sin (incidentWaveAngle2));
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMz), GridCoordinate2DTemplate, E_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  return valH * (FPValue) (- cos (incidentWaveAngle2));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMz), GridCoordinate2DTemplate, H_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  return valH * (FPValue) (- cos (incidentWaveAngle2));
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMz), GridCoordinate2DTemplate, E_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim2_TMz), GridCoordinate2DTemplate, H_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  UNREACHABLE;
  return FPValue (0);
}

/* Dim3 */
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim3), GridCoordinate3DTemplate, E_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) (cos (incidentWaveAngle3) * sin (incidentWaveAngle2) - sin (incidentWaveAngle3) * cos (incidentWaveAngle1) * cos (incidentWaveAngle2));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim3), GridCoordinate3DTemplate, H_CENTERED>::getExFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) (cos (incidentWaveAngle3) * sin (incidentWaveAngle2) - sin (incidentWaveAngle3) * cos (incidentWaveAngle1) * cos (incidentWaveAngle2));
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim3), GridCoordinate3DTemplate, E_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) ( - cos (incidentWaveAngle3) * cos (incidentWaveAngle2) - sin (incidentWaveAngle3) * cos (incidentWaveAngle1) * sin (incidentWaveAngle2));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim3), GridCoordinate3DTemplate, H_CENTERED>::getEyFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) ( - cos (incidentWaveAngle3) * cos (incidentWaveAngle2) - sin (incidentWaveAngle3) * cos (incidentWaveAngle1) * sin (incidentWaveAngle2));
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim3), GridCoordinate3DTemplate, E_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) (sin (incidentWaveAngle3) * sin (incidentWaveAngle1));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim3), GridCoordinate3DTemplate, H_CENTERED>::getEzFromIncidentE (FieldValue valE) const
{
  return valE * (FPValue) (sin (incidentWaveAngle3) * sin (incidentWaveAngle1));
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim3), GridCoordinate3DTemplate, E_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  return valH * (FPValue) (sin (incidentWaveAngle3) * sin (incidentWaveAngle2) + cos (incidentWaveAngle3) * cos (incidentWaveAngle1) * cos (incidentWaveAngle2));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim3), GridCoordinate3DTemplate, H_CENTERED>::getHxFromIncidentH (FieldValue valH) const
{
  return valH * (FPValue) (sin (incidentWaveAngle3) * sin (incidentWaveAngle2) + cos (incidentWaveAngle3) * cos (incidentWaveAngle1) * cos (incidentWaveAngle2));
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim3), GridCoordinate3DTemplate, E_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  return valH * (FPValue) (- sin (incidentWaveAngle3) * cos (incidentWaveAngle2) + cos (incidentWaveAngle3) * cos (incidentWaveAngle1) * sin (incidentWaveAngle2));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim3), GridCoordinate3DTemplate, H_CENTERED>::getHyFromIncidentH (FieldValue valH) const
{
  return valH * (FPValue) (- sin (incidentWaveAngle3) * cos (incidentWaveAngle2) + cos (incidentWaveAngle3) * cos (incidentWaveAngle1) * sin (incidentWaveAngle2));
}

template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim3), GridCoordinate3DTemplate, E_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  return - valH * (FPValue) (cos (incidentWaveAngle3) * sin (incidentWaveAngle1));
}
template <>
CUDA_DEVICE CUDA_HOST inline FieldValue
YeeGridLayout<static_cast<SchemeType_t> (SchemeType::Dim3), GridCoordinate3DTemplate, H_CENTERED>::getHzFromIncidentH (FieldValue valH) const
{
  return - valH * (FPValue) (cos (incidentWaveAngle3) * sin (incidentWaveAngle1));
}

#endif /* YEE_GRID_LAYOUT_H */
