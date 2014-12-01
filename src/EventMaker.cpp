/****************************************************************************
** Author: Stefano Terzo
**
** Max-Planck-instiut fuer Physik (Werner-Heisenberg-Institut)
** Foehringer Ring 6, D-80805 Muenchen, Germany
**
****************************************************************************/

#include "EventMaker.h"

EventMaker::EventMaker(bool quiet, bool readTimeStamp)
{
  quiet_ = quiet;
  readTimeStamp_ = readTimeStamp;
}

//============================================================================
int EventMaker::string_to_int(std::string theString)
{
  std::istringstream data_s;
  data_s.str(theString)	  ;
  int n			  ;
  data_s >> n		  ;
  
  return n                ;
}
//=============
