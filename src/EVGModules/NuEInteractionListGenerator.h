//____________________________________________________________________________
/*!

\class    genie::NuEInteractionListGenerator

\brief    Concrete implementations of the InteractionListGeneratorI interface.
          Generates a list of all the Interaction (= event summary) objects that
          can be generated by the NUE EventGenerator.

\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          STFC, Rutherford Appleton Laboratory

\created  July 13, 2005

\cpright  Copyright (c) 2003-2008, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________

#ifndef _NUE_INTERACTION_GENERATOR_H_
#define _NUE_INTERACTION_GENERATOR_H_

#include "EVGCore/InteractionListGeneratorI.h"

namespace genie {

class NuEInteractionListGenerator : public InteractionListGeneratorI {

public :
  NuEInteractionListGenerator();
  NuEInteractionListGenerator(string config);
 ~NuEInteractionListGenerator();

  //-- implement the InteractionListGeneratorI interface
  InteractionList * CreateInteractionList(const InitialState & init) const;

  //-- overload the Algorithm::Configure() methods to load private data
  //   members from configuration options
  void Configure(const Registry & config);
  void Configure(string config);

private:
  InteractionList * IMDInteractionList   (const InitialState & init_state) const;
  InteractionList * NuEELInteractionList (const InitialState & init_state) const;
 
  void LoadConfig (void);

  bool fIsIMD;
};

}      // genie namespace
#endif // _NUE_INTERACTION_GENERATOR_H_
