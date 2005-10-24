//____________________________________________________________________________
/*!

\class    genie::DecayModelI

\brief    Pure abstract base class. Defines the DecayModelI interface to be
          implemented by any algorithmic class decaying a particle.

\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          CCLRC, Rutherford Appleton Laboratory

\created  June 20, 2004
 
*/
//____________________________________________________________________________


#ifndef _DECAY_MODEL_I_H_
#define _DECAY_MODEL_I_H_

#include <TClonesArray.h>
#include <TDecayChannel.h>
#include <TLorentzVector.h>

#include "Algorithm/Algorithm.h"
#include "Decay/DecayerInputs.h"

namespace genie {

class DecayModelI : public Algorithm {

public:

  virtual ~DecayModelI();

  //-- define DecayModelI interface

  virtual bool           IsHandled  (int pdgc)                    const = 0;
  virtual void           Initialize (void)                        const = 0;  
  virtual TClonesArray * Decay      (const DecayerInputs_t & inp) const = 0;

protected:

  DecayModelI();
  DecayModelI(string name);
  DecayModelI(string name, string config);
};

}         // genie namespace

#endif    // _DECAY_MODEL_I_H_
