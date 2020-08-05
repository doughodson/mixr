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
 *  FILENAME:   CigiLosXRespV3_2.cpp
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
 *  04/14/2006 Greg Basler                       Version 1.7
 *  Initial Release for CIGI 3.2 compatibility.
 *  Based on CigiLosXRespV3.cpp"
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  04/24/2007 Greg Basler                       Version 1.7.5
 *  Changed the way ValidV1or2 and IntersectionCoordSys is calculated.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  
 *  05/15/2008 Greg Basler                       Version 2.2.0
 *  Corrected Version number.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiLosXRespV3_2.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiLosXRespV3_2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiLosXRespV3_2::CigiLosXRespV3_2()
{

   PacketID = CIGI_LOS_XRESP_PACKET_ID_V3_2;
   PacketSize = CIGI_LOS_XRESP_PACKET_SIZE_V3_2;
   Version = 3;
   MinorVersion = 2;

   LosID = 0;
   Valid = false;
   EntityIDValid = false;
   RangeValid = false;
   Visible = false;
   IntersectionCoordSys = Geodetic;
   HostFrame = 0;
   RespCount = 0;
   EntityID = 0;
   Range = 0.0;
   LatOrXoff = 0.0;
   LonOrYoff = 0.0;
   AltOrZoff = 0.0;
   Red = 0;
   Green = 0;
   Blue = 0;
   Alpha = 0;
   Material = 0;
   NormalAz = 0.0;
   NormalEl = 0.0;
   ValidV1or2 = true;

}

// ================================================
// ~CigiLosXRespV3_2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiLosXRespV3_2::~CigiLosXRespV3_2()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosXRespV3_2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseLosResp * Data = ( CigiBaseLosResp *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->LosID;

   Cigi_uint8 HDta = (Data->Valid) ? 0x01 : 0x00;
   HDta |= (Data->EntityIDValid) ? 0x02 : 0x00;
   HDta |= (Data->RangeValid) ? 0x04 : 0x00;
   HDta |= (Data->Visible) ? 0x08 : 0x00;
   HDta |= (Data->HostFrame << 4) & 0xf0;
   *CDta.c++ = HDta;

   *CDta.c++ = Data->RespCount;
   *CDta.s++ = Data->EntityID;
   *CDta.d++ = Data->Range;
   *CDta.d++ = Data->LatOrXoff;
   *CDta.d++ = Data->LonOrYoff;
   *CDta.d++ = Data->AltOrZoff;
   *CDta.c++ = Data->Red;
   *CDta.c++ = Data->Green;
   *CDta.c++ = Data->Blue;
   *CDta.c++ = Data->Alpha;
   *CDta.l++ = Data->Material;
   *CDta.f++ = Data->NormalAz;
   *CDta.f++ = Data->NormalEl;

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosXRespV3_2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {
      LosID = *CDta.s++;

      Cigi_uint8 HDta = *CDta.c++;
      Valid = ((HDta & 0x01) != 0);
      EntityIDValid = ((HDta & 0x02) != 0);
      RangeValid = ((HDta & 0x04) != 0);
      Visible = ((HDta & 0x08) != 0);
      HostFrame = (Cigi_uint8)((HDta >> 4) & 0x0f);

      RespCount = *CDta.c++;
      EntityID = *CDta.s++;
      Range = *CDta.d++;
      LatOrXoff = *CDta.d++;
      LonOrYoff = *CDta.d++;
      AltOrZoff = *CDta.d++;
      Red = *CDta.c++;
      Green = *CDta.c++;
      Blue = *CDta.c++;
      Alpha = *CDta.c++;
      Material = *CDta.l++;
      NormalAz = *CDta.f++;
      NormalEl = *CDta.f++;

   }
   else
   {
      CigiSwap2(&LosID, CDta.s++);

      Cigi_uint8 HDta = *CDta.c++;
      Valid = ((HDta & 0x01) != 0);
      EntityIDValid = ((HDta & 0x02) != 0);
      RangeValid = ((HDta & 0x04) != 0);
      Visible = ((HDta & 0x08) != 0);
      HostFrame = (Cigi_uint8)((HDta >> 4) & 0x0f);

      RespCount = *CDta.c++;
      CigiSwap2(&EntityID, CDta.s++);
      CigiSwap8(&Range, CDta.d++);
      CigiSwap8(&LatOrXoff, CDta.d++);
      CigiSwap8(&LonOrYoff, CDta.d++);
      CigiSwap8(&AltOrZoff, CDta.d++);
      Red = *CDta.c++;
      Green = *CDta.c++;
      Blue = *CDta.c++;
      Alpha = *CDta.c++;
      CigiSwap4(&Material, CDta.l++);
      CigiSwap4(&NormalAz, CDta.f++);
      CigiSwap4(&NormalEl, CDta.f++);

   }

   if(EntityIDValid)
   {
      ValidV1or2 = false;
      IntersectionCoordSys = Entity;
   }
   else
   {
      ValidV1or2 = true;
      IntersectionCoordSys = Geodetic;
   }

   return(PacketSize);

}


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosXRespV3_2::GetCnvt(CigiVersionID &CnvtVersion,
                              CigiCnvtInfoType::Type &CnvtInfo)
{
   CnvtInfo.ProcID = CigiProcessType::ProcStd;

   // Note:
   // CIGI_LOS_RESP_PACKET_ID_V1 &
   // CIGI_LOS_RESP_PACKET_ID_V2 are the same
   // CIGI_LOS_XRESP_PACKET_ID_V3 &
   // CIGI_LOS_XRESP_PACKET_ID_V3_2 are the same
   if(CnvtVersion.CigiMajorVersion < 3)
      CnvtInfo.CnvtPacketID = CIGI_LOS_RESP_PACKET_ID_V2;
   else
      CnvtInfo.CnvtPacketID = CIGI_LOS_XRESP_PACKET_ID_V3;

   return(CIGI_SUCCESS);
}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// NormalAz
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosXRespV3_2::SetNormalAz(const float NormalAzIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((NormalAzIn < -180.0)||(NormalAzIn > 180.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("NormalAz",(float)NormalAzIn,-180.0,180.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   NormalAz = NormalAzIn;
   return(CIGI_SUCCESS);

}


// ================================================
// NormalEl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosXRespV3_2::SetNormalEl(const float NormalElIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((NormalElIn < -90.0)||(NormalElIn > 90.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("NormalEl",(float)NormalElIn,-90.0,90.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   NormalEl = NormalElIn;
   return(CIGI_SUCCESS);

}


