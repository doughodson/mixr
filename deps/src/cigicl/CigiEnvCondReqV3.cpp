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
 *  FILENAME:   CigiEnvCondReqV3.cpp
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
 *  
 *  12/14/2007 Greg Basler                       Version 2.0.0
 *  Added the Request ID parameter.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiEnvCondReqV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiEnvCondReqV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiEnvCondReqV3::CigiEnvCondReqV3()
{

   PacketID = CIGI_ENV_COND_REQ_PACKET_ID_V3;
   PacketSize = CIGI_ENV_COND_REQ_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

   ReqType = Maritime;
   ReqID = 0;
   Lat = 0.0;
   Lon = 0.0;
   Alt = 0.0;

}

// ================================================
// ~CigiEnvCondReqV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiEnvCondReqV3::~CigiEnvCondReqV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEnvCondReqV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseEnvCondReq * Data = ( CigiBaseEnvCondReq *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.c++ = Data->ReqType & 0x0f;

   *CDta.c++ = Data->ReqID;
   *CDta.l++ = 0;

   *CDta.d++ = Data->Lat;
   *CDta.d++ = Data->Lon;
   *CDta.d++ = Data->Alt;

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEnvCondReqV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   ReqType = (ReqTypeGrp)(*CDta.c++ & 0x0f);

   ReqID = *CDta.c++;

   CDta.l++;


   if(!Swap)
   {
      Lat = *CDta.d++;
      Lon = *CDta.d++;
      Alt = *CDta.d++;
   }
   else
   {
      CigiSwap8(&Lat, CDta.d++);
      CigiSwap8(&Lon, CDta.d++);
      CigiSwap8(&Alt, CDta.d++);
   }

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


