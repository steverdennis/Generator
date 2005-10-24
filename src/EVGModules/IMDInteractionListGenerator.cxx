//____________________________________________________________________________
/*!

\class   genie::IMDInteractionListGenerator

\brief   Concrete implementations of the InteractionListGeneratorI interface.
         Generate a list of all the Interaction (= event summary) objects that
         can be generated by the IMD EventGenerator.

\author  Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
         CCLRC, Rutherford Appleton Laboratory

\created July 13, 2005

*/
//____________________________________________________________________________

#include "EVGModules/IMDInteractionListGenerator.h"
#include "EVGCore/InteractionList.h"
#include "Interaction/Interaction.h"
#include "Messenger/Messenger.h"
#include "PDG/PDGCodes.h"
#include "PDG/PDGUtils.h"

using namespace genie;

//___________________________________________________________________________
IMDInteractionListGenerator::IMDInteractionListGenerator() :
InteractionListGeneratorI("genie::IMDInteractionListGenerator")
{

}
//___________________________________________________________________________
IMDInteractionListGenerator::IMDInteractionListGenerator(string config) :
InteractionListGeneratorI("genie::IMDInteractionListGenerator", config)
{

}
//___________________________________________________________________________
IMDInteractionListGenerator::~IMDInteractionListGenerator()
{

}
//___________________________________________________________________________
InteractionList * IMDInteractionListGenerator::CreateInteractionList(
                                       const InitialState & init_state) const
{
  LOG("InteractionList", pINFO) << "InitialState = " << init_state.AsString();

  assert(init_state.GetProbePDGCode() == kPdgNuMu);

  InteractionList * intlist = new InteractionList;

  // clone init state and de-activate the struck nucleon info
  InitialState init(init_state);
  init_state.GetTargetPtr()->SetStruckNucleonPDGCode(0);

  ProcessInfo   proc_info(kScInverseMuDecay, kIntWeakCC);
  Interaction * interaction = new Interaction(init, proc_info);

  intlist->push_back(interaction);

  return intlist;
}
//___________________________________________________________________________
