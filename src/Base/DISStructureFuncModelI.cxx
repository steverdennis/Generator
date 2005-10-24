//____________________________________________________________________________
/*!

\class    genie::DISStructureFuncModelI

\brief    Pure Abstract Base Class. Defines the DISStructureFuncModelI
          interface to be implemented by any algorithmic class computing DIS
          structure functions.

\author   Costas Andreopoulos <C.V.Andreopoulos@rl.ac.uk>
          CCLRC, Rutherford Appleton Laboratory

\created  May 03, 2004

*/
//____________________________________________________________________________

#include "Base/DISStructureFuncModelI.h"

using namespace genie;

//____________________________________________________________________________
DISStructureFuncModelI::DISStructureFuncModelI() :
Algorithm()
{

}
//____________________________________________________________________________
DISStructureFuncModelI::DISStructureFuncModelI(string name):
Algorithm(name)
{

}
//____________________________________________________________________________
DISStructureFuncModelI::DISStructureFuncModelI(string name, string config):
Algorithm(name, config)
{

}
//____________________________________________________________________________
DISStructureFuncModelI::~DISStructureFuncModelI()
{

}
//____________________________________________________________________________





