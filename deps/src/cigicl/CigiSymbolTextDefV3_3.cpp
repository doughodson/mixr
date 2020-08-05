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
 *  FILENAME:   CigiSymbolTextDefV3_3.cpp
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


#include "CigiSymbolTextDefV3_3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


using namespace std;

// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSymbolTextDefV3_3::CigiSymbolTextDefV3_3(void)
{
   PacketID = CIGI_SYMBOL_TEXT_DEFINITION_PACKET_ID_V3_3;
   PacketSize = CIGI_SYMBOL_TEXT_DEFINITION_PACKET_SIZE_V3_3;
   Version = 3;
   MinorVersion = 3;
   VariableDataSize = 4;

   SymbolID = 0;
   Alignment = TopLeft;
   Orientation = LeftToRight;
   FontID = 0;
   FontSize = 10.0;
   Text.clear();
}

// ================================================
// ~CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSymbolTextDefV3_3::~CigiSymbolTextDefV3_3(void)
{
}

// ================================================
// CigiSymbolTextDefV3_3 - Constructor with text
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSymbolTextDefV3_3::CigiSymbolTextDefV3_3(const std::string &TextIn)
{
   PacketID = CIGI_SYMBOL_TEXT_DEFINITION_PACKET_ID_V3_3;
   PacketSize = CIGI_SYMBOL_TEXT_DEFINITION_PACKET_SIZE_V3_3;
   Version = 3;
   MinorVersion = 3;
   VariableDataSize = 4;

   SymbolID = 0;
   Alignment = TopLeft;
   Orientation = LeftToRight;
   FontID = 0;
   FontSize = 10.0;

   SetText(TextIn,false);
}

// ================================================
// CigiSymbolTextDefV3_3 - Copy Constructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSymbolTextDefV3_3::CigiSymbolTextDefV3_3(const CigiSymbolTextDefV3_3 &BaseIn)
: CigiBaseSymbolTextDef((CigiBaseSymbolTextDef &)BaseIn)
{
   PacketID = CIGI_SYMBOL_TEXT_DEFINITION_PACKET_ID_V3_3;
   PacketSize = CIGI_SYMBOL_TEXT_DEFINITION_PACKET_SIZE_V3_3;
   Version = 3;
   MinorVersion = 3;
}



// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSymbolTextDefV3_3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiSymbolTextDefV3_3 * Data =
      ( CigiSymbolTextDefV3_3 *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = (PacketSize + Data->VariableDataSize);

   *CDta.s++ = Data->SymbolID;

   unsigned char HDta = 0;
   HDta |= (Data->Alignment & 0x0F);
   HDta |= ((Data->Orientation << 4) & 0x30);
   *CDta.c++ = HDta;

   *CDta.c++ = Data->FontID;
   *CDta.s++ = 0;

   *CDta.f++ = Data->FontSize;

   int tSz = Data->Text.size();
   memcpy(CDta.c,Data->Text.c_str(),tSz);
   CDta.c += tSz;
   memset(CDta.c,0,(Data->VariableDataSize - tSz));

   return(PacketSize + Data->VariableDataSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSymbolTextDefV3_3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;
   unsigned char HDta = 0;

   CDta.c = Buff;

   CDta.c++;  // Step over packet id

   // Determine variable data size;
   VariableDataSize = *CDta.c - CIGI_SYMBOL_TEXT_DEFINITION_PACKET_SIZE_V3_3;
   CDta.c++;

   if(!Swap)
   {
      SymbolID = *CDta.s++;

      HDta = *CDta.c++;

      FontID = *CDta.c++;

      CDta.s++;

      FontSize = *CDta.f++;
   }
   else
   {
      CigiSwap2(&SymbolID, CDta.s++);

      HDta = *CDta.c++;

      FontID = *CDta.c++;

      CDta.s++;

      CigiSwap4(&FontSize, CDta.f++);
   }

#ifdef WIN32
   // this version will walk off the end of the array if the array is not 
   // null-terminated
   Text = (char*)CDta.c;
#elif defined( __GNUC__ )
   // this version adds bogus nulls to the end
//   Text.assign( (char*)CDta.c, VariableDataSize );
   // this version is correct, but traverses the string twice (once to 
   // determine length, and again to copy)
   Text.assign( (char*)CDta.c, strnlen( (char*)CDta.c, VariableDataSize ) );
#else
   // unsafe version for unrecognized compilers
   // this version will walk off the end of the array if the array is not 
   // null-terminated
   Text = (char*)CDta.c;
#endif

   Alignment = (AlignmentGrp)(HDta & 0x0f);
   Orientation = (OrientationGrp)((HDta >> 4) & 0x03);

   return(PacketSize + VariableDataSize);

}




