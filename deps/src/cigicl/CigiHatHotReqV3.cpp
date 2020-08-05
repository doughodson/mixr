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
 *  FILENAME:   CigiHatHotReqV3.cpp
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
 *  UpdatePeriod member variables.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the Unpack method to update the UpdatePeriod variable.
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

#include "CigiHatHotReqV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiHatHotReqV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiHatHotReqV3::CigiHatHotReqV3()
{

   PacketID = CIGI_HAT_HOT_REQ_PACKET_ID_V3;
   PacketSize = CIGI_HAT_HOT_REQ_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

   HatHotID = 0;
   ReqType = HAT;
   SrcCoordSys = Geodetic;
   UpdatePeriod = 0;
   EntityID = 0;
   LatOrXoff = 0.0;
   LonOrYoff = 0.0;
   AltOrZoff = 0.0;
   IsValidV1or2 = true;

}

// ================================================
// ~CigiHatHotReqV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiHatHotReqV3::~CigiHatHotReqV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiHatHotReqV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseHatHotReq * Data = ( CigiBaseHatHotReq *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->HatHotID;

   Cigi_uint8 HDta = (Cigi_uint8)(Data->ReqType & 0x03);
   HDta |= (Cigi_uint8)((Data->SrcCoordSys << 2) & 0x04);
   *CDta.c++ = HDta;

   *CDta.c++ = 0;

   *CDta.s++ = Data->EntityID;
   *CDta.d++ = Data->LatOrXoff;
   *CDta.d++ = Data->LonOrYoff;
   *CDta.d++ = Data->AltOrZoff;

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiHatHotReqV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {
      HatHotID = *CDta.s++;

      Cigi_uint8 HDta = *CDta.c++;
      ReqType = (ReqTypeGrp)(HDta & 0x03);
      SrcCoordSys = (CoordSysGrp)((HDta >> 2) & 0x01);

      CDta.c++;

      EntityID = *CDta.s++;
      LatOrXoff = *CDta.d++;
      LonOrYoff = *CDta.d++;
      AltOrZoff = *CDta.d++;
   }
   else
   {
      CigiSwap2(&HatHotID, CDta.s++);

      Cigi_uint8 HDta = *CDta.c++;
      ReqType = (ReqTypeGrp)(HDta & 0x03);
      SrcCoordSys = (CoordSysGrp)((HDta >> 2) & 0x01);

      CDta.c++;

      CigiSwap2(&EntityID, CDta.s++);
      CigiSwap8(&LatOrXoff, CDta.d++);
      CigiSwap8(&LonOrYoff, CDta.d++);
      CigiSwap8(&AltOrZoff, CDta.d++);
   }

   IsValidV1or2 = (SrcCoordSys == Geodetic);

   UpdatePeriod = 0;

   return(PacketSize);

}


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiHatHotReqV3::GetCnvt(CigiVersionID &CnvtVersion,
                             CigiCnvtInfoType::Type &CnvtInfo)
{
   CnvtInfo.ProcID = CigiProcessType::TwoPassCnvtProcStd;

   switch(CnvtVersion.CigiMajorVersion)
   {
   case 1:
      CnvtInfo.CnvtPacketID = CIGI_HAT_REQ_PACKET_ID_V1;
      break;
   case 2:
      if(ReqType == CigiBaseHatHotReq::HAT)
         CnvtInfo.CnvtPacketID = CIGI_HAT_REQ_PACKET_ID_V2;
      else
         CnvtInfo.CnvtPacketID = CIGI_HOT_REQ_PACKET_ID_V2;
      break;
   default:
      // The Packet ID for all V3 HatHotReq are the same ID
      CnvtInfo.CnvtPacketID = CIGI_HAT_HOT_REQ_PACKET_ID_V3;
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
int CigiHatHotReqV3::SetReqType(const ReqTypeGrp ReqTypeIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((ReqTypeIn < 0)||(ReqTypeIn > 2)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("ReqType",(ReqTypeGrp)ReqTypeIn,0,2);
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
int CigiHatHotReqV3::SetSrcCoordSys(const CoordSysGrp SrcCoordSysIn, bool bndchk)
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
   return(CIGI_SUCCESS);

}


