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
 *  FILENAME:   CigiWeatherCtrlV1.cpp
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

#include "CigiWeatherCtrlV1.h"
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
// CigiWeatherCtrlV1
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiWeatherCtrlV1::CigiWeatherCtrlV1()
{

   PacketID = CIGI_WEATHER_CTRL_PACKET_ID_V1;
   PacketSize = CIGI_WEATHER_CTRL_PACKET_SIZE_V1;
   Version = 1;
   MinorVersion = 0;

   EntityID = 0;
   RegionID = 0;
   LayerID = 0;
   Humidity = 0;
   PhenomenonType = 0;
   WeatherEn = false;
   ScudEn = false;
   RandomWindsEn = false;
   RandomLightningEn = false;
   CloudType = None;
   Scope = Global;
   Severity = 0;
   AirTemp = 0.0;
   VisibilityRng = 0.0;
   Opacity = 0.0;
   ScudFreq = 0.0;
   Coverage = 0.0;
   BaseElev = 0.0;
   Thickness = 0.0;
   Transition = 0.0;
   HorizWindSp = 0.0;
   VertWindSp = 0.0;
   WindDir = 0.0;
   BaroPress = 1013.25;
   Aerosol = 0.0;

}

// ================================================
// ~CigiWeatherCtrlV1
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiWeatherCtrlV1::~CigiWeatherCtrlV1()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiWeatherCtrlV1::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseWeatherCtrl * Data = ( CigiBaseWeatherCtrl *)Base;

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   CIGI_SCOPY2(CDta.s++, &Data->EntityID);

   Cigi_uint8 HDta = (Data->WeatherEn) ? 0x80 : 0x00;
   HDta |= (Data->ScudEn) ? 0x40 : 0x00;
   HDta |= (Data->RandomWindsEn) ? 0x20 : 0x00;
   HDta |= (Data->Severity << 2) & 0x1c;

   *CDta.c++ = HDta;
   *CDta.c++ = 0x00;


   Cigi_uint16 tType = 0;
   if(Data->Version <= 2)
      tType = Data->PhenomenonType;
   else
   {
      if(Data->Scope == CigiBaseWeatherCtrl::Entity)
         tType = 0;
      else
      {
         tType = (Cigi_uint16)Data->LayerID;
         if(tType == 0)
            tType = 3;
      }
   }
   CIGI_SCOPY2(CDta.s++, &tType);


   CIGI_SCOPY4(CDta.f++, &Data->AirTemp);

   if(Data->PhenomenonType == 3)
      CIGI_SCOPY4(CDta.f++, &Data->VisibilityRng);
   else
      CIGI_SCOPY4(CDta.f++, &Data->Opacity);


   CIGI_SCOPY4(CDta.f++, &Data->ScudFreq);
   CIGI_SCOPY4(CDta.f++, &Data->Coverage);
   CIGI_SCOPY4(CDta.f++, &Data->BaseElev);
   CIGI_SCOPY4(CDta.f++, &Data->Thickness);
   CIGI_SCOPY4(CDta.f++, &Data->Transition);
   CIGI_SCOPY4(CDta.f++, &Data->HorizWindSp);

   float Wndir = Data->WindDir;
   if(Wndir < 0.0f)
      Wndir += 360.0;
   CIGI_SCOPY4(CDta.f++, &Wndir);

   memcpy(Buff,tBuf,CIGI_WEATHER_CTRL_PACKET_SIZE_V1);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiWeatherCtrlV1::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   Cigi_uint16 LayerTbl[7] = { 1,1,2,0,4,5,8 };


   memcpy(tBuf,Buff,CIGI_WEATHER_CTRL_PACKET_SIZE_V1);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   CIGI_SCOPY2(&EntityID, CDta.s++);

   Cigi_uint8 HDta = *CDta.c++;
   WeatherEn = ((HDta & 0x80) != 0);
   ScudEn = ((HDta & 0x40) != 0);
   RandomWindsEn = ((HDta & 0x20) != 0);
   Severity = (HDta >> 2) & 0x07;

   CDta.c++;

   CIGI_SCOPY2(&PhenomenonType, CDta.s++);

   CIGI_SCOPY4(&AirTemp, CDta.f++);

   if(PhenomenonType == 3)
   {
      CIGI_SCOPY4(&VisibilityRng, CDta.f++);
      Opacity = ((VisibilityRng/70000.0f)*100.0f);
   }
   else
   {
      CIGI_SCOPY4(&Opacity, CDta.f++);
      VisibilityRng = ((Opacity/100.0f)*70000.0f);
   }

   CIGI_SCOPY4(&ScudFreq, CDta.f++);
   CIGI_SCOPY4(&Coverage, CDta.f++);
   CIGI_SCOPY4(&BaseElev, CDta.f++);
   CIGI_SCOPY4(&Thickness, CDta.f++);
   CIGI_SCOPY4(&Transition, CDta.f++);
   CIGI_SCOPY4(&HorizWindSp, CDta.f++);
   CIGI_SCOPY4(&WindDir, CDta.f++);

   RegionID = 0;
   Humidity = 0;
   RandomLightningEn = false;
   CloudType = None;

   if(PhenomenonType == 0)
   {
      LayerID = 1;
      Scope = Entity;
   }
   else
   {
      Scope = Global;

      if(PhenomenonType < 7)
         LayerID = (Cigi_uint8)LayerTbl[PhenomenonType];
      else if(PhenomenonType < 256)
         LayerID = (Cigi_uint8)PhenomenonType;
      else
         LayerID = 255;
   }

   VertWindSp = 0.0f;
   BaroPress = 1013.25f;
   Aerosol = 0.0f;

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// Phenomenon Type
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiWeatherCtrlV1::SetPhenomenonType(const Cigi_uint16 PhenomenonTypeIn, bool bndchk)
{
   Cigi_uint16 LayerTbl[7] = { 1,1,2,0,4,5,8 };

   PhenomenonType = PhenomenonTypeIn;

   if(PhenomenonType == 0)
   {
      LayerID = 1;
      Scope = Entity;
   }
   else
   {
      Scope = Global;

      if(PhenomenonType < 7)
         LayerID = (Cigi_uint8)LayerTbl[PhenomenonType];
      else if(PhenomenonType < 256)
         LayerID = (Cigi_uint8)PhenomenonType;
      else
         LayerID = 255;
   }

   return(CIGI_SUCCESS);
}


// ================================================
// Opacity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiWeatherCtrlV1::SetOpacity(const float OpacityIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((OpacityIn < 0.0)||(OpacityIn > 100.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Opacity",(float)OpacityIn,0.0,100.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Opacity = OpacityIn;
   VisibilityRng = ((Opacity/100.0f)*70000.0f);

   return(CIGI_SUCCESS);

}


// ================================================
// WindDir
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiWeatherCtrlV1::SetWindDir(const float WindDirIn, bool bndchk)
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


