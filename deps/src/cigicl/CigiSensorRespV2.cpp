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
 *  FILENAME:   CigiSensorRespV2.cpp
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
 *  Modified the class constructor to initialize the MinorVersion member 
 *  variable.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiSensorRespV2.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"

#include <string.h>


#ifdef CIGI_LITTLE_ENDIAN
   #define CIGI_SCOPY2 CigiSwap2
   #define CIGI_SCOPY4 CigiSwap4
   #define CIGI_SCOPY8 CigiSwap8
#else
   #define CIGI_SCOPY2 CigiCopy2
   #define CIGI_SCOPY4 CigiCopy4
   #define CIGI_SCOPY8 CigiCopy8
#endif




// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiSensorRespV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSensorRespV2::CigiSensorRespV2()
{

   PacketID = CIGI_SENSOR_RESP_PACKET_ID_V2;
   PacketSize = CIGI_SENSOR_RESP_PACKET_SIZE_V2;
   Version = 2;
   MinorVersion = 0;

   ViewID = 0;
   SensorID = 0;
   SensorStat = Searching;
   EntityTgt = false;
   EntityID = 0;
   GateSzX = 0;
   GateSzY = 0;
   GateXoff = 0.0;
   GateYoff = 0.0;
   FrameCntr = 0;
   TrackPntLat = 0.0;
   TrackPntLon = 0.0;
   TrackPntAlt = 0.0;

}

// ================================================
// ~CigiSensorRespV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSensorRespV2::~CigiSensorRespV2()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSensorRespV2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseSensorResp * Data = ( CigiBaseSensorResp *)Base;

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   Cigi_uint8 HDta = (Cigi_uint8)((Data->ViewID << 3) & 0x00f8);
   HDta |= (Cigi_uint8)((Data->SensorStat << 1) & 0x06);
   *CDta.c++ = HDta;

   *CDta.c++ = Data->SensorID;

   float tfoff = (float)Data->GateXoff / 0.015625f;
   Cigi_int16 tsoff = (Cigi_int16)tfoff;
   CIGI_SCOPY2(CDta.s++, &tsoff);

   tfoff = Data->GateYoff / 0.015625f;
   tsoff = (Cigi_int16)tfoff;
   CIGI_SCOPY2(CDta.s++, &tsoff);

   CIGI_SCOPY2(CDta.s++, &Data->GateSzX);
   CIGI_SCOPY2(CDta.s++, &Data->GateSzY);

   memcpy(Buff,tBuf,CIGI_SENSOR_RESP_PACKET_SIZE_V2);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSensorRespV2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_SENSOR_RESP_PACKET_SIZE_V2);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   Cigi_uint8 HDta = *CDta.c++;
   ViewID = (Cigi_uint16)((HDta >> 3) & 0x1f);
   SensorStat = (SensorStatGrp)((HDta >> 1) & 0x03);
   SensorID = *CDta.c++;

   Cigi_int16 tsoff;
   CIGI_SCOPY2(&tsoff, CDta.s++);
   float tfoff = (float)tsoff;
   GateXoff = tfoff * 0.015625f;

   CIGI_SCOPY2(&tsoff, CDta.s++);
   tfoff = (float)tsoff;
   GateYoff = tfoff * 0.015625f;

   CIGI_SCOPY2(&GateSzX, CDta.s++);
   CIGI_SCOPY2(&GateSzY, CDta.s++);

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// ViewID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSensorRespV2::SetViewID(const Cigi_uint16 ViewIDIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((ViewIDIn < 0)||(ViewIDIn > 32)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("ViewID",(Cigi_uint16)ViewIDIn,0,32);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   ViewID = ViewIDIn;
   return(CIGI_SUCCESS);

}


