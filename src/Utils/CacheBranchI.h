//____________________________________________________________________________
/*!

\class    genie::CacheBranchI

\brief    The TObject at the root of concrete cache branches

\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          STFC, Rutherford Appleton Laboratory

\created  November 26, 2004

\cpright  Copyright (c) 2003-2008, GENIE Neutrino MC Generator Collaboration
          For the full text of the license visit http://copyright.genie-mc.org
          or see $GENIE/LICENSE
*/
//____________________________________________________________________________

#ifndef _CACHE_BRANCH_I_H_
#define _CACHE_BRANCH_I_H_

#include <TObject.h>

namespace genie {

class CacheBranchI : public TObject
{
public:
  virtual ~CacheBranchI() {}
protected:
  CacheBranchI() : TObject() {}

ClassDef(CacheBranchI,0)
};

}      // genie namespace
#endif // _CACHE_BRANCH_I_H_
