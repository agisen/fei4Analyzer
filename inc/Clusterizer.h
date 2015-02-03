/****************************************************************************
** Author: Stefano Terzo
**
** Max-Planck-instiut fuer Physik (Werner-Heisenberg-Institut)
** Foehringer Ring 6, D-80805 Muenchen, Germany
**
****************************************************************************/

#ifndef __CLUSTERIZER_H__
#define __CLUSTERIZER_H__

#include <vector>
#include <map>
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <string>

#include "EventMaker.h"

class Clusterizer
{
 public :
 
   //          #clusterID
   typedef std::map<int, EventMaker::hitsDef>             clustersDef   ;
   //              #l1id         #chip
   typedef std::map<int , std::map<int, clustersDef> >    clusterMapDef ;
 
   Clusterizer(void                    ) {;}
  ~Clusterizer(void                    ) {;}
  
   clusterMapDef makeCluster(EventMaker::hitMapDef hitMap, unsigned int cdCol = 1, unsigned int cdRow = 1, unsigned int lv1diff = 3);
   void          mergeLv1(Clusterizer::clusterMapDef &clusterMap, int maxMerge);
  
 private:
   
   std::stringstream  ss_;
 
} ;

#endif
