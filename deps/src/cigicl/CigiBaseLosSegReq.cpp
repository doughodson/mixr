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
 *  FILENAME:   CigiBaseLosSegReq.cpp
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

#include "CigiBaseLosSegReq.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiBaseLosSegReq
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseLosSegReq::CigiBaseLosSegReq()
{

}



// ================================================
// ~CigiBaseLosSegReq
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseLosSegReq::~CigiBaseLosSegReq()
{

}


// ====================================================================
// Conversion Control
// ====================================================================


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseLosSegReq::GetCnvt(CigiVersionID &CnvtVersion,
                             CigiCnvtInfoType::Type &CnvtInfo)
{
   CnvtInfo.ProcID = CigiProcessType::ProcStd;

   // Note: CIGI_LOS_SEG_REQ_PACKET_ID_V1 &
   // CIGI_LOS_SEG_REQ_PACKET_ID_V2 are the same
   if(CnvtVersion.CigiMajorVersion < 3)
      CnvtInfo.CnvtPacketID = CIGI_LOS_SEG_REQ_PACKET_ID_V2;
   else
      CnvtInfo.CnvtPacketID = CIGI_LOS_SEG_REQ_PACKET_ID_V3;

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================


// ================================================
// SrcLat
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseLosSegReq::SetSrcLat(const double SrcLatIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((SrcLatIn < -90.0)||(SrcLatIn > 90.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("SrcLat",(double)SrcLatIn,-90.0,90.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   SrcXLat = SrcLatIn;
   return(CIGI_SUCCESS);

}


// ================================================
// SrcLon
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseLosSegReq::SetSrcLon(const double SrcLonIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((SrcLonIn < -180.0)||(SrcLonIn > 180.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("SrcLon",(double)SrcLonIn,-180.0,180.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   SrcYLon = SrcLonIn;
   return(CIGI_SUCCESS);

}


// ================================================
// DstLat
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseLosSegReq::SetDstLat(const double DstLatIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((DstLatIn < -90.0)||(DstLatIn > 90.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DstLat",(double)DstLatIn,-90.0,90.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   DstXLat = DstLatIn;
   return(CIGI_SUCCESS);

}


// ================================================
// DstLon
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseLosSegReq::SetDstLon(const double DstLonIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((DstLonIn < -180.0)||(DstLonIn > 180.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DstLon",(double)DstLonIn,-180.0,180.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   DstYLon = DstLonIn;
   return(CIGI_SUCCESS);

}


