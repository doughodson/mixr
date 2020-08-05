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
 *  FILENAME:   CigiSOFV3_2.cpp
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
 *  Based on CigiSOFV3.cpp"
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  08/11/2006 Greg Basler                       Version 1.7.3
 *  Corrected the packing routine so that the Minor Version number is
 *  is packed from the packing object (a CigiSOFV3_2 object in this case)
 *  not the object with the user filled data.  This will put the correct
 *  Minor Version number in the ig to host message.
 *  
 *  01/22/2007 Greg Basler                       Version 1.7.5
 *  Corrected a bug noted by C. Schroeder.  When byteswapping, the
 *  Unpack() method now unpacks the CIGI Version 3 timestamp not the
 *  version 2 timestamp.
 *  
 *  06/18/2008 Greg Basler                       Version 2.2.0
 *  Corrected a bug noted on SourceForge.  When byteswapping, the
 *  Unpack() method now unpacks on unsigned long CIGI Version 3
 *  timestamp not the float timestamp.
 *
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiSOFV3_2.h"
#include "CigiExceptions.h"
#include "CigiSwapping.h"
#include "CigiVersionID.h"


// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiSOFV3_2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSOFV3_2::CigiSOFV3_2()
{

   PacketID = CIGI_SOF_PACKET_ID_V3_2;
   PacketSize = CIGI_SOF_PACKET_SIZE_V3_2;
   Version = 3;
   MinorVersion = 2;

   BSwapEn = false;
   DatabaseID = 0;
   FrameCntr = 0;
   TimeStampV2 = 0.0f;
   TimeStampV3 = 0;
   ByteSwap = 0x8000;
   IGMode = Standby;
   TimestampValid = false;
   EarthRefModel = WGS84;
   LastRcvdHostFrame = 0;

}

// ================================================
// ~CigiSOFV3_2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSOFV3_2::~CigiSOFV3_2()
{

}





// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSOFV3_2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;
   CigiVersionID PackingVer = *((CigiVersionID *)Spec);

   CigiBaseSOF *Data = ( CigiBaseSOF *) Base;

   if(PackingVer.GetCombinedCigiVersion() < 0x0302)
      PackingVer.SetCigiVersion(3,2);

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;
   *CDta.c++ = PackingVer.CigiMajorVersion;

   *CDta.b++ = Data->DatabaseID;
   *CDta.c++ = Data->IGStatus;

   Cigi_uint8 HDta = (PackingVer.CigiMinorVersion << 4) & 0xf0;
   HDta |= (Data->TimestampValid) ? 0x04 : 0;
   HDta |= (Cigi_uint8)(Data->IGMode & 0x03);
   HDta |= (Cigi_uint8)((Data->EarthRefModel << 3) & 0x08);

   *CDta.c++ = HDta;

   *CDta.s++ = 0x8000;  // Byte Swap
   *CDta.l++ = Data->FrameCntr;
   *CDta.l++ = Data->TimeStampV3;
   *CDta.l++ = Data->LastRcvdHostFrame;
   *CDta.l++ = 0;


   return(PacketSize);

}


// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSOFV3_2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{

   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 3;

   DatabaseID = *CDta.b++;
   IGStatus = *CDta.c++;

   Cigi_uint8 HDta = *CDta.c++;

   IGMode = (IGModeGrp)(HDta & 0x03);
   EarthRefModel = (EarthRefModelGrp)((HDta >> 3) & 0x01);
   TimestampValid = ((HDta & 0x04) != 0);

   ByteSwap = *CDta.s++;

   BSwapEn = (ByteSwap == 0x0080);

   if(!BSwapEn)
   {
      FrameCntr = *CDta.l++;
      TimeStampV3 = *CDta.l++;
      LastRcvdHostFrame = *CDta.l++;
   }
   else
   {
      CigiSwap4(&FrameCntr,CDta.l++);
      CigiSwap4(&TimeStampV3,CDta.l++);
      CigiSwap4(&LastRcvdHostFrame,CDta.l++);
   }

   TimeStampV2 = ((float)TimeStampV3) * 10.0f;


   return(CIGI_SUCCESS);

}




// ====================================================================
// Accessors
// ====================================================================



// ================================================
// SetTimeStamp
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSOFV3_2::SetTimeStamp(const Cigi_uint32 TimeStamp, bool bndchk)
{

   TimeStampV3 = TimeStamp;
   TimeStampV2 = ((float)TimeStampV3) * 10.0f;

   return(CIGI_SUCCESS);

}

// ================================================
// SetEarthRefModel
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSOFV3_2::SetEarthRefModel(const EarthRefModelGrp EarthRefModelIn,
                                bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((EarthRefModelIn < 0)||(EarthRefModelIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("EarthRefModel",EarthRefModelIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   EarthRefModel = EarthRefModelIn;

   return(CIGI_SUCCESS);

}

