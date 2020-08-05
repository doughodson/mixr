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
 *  FILENAME:   CigiBaseWeatherCtrl.cpp
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
 *  09/17/2003 Greg Basler                       CIGI_CR_DR_1
 *  Initial Release.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Defined _EXPORT_CCL_ for exporting the class in a Windows DLL.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiBaseWeatherCtrl.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiBaseWeatherCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseWeatherCtrl::CigiBaseWeatherCtrl()
{

}



// ================================================
// ~CigiBaseWeatherCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseWeatherCtrl::~CigiBaseWeatherCtrl()
{

}


// ====================================================================
// Conversion Control
// ====================================================================


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseWeatherCtrl::GetCnvt(CigiVersionID &CnvtVersion,
                                 CigiCnvtInfoType::Type &CnvtInfo)
{
   CnvtInfo.ProcID = CigiProcessType::ProcStd;

   // V1 & V2 have the same packet id number
   if(CnvtVersion.CigiMajorVersion < 3)
      CnvtInfo.CnvtPacketID = CIGI_WEATHER_CTRL_PACKET_ID_V2;
   else
      CnvtInfo.CnvtPacketID = CIGI_WEATHER_CTRL_PACKET_ID_V3;

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================


// ================================================
// VisibilityRng
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseWeatherCtrl::SetVisibilityRng(const float VisibilityRngIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && (VisibilityRngIn < 0))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("VisibilityRng",(double)VisibilityRngIn,">=",0.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   VisibilityRng = VisibilityRngIn;

   if(VisibilityRng > 70000.0f)
      Opacity = 100.0f;
   else
      Opacity = ((VisibilityRng/70000.0f)*100.0f);

   return(CIGI_SUCCESS);

}


// ================================================
// Severity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseWeatherCtrl::SetSeverity(const Cigi_uint8 SeverityIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((SeverityIn < 0)||(SeverityIn > 5)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Severity",(Cigi_uint8)SeverityIn,0,5);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Severity = SeverityIn;
   return(CIGI_SUCCESS);

}


// ================================================
// ScudFreq
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseWeatherCtrl::SetScudFreq(const float ScudFreqIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((ScudFreqIn < 0.0)||(ScudFreqIn > 100.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("ScudFreq",(float)ScudFreqIn,0.0,100.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   ScudFreq = ScudFreqIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Coverage
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseWeatherCtrl::SetCoverage(const float CoverageIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((CoverageIn < 0.0)||(CoverageIn > 100.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Coverage",(float)CoverageIn,0.0,100.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Coverage = CoverageIn;
   return(CIGI_SUCCESS);

}


