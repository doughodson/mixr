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
 *  FILENAME:   CigiBaseSpecEffDef.cpp
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
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiBaseSpecEffDef.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiBaseSpecEffDef
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSpecEffDef::CigiBaseSpecEffDef()
{

}



// ================================================
// ~CigiBaseSpecEffDef
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSpecEffDef::~CigiBaseSpecEffDef()
{

}


// ====================================================================
// Conversion Control
// ====================================================================


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSpecEffDef::GetCnvt(CigiVersionID &CnvtVersion,
                                CigiCnvtInfoType::Type &CnvtInfo)
{
   // V1 & V2 use the same packet id number
   if(CnvtVersion.CigiMajorVersion > 2)
   {
      CnvtInfo.ProcID = CigiProcessType::ProcNone;
      CnvtInfo.CnvtPacketID = 0;
   }
   else
   {
      CnvtInfo.ProcID = CigiProcessType::ProcStd;
      CnvtInfo.CnvtPacketID = CIGI_SPEC_EFF_DEF_PACKET_ID_V2;
   }

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================


// ================================================
// SeqDir
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSpecEffDef::SetSeqDir(const SeqDirGrp SeqDirIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((SeqDirIn < 0)||(SeqDirIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("SeqDir",(SeqDirGrp)SeqDirIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   SeqDir = SeqDirIn;
   return(CIGI_SUCCESS);

}


// ================================================
// XScale
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSpecEffDef::SetXScale(const float XScaleIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((XScaleIn < 0.0)||(XScaleIn > 512.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("XScale",(float)XScaleIn,0.0,512.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   XScale = XScaleIn;
   return(CIGI_SUCCESS);

}


// ================================================
// YScale
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSpecEffDef::SetYScale(const float YScaleIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((YScaleIn < 0.0)||(YScaleIn > 512.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("YScale",(float)YScaleIn,0.0,512.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   YScale = YScaleIn;
   return(CIGI_SUCCESS);

}


// ================================================
// ZScale
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSpecEffDef::SetZScale(const float ZScaleIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((ZScaleIn < 0.0)||(ZScaleIn > 512.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("ZScale",(float)ZScaleIn,0.0,512.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   ZScale = ZScaleIn;
   return(CIGI_SUCCESS);

}


// ================================================
// TimeScale
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSpecEffDef::SetTimeScale(const float TimeScaleIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((TimeScaleIn < 0.0)||(TimeScaleIn > 512.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("TimeScale",(float)TimeScaleIn,0.0,512.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   TimeScale = TimeScaleIn;
   return(CIGI_SUCCESS);

}


