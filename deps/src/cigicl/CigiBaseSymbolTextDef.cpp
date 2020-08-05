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
 *  FILENAME:   CigiBaseSymbolTextDef.cpp
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


#include "CigiBaseSymbolTextDef.h"
#include "CigiExceptions.h"

using namespace std;

// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolTextDef::CigiBaseSymbolTextDef(void)
{
}

// ================================================
// ~CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolTextDef::~CigiBaseSymbolTextDef(void)
{
}


// ================================================
// CigiBaseSymbolTextDef - Copy constructor
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolTextDef::CigiBaseSymbolTextDef(const CigiBaseSymbolTextDef &BaseIn)
{
   SymbolID = BaseIn.SymbolID;
   Alignment = BaseIn.Alignment;
   Orientation = BaseIn.Orientation;
   FontSize = BaseIn.FontSize;
   VariableDataSize = BaseIn.VariableDataSize;
   Text = BaseIn.Text;
}

// ================================================
// Copy operator
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSymbolTextDef & CigiBaseSymbolTextDef::operator=(const CigiBaseSymbolTextDef &BaseIn)
{
   SymbolID = BaseIn.SymbolID;
   Alignment = BaseIn.Alignment;
   Orientation = BaseIn.Orientation;
   FontSize = BaseIn.FontSize;
   VariableDataSize = BaseIn.VariableDataSize;
   Text = BaseIn.Text;

   return(*this);
}


// ====================================================================
// Conversion Control
// ====================================================================
int CigiBaseSymbolTextDef::GetCnvt(CigiVersionID &CnvtVersion,
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
      CnvtInfo.CnvtPacketID = CIGI_SYMBOL_TEXT_DEFINITION_PACKET_ID_V3_3;
   }

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================


// ================================================
// SetAlignment
// ================================================
int CigiBaseSymbolTextDef::SetAlignment(const AlignmentGrp AlignmentIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((AlignmentIn < 0)||(AlignmentIn > 8)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Alignment", (int)AlignmentIn,0,8);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Alignment = AlignmentIn;

   return(CIGI_SUCCESS);

}

// ================================================
// SetOrientation
// ================================================
int CigiBaseSymbolTextDef::SetOrientation(const OrientationGrp OrientationIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((OrientationIn < 0)||(OrientationIn > 3)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Orientation", (int)OrientationIn,0,3);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Orientation = OrientationIn;

   return(CIGI_SUCCESS);

}

// ================================================
// SetFontSize
// ================================================
int CigiBaseSymbolTextDef::SetFontSize(const float FontSizeIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && (FontSize < 0.0))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("FontSize", (double)FontSizeIn,">=",0.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   FontSize = FontSizeIn;

   return(CIGI_SUCCESS);

}

// ================================================
// SetText
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSymbolTextDef::SetText(const std::string &TextIn, bool bndchk)
{

   int TxtSz = TextIn.size();

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((TxtSz < 0)||(TxtSz > MaxCharCnt)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("TextSize", (int)TxtSz,1,MaxCharCnt);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif


   Text = TextIn;

   if(TxtSz > MaxCharCnt)
   {
      Text.resize(MaxCharCnt,0);
      TxtSz = MaxCharCnt;
   }

   int Term = 0;

   if(TxtSz < 4)
      Term = 4 - TxtSz;
   else
      Term = 8 - ((TxtSz - 4) % 8);

   VariableDataSize = TxtSz + Term;

   return(CIGI_SUCCESS);

}


