//____________________________________________________________________________
/*!

\class    genie::NuclearTargetXSec

\brief    Computes the cross section for a nuclear target from the free
          nucleon cross sections.

          Is a concrete implementation of the XSecAlgorithmI interface.
          
\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          CCLRC, Rutherford Appleton Laboratory

\created  June 10, 2004

*/
//____________________________________________________________________________

#include "Nuclear/NuclearTargetXSec.h"
#include "Messenger/Messenger.h"
#include "PDG/PDGCodes.h"

using namespace genie;

//____________________________________________________________________________
NuclearTargetXSec::NuclearTargetXSec() :
XSecAlgorithmI()
{
  fName = "genie::NuclearTargetXSec";
}
//____________________________________________________________________________
NuclearTargetXSec::NuclearTargetXSec(const char * param_set) :
XSecAlgorithmI(param_set)
{
  fName = "genie::NuclearTargetXSec";

  FindConfig();
}
//____________________________________________________________________________
NuclearTargetXSec::~NuclearTargetXSec()
{

}
//____________________________________________________________________________
double NuclearTargetXSec::XSec(const Interaction * interaction) const
{
  //--- get free nucleon cross section calculator

  const Algorithm * xsec_alg_base = this->SubAlg(
                "free-nucleon-xsec-alg-name", "free-nucleon-xsec-param-set");

  const XSecAlgorithmI * free_nucleon_xsec_alg =
                         dynamic_cast<const XSecAlgorithmI *>(xsec_alg_base);

  //--- get a cloned interaction

  Interaction ci(*interaction);
  
  //--- get cross section for free nucleons 

  ci.GetInitialStatePtr()->GetTargetPtr()->SetStruckNucleonPDGCode(kPdgProton);

  double xsec_p = free_nucleon_xsec_alg->XSec(&ci);

  ci.GetInitialStatePtr()->GetTargetPtr()->SetStruckNucleonPDGCode(kPdgNeutron);

  double xsec_n = free_nucleon_xsec_alg->XSec(&ci);

  //--- compute a weighted average of the free nucleon cross sections

  const Target & tgt = interaction->GetInitialState().GetTarget();
  
  double xsec = (tgt.Z() * xsec_p + tgt.N() * xsec_n) / tgt.A();

  return xsec;
}
//____________________________________________________________________________
