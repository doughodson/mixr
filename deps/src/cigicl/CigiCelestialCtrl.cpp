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
 *  FILENAME:   CigiCelestialCtrlV3.cpp
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

#include "CigiCelestialCtrl.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiCelestialCtrlV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCelestialCtrlV3::CigiCelestialCtrlV3()
{

   PacketID = CIGI_CELESTIAL_CTRL_PACKET_ID_V3;
   PacketSize = CIGI_CELESTIAL_CTRL_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

   Hour = 0;
   Minute = 0;
   Month = 1;
   Day = 1;
   Year = 2000;
   StarInt = 0.0;
   EphemerisEn = false;
   SunEn = false;
   MoonEn = false;
   StarEn = false;
   DateVld = false;
   AtmosEn = false;
   Aerosol = 0.0;
   Humidity = 30;
   AirTemp = 0.0;
   Visibility = 64373.76f;
   HorizWindSp = 0.0;
   VertWindSp = 0.0;
   WindDir = 0.0;
   BaroPress = 1013.25;


}

// ================================================
// ~CigiCelestialCtrlV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCelestialCtrlV3::~CigiCelestialCtrlV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCelestialCtrlV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseEnvCtrl * Data;
   if(Spec == NULL)
      Data = ( CigiBaseEnvCtrl *)Base;
   else
      Data = ( CigiBaseEnvCtrl *)Spec;


   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.c++ = Data->Hour;
   *CDta.c++ = Data->Minute;

   Cigi_uint8 HDta = (Data->EphemerisEn) ? 0x01 : 0;
   HDta |= (Data->SunEn) ? 0x02 : 0;
   HDta |= (Data->MoonEn) ? 0x04 : 0;
   HDta |= (Data->StarEn) ? 0x08 : 0;
   HDta |= (Data->DateVld) ? 0x10 : 0;
   *CDta.c++ = HDta;

   *CDta.c++ = 0;
   *CDta.s++ = 0;

   // Determine date
   Cigi_uint32 date = ((Cigi_uint32)Data->Month * 1000000) +
                        ((Cigi_uint32)Data->Day   *   10000) + (Cigi_uint32)Data->Year;
   *CDta.l++ = date;

   *CDta.f++ = Data->StarInt;


   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCelestialCtrlV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   Cigi_uint32 date;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {
      Hour = *CDta.c++;
      Minute = *CDta.c++;

      Cigi_uint8 HDta = *CDta.c++;
      EphemerisEn = ((HDta & 0x01) != 0);
      SunEn = ((HDta & 0x02) != 0);
      MoonEn = ((HDta & 0x04) != 0);
      StarEn = ((HDta & 0x08) != 0);
      DateVld = ((HDta & 0x10) != 0);

      CDta.c += 3;

      date = *CDta.l++;

      StarInt = *CDta.f++;
   }
   else
   {
      Hour = *CDta.c++;
      Minute = *CDta.c++;

      Cigi_uint8 HDta = *CDta.c++;
      EphemerisEn = ((HDta & 0x01) != 0);
      SunEn = ((HDta & 0x02) != 0);
      MoonEn = ((HDta & 0x04) != 0);
      StarEn = ((HDta & 0x08) != 0);
      DateVld = ((HDta & 0x10) != 0);

      CDta.c += 3;

      CigiSwap4(&date, CDta.l++);
      CigiSwap4(&StarInt, CDta.f++);
   }

   // Determine date
   Month = (Cigi_uint8)(date / 1000000);
   date -= ((Cigi_uint32)Month * 1000000);

   Day = (Cigi_uint8)(date / 10000);
   date -= ((Cigi_uint32)Day * 10000);

   Year = (Cigi_uint16)date;



   if(Spec != NULL)
   {

      CigiBaseEnvCtrl * Data = (CigiBaseEnvCtrl *)Spec;

      Data->Hour = Hour;
      Data->Minute = Minute;

      Data->EphemerisEn = EphemerisEn;
      Data->SunEn = SunEn;
      Data->MoonEn = MoonEn;
      Data->StarEn = StarEn;
      Data->DateVld = DateVld;

      Data->StarInt = StarInt;

      Data->Month = Month;
      Data->Day = Day;
      Data->Year = Year;

   }



   return(PacketSize);

}



// ====================================================================
// Filling the Hold object
// ====================================================================

// ================================================
// FillHold
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CigiCelestialCtrlV3::FillHold(CigiBaseEnvCtrl * Hold) const
{
   Hold->Hour = Hour;
   Hold->Minute = Minute;
   Hold->EphemerisEn = EphemerisEn;
   Hold->SunEn = SunEn;
   Hold->MoonEn = MoonEn;
   Hold->StarEn = StarEn;
   Hold->DateVld = DateVld;
   Hold->Month = Month;
   Hold->Day = Day;
   Hold->Year = Year;
   Hold->StarInt = StarInt;
}




// ====================================================================
// Accessors
// ====================================================================


// ================================================
// Hour
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCelestialCtrlV3::SetHour(const Cigi_uint8 HourIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((HourIn < 0)||(HourIn > 23)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Hour",(Cigi_uint8)HourIn,0,23);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Hour = HourIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Minute
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCelestialCtrlV3::SetMinute(const Cigi_uint8 MinuteIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((MinuteIn < 0)||(MinuteIn > 59)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Minute",(Cigi_uint8)MinuteIn,0,59);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Minute = MinuteIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Month
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCelestialCtrlV3::SetMonth(const Cigi_uint8 MonthIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((MonthIn < 1)||(MonthIn > 12)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Month",(Cigi_uint8)MonthIn,1,12);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Month = MonthIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Day
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCelestialCtrlV3::SetDay(const Cigi_uint8 DayIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((DayIn < 1)||(DayIn > 31)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Day",(Cigi_uint8)DayIn,1,31);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Day = DayIn;
   return(CIGI_SUCCESS);

}



// ================================================
// Star Field Intensity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCelestialCtrlV3::SetStarInt(const float StarIntIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((StarIntIn < 0.0)||(StarIntIn > 100.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("StarInt",StarIntIn,0.0,100.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   StarInt = StarIntIn;
   return(CIGI_SUCCESS);

}

