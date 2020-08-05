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
 *  FILENAME:   CigiWaveCtrlV3.cpp
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

#include "CigiWaveCtrlV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiWaveCtrlV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiWaveCtrlV3::CigiWaveCtrlV3()
{

   PacketID = CIGI_WAVE_CTRL_PACKET_ID_V3;
   PacketSize = CIGI_WAVE_CTRL_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

   EntityRgnID = 0;
   WaveID = 0;
   WaveEn = false;
   Scope = Global;
   Breaker = Plunging;
   WaveHt = 0.0;
   WaveLen = 0.0;
   Period = 0.0;
   Direction = 0.0;
   PhaseOff = 0.0;
   Leading = 0.0;

}

// ================================================
// ~CigiWaveCtrlV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiWaveCtrlV3::~CigiWaveCtrlV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiWaveCtrlV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseWaveCtrl * Data = ( CigiBaseWaveCtrl *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->EntityRgnID;
   *CDta.c++ = Data->WaveID;

   Cigi_uint8 HDta = (Data->WaveEn) ? 0x01 : 0;
   HDta |= (Data->Scope<< 1) & 0x06;
   HDta |= (Data->Breaker << 3) & 0x18;
   *CDta.c++ = HDta;

   *CDta.s++ = 0;

   *CDta.f++ = Data->WaveHt;
   *CDta.f++ = Data->WaveLen;
   *CDta.f++ = Data->Period;
   *CDta.f++ = Data->Direction;
   *CDta.f++ = Data->PhaseOff;
   *CDta.f++ = Data->Leading;

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiWaveCtrlV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {
      EntityRgnID = *CDta.s++;
      WaveID = *CDta.c++;

      Cigi_uint8 HDta = *CDta.c++;
      WaveEn = ((HDta & 0x01) != 0);
      Scope = (ScopeGrp)((HDta >> 1) & 0x03);
      Breaker = (BreakerGrp)((HDta >> 3) & 0x03);

      CDta.s++;

      WaveHt = *CDta.f++;
      WaveLen = *CDta.f++;
      Period = *CDta.f++;
      Direction = *CDta.f++;
      PhaseOff = *CDta.f++;
      Leading = *CDta.f++;
   }
   else
   {
      CigiSwap2(&EntityRgnID, CDta.s++);
      WaveID = *CDta.c++;

      Cigi_uint8 HDta = *CDta.c++;
      WaveEn = ((HDta & 0x01) != 0);
      Scope = (ScopeGrp)((HDta >> 1) & 0x03);
      Breaker = (BreakerGrp)((HDta >> 3) & 0x03);

      CDta.s++;

      CigiSwap4(&WaveHt, CDta.f++);
      CigiSwap4(&WaveLen, CDta.f++);
      CigiSwap4(&Period, CDta.f++);
      CigiSwap4(&Direction, CDta.f++);
      CigiSwap4(&PhaseOff, CDta.f++);
      CigiSwap4(&Leading, CDta.f++);
   }

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


