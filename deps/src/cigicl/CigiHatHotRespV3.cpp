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
 *  FILENAME:   CigiHatHotRespV3.cpp
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
 *  Modified the Unpack method to use the HostFrame member variable.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  
 *  05/14/2008 Greg Basler                       Version 2.2.0
 *  Fixed the conversion process.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiHatHotRespV3.h"
#include "CigiBaseHatHotReq.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiHatHotRespV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiHatHotRespV3::CigiHatHotRespV3()
{

   PacketID = CIGI_HAT_HOT_RESP_PACKET_ID_V3;
   PacketSize = CIGI_HAT_HOT_RESP_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

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
// ~CigiHatHotRespV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiHatHotRespV3::~CigiHatHotRespV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiHatHotRespV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseHatHotResp * Data = ( CigiBaseHatHotResp *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->HatHotID;

   bool tValid = Data->Valid;
   ReqTypeGrp tReqType = Data->ReqType;

   if(tValid)
   {
      if(!((Data->VldHat && (tReqType == HAT)) ||
           (Data->VldHot && (tReqType == HOT))))
      {
         if(Data->VldHat)
            tReqType = HAT;
         else if(Data->VldHot)
            tReqType = HOT;
         else
            tValid = false;
      }
   }


   Cigi_uint8 HDta = (tValid) ? 0x01 : 0;
   HDta |= (Cigi_uint8)((tReqType << 1) & 0x02);
   *CDta.c++ = HDta;
   *CDta.c++ = 0;
   *CDta.s++ = 0;

   if(tValid)
   {
      if(tReqType == HAT)
         *CDta.d++ = Data->Hat;
      else
         *CDta.d++ = Data->Hot;
   }
   else
      *CDta.d++ = 0.0;


   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiHatHotRespV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {
      HatHotID = *CDta.s++;

      Cigi_uint8 HDta = *CDta.c++;
      Valid = ((HDta & 0x01) != 0);
      ReqType = (ReqTypeGrp)((HDta >> 1) & 0x01);

      CDta.c += 3;

      if(Valid)
      {
         if(ReqType == HAT)
         {
            Hat = *CDta.d++;
            Hot = 0.0;
            VldHat = true;
            VldHot = false;
         }
         else
         {
            Hat = 0.0;
            Hot = *CDta.d++;
            VldHat = false;
            VldHot = true;
         }

      }
      else
      {
         Hat = 0.0;
         Hot = 0.0;
         VldHat = false;
         VldHot = false;
      }

   }
   else
   {
      CigiSwap2(&HatHotID, CDta.s++);

      Cigi_uint8 HDta = *CDta.c++;
      Valid = ((HDta & 0x01) != 0);
      ReqType = (ReqTypeGrp)((HDta >> 1) & 0x01);

      CDta.c += 3;

      if(Valid)
      {
         if(ReqType == HAT)
         {
            CigiSwap8(&Hat, CDta.d++);
            Hot = 0.0;
            VldHat = true;
            VldHot = false;
         }
         else
         {
            Hat = 0.0;
            CigiSwap8(&Hot, CDta.d++);
            VldHat = false;
            VldHot = true;
         }

      }
      else
      {
         Hat = 0.0;
         Hot = 0.0;
         VldHat = false;
         VldHot = false;
      }

   }

   HostFrame = 0;

   Material = 0;

   return(PacketSize);

}


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiHatHotRespV3::GetCnvt(CigiVersionID &CnvtVersion,
                              CigiCnvtInfoType::Type &CnvtInfo)
{
   CnvtInfo.ProcID = CigiProcessType::TwoPassCnvtProcStd;

   switch(CnvtVersion.CigiMajorVersion)
   {
   case 1:
      // Note: This will result in an incorrect value
      // if the V3 response is a HOT value.
      CnvtInfo.CnvtPacketID = CIGI_HAT_RESP_PACKET_ID_V1;
      break;
   case 2:
      if(ReqType == CigiBaseHatHotReq::HAT)
         CnvtInfo.CnvtPacketID = CIGI_HAT_RESP_PACKET_ID_V2;
      else
         CnvtInfo.CnvtPacketID = CIGI_HOT_RESP_PACKET_ID_V2;
      break;
   default:
      // The Packet ID for all V3 HatHotResp are the same ID
      CnvtInfo.CnvtPacketID = CIGI_HAT_HOT_RESP_PACKET_ID_V3;
      break;
   }

   return(CIGI_SUCCESS);
}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// ReqType
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiHatHotRespV3::SetReqType(const ReqTypeGrp ReqTypeIn, bool bndchk)
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


