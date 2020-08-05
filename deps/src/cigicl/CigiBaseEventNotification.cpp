/** <pre>
 *  The SDK is used to create and format CIGI compliant messages.
 *  Copyright (c) 2001-2005 The Boeing Company
 *  
 *  This library is free software; you can redistribute it and/or modify it 
 *  under the terms of the GNU Lesser General Public License as published by 
 *  the Free Software Foundation; either version 2.1 of the License, or (at 
 *  your option) any later version.
 *  
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for more 
 *  details.
 *  
 *  You should have received a copy of the GNU Lesser General Public License 
 *  along with this library; if not, write to the Free Software Foundation, 
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *  
 *  FILENAME:   CigiBaseEventNotification.cpp
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface (CIGI) SDK
 *  
 *  PROGRAM DESCRIPTION: 
 *  ...
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  09/17/2003 Greg Basler                       CIGI_CR_DR_1
 *  Initial Release.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Defined _EXPORT_CCL_ for exporting the class in a Windows DLL.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Changed native C++ types to use CCL-defined types instead where appropriate.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiBaseEventNotification.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"

// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiBaseEnvRgnCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseEventNotification::CigiBaseEventNotification()
{

}



// ================================================
// ~CigiBaseEnvRgnCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseEventNotification::~CigiBaseEventNotification()
{

}


// ====================================================================
// Conversion Control
// ====================================================================


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEventNotification::GetCnvt(CigiVersionID &CnvtVersion,
                                       CigiCnvtInfoType::Type &CnvtInfo)
{
   if(CnvtVersion.CigiMajorVersion < 3)
   {
      CnvtInfo.ProcID = CigiProcessType::ProcNone;
      CnvtInfo.CnvtPacketID = 0;
   }
   else
   {
      CnvtInfo.ProcID = CigiProcessType::ProcStd;
      CnvtInfo.CnvtPacketID = CIGI_EVENT_NOTIFICATION_PACKET_ID_V3;
   }
   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================



// ================================================
// RgnState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEventNotification::SetEventData(const Cigi_uint32 EventDataIn, int ndx, bool bndchk)
{
   if((ndx < 0)||(ndx > 2))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Event Data Index",ndx,0,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   EventData.ul[ndx] = EventDataIn;
   return(CIGI_SUCCESS);
}



// ================================================
// RgnState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEventNotification::SetEventData(const Cigi_int32 EventDataIn, int ndx, bool bndchk)
{
   if((ndx < 0)||(ndx > 2))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Event Data Index",ndx,0,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   EventData.l[ndx] = EventDataIn;
   return(CIGI_SUCCESS);
}



// ================================================
// RgnState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEventNotification::SetEventData(const float EventDataIn, int ndx, bool bndchk)
{
   if((ndx < 0)||(ndx > 2))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Event Data Index",ndx,0,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }

   EventData.f[ndx] = EventDataIn;
   return(CIGI_SUCCESS);
}



// ================================================
// RgnState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Cigi_uint32 CigiBaseEventNotification::GetULEventData(int ndx)
{
   if((ndx < 0)||(ndx > 2))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Event Data Index",ndx,0,2);
#endif
      if(ndx < 0)
         return(EventData.ul[0]);
      else
         return(EventData.ul[2]);
   }

   return(EventData.ul[ndx]);
}




// ================================================
// RgnState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Cigi_int32 CigiBaseEventNotification::GetLEventData(int ndx)
{
   if((ndx < 0)||(ndx > 2))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Event Data Index",ndx,0,2);
#endif
      if(ndx < 0)
         return(EventData.l[0]);
      else
         return(EventData.l[2]);
   }

   return(EventData.l[ndx]);
}




// ================================================
// RgnState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
float CigiBaseEventNotification::GetFEventData(int ndx)
{
   if((ndx < 0)||(ndx > 2))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Event Data Index",ndx,0,2);
#endif
      if(ndx < 0)
         return((float)EventData.ul[0]);
      else
         return((float)EventData.ul[2]);
   }

   return(EventData.f[ndx]);
}





