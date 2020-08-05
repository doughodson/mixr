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
 *  FILENAME:   CigiBaseIGCtrl.cpp
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
 *  Implemented the SetDatabaseID method in the implemenation file instead of 
 *  inlining it in the header file.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiBaseIGCtrl.h"
#include "CigiExceptions.h"
#include "CigiSwapping.h"

// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiBaseIGCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseIGCtrl::CigiBaseIGCtrl()
{

}

// ================================================
// ~CigiBaseIGCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseIGCtrl::~CigiBaseIGCtrl()
{

}


// ====================================================================
// Conversion Control
// ====================================================================


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseIGCtrl::GetCnvt(CigiVersionID &CnvtVersion,
                            CigiCnvtInfoType::Type &CnvtInfo)
{
   CnvtInfo.ProcID = CigiProcessType::ProcIGCtrl;

   // All versions of this packet have the same packet id number
   CnvtInfo.CnvtPacketID = CIGI_IG_CTRL_PACKET_ID_V1;

   return(CIGI_SUCCESS);
}




// ====================================================================
// Accessors
// ====================================================================



// ================================================
// SetDatabaseID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseIGCtrl::SetDatabaseID(const Cigi_int8 DataBaseIDIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((DataBaseIDIn < 0)||(DataBaseIDIn > 127)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DataBaseID",DataBaseIDIn,0,127);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   DatabaseID = DataBaseIDIn;

   return(CIGI_SUCCESS);

}


// ================================================
// SetIGMode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseIGCtrl::SetIGMode(IGModeGrp IGModeIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((IGModeIn < 0)||(IGModeIn > 2)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("IGMode",IGModeIn,0,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   IGMode = IGModeIn;

   return(CIGI_SUCCESS);

}
