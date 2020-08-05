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
 *  FILENAME:   CigiLosSegReqV3.cpp
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
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Removed the inline defnitions for the PackedPointer union.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Changed native C++ types to use CCL-defined types instead where appropriate.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the class constructor to initialize the MinorVersion, 
 *  DestEntityIDValid, UpdatePeriod, and DestEntityID member variables.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the Unpack method to set the DestEntityIDValid, UpdatePeriod, and 
 *  DestEntityID member variables.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiLosSegReqV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiLosSegReqV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiLosSegReqV3::CigiLosSegReqV3()
{

   PacketID = CIGI_LOS_SEG_REQ_PACKET_ID_V3;
   PacketSize = CIGI_LOS_SEG_REQ_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

   LosID = 0;
   ReqType = Basic;
   SrcCoordSys = Geodetic;
   DstCoordSys = Geodetic;
   ResponseCoordSys = Geodetic;
   AlphaThresh = 0;
   EntityID = 0;
   SrcXLat = 0.0;
   SrcYLon = 0.0;
   SrcZAlt = 0.0;
   DstXLat = 0.0;
   DstYLon = 0.0;
   DstZAlt = 0.0;
   Mask = 0;
   DestEntityIDValid = false;
   UpdatePeriod = 0;
   DestEntityID = 0;
   ValidV1or2 = true;

}

// ================================================
// ~CigiLosSegReqV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiLosSegReqV3::~CigiLosSegReqV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosSegReqV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseLosSegReq * Data = ( CigiBaseLosSegReq *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->LosID;

   Cigi_uint8 HDta = (Data->ReqType == Basic) ? 0x00 : 0x01;
   HDta |= (Data->SrcCoordSys == Geodetic) ? 0x00 : 0x02;
   HDta |= (Data->DstCoordSys == Geodetic) ? 0x00 : 0x04;
   HDta |= (Data->ResponseCoordSys == Geodetic) ? 0x00 : 0x08;
   *CDta.c++ = HDta;

   *CDta.c++ = Data->AlphaThresh;
   *CDta.s++ = Data->EntityID;
   *CDta.d++ = Data->SrcXLat;
   *CDta.d++ = Data->SrcYLon;
   *CDta.d++ = Data->SrcZAlt;
   *CDta.d++ = Data->DstXLat;
   *CDta.d++ = Data->DstYLon;
   *CDta.d++ = Data->DstZAlt;
   *CDta.l++ = Data->Mask;
   *CDta.l++ = 0;

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosSegReqV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {
      LosID = *CDta.s++;

      Cigi_uint16 HDta = *CDta.c++;
      ReqType = (ReqTypeGrp)(HDta & 0x01);
      SrcCoordSys = (CoordSysGrp)((HDta >> 1) & 0x01);
      DstCoordSys = (CoordSysGrp)((HDta >> 2) & 0x01);
      ResponseCoordSys = (CoordSysGrp)((HDta >> 3) & 0x01);

      AlphaThresh = *CDta.c++;
      EntityID = *CDta.s++;
      SrcXLat = *CDta.d++;
      SrcYLon = *CDta.d++;
      SrcZAlt = *CDta.d++;
      DstXLat = *CDta.d++;
      DstYLon = *CDta.d++;
      DstZAlt = *CDta.d++;
      Mask = *CDta.l++;
   }
   else
   {
      CigiSwap2(&LosID, CDta.s++);

      Cigi_uint16 HDta = *CDta.c++;
      ReqType = (ReqTypeGrp)(HDta & 0x01);
      SrcCoordSys = (CoordSysGrp)((HDta >> 1) & 0x01);
      DstCoordSys = (CoordSysGrp)((HDta >> 2) & 0x01);
      ResponseCoordSys = (CoordSysGrp)((HDta >> 3) & 0x01);

      AlphaThresh = *CDta.c++;
      CigiSwap2(&EntityID, CDta.s++);
      CigiSwap8(&SrcXLat, CDta.d++);
      CigiSwap8(&SrcYLon, CDta.d++);
      CigiSwap8(&SrcZAlt, CDta.d++);
      CigiSwap8(&DstXLat, CDta.d++);
      CigiSwap8(&DstYLon, CDta.d++);
      CigiSwap8(&DstZAlt, CDta.d++);
      CigiSwap4(&Mask, CDta.l++);
   }

   
   ValidV1or2 = ((SrcCoordSys == Geodetic)&&
                 (DstCoordSys == Geodetic));


   DestEntityIDValid = false;
   UpdatePeriod = 0;
   DestEntityID = 0;

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// ReqType
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosSegReqV3::SetReqType(const ReqTypeGrp ReqTypeIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((ReqTypeIn < 0)||(ReqTypeIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("ReqType",(ReqTypeGrp)ReqTypeIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   ReqType = ReqTypeIn;
   return(CIGI_SUCCESS);

}


// ================================================
// SrcCoordSys
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosSegReqV3::SetSrcCoordSys(const CoordSysGrp SrcCoordSysIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((SrcCoordSysIn < 0)||(SrcCoordSysIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("SrcCoordSys",(CoordSysGrp)SrcCoordSysIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   SrcCoordSys = SrcCoordSysIn;

   ValidV1or2 = ((SrcCoordSys == Geodetic)&&
                 (DstCoordSys == Geodetic));

   return(CIGI_SUCCESS);

}


// ================================================
// DstCoordSys
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosSegReqV3::SetDstCoordSys(const CoordSysGrp DstCoordSysIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((DstCoordSysIn < 0)||(DstCoordSysIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DstCoordSys",(CoordSysGrp)DstCoordSysIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   DstCoordSys = DstCoordSysIn;

   ValidV1or2 = ((SrcCoordSys == Geodetic)&&
                 (DstCoordSys == Geodetic));

   return(CIGI_SUCCESS);

}


// ================================================
// ResponseCoordSys
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosSegReqV3::SetResponseCoordSys(const CoordSysGrp ResponseCoordSysIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((ResponseCoordSysIn < 0)||(ResponseCoordSysIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("ResponseCoordSys",(CoordSysGrp)ResponseCoordSysIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   ResponseCoordSys = ResponseCoordSysIn;
   return(CIGI_SUCCESS);

}


