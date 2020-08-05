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
 *  FILENAME:   CigiHatHotXRespV3_2.cpp
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
 *  Based on CigiHatHotXRespV3.cpp"
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  01/22/2007 Greg Basler                       Version 1.7.5
 *  Set Request Type or Response Type (ReqType) to HAT in the Unpack()
 *  method for Cigi Version 1 and 2 compatibility.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  
 *  05/15/2008 Greg Basler                       Version 2.2.0
 *  Corrected Version Number
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiHatHotXRespV3_2.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiHatHotXRespV3_2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiHatHotXRespV3_2::CigiHatHotXRespV3_2()
{

   PacketID = CIGI_HAT_HOT_XRESP_PACKET_ID_V3_2;
   PacketSize = CIGI_HAT_HOT_XRESP_PACKET_SIZE_V3_2;
   Version = 3;
   MinorVersion = 2;

   HatHotID = 0;
   Valid = false;
   ReqType = HAT;
   HostFrame = 0;
   Hat = 0.0;
   Hot = 0.0;
   Material = 0;
   NormAz = 0.0;
   NormEl = 0.0;
   VldHat = false;
   VldHot = false;

}

// ================================================
// ~CigiHatHotXRespV3_2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiHatHotXRespV3_2::~CigiHatHotXRespV3_2()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiHatHotXRespV3_2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseHatHotResp * Data = ( CigiBaseHatHotResp *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->HatHotID;

   bool tValid = Data->Valid;
   if(!Data->VldHat && !Data->VldHot)
      tValid = false;

   Cigi_uint8 HDta = (tValid) ? 0x01 : 0;
   HDta |= (Data->HostFrame << 4) & 0xf0;

   *CDta.c++ = HDta;
   *CDta.c++ = 0;
   *CDta.s++ = 0;

   if(tValid)
   {
      if(Data->VldHat)
         *CDta.d++ = Data->Hat;
      else
         *CDta.d++ = 0.0;

      if(Data->VldHot)
         *CDta.d++ = Data->Hot;
      else
         *CDta.d++ = 0.0;

      *CDta.l++ = Data->Material;
      *CDta.f++ = Data->NormAz;
      *CDta.f++ = Data->NormEl;

   }
   else
   {
      *CDta.d++ = 0.0;
      *CDta.d++ = 0.0;
      *CDta.l++ = 0;
      *CDta.f++ = 0.0f;
      *CDta.f++ = 0.0f;
   }


   *CDta.l++ = 0;

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiHatHotXRespV3_2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {
      HatHotID = *CDta.s++;

      Cigi_uint8 HDta = *CDta.c++;
      Valid = ((HDta & 0x01) != 0);
      HostFrame = (Cigi_uint8)((HDta >> 4) & 0x0f);

      CDta.c += 3;

      Hat = *CDta.d++;
      Hot = *CDta.d++;
      Material = *CDta.l++;
      NormAz = *CDta.f++;
      NormEl = *CDta.f++;

      if(Valid)
      {
         VldHat = true;
         VldHot = true;
      }
      else
      {
         VldHat = false;
         VldHot = false;
      }
   }
   else
   {
      CigiSwap2(&HatHotID, CDta.s++);

      Cigi_uint8 HDta = *CDta.c++;
      Valid = ((HDta & 0x01) != 0);
      HostFrame = (Cigi_uint8)((HDta >> 4) & 0x0f);

      CDta.c += 3;

      CigiSwap8(&Hat, CDta.d++);
      CigiSwap8(&Hot, CDta.d++);
      CigiSwap4(&Material, CDta.l++);
      CigiSwap4(&NormAz, CDta.f++);
      CigiSwap4(&NormEl, CDta.f++);

      if(Valid)
      {
         VldHat = true;
         VldHot = true;
      }
      else
      {
         VldHat = false;
         VldHot = false;
      }
   }

   ReqType = HAT;  // For Cigi Version 1 and 2 compatibility

   return(PacketSize);

}


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiHatHotXRespV3_2::GetCnvt(CigiVersionID &CnvtVersion,
                                 CigiCnvtInfoType::Type &CnvtInfo)
{
   CnvtInfo.ProcID = CigiProcessType::ProcStd;

   switch(CnvtVersion.CigiMajorVersion)
   {
   case 1:
      CnvtInfo.CnvtPacketID = CIGI_HAT_RESP_PACKET_ID_V1;
      break;
   case 2:
      CnvtInfo.CnvtPacketID = CIGI_HOT_RESP_PACKET_ID_V2;
      break;
   default:
      // The Packet ID for all V3 HatHotXResp are the same ID
      CnvtInfo.CnvtPacketID = CIGI_HAT_HOT_XRESP_PACKET_ID_V3;
      break;
   }

   return(CIGI_SUCCESS);
}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// NormAz
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiHatHotXRespV3_2::SetNormAz(const float NormAzIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((NormAzIn < -180.0)||(NormAzIn > 180.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("NormAz",(float)NormAzIn,-180.0,180.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   NormAz = NormAzIn;
   return(CIGI_SUCCESS);

}


// ================================================
// NormEl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiHatHotXRespV3_2::SetNormEl(const float NormElIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((NormElIn < -90.0)||(NormElIn > 90.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("NormEl",(float)NormElIn,-90.0,90.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   NormEl = NormElIn;
   return(CIGI_SUCCESS);

}


