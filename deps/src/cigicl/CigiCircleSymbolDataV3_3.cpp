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
 *  FILENAME:   CigiCircleSymbolDataV3_3.cpp
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
 *  01/17/2008 Greg Basler                       CIGI_CR_DR_1
 *  Initial Release.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#define _EXPORT_CCL_


#include "CigiCircleSymbolDataV3_3.h"
#include "CigiExceptions.h"
#include "CigiSwapping.h"



// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCircleSymbolDataV3_3::CigiCircleSymbolDataV3_3(void)
{
   CenterUPosition = 0.0f;
   CenterVPosition = 0.0f;
   Radius = 0.0f;
   InnerRadius = 0.0f;
   StartAngle = 0.0f;
   EndAngle = 0.0f;
}

// ================================================
// ~CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCircleSymbolDataV3_3::~CigiCircleSymbolDataV3_3(void)
{
}


// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCircleSymbolDataV3_3::Pack(CigiBaseCircleSymbolData * Base, Cigi_uint8 * Buff) const
{
   PackPointer CDta;

   CigiCircleSymbolDataV3_3 * Data =
      ( CigiCircleSymbolDataV3_3 *)Base;

   CDta.c = Buff;

   *CDta.f++ = Data->CenterUPosition;
   *CDta.f++ = Data->CenterVPosition;
   *CDta.f++ = Data->Radius;
   *CDta.f++ = Data->InnerRadius;
   *CDta.f++ = Data->StartAngle;
   *CDta.f++ = Data->EndAngle;

   return(CIGI_CIRCLE_SYMBOL_DATA_SIZE_V3_3);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCircleSymbolDataV3_3::Unpack(Cigi_uint8 * Buff, bool Swap)
{
   PackPointer CDta;
   unsigned char HDta = 0;

   CDta.c = Buff;

   if(!Swap)
   {
      CenterUPosition = *CDta.f++;
      CenterVPosition = *CDta.f++;
      Radius = *CDta.f++;
      InnerRadius = *CDta.f++;
      StartAngle = *CDta.f++;
      EndAngle = *CDta.f++;
   }
   else
   {
      CigiSwap4(&CenterUPosition, CDta.f++);
      CigiSwap4(&CenterVPosition, CDta.f++);
      CigiSwap4(&Radius, CDta.f++);
      CigiSwap4(&InnerRadius, CDta.f++);
      CigiSwap4(&StartAngle, CDta.f++);
      CigiSwap4(&EndAngle, CDta.f++);
   }

   return(CIGI_CIRCLE_SYMBOL_DATA_SIZE_V3_3);

}




