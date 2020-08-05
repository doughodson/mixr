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
 *  FILENAME:   CigiSymbolCircleDefV3_3.cpp
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


#include "CigiSymbolCircleDefV3_3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"

using namespace std;

// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSymbolCircleDefV3_3::CigiSymbolCircleDefV3_3(void)
{
   PacketID = CIGI_SYMBOL_CIRCLE_DEFINITION_PACKET_ID_V3_3;
   PacketSize = CIGI_SYMBOL_CIRCLE_DEFINITION_PACKET_SIZE_V3_3;
   Version = 3;
   MinorVersion = 3;
   VariableDataSize = 0;

   SymbolID = 0;
   DrawingStyle = Line;
   StipplePattern = 0;
   LineWidth = 0.0f;
   StipplePatternLen = 0.0f;

}

// ================================================
// ~CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSymbolCircleDefV3_3::~CigiSymbolCircleDefV3_3(void)
{
}

// ================================================
// CigiSymbolCircleDefV3_3 - Copy constructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSymbolCircleDefV3_3::CigiSymbolCircleDefV3_3(const CigiSymbolCircleDefV3_3 &BaseIn)
: CigiBaseSymbolCircleDef((CigiBaseSymbolCircleDef &)BaseIn)
{
   PacketID = CIGI_SYMBOL_CIRCLE_DEFINITION_PACKET_ID_V3_3;
   PacketSize = CIGI_SYMBOL_CIRCLE_DEFINITION_PACKET_SIZE_V3_3;
   Version = 3;
   MinorVersion = 3;

   VariableDataSize = 0;

   SymbolID = BaseIn.SymbolID;
   DrawingStyle = BaseIn.DrawingStyle;
   StipplePattern = BaseIn.StipplePattern;
   LineWidth = BaseIn.LineWidth;
   StipplePatternLen = BaseIn.StipplePatternLen;

   CigiCircleSymbolDataV3_3 *NewCircle = NULL;
   CigiCircleSymbolDataV3_3 *pCircle;
   vector<CigiBaseCircleSymbolData *>::const_iterator iCircle;
   for(iCircle=BaseIn.Circles.begin();iCircle!=BaseIn.Circles.end();iCircle++)
   {
      pCircle = (CigiCircleSymbolDataV3_3 *)*iCircle;
      if(pCircle != NULL)
      {
         NewCircle = new CigiCircleSymbolDataV3_3;
         *NewCircle = *pCircle;
         Circles.push_back((CigiBaseCircleSymbolData *)NewCircle);
         VariableDataSize += CIGI_CIRCLE_SYMBOL_DATA_SIZE_V3_3;
      }
   }
}

// ================================================
// Copy Operator
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSymbolCircleDefV3_3 & CigiSymbolCircleDefV3_3::operator=(const CigiSymbolCircleDefV3_3 &BaseIn)
{
   PacketID = CIGI_SYMBOL_CIRCLE_DEFINITION_PACKET_ID_V3_3;
   PacketSize = CIGI_SYMBOL_CIRCLE_DEFINITION_PACKET_SIZE_V3_3;
   Version = 3;
   MinorVersion = 3;

   CigiBaseCircleSymbolData *pbCircle;
   vector<CigiBaseCircleSymbolData *>::iterator ibCircle;
   for(ibCircle=Circles.begin();ibCircle!=Circles.end();ibCircle++)
   {
      pbCircle = *ibCircle;
      if(pbCircle != NULL)
         delete pbCircle;
   }
   Circles.clear();
   VariableDataSize = 0;

   SymbolID = BaseIn.SymbolID;
   DrawingStyle = BaseIn.DrawingStyle;
   StipplePattern = BaseIn.StipplePattern;
   LineWidth = BaseIn.LineWidth;
   StipplePatternLen = BaseIn.StipplePatternLen;

   CigiCircleSymbolDataV3_3 *NewCircle = NULL;
   CigiCircleSymbolDataV3_3 *pCircle;
   vector<CigiBaseCircleSymbolData *>::const_iterator iCircle;
   for(iCircle=BaseIn.Circles.begin();iCircle!=BaseIn.Circles.end();iCircle++)
   {
      pCircle = (CigiCircleSymbolDataV3_3 *)*iCircle;
      if(pCircle != NULL)
      {
         NewCircle = new CigiCircleSymbolDataV3_3;
         *NewCircle = *pCircle;
         Circles.push_back((CigiBaseCircleSymbolData *)NewCircle);
         VariableDataSize += CIGI_CIRCLE_SYMBOL_DATA_SIZE_V3_3;
      }
   }

   return(*this);
}



// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSymbolCircleDefV3_3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiSymbolCircleDefV3_3 * Data =
      ( CigiSymbolCircleDefV3_3 *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = (PacketSize + Data->VariableDataSize);

   *CDta.s++ = Data->SymbolID;

   *CDta.c++ = (Data->DrawingStyle != 0) ? 1 : 0;

   *CDta.c++ = 0;

   *CDta.s++ = Data->StipplePattern;
   *CDta.f++ = Data->LineWidth;
   *CDta.f++ = Data->StipplePatternLen;

   int Cnt;
   CigiCircleSymbolDataV3_3 *pCircle;
   vector<CigiBaseCircleSymbolData *>::iterator iCircle;
   for(iCircle=Data->Circles.begin();iCircle!=Data->Circles.end();iCircle++)
   {
      pCircle = (CigiCircleSymbolDataV3_3 *)*iCircle;
      if(pCircle != NULL)
      {
         Cnt = pCircle->Pack((CigiBaseCircleSymbolData *)pCircle,CDta.c);
         CDta.c += Cnt;
      }
   }

   return(PacketSize + Data->VariableDataSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSymbolCircleDefV3_3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;
   unsigned char HDta = 0;

   CDta.c = Buff;

   // Clear the current Circle Vector
   CigiBaseCircleSymbolData *pbCircle;
   vector<CigiBaseCircleSymbolData *>::iterator ibCircle;
   for(ibCircle=Circles.begin();ibCircle!=Circles.end();ibCircle++)
   {
      pbCircle = *ibCircle;
      if(pbCircle != NULL)
         delete pbCircle;
   }
   Circles.clear();
   VariableDataSize = 0;

   CDta.c++;  // Step over packet id

   // Determine variable data size;
   int PcktRemain = *CDta.c++;
   PcktRemain -= CIGI_SYMBOL_CIRCLE_DEFINITION_PACKET_SIZE_V3_3;
   VariableDataSize = PcktRemain;

   if(!Swap)
   {
      SymbolID = *CDta.s++;
      HDta = *CDta.c++;
      CDta.c++;
      StipplePattern = *CDta.s++;
      LineWidth = *CDta.f++;
      StipplePatternLen = *CDta.f++;
   }
   else
   {
      CigiSwap2(&SymbolID, CDta.s++);
      HDta = *CDta.c++;
      CDta.c++;
      CigiSwap2(&StipplePattern, CDta.s++);
      CigiSwap4(&LineWidth, CDta.f++);
      CigiSwap4(&StipplePatternLen, CDta.f++);
   }

   DrawingStyle = (HDta != 0) ? Fill : Line ;

   // Unpack Circles
   int cnt = 0;
   CigiCircleSymbolDataV3_3 *pCircle;
   while(PcktRemain >= CIGI_CIRCLE_SYMBOL_DATA_SIZE_V3_3)
   {
      pCircle = new CigiCircleSymbolDataV3_3;
      Circles.push_back((CigiBaseCircleSymbolData *)pCircle);
      cnt = pCircle->Unpack(CDta.c,Swap);
      PcktRemain -= cnt;
      CDta.c += cnt;
   }

   return(PacketSize + VariableDataSize);

}



// ====================================================================
// Circle Operation
// ====================================================================

// ================================================
// AddCircle
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseCircleSymbolData * CigiSymbolCircleDefV3_3::AddCircle(void)
{
   CigiCircleSymbolDataV3_3 *NewCircle = NULL;
   int Cnt = Circles.size();

   if(Cnt < 10)
   {
      NewCircle = new CigiCircleSymbolDataV3_3;
      Circles.push_back((CigiBaseCircleSymbolData *)NewCircle);
      VariableDataSize += CIGI_CIRCLE_SYMBOL_DATA_SIZE_V3_3;
   }

   return((CigiBaseCircleSymbolData *)NewCircle);
}

// ================================================
// RemoveCircle
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSymbolCircleDefV3_3::RemoveCircle(int CircleIndex, bool bndchk)
{
   int stat = CIGI_SUCCESS;

   int Cnt = Circles.size();

   if((CircleIndex >= 0) && (CircleIndex < Cnt))
   {
      vector<CigiBaseCircleSymbolData *>::iterator iCircle = Circles.begin();

      iCircle += CircleIndex;

      CigiCircleSymbolDataV3_3 *pCircle =
         (CigiCircleSymbolDataV3_3 *)*iCircle;
      if(pCircle != NULL)
         delete pCircle;

      Circles.erase(iCircle);

      VariableDataSize -= CIGI_CIRCLE_SYMBOL_DATA_SIZE_V3_3;
      if(VariableDataSize < 0)
         VariableDataSize = 0;

   }
   else
   {
#ifndef CIGI_NO_BND_CHK
      if(bndchk)
      {
#ifndef CIGI_NO_EXCEPT
         throw CigiValueOutOfRangeException("CircleIndex", (int)CircleIndex,0,Cnt);
#endif
         stat = CIGI_ERROR_VALUE_OUT_OF_RANGE;
      }
#endif
   }

   return(stat);
}



