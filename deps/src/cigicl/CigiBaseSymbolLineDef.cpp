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
 *  FILENAME:   CigiBaseSymbolLineDef.cpp
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


#include "CigiBaseSymbolLineDef.h"
#include "CigiExceptions.h"

using namespace std;

// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiBaseSymbolLineDef
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolLineDef::CigiBaseSymbolLineDef(void)
{
}

// ================================================
// ~CigiBaseSymbolLineDef
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolLineDef::~CigiBaseSymbolLineDef(void)
{
   CigiBaseVertexSymbolData *pVtx;
   vector<CigiBaseVertexSymbolData *>::iterator iVtx;
   for(iVtx=Vertices.begin();iVtx!=Vertices.end();iVtx++)
   {
      pVtx = *iVtx;
      if(pVtx != NULL)
         delete pVtx;
   }
   Vertices.clear();
}


// ================================================
// CigiBaseSymbolLineDef - Copy constructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolLineDef::CigiBaseSymbolLineDef(const CigiBaseSymbolLineDef &BaseIn)
{
   SymbolID = BaseIn.SymbolID;
   Primitive = BaseIn.Primitive;
   StipplePattern = BaseIn.StipplePattern;
   LineWidth = BaseIn.LineWidth;
   StipplePatternLen = BaseIn.StipplePatternLen;

   // The Vertex Vector should be copied in the specific version class
}

// ================================================
// Copy operator
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolLineDef & CigiBaseSymbolLineDef::operator=(const CigiBaseSymbolLineDef &BaseIn)
{
   SymbolID = BaseIn.SymbolID;
   Primitive = BaseIn.Primitive;
   StipplePattern = BaseIn.StipplePattern;
   LineWidth = BaseIn.LineWidth;
   StipplePatternLen = BaseIn.StipplePatternLen;

   // The Vertex Vector should be copied in the specific version class

   return(*this);
}


// ====================================================================
// Conversion Control
// ====================================================================
int CigiBaseSymbolLineDef::GetCnvt(CigiVersionID &CnvtVersion,
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
      CnvtInfo.CnvtPacketID = CIGI_SYMBOL_LINE_DEFINITION_PACKET_ID_V3_3;
   }

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================


// ================================================
// SetPrimitive
// ================================================
int CigiBaseSymbolLineDef::SetPrimitive(const PrimitiveGrp PrimitiveIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((PrimitiveIn < 0)||(PrimitiveIn > 6)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Primitive", (int)PrimitiveIn,0,6);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Primitive = PrimitiveIn;

   return(CIGI_SUCCESS);

}

// ================================================
// SetLineWidth
// ================================================
int CigiBaseSymbolLineDef::SetLineWidth(const float LineWidthIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && (LineWidthIn < 0.0))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("LineWidth", (double)LineWidthIn,">=",0.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   LineWidth = LineWidthIn;

   return(CIGI_SUCCESS);

}

// ================================================
// SetStipplePatternLen
// ================================================
int CigiBaseSymbolLineDef::SetStipplePatternLen(const float StipplePatternLenIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && (StipplePatternLenIn < 0.0))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("StipplePatternLen", (double)StipplePatternLenIn,">=",0.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   StipplePatternLen = StipplePatternLenIn;

   return(CIGI_SUCCESS);

}

// ================================================
// GetVertexCount
// ================================================
int CigiBaseSymbolLineDef::GetVertexCount(void)
{
   return(Vertices.size());
}

// ================================================
// GetVertex
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseVertexSymbolData * CigiBaseSymbolLineDef::GetVertex(int VertexIndex, bool bndchk)
{
   CigiBaseVertexSymbolData * pVtx = NULL;

   if((VertexIndex >= 0) && ((Cigi_uint8)VertexIndex < Vertices.size()))
   {
      pVtx = Vertices[VertexIndex];
   }
   else
   {
#ifndef CIGI_NO_BND_CHK
      if(bndchk)
      {
#ifndef CIGI_NO_EXCEPT
         throw CigiValueOutOfRangeException("VertexIndex", (int)VertexIndex,0,Vertices.size());
#endif
      }
#endif
   }

   return(pVtx);
}

// ================================================
// ClearVertices
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CigiBaseSymbolLineDef::ClearVertices(void)
{
   CigiBaseVertexSymbolData *pVtx;
   vector<CigiBaseVertexSymbolData *>::iterator iVtx;
   for(iVtx=Vertices.begin();iVtx!=Vertices.end();iVtx++)
   {
      pVtx = *iVtx;
      if(pVtx != NULL)
         delete pVtx;
   }
   Vertices.clear();
   VariableDataSize = 0;
}


