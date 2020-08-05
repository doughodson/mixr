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
 *  FILENAME:   CigiShortSymbolCtrlV3_3.cpp
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
 *  04/03/2008 Greg Basler                       2.1.0
 *  Corrected pointer incrementing.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#define _EXPORT_CCL_


#include "CigiShortSymbolCtrlV3_3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"



// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiShortSymbolCtrlV3_3::CigiShortSymbolCtrlV3_3(void)
{
   PacketID = CIGI_SHORT_SYMBOL_CONTROL_PACKET_ID_V3_3;
   PacketSize = CIGI_SHORT_SYMBOL_CONTROL_PACKET_SIZE_V3_3;
   Version = 3;
   MinorVersion = 3;

   SymbolID = 0;
   SymbolState = CigiBaseSymbolCtrl::Hidden;
   AttachState = CigiBaseSymbolCtrl::Detach;
   FlashCtrl = CigiBaseSymbolCtrl::Continue;
   InheritColor = CigiBaseSymbolCtrl::NotInherit;
   DatumType[0] = None;
   DatumType[1] = None;
   Datum[0].UIntValue = 0;
   Datum[1].UIntValue = 0;

}

// ================================================
// ~CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiShortSymbolCtrlV3_3::~CigiShortSymbolCtrlV3_3(void)
{
}



// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiShortSymbolCtrlV3_3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiShortSymbolCtrlV3_3 * Data =
      ( CigiShortSymbolCtrlV3_3 *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->SymbolID;

   unsigned char HDta = 0;
   HDta |= (Data->SymbolState & 0x03);
   HDta |= ((Data->AttachState << 2) & 0x04);
   HDta |= ((Data->FlashCtrl << 3) & 0x08);
   HDta |= ((Data->InheritColor << 4) & 0x10);
   *CDta.c++ = HDta;

   *CDta.c++ = 0;

   *CDta.c++ = Data->DatumType[0];
   *CDta.c++ = Data->DatumType[1];
   *CDta.l++ = Data->Datum[0].UIntValue;
   *CDta.l++ = Data->Datum[1].UIntValue;


   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiShortSymbolCtrlV3_3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;
   unsigned char HDta = 0;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {

      SymbolID = *CDta.s++;

      HDta = *CDta.c++;

      CDta.c++;

      DatumType[0] = (DatumTypeGrp)*CDta.c++;
      DatumType[1] = (DatumTypeGrp)*CDta.c++;
      Datum[0].UIntValue = *CDta.l++;
      Datum[1].UIntValue = *CDta.l++;

   }
   else
   {

      CigiSwap2(&SymbolID, CDta.s++);

      HDta = *CDta.c++;

      CDta.c++;

      DatumType[0] = (DatumTypeGrp)*CDta.c++;
      DatumType[1] = (DatumTypeGrp)*CDta.c++;
      CigiSwap4(&Datum[0].UIntValue, CDta.l++);
      CigiSwap4(&Datum[1].UIntValue, CDta.l++);

   }

   SymbolState = (CigiBaseSymbolCtrl::SymbolStateGrp)(HDta & 0x03);
   AttachState = (CigiBaseSymbolCtrl::AttachStateGrp)((HDta >> 2) & 0x01);
   FlashCtrl = (CigiBaseSymbolCtrl::FlashCtrlGrp)((HDta >> 3) & 0x01);
   InheritColor = (CigiBaseSymbolCtrl::InheritColorGrp)((HDta >> 4) & 0x01);

   return(PacketSize);

}




