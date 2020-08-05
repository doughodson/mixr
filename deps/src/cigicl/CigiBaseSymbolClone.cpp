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
 *  FILENAME:   CigiBaseSymbolClone.cpp
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
 *  04/03/2008 Greg Basler                       CIGI_SYM_1
 *  Initial Release.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#define _EXPORT_CCL_


#include "CigiBaseSymbolClone.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolClone::CigiBaseSymbolClone(void)
{
}

// ================================================
// ~CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolClone::~CigiBaseSymbolClone(void)
{
}


// ====================================================================
// Conversion Control
// ====================================================================
int CigiBaseSymbolClone::GetCnvt(CigiVersionID &CnvtVersion,
                                CigiCnvtInfoType::Type &CnvtInfo)
{
   if(CnvtVersion.GetCombinedCigiVersion() < 0x303)
   {
      CnvtInfo.ProcID = CigiProcessType::ProcNone;
      CnvtInfo.CnvtPacketID = 0;
   }
   else
   {
      CnvtInfo.ProcID = CigiProcessType::ProcStd;
      CnvtInfo.CnvtPacketID = CIGI_SYMBOL_CLONE_PACKET_ID_V3_3;
   }

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================

// ================================================
// SetSymbolState
// ================================================
int CigiBaseSymbolClone::SetSourceType(const SymbolSourceGrp SourceTypeIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((SourceTypeIn < 0)||(SourceTypeIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("SymbolSourceType", (int)SourceTypeIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   SourceType = SourceTypeIn;

   return(CIGI_SUCCESS);

}


