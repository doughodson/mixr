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
 *  FILENAME:   CigiSensorCtrlV3.cpp
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
 *  02/20/2006 Greg Basler                       Version 1.6.0
 *  Corrected the unpacking of the polarity flag.
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

#include "CigiSensorCtrlV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiSensorCtrlV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSensorCtrlV3::CigiSensorCtrlV3()
{

   PacketID = CIGI_SENSOR_CTRL_PACKET_ID_V3;
   PacketSize = CIGI_SENSOR_CTRL_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

   ViewID = 0;
   SensorID = 0;
   SensorOn = false;
   Polarity = WhiteHot;
   LineDropEn = false;
   TrackMode = TrackOff;
   AutoGainEn = false;
   TrackPolarity = TrackWhite;
   ResponseType = GatePos;
   Gain = 0.0;
   Level = 0.0;
   ACCoupling = 0.0;
   Noise = 0.0;

}

// ================================================
// ~CigiSensorCtrlV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiSensorCtrlV3::~CigiSensorCtrlV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSensorCtrlV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseSensorCtrl * Data = ( CigiBaseSensorCtrl *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->ViewID;
   *CDta.c++ = Data->SensorID;

   Cigi_uint8 HDta = (Cigi_uint8)(Data->TrackMode << 5);

   if(Data->TrackPolarity == TrackBlack)
      HDta |= 0x10;

   if(Data->AutoGainEn)
      HDta |= 0x08;

   if(Data->LineDropEn)
      HDta |= 0x04;

   if(Data->Polarity == BlackHot)
      HDta |= 0x02;

   if(Data->SensorOn)
      HDta |= 0x01;

   *CDta.c++ = HDta;

   *CDta.c++ = (Cigi_uint8)(Data->ResponseType & 0x01);

   *CDta.c++ = 0;

   *CDta.f++ = Data->Gain;
   *CDta.f++ = Data->Level;
   *CDta.f++ = Data->ACCoupling;
   *CDta.f++ = Data->Noise;

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSensorCtrlV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {
      ViewID = *CDta.s++;
      SensorID = *CDta.c++;

      Cigi_uint8 HDta = *CDta.c++;

      SensorOn = ((HDta & 0x01) != 0);
      Polarity = (PolarityGrp)((HDta >> 1) & 0x01);
      LineDropEn = ((HDta & 0x04) != 0);
      AutoGainEn = ((HDta & 0x08) != 0);
      TrackPolarity = (TrackPolarityGrp)((HDta >> 4) & 0x01);
      TrackMode = (TrackModeGrp)((HDta >> 5) & 0x07);

      HDta = *CDta.c++;

      ResponseType = (ResponseTypeGrp)(HDta & 0x01);

      CDta.c++;

      Gain = *CDta.f++;
      Level = *CDta.f++;
      ACCoupling = *CDta.f++;
      Noise = *CDta.f++;
   }
   else
   {
      CigiSwap2(&ViewID, CDta.s++);
      SensorID = *CDta.c++;

      Cigi_uint8 HDta = *CDta.c++;

      SensorOn = ((HDta & 0x01) != 0);
      Polarity = (PolarityGrp)((HDta >> 1) & 0x01);
      LineDropEn = ((HDta & 0x04) != 0);
      AutoGainEn = ((HDta & 0x08) != 0);
      TrackPolarity = (TrackPolarityGrp)((HDta >> 4) & 0x01);
      TrackMode = (TrackModeGrp)((HDta >> 5) & 0x07);

      HDta = *CDta.c++;

      ResponseType = (ResponseTypeGrp)(HDta & 0x01);

      CDta.c++;

      CigiSwap4(&Gain, CDta.f++);
      CigiSwap4(&Level, CDta.f++);
      CigiSwap4(&ACCoupling, CDta.f++);
      CigiSwap4(&Noise, CDta.f++);
   }

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// ResponseType
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSensorCtrlV3::SetResponseType(const ResponseTypeGrp ResponseTypeIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((ResponseTypeIn < 0)||(ResponseTypeIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("ResponseType",(ResponseTypeGrp)ResponseTypeIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   ResponseType = ResponseTypeIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Gain
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiSensorCtrlV3::SetGain(const float GainIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((GainIn < 0.0)||(GainIn > 1.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Gain",(float)GainIn,0.0,1.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Gain = GainIn;
   return(CIGI_SUCCESS);

}


