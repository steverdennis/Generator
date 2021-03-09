//____________________________________________________________________________
/*
 Copyright (c) 2003-2020, The GENIE Collaboration
 For the full text of the license visit http://copyright.genie-mc.org

 Author: Iker de Icaza <i.de-icaza-astiz \at sussex.ac.uk>
 University of Sussex

 Costas Andreopoulos <constantinos.andreopoulos \at cern.ch>
 University of Liverpool & STFC Rutherford Appleton Laboratory
*/
//____________________________________________________________________________

#include <TMath.h>
#include <Math/IFunction.h>
#include <Math/Integrator.h>

#include "Framework/Conventions/Constants.h"
#include "Framework/Conventions/Units.h"
#include "Framework/Messenger/Messenger.h"
#include "Framework/Numerical/GSLUtils.h"
#include "Physics/DarkNeutrino/XSection/COHDNuXSec.h"
#include "Physics/XSectionIntegration/GSLXSecFunc.h"

using namespace genie;
using namespace genie::constants;
using namespace genie::utils;

//____________________________________________________________________________
COHDNuXSec::COHDNuXSec() :
XSecIntegratorI("genie::COHDNuXSec")
{

}
//____________________________________________________________________________
COHDNuXSec::COHDNuXSec(string config) :
XSecIntegratorI("genie::COHDNuXSec", config)
{

}
//____________________________________________________________________________
COHDNuXSec::~COHDNuXSec()
{

}
//____________________________________________________________________________
double COHDNuXSec::Integrate(
      const XSecAlgorithmI * model, const Interaction * in) const
{
  if(!model->ValidProcess(in) ) return 0.;
  if(!in->PhaseSpace().IsAboveThreshold()) return 0.;

  Interaction interaction(*in);
  interaction.SetBit(kISkipProcessChk);
  interaction.SetBit(kISkipKinematicChk);

  ROOT::Math::IntegrationOneDim::Type ig_type =
          utils::gsl::Integration1DimTypeFromString(fGSLIntgType);

  utils::gsl::dXSec_dEDNu_E func( model, & interaction, fDNuMass );
  Range1D_t DNuEnergy = func.IntegrationRange();

  double abstol = 1; // We mostly care about relative tolerance
  ROOT::Math::Integrator ig( func, ig_type, abstol, fGSLRelTol, fGSLMaxEval);
  double xsec = ig.Integral(DNuEnergy.min, DNuEnergy.max) * (1E-38 * units::cm2); // units: GeV^-2

  const InitialState & init_state = in->InitState();
  double Ev = init_state.ProbeE(kRfLab);
  LOG("COHDNuXSec", pINFO)
    << "XSec[COHDNu] (E = " << Ev << " GeV) = " << xsec/(units::cm2) << " cm^2";

  return xsec;
}
//____________________________________________________________________________
void COHDNuXSec::Configure(const Registry & config)
{
  Algorithm::Configure(config);
  this->LoadConfig();
}
//____________________________________________________________________________
void COHDNuXSec::Configure(string config)
{
  Algorithm::Configure(config);
  this->LoadConfig();
}
//____________________________________________________________________________
void COHDNuXSec::LoadConfig(void)
{
  fDNuMass = 0.;
  this->GetParam("Dark-NeutrinoMass", fDNuMass);

  // Get GSL integration type & relative tolerance
  this->GetParamDef("gsl-integration-type",   fGSLIntgType, string("adaptive"));
  this->GetParamDef("gsl-relative-tolerance", fGSLRelTol,   1E-2);

  // max and minimum number of integrand evaluations
  int max_eval, min_eval ;
  this->GetParamDef("gsl-max-eval", max_eval, 500000);
  this->GetParamDef("gsl-min-eval", min_eval, 5000  );
  fGSLMaxEval = (unsigned int) max_eval ;
  fGSLMinEval = (unsigned int) min_eval ;
}
//____________________________________________________________________________
