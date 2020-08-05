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
 *  FILENAME:   CigiBaseSymbolCircleDef.cpp
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


#include "CigiBaseSymbolCircleDef.h"
#include "CigiExceptions.h"

using namespace std;

// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiBaseSymbolCircleDef
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolCircleDef::CigiBaseSymbolCircleDef(void)
{
}

// ================================================
// ~CigiBaseSymbolCircleDef
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolCircleDef::~CigiBaseSymbolCircleDef(void)
{
   CigiBaseCircleSymbolData *pCircle;
   vector<CigiBaseCircleSymbolData *>::iterator iCircle;
   for(iCircle=Circles.begin();iCircle!=Circles.end();iCircle++)
   {
      pCircle = *iCircle;
      if(pCircle != NULL)
         delete pCircle;
   }
   Circles.clear();
}


// ================================================
// CigiBaseSymbolCircleDef - Copy constructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolCircleDef::CigiBaseSymbolCircleDef(const CigiBaseSymbolCircleDef &BaseIn)
{
   SymbolID = BaseIn.SymbolID;
   DrawingStyle = BaseIn.DrawingStyle;
   StipplePattern = BaseIn.StipplePattern;
   LineWidth = BaseIn.LineWidth;
   StipplePatternLen = BaseIn.StipplePatternLen;

   // The Circle Vector should be copied in the specific version class
}

// ================================================
// Copy operator
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolCircleDef & CigiBaseSymbolCircleDef::operator=(const CigiBaseSymbolCircleDef &BaseIn)
{
   SymbolID = BaseIn.SymbolID;
   DrawingStyle = BaseIn.DrawingStyle;
   StipplePattern = BaseIn.StipplePattern;
   LineWidth = BaseIn.LineWidth;
   StipplePatternLen = BaseIn.StipplePatternLen;

   // The Circle Vector should be copied in the specific version class

   return(*this);
}


// ====================================================================
// Conversion Control
// ====================================================================
int CigiBaseSymbolCircleDef::GetCnvt(CigiVersionID &CnvtVersion,
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
      CnvtInfo.CnvtPacketID = CIGI_SYMBOL_CIRCLE_DEFINITION_PACKET_ID_V3_3;
   }

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================


// ================================================
// SetDrawingStyle
// ================================================
int CigiBaseSymbolCircleDef::SetDrawingStyle(const DrawingStyleGrp DrawingStyleIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((DrawingStyleIn < 0)||(DrawingStyleIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DrawingStyle", (int)DrawingStyleIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   DrawingStyle = DrawingStyleIn;

   return(CIGI_SUCCESS);

}

// ================================================
// SetLineWidth
// ================================================
int CigiBaseSymbolCircleDef::SetLineWidth(const float LineWidthIn, bool bndchk)
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
int CigiBaseSymbolCircleDef::SetStipplePatternLen(const float StipplePatternLenIn, bool bndchk)
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
// GetCircleCount
// ================================================
int CigiBaseSymbolCircleDef::GetCircleCount(void)
{
   return(Circles.size());
}

// ================================================
// GetCircle
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseCircleSymbolData * CigiBaseSymbolCircleDef::GetCircle(int CircleIndex, bool bndchk)
{
   CigiBaseCircleSymbolData * pCircle = NULL;

   if((CircleIndex >= 0) && ((Cigi_uint8)CircleIndex < Circles.size()))
   {
      pCircle = Circles[CircleIndex];
   }
   else
   {
#ifndef CIGI_NO_BND_CHK
      if(bndchk)
      {
#ifndef CIGI_NO_EXCEPT
         throw CigiValueOutOfRangeException("CircleIndex", (int)CircleIndex,0,Circles.size());
#endif
      }
#endif
   }

   return(pCircle);
}

// ================================================
// ClearCircles
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CigiBaseSymbolCircleDef::ClearCircles(void)
{
   CigiBaseCircleSymbolData *pCircle;
   vector<CigiBaseCircleSymbolData *>::iterator iCircle;
   for(iCircle=Circles.begin();iCircle!=Circles.end();iCircle++)
   {
      pCircle = *iCircle;
      if(pCircle != NULL)
         delete pCircle;
   }
   Circles.clear();
   VariableDataSize = 0;
}


