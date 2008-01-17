//____________________________________________________________________________
/*!

\namespace  genie::utils::kinematics

\brief      Kinematical utilities

\author     Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
            STFC, Rutherford Appleton Laboratory

\created    November 26, 2004

\cpright    Copyright (c) 2003-2008, GENIE Neutrino MC Generator Collaboration
            For the full text of the license visit http://copyright.genie-mc.org
            or see $GENIE/LICENSE
*/
//____________________________________________________________________________

#ifndef _KINE_UTILS_H_
#define _KINE_UTILS_H_

#include "Conventions/KineVar.h"
#include "Conventions/KinePhaseSpace.h"
#include "Interaction/Interaction.h"
#include "Utils/Range1.h"

namespace genie {
namespace utils {

namespace kinematics
{
  //-- methods used for computing phase space volumes
  double PhaseSpaceVolume (const Interaction * const i, KinePhaseSpace_t ps);

  //-- methods used for computing the Jacobians associated with phase space transformations
  double Jacobian (const Interaction * const i, KinePhaseSpace_t f, KinePhaseSpace_t t);
  bool   TransformMatched (KinePhaseSpace_t ia, KinePhaseSpace_t ib,
                                    KinePhaseSpace_t a, KinePhaseSpace_t b, bool & fwd);

  //-- kinematical limits
  Range1D_t  InelWLim    (double Ev, double M, double ml);
  Range1D_t  InelQ2Lim_W (double Ev, double M, double ml, double W);
  Range1D_t  Inelq2Lim_W (double Ev, double M, double ml, double W);
  Range1D_t  InelQ2Lim   (double Ev, double M, double ml);
  Range1D_t  Inelq2Lim   (double Ev, double M, double ml);
  Range1D_t  InelXLim    (double Ev, double M, double ml);
  Range1D_t  InelYLim    (double Ev, double M, double ml);
  Range1D_t  InelYLim_X  (double Ev, double M, double ml, double x);
  Range1D_t  CohYLim     (double EvL, double ml);
  Range1D_t  CohXLim     (void);

  //-- kinematical variable transforms
  double QD2toQ2 (double QD2);
  double Q2toQD2 (double Q2);
  void   WQ2toXY (double Ev, double M, double W, double Q2, double & x, double & y);
  void   XYtoWQ2 (double Ev, double M, double & W, double & Q2, double x, double y);
  double XYtoW   (double Ev, double M, double x, double y);
  double XYtoQ2  (double Ev, double M, double x, double y);

  void  UpdateWQ2FromXY(const Interaction * in);
  void  UpdateXYFromWQ2(const Interaction * in);

  //-- methods used to apply cuts to kinematical limits
  void ApplyCutsToKineLimits (Range1D_t & r, double min, double max);

  double Q2 (const Interaction * const i);
  double W  (const Interaction * const i);

  //-- charm threshold and slow rescaling variable
  bool   IsAboveCharmThreshold (double x, double Q2, double M, double mc);
  double SlowRescalingVar      (double x, double Q2, double M, double mc);

  //-- Functions used to define differential cross section distribution envelopes 
  //-- for importance sampling in kinematical selection modules
  double RESImportanceSamplingEnvelope(double * x, double * par);
  double DISImportanceSamplingEnvelope(double * x, double * par);
  double COHImportanceSamplingEnvelope(double * x, double * par);

} // kinematics namespace
} // utils namespace
} // genie namespace

#endif // _KINE_UTILS_H_
