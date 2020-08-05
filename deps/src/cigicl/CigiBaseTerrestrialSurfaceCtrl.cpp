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
 *  FILENAME:   CigiBaseTerrestrialSurfaceCtrl.cpp
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

#include "CigiBaseTerrestrialSurfaceCtrl.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiBaseTerrestrialSurfaceCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseTerrestrialSurfaceCtrl::CigiBaseTerrestrialSurfaceCtrl()
{

}



// ================================================
// ~CigiBaseTerrestrialSurfaceCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseTerrestrialSurfaceCtrl::~CigiBaseTerrestrialSurfaceCtrl()
{

}


// ====================================================================
// Conversion Control
// ====================================================================


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseTerrestrialSurfaceCtrl::GetCnvt(CigiVersionID &CnvtVersion,
                                            CigiCnvtInfoType::Type &CnvtInfo)
{
   if(CnvtVersion.CigiMajorVersion < 3)
   {
      CnvtInfo.ProcID = CigiProcessType::ProcNone;
      CnvtInfo.CnvtPacketID = 0;
   }
   else
   {
      CnvtInfo.ProcID = CigiProcessType::ProcStd;
      CnvtInfo.CnvtPacketID = CIGI_TERRESTRIAL_SURFACE_CTRL_PACKET_ID_V3;
   }

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================


// ================================================
// Scope
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseTerrestrialSurfaceCtrl::SetScope(const ScopeGrp ScopeIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((ScopeIn < 0)||(ScopeIn > 2)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Scope",(ScopeGrp)ScopeIn,0,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Scope = ScopeIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Severity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseTerrestrialSurfaceCtrl::SetSeverity(const Cigi_uint8 SeverityIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((SeverityIn < 0)||(SeverityIn > 31)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Severity",(Cigi_uint8)SeverityIn,0,31);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Severity = SeverityIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Coverage
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseTerrestrialSurfaceCtrl::SetCoverage(const Cigi_uint8 CoverageIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((CoverageIn < 0)||(CoverageIn > 100)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Coverage",(Cigi_uint8)CoverageIn,0,100);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Coverage = CoverageIn;
   return(CIGI_SUCCESS);

}


