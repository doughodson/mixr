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
 *  FILENAME:   CigiBaseCircleSymbolData.cpp
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


#include "CigiBaseCircleSymbolData.h"
#include "CigiExceptions.h"



// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseCircleSymbolData::CigiBaseCircleSymbolData(void)
{
}

// ================================================
// ~CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseCircleSymbolData::~CigiBaseCircleSymbolData(void)
{
}


// ====================================================================
// Accessors
// ====================================================================

// ================================================
// SetRadius
// ================================================
int CigiBaseCircleSymbolData::SetRadius(const float RadiusIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((RadiusIn < 0.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("RadiusIn", (int)RadiusIn,">=",0.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Radius = RadiusIn;

   return(CIGI_SUCCESS);

}


// ================================================
// InnerRadius
// ================================================
int CigiBaseCircleSymbolData::SetInnerRadius(const float InnerRadiusIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((InnerRadiusIn < 0.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("InnerRadiusIn", (int)InnerRadiusIn,">=",0.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   InnerRadius = InnerRadiusIn;

   return(CIGI_SUCCESS);

}


// ================================================
// StartAngle
// ================================================
int CigiBaseCircleSymbolData::SetStartAngle(const float StartAngleIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((StartAngleIn < 0.0)||(StartAngleIn > 360.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("StartAngle", (double)StartAngleIn,0.0,360.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   StartAngle = StartAngleIn;

   return(CIGI_SUCCESS);

}


// ================================================
// EndAngle
// ================================================
int CigiBaseCircleSymbolData::SetEndAngle(const float EndAngleIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((EndAngleIn < 0.0)||(EndAngleIn > 360.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("EndAngle", (double)EndAngleIn,0.0,360.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   EndAngle = EndAngleIn;

   return(CIGI_SUCCESS);

}


