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
 *  FILENAME:   CigiCollDetVolDefV2.cpp
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

#include "CigiCollDetVolDefV2.h"
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
// CigiCollDetVolDefV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCollDetVolDefV2::CigiCollDetVolDefV2()
{

   PacketID = CIGI_COLL_DET_VOL_DEF_PACKET_ID_V2;
   PacketSize = CIGI_COLL_DET_VOL_DEF_PACKET_SIZE_V2;
   Version = 2;
   MinorVersion = 0;

   EntityID = 0;
   VolID = 0;
   VolEn = false;
   VolType = Sphere;
   Xoff = 0.0;
   Yoff = 0.0;
   Zoff = 0.0;
   HeightOrRadius = 0.0;
   Width = 0.0;
   Depth = 0.0;
   Roll = 0.0;
   Pitch = 0.0;
   Yaw = 0.0;

}

// ================================================
// ~CigiCollDetVolDefV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiCollDetVolDefV2::~CigiCollDetVolDefV2()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetVolDefV2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseCollDetVolDef * Data = ( CigiBaseCollDetVolDef *)Base;

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   CIGI_SCOPY2(CDta.s++, &Data->EntityID);

   Cigi_uint8 HDta = Data->VolID & 0x7f;
   HDta |= (Data->VolEn) ? 0x80 : 0;
   *CDta.c++ = HDta;

   *CDta.c++ = 0;
   *CDta.s++ = 0;

   float tfDim = Data->Xoff / 0.015625f;
   Cigi_int16 tsDim = (short)tfDim;
   CIGI_SCOPY2(CDta.s++, &tsDim);

   tfDim = Data->Yoff / 0.015625f;
   tsDim = (Cigi_int16)tfDim;
   CIGI_SCOPY2(CDta.s++, &tsDim);

   tfDim = Data->Zoff / 0.015625f;
   tsDim = (Cigi_int16)tfDim;
   CIGI_SCOPY2(CDta.s++, &tsDim);

   tfDim = Data->HeightOrRadius / 0.015625f;
   tsDim = (Cigi_int16)tfDim;
   CIGI_SCOPY2(CDta.s++, &tsDim);

   tfDim = Data->Width / 0.015625f;
   tsDim = (Cigi_int16)tfDim;
   CIGI_SCOPY2(CDta.s++, &tsDim);

   tfDim = Data->Depth / 0.015625f;
   tsDim = (Cigi_int16)tfDim;
   CIGI_SCOPY2(CDta.s++, &tsDim);



   memcpy(Buff,tBuf,CIGI_COLL_DET_VOL_DEF_PACKET_SIZE_V2);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiCollDetVolDefV2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_COLL_DET_VOL_DEF_PACKET_SIZE_V2);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   CIGI_SCOPY2(&EntityID, CDta.s++);

   Cigi_uint8 HDta = *CDta.c++;
   VolID = HDta & 0x7f;
   VolEn = ((HDta & 0x80) != 0);

   CDta.c += 3;


   Cigi_int16 tsScl;

   CIGI_SCOPY2(&tsScl, CDta.s++);
   float tfScl = (float)tsScl;
   Xoff = tfScl * 0.015625f;

   CIGI_SCOPY2(&tsScl, CDta.s++);
   tfScl = (float)tsScl;
   Yoff = tfScl * 0.015625f;


   CIGI_SCOPY2(&tsScl, CDta.s++);
   tfScl = (float)tsScl;
   Zoff = tfScl * 0.015625f;

   CIGI_SCOPY2(&tsScl, CDta.s++);
   tfScl = (float)tsScl;
   HeightOrRadius = tfScl * 0.015625f;

   CIGI_SCOPY2(&tsScl, CDta.s++);
   tfScl = (float)tsScl;
   Width = tfScl * 0.015625f;

   CIGI_SCOPY2(&tsScl, CDta.s++);
   tfScl = (float)tsScl;
   Depth = tfScl * 0.015625f;

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


