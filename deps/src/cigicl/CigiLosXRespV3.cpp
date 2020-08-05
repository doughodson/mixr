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
 *  FILENAME:   CigiLosXRespV3.cpp
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
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the class constructor to initialize the MinorVersion and 
 *  HostFrame member variables.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the Unpack method to set the HostFrame member variable.
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

#include "CigiLosXRespV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiLosXRespV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiLosXRespV3::CigiLosXRespV3()
{

   PacketID = CIGI_LOS_XRESP_PACKET_ID_V3;
   PacketSize = CIGI_LOS_XRESP_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

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
// ~CigiLosXRespV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiLosXRespV3::~CigiLosXRespV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosXRespV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
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
   HDta |= (Cigi_uint8)((Data->IntersectionCoordSys << 4) & 0x10);
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
int CigiLosXRespV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
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
      IntersectionCoordSys = (LOSCoordGrp)((HDta >> 4) & 0x01);

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
      IntersectionCoordSys = (LOSCoordGrp)((HDta >> 4) & 0x01);

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

   HostFrame = 0;

   ValidV1or2 = (IntersectionCoordSys == Geodetic);

   return(PacketSize);

}


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosXRespV3::GetCnvt(CigiVersionID &CnvtVersion,
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
// IntersectionCoordSys
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosXRespV3::SetIntersectionCoordSys(const LOSCoordGrp IntersectionCoordSysIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((IntersectionCoordSysIn < 0)||(IntersectionCoordSysIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("IntersectionCoordSys",(LOSCoordGrp)IntersectionCoordSysIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   IntersectionCoordSys = IntersectionCoordSysIn;

   ValidV1or2 = (IntersectionCoordSys == Geodetic);
   return(CIGI_SUCCESS);

}


// ================================================
// NormalAz
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiLosXRespV3::SetNormalAz(const float NormalAzIn, bool bndchk)
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
int CigiLosXRespV3::SetNormalEl(const float NormalElIn, bool bndchk)
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


