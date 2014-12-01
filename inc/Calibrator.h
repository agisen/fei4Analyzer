/****************************************************************************
** Author: Stefano Terzo
**
** Max-Planck-instiut fuer Physik (Werner-Heisenberg-Institut)
** Foehringer Ring 6, D-80805 Muenchen, Germany
**
****************************************************************************/

#ifndef __CALIBRATOR_H__
#define __CALIBRATOR_H__

#include <vector>
#include <map>
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <TH2.h>
#include <TFile.h>

#include "EventMaker.h"

class Calibrator
{
 public :
   Calibrator(void 				);
  ~Calibrator(void                    		){;}
  
  double calib(EventMaker::hitDef hit);
  
 private:
   
   std::stringstream  ss_;

   int totmap_[81] [337];
 
} ;

#endif
