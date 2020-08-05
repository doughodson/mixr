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
 *  FILENAME:   CigiHoldEnvCtrl.h
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
 *  Added the CIGI_SPEC modifier to the class declaration for exporting the 
 *  class in a Windows DLL.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Initialized the MinorVersion member variable in the constructor.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 * </pre>
 *  Author: The Boeing Company
 *
 */


#if !defined(_CIGI_HOLD_ENV_CTRL_INCLUDED_)
#define _CIGI_HOLD_ENV_CTRL_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CigiBaseEnvCtrl.h"

// ====================================================================
// preprocessor definitions
// ====================================================================
#define CIGI_HOLD_ENV_CTRL_PACKET_ID_V3 0
#define CIGI_HOLD_ENV_CTRL_PACKET_SIZE_V3 0


class CIGI_SPEC CigiHoldEnvCtrl : public CigiBaseEnvCtrl  
{
public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiHoldEnvCtrl()
   {
      PacketID = CIGI_HOLD_ENV_CTRL_PACKET_ID_V3;
      PacketSize = CIGI_HOLD_ENV_CTRL_PACKET_SIZE_V3;
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

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiHoldEnvCtrl() { };


   //==> Buffer Packing/Unpacking

   //=========================================================
   //! The virtual Pack function for CIGI 3
   //! \param Base - A pointer to the instance of the packet 
   //!          to be packed. (Downcast to CigiBasePacket)
   //! \param Buff - A pointer to the current pack point.
   //! \param Spec - A pointer to special data -
   //!          This is used to point to the Environmental Control 
   //!            holding object when going between V2 and V3.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const { return(0); }
   //=========================================================
   //! The virtual Unpack function for CIGI 3
   //! \param Buff - A pointer to the current pack point.
   //! \param Swap - N/A for V1 & V2
   //! \param Spec - A pointer to special data -
   //!          This is used to point to the Environmental Control 
   //!            holding object when going between V2 and V3.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec) { return(0); }


   //==> Hold Object Manipulation

   //=========================================================
   //! A virtual FillHold function.  
   //! This function is not implemented in this class.
   //!
   virtual void FillHold(CigiBaseEnvCtrl * Hold) const  { };




};

#endif // !defined(_CIGI_HOLD_ENV_CTRL_INCLUDED_)
