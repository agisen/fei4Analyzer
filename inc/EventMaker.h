/****************************************************************************
** Author: Stefano Terzo
**
** Max-Planck-instiut fuer Physik (Werner-Heisenberg-Institut)
** Foehringer Ring 6, D-80805 Muenchen, Germany
**
****************************************************************************/

#ifndef __EVENTMAKER_H__
#define __EVENTMAKER_H__


#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "FormattedRecord.hh"
#include "macros.h"
#include "ANSIColors.h" 

class EventMaker
{
 public :
 
   typedef struct hit
   {
      int tot ;
      int col ;
      int row ;
      int bcid;
      int l1id;
   } aHit, hitDef ;
 
   typedef std::vector<hitDef>                            hitsDef       ;
   //              #bcid+l1id         #chip  
   typedef std::map<int , std::map<int, hitsDef>     >    hitMapDef     ;
 
 
   EventMaker(bool quiet =  false, bool readTimeStamp = false)  ;
   virtual ~EventMaker(void                    ) {;}
  
   virtual hitMapDef makeEvents(std::string infilename, std::string outfilename = "", int lv1diff = 1, int nevt = -1) = 0;
   void setQuiet(bool quiet) {quiet_=quiet;}
   void setReadTimeStamp(bool readTimeStamp){readTimeStamp_=readTimeStamp;}
   
 protected:
 
   int      string_to_int(std::string theString);
   
 private:
 
   std::stringstream  ss_;
   bool quiet_;
   
   bool readTimeStamp_   ;
 
} ;

#endif
