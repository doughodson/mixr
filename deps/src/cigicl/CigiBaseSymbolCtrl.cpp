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
 *  FILENAME:   CigiBaseSymbolCtrl.cpp
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
 *  03/11/2008 Greg Basler                       CIGI_SYM_1
 *  Initial Release.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#define _EXPORT_CCL_


#include "CigiBaseSymbolCtrl.h"
#include "CigiExceptions.h"



// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolCtrl::CigiBaseSymbolCtrl(void)
{
}

// ================================================
// ~CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolCtrl::~CigiBaseSymbolCtrl(void)
{
}


// ====================================================================
// Conversion Control
// ====================================================================
int CigiBaseSymbolCtrl::GetCnvt(CigiVersionID &CnvtVersion,
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
      CnvtInfo.CnvtPacketID = CIGI_SYMBOL_CONTROL_PACKET_ID_V3_3;
   }

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================

// ================================================
// SetSymbolState
// ================================================
int CigiBaseSymbolCtrl::SetSymbolState(const SymbolStateGrp SymbolStateIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((SymbolStateIn < 0)||(SymbolStateIn > 2)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("SymbolState", (int)SymbolStateIn,0,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   SymbolState = SymbolStateIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetAttachState
// ================================================
int CigiBaseSymbolCtrl::SetAttachState(const AttachStateGrp AttachStateIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((AttachStateIn < 0)||(AttachStateIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("AttachState", (int)AttachStateIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   AttachState = AttachStateIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetFlashCtrl
// ================================================
int CigiBaseSymbolCtrl::SetFlashCtrl(const FlashCtrlGrp FlashCtrlIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((FlashCtrlIn < 0)||(FlashCtrlIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("FlashCtrl", (int)FlashCtrlIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   FlashCtrl = FlashCtrlIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetInheritColor
// ================================================
int CigiBaseSymbolCtrl::SetInheritColor(const InheritColorGrp InheritColorIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((InheritColorIn < 0)||(InheritColorIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("InheritColor", (int)InheritColorIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   InheritColor = InheritColorIn;

   return(CIGI_SUCCESS);

}



// ================================================
// SetFlashDutyCycle
// ================================================
int CigiBaseSymbolCtrl::SetFlashDutyCycle(const Cigi_uint8 FlashDutyCycleIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((FlashDutyCycleIn < 0)||(FlashDutyCycleIn > 100)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("FlashDutyCycle", (int)FlashDutyCycleIn,0,100);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   FlashDutyCycle = FlashDutyCycleIn;

   return(CIGI_SUCCESS);

}




