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
 *  FILENAME:   CigiRateCtrlV1.cpp
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
 *  CoordSys member variables.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the Unpack method to set the CoordSys member variable.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiRateCtrlV1.h"
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
// CigiRateCtrlV1
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiRateCtrlV1::CigiRateCtrlV1()
{

   PacketID = CIGI_RATE_CTRL_PACKET_ID_V1;
   PacketSize = CIGI_RATE_CTRL_PACKET_SIZE_V1;
   Version = 1;
   MinorVersion = 0;

   EntityID = 0;
   ArtPartIDV1 = 0;
   ArtPartIDV3 = 0;
   ApplyToArtPart = false;
   CoordSys = Local;
   XRate = 0.0;
   YRate = 0.0;
   ZRate = 0.0;
   RollRate = 0.0;
   PitchRate = 0.0;
   YawRate = 0.0;

}

// ================================================
// ~CigiRateCtrlV1
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiRateCtrlV1::~CigiRateCtrlV1()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiRateCtrlV1::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseRateCtrl * Data = ( CigiBaseRateCtrl *)Base;

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   CIGI_SCOPY2(CDta.s++, &Data->EntityID);
   *CDta.c++ = Data->ArtPartIDV1;

   *CDta.c++ = 0;
   *CDta.s++ = 0;

   CIGI_SCOPY4(CDta.f++, &Data->XRate);
   CIGI_SCOPY4(CDta.f++, &Data->YRate);
   CIGI_SCOPY4(CDta.f++, &Data->ZRate);
   CIGI_SCOPY4(CDta.f++, &Data->RollRate);
   CIGI_SCOPY4(CDta.f++, &Data->PitchRate);
   CIGI_SCOPY4(CDta.f++, &Data->YawRate);

   memcpy(Buff,tBuf,CIGI_RATE_CTRL_PACKET_SIZE_V1);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiRateCtrlV1::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   union ArtPartConv
   {
      Cigi_uint8 uc;
      Cigi_int8 c;
   } ArtC;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_RATE_CTRL_PACKET_SIZE_V1);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   CIGI_SCOPY2(&EntityID, CDta.s++);
   ArtPartIDV1 = *CDta.c++;

   CDta.c += 3;

   CIGI_SCOPY4(&XRate, CDta.f++);
   CIGI_SCOPY4(&YRate, CDta.f++);
   CIGI_SCOPY4(&ZRate, CDta.f++);
   CIGI_SCOPY4(&RollRate, CDta.f++);
   CIGI_SCOPY4(&PitchRate, CDta.f++);
   CIGI_SCOPY4(&YawRate, CDta.f++);

   if(ArtPartIDV1 == -1)
   {

      ApplyToArtPart = false;

      ArtPartIDV3 = 0;

   }
   else
   {

      ApplyToArtPart = true;

      ArtC.c = ArtPartIDV1;
      ArtPartIDV3 = ArtC.uc;
      

   }

   CoordSys = Local;

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// SetArtPartID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiRateCtrlV1::SetArtPartID(const Cigi_int8 ArtPartIDIn, bool bndchk)
{
   union ArtPartConv
   {
      Cigi_uint8 uc;
      Cigi_int8 c;
   } ArtC;

   ArtPartIDV1 = ArtPartIDIn;

   if(ArtPartIDV1 == -1)
   {

      ApplyToArtPart = false;

      ArtPartIDV3 = 0;

   }
   else
   {

      ApplyToArtPart = true;

      ArtC.c = ArtPartIDV1;
      ArtPartIDV3 = ArtC.uc;
      

   }

   return(CIGI_SUCCESS);
}



