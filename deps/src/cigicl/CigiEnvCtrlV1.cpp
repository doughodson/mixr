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
 *  FILENAME:   CigiEnvCtrlV1.cpp
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
 *  
 *  10/16/2006 Greg Basler                       Version 1.7.4
 *  Changed the way some CIGI v3 enable flags are set during
 *  the version 1 unpacking
 *
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiEnvCtrlV1.h"
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
// CigiEnvCtrlV1
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiEnvCtrlV1::CigiEnvCtrlV1()
{

   PacketID = CIGI_ENV_CTRL_PACKET_ID_V1;
   PacketSize = CIGI_ENV_CTRL_PACKET_SIZE_V1;
   Version = 1;
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
   DateVld = true;
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
// ~CigiEnvCtrlV1
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiEnvCtrlV1::~CigiEnvCtrlV1()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEnvCtrlV1::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseEnvCtrl * Data = ( CigiBaseEnvCtrl *)Base;

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.c++ = Data->Hour;
   *CDta.c++ = Data->Minute;

   *CDta.c++ = ((Data->Humidity) & 0x7f) | ((Data->EphemerisEn) ? 0x80 : 0);

   *CDta.c++ = 0;
   *CDta.s++ = 0;

   // Determine date
   Cigi_uint32 date = ((Cigi_uint32)Data->Month * 1000000) +
                        ((Cigi_uint32)Data->Day   *   10000) + (Cigi_uint32)Data->Year;
   CIGI_SCOPY4(CDta.l++, &date);

   CIGI_SCOPY4(CDta.f++, &Data->AirTemp);
   CIGI_SCOPY4(CDta.f++, &Data->Visibility);
   CIGI_SCOPY4(CDta.f++, &Data->HorizWindSp);
   CIGI_SCOPY4(CDta.f++, &Data->WindDir);


   memcpy(Buff,tBuf,CIGI_ENV_CTRL_PACKET_SIZE_V1);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEnvCtrlV1::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_ENV_CTRL_PACKET_SIZE_V1);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   Hour = *CDta.c++;
   Minute = *CDta.c++;

   Cigi_uint8 HDta = *CDta.c++;
   EphemerisEn = ((HDta & 0x80) != 0);
   Humidity = HDta & 0x7f;

   SunEn = MoonEn = StarEn = DateVld = EphemerisEn;

   CDta.c += 3;

   // Determine date
   Cigi_uint32 date;
   CIGI_SCOPY4(&date, CDta.l++);

   Month = (Cigi_uint8)(date / 1000000);
   date -= ((Cigi_uint32)Month * 1000000);

   Day = (Cigi_uint8)(date / 10000);
   date -= ((Cigi_uint32)Day * 10000);

   Year = (Cigi_uint16)date;


   CIGI_SCOPY4(&AirTemp, CDta.f++);
   CIGI_SCOPY4(&Visibility, CDta.f++);
   CIGI_SCOPY4(&HorizWindSp, CDta.f++);
   CIGI_SCOPY4(&WindDir, CDta.f++);



   return(PacketSize);

}



// ====================================================================
// Filling the Hold object
// ====================================================================

// ================================================
// FillHold
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void CigiEnvCtrlV1::FillHold(CigiBaseEnvCtrl * Hold) const
{
   Hold->Hour = Hour;
   Hold->Minute = Minute;
   Hold->EphemerisEn = EphemerisEn;
   Hold->SunEn = SunEn;
   Hold->MoonEn = MoonEn;
   Hold->StarEn = StarEn;
   Hold->DateVld = DateVld;
   Hold->AtmosEn = AtmosEn;
   Hold->Humidity = Humidity;
   Hold->Month = Month;
   Hold->Day = Day;
   Hold->Year = Year;
   Hold->AirTemp = AirTemp;
   Hold->Visibility = Visibility;
   Hold->HorizWindSp = HorizWindSp;
   Hold->WindDir = WindDir;
   Hold->BaroPress = BaroPress;
   Hold->Aerosol = Aerosol;
}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// Hour
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEnvCtrlV1::SetHour(const Cigi_uint8 HourIn, bool bndchk)
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
int CigiEnvCtrlV1::SetMinute(const Cigi_uint8 MinuteIn, bool bndchk)
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
// Humidity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEnvCtrlV1::SetHumidity(const Cigi_uint8 HumidityIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((HumidityIn < 0)||(HumidityIn > 100)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Humidity",(Cigi_uint8)HumidityIn,0,100);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Humidity = HumidityIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Month
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEnvCtrlV1::SetMonth(const Cigi_uint8 MonthIn, bool bndchk)
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
int CigiEnvCtrlV1::SetDay(const Cigi_uint8 DayIn, bool bndchk)
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
// Visibility
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEnvCtrlV1::SetVisibility(const float VisibilityIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && (VisibilityIn < 0.0))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Visibility",(float)VisibilityIn,">",0.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Visibility = VisibilityIn;
   return(CIGI_SUCCESS);

}


// ================================================
// HorizWindSp
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEnvCtrlV1::SetHorizWindSp(const float HorizWindSpIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && (HorizWindSpIn < 0.0))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("HorizWindSp",(float)HorizWindSpIn,">",0.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   HorizWindSp = HorizWindSpIn;
   return(CIGI_SUCCESS);

}


// ================================================
// WindDir
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEnvCtrlV1::SetWindDir(const float WindDirIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((WindDirIn < 0.0)||(WindDirIn > 360.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("WindDir",(float)WindDirIn,0.0,360.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   WindDir = WindDirIn;
   return(CIGI_SUCCESS);

}


