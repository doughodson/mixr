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
 *  FILENAME:   CigiSymbolLineDefV3_3.cpp
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


#include "CigiSymbolLineDefV3_3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"

using namespace std;

// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSymbolLineDefV3_3::CigiSymbolLineDefV3_3(void)
{
   PacketID = CIGI_SYMBOL_LINE_DEFINITION_PACKET_ID_V3_3;
   PacketSize = CIGI_SYMBOL_LINE_DEFINITION_PACKET_SIZE_V3_3;
   Version = 3;
   MinorVersion = 3;
   VariableDataSize = 0;

   SymbolID = 0;
   Primitive = Points;
   StipplePattern = 0;
   LineWidth = 0.0f;
   StipplePatternLen = 0.0f;

}

// ================================================
// ~CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSymbolLineDefV3_3::~CigiSymbolLineDefV3_3(void)
{
}

// ================================================
// CigiSymbolLineDefV3_3 - Copy constructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSymbolLineDefV3_3::CigiSymbolLineDefV3_3(const CigiSymbolLineDefV3_3 &BaseIn)
: CigiBaseSymbolLineDef((CigiBaseSymbolLineDef &)BaseIn)
{
   PacketID = CIGI_SYMBOL_LINE_DEFINITION_PACKET_ID_V3_3;
   PacketSize = CIGI_SYMBOL_LINE_DEFINITION_PACKET_SIZE_V3_3;
   Version = 3;
   MinorVersion = 3;

   VariableDataSize = 0;

   SymbolID = BaseIn.SymbolID;
   Primitive = BaseIn.Primitive;
   StipplePattern = BaseIn.StipplePattern;
   LineWidth = BaseIn.LineWidth;
   StipplePatternLen = BaseIn.StipplePatternLen;

   CigiVertexSymbolDataV3_3 *NewVertex = NULL;
   CigiVertexSymbolDataV3_3 *pVtx;
   vector<CigiBaseVertexSymbolData *>::const_iterator iVtx;
   for(iVtx=BaseIn.Vertices.begin();iVtx!=BaseIn.Vertices.end();iVtx++)
   {
      pVtx = (CigiVertexSymbolDataV3_3 *)*iVtx;
      if(pVtx != NULL)
      {
         NewVertex = new CigiVertexSymbolDataV3_3;
         *NewVertex = *pVtx;
         Vertices.push_back((CigiBaseVertexSymbolData *)NewVertex);
         VariableDataSize += CIGI_VERTEX_SYMBOL_DATA_SIZE_V3_3;
      }
   }
}

// ================================================
// Copy operator
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSymbolLineDefV3_3 & CigiSymbolLineDefV3_3::operator=(const CigiSymbolLineDefV3_3 &BaseIn)
{
   PacketID = CIGI_SYMBOL_LINE_DEFINITION_PACKET_ID_V3_3;
   PacketSize = CIGI_SYMBOL_LINE_DEFINITION_PACKET_SIZE_V3_3;
   Version = 3;
   MinorVersion = 3;

   CigiBaseVertexSymbolData *pbVertex;
   vector<CigiBaseVertexSymbolData *>::iterator ibVertex;
   for(ibVertex=Vertices.begin();ibVertex!=Vertices.end();ibVertex++)
   {
      pbVertex = *ibVertex;
      if(pbVertex != NULL)
         delete pbVertex;
   }
   Vertices.clear();
   VariableDataSize = 0;

   SymbolID = BaseIn.SymbolID;
   Primitive = BaseIn.Primitive;
   StipplePattern = BaseIn.StipplePattern;
   LineWidth = BaseIn.LineWidth;
   StipplePatternLen = BaseIn.StipplePatternLen;

   CigiVertexSymbolDataV3_3 *NewVertex = NULL;
   CigiVertexSymbolDataV3_3 *pVtx;
   vector<CigiBaseVertexSymbolData *>::const_iterator iVtx;
   for(iVtx=BaseIn.Vertices.begin();iVtx!=BaseIn.Vertices.end();iVtx++)
   {
      pVtx = (CigiVertexSymbolDataV3_3 *)*iVtx;
      if(pVtx != NULL)
      {
         NewVertex = new CigiVertexSymbolDataV3_3;
         *NewVertex = *pVtx;
         Vertices.push_back((CigiBaseVertexSymbolData *)NewVertex);
         VariableDataSize += CIGI_VERTEX_SYMBOL_DATA_SIZE_V3_3;
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
int CigiSymbolLineDefV3_3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiSymbolLineDefV3_3 * Data =
      ( CigiSymbolLineDefV3_3 *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = (PacketSize + Data->VariableDataSize);

   *CDta.s++ = Data->SymbolID;

   *CDta.c++ = (Cigi_uint8)(Data->Primitive & 0x0f);

   *CDta.c++ = 0;

   *CDta.s++ = Data->StipplePattern;
   *CDta.f++ = Data->LineWidth;
   *CDta.f++ = Data->StipplePatternLen;

   int Cnt;
   CigiVertexSymbolDataV3_3 *pVtx;
   vector<CigiBaseVertexSymbolData *>::iterator iVtx;
   for(iVtx=Data->Vertices.begin();iVtx!=Data->Vertices.end();iVtx++)
   {
      pVtx = (CigiVertexSymbolDataV3_3 *)*iVtx;
      if(pVtx != NULL)
      {
         Cnt = pVtx->Pack((CigiBaseVertexSymbolData *)pVtx,CDta.c);
         CDta.c += Cnt;
      }
   }

   return(PacketSize + Data->VariableDataSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSymbolLineDefV3_3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;
   unsigned char HDta = 0;

   CDta.c = Buff;

   // Clear the current Vertex Vector
   CigiBaseVertexSymbolData *pbVtx;
   vector<CigiBaseVertexSymbolData *>::iterator ibVtx;
   for(ibVtx=Vertices.begin();ibVtx!=Vertices.end();ibVtx++)
   {
      pbVtx = *ibVtx;
      if(pbVtx != NULL)
         delete pbVtx;
   }
   Vertices.clear();
   VariableDataSize = 0;

   CDta.c++;  // Step over packet id

   // Determine variable data size;
   int PcktRemain = *CDta.c++;
   PcktRemain -= CIGI_SYMBOL_LINE_DEFINITION_PACKET_SIZE_V3_3;
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

   Primitive = (PrimitiveGrp)(HDta & 0x0f);

   // Unpack Vertices
   int cnt = 0;
   CigiVertexSymbolDataV3_3 *pVtx;
   while(PcktRemain >= CIGI_VERTEX_SYMBOL_DATA_SIZE_V3_3)
   {
      pVtx = new CigiVertexSymbolDataV3_3;
      Vertices.push_back((CigiBaseVertexSymbolData *)pVtx);
      cnt = pVtx->Unpack(CDta.c,Swap);
      PcktRemain -= cnt;
      CDta.c += cnt;
   }

   return(PacketSize + VariableDataSize);

}



// ====================================================================
// Vertex Operation
// ====================================================================

// ================================================
// AddVertex
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseVertexSymbolData * CigiSymbolLineDefV3_3::AddVertex(void)
{
   CigiVertexSymbolDataV3_3 *NewVertex = NULL;
   int Cnt = Vertices.size();

   if(Cnt < 29)
   {
      NewVertex = new CigiVertexSymbolDataV3_3;
      Vertices.push_back((CigiBaseVertexSymbolData *)NewVertex);
      VariableDataSize += CIGI_VERTEX_SYMBOL_DATA_SIZE_V3_3;
   }

   return((CigiBaseVertexSymbolData *)NewVertex);
}

// ================================================
// RemoveVertex
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSymbolLineDefV3_3::RemoveVertex(int VertexIndex, bool bndchk)
{
   int stat = CIGI_SUCCESS;

   int Cnt = Vertices.size();

   if((VertexIndex >= 0) && (VertexIndex < Cnt))
   {
      vector<CigiBaseVertexSymbolData *>::iterator iVtx = Vertices.begin();

      iVtx += VertexIndex;

      CigiVertexSymbolDataV3_3 *pVtx =
         (CigiVertexSymbolDataV3_3 *)*iVtx;
      if(pVtx != NULL)
         delete pVtx;

      Vertices.erase(iVtx);

      VariableDataSize -= CIGI_VERTEX_SYMBOL_DATA_SIZE_V3_3;
      if(VariableDataSize < 0)
         VariableDataSize = 0;

   }
   else
   {
#ifndef CIGI_NO_BND_CHK
      if(bndchk)
      {
#ifndef CIGI_NO_EXCEPT
         throw CigiValueOutOfRangeException("VertexIndex", (int)VertexIndex,0,Cnt);
#endif
         stat = CIGI_ERROR_VALUE_OUT_OF_RANGE;
      }
#endif
   }

   return(stat);
}



