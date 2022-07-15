# Load GENIE
# If imported, becomes namespace ROOT.genie (not genie as might be expected), requires import ROOT
# There's a hack here to fix this which I'm not super happy about
# If loaded in interpreter, sets up genie as G.

import ROOT
import sys
import os
import glob
import math
try:
  import numpy as np
except:
  print("Warning: Numpy not loaded")

ROOT.gROOT.ProcessLine(".x ${GENIE}/src/scripts/gcint/loadincs.C");    
ROOT.gROOT.ProcessLine(".x ${GENIE}/src/scripts/gcint/loadlibs.C");

if __name__=="__main__":
  print("\n===========================")
  print("Loading python with GENIE")
  print("===========================\n")
  rootargs = [x for x in sys.argv if x.endswith(".root")]
  i=0
  for f in rootargs:
    varname = "_file"+repr(i)
    print("Attaching file ",f," as ",varname)
    globals()[varname] = ROOT.TFile(f)
    i+=1

  R = ROOT
  genie = ROOT.genie
  G = ROOT.genie 
  
  G.RunOpt.Instance().SetTuneName("Default")
  # ~ G.RunOpt.Instance().BuildTune()
  
  # Quick and dirty function to get every object in a file
  def fileburst(f):
    for k in f.GetListOfKeys():
      obj = k.ReadObj()
      if (obj.ClassName()=="TTree"):
        if obj.GetEntries():
          obj.GetEntry(0)
      globals()[k.GetName()] = obj
    return
  
  if (i==1):
    fileburst(_file0)
  
  # Touch every GENIE object so they appear in tab completion
  for root, dirs, files in os.walk(os.environ['GENIE']+'/src/'):
    for f in files:
      if f.endswith(".cxx"):
        classname = f[:-4]
        try:
          getattr(genie,classname)
          pass
        except:
          pass

# Make the "import genie" thing work as expected (HACK!)
else:
  # Touch every GENIE class
  for root, dirs, files in os.walk(os.environ['GENIE']+'/src/'):
    for f in files:
      if f.endswith(".cxx"):
        classname = f[:-4]
        try:
          getattr(ROOT.genie,classname)
        except:
          pass
  # And then copy them into the main namespace for this module
  for member_name in dir(ROOT.genie):
    if member_name[:2] == "__": continue
    if "::" in member_name: continue
    try:
      member = getattr(ROOT.genie,member_name)
      if type(member) == "module": continue
      locals()[member_name] = member
    except AttributeError:
      pass
      
