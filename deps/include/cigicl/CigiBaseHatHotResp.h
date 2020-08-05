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
 *  FILENAME:   CigiBaseHatHotResp.h
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
 *  Added forward references and granted friend access to the 
 *  CigiHatHotRespV3_2 and CigiHatHotXRespV3_2 classes.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added a HostFrame member variable to track when the hat/hot was calculated.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Moved Packet information to base packet.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#if !defined(_CIGI_BASE_HAT_HOT_RESP_INCLUDED_)
#define _CIGI_BASE_HAT_HOT_RESP_INCLUDED_

#include "CigiBasePacket.h"


// Note:
// When converting from a HOT Response of Version 2 or greater to a
//  HAT Response of Version 1, the conversion is not a good value.
//  The value ends up being a HOT not a HAT.  At this time there
//  is no way of actually providing a HAT.

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_HAT_RESP_PACKET_ID_V1 102
#define CIGI_HAT_RESP_PACKET_SIZE_V1 16

#define CIGI_HAT_RESP_PACKET_ID_V2 102
#define CIGI_HAT_RESP_PACKET_SIZE_V2 24

#define CIGI_HOT_RESP_PACKET_ID_V2 106
#define CIGI_HOT_RESP_PACKET_SIZE_V2 24

#define CIGI_HAT_HOT_RESP_PACKET_ID_V3 102
#define CIGI_HAT_HOT_RESP_PACKET_SIZE_V3 16

#define CIGI_HAT_HOT_XRESP_PACKET_ID_V3 103
#define CIGI_HAT_HOT_XRESP_PACKET_SIZE_V3 40

#define CIGI_HAT_HOT_RESP_PACKET_ID_V3_2 102
#define CIGI_HAT_HOT_RESP_PACKET_SIZE_V3_2 16

#define CIGI_HAT_HOT_XRESP_PACKET_ID_V3_2 103
#define CIGI_HAT_HOT_XRESP_PACKET_SIZE_V3_2 40


class CigiHatRespV1;
class CigiHatRespV2;
class CigiHotRespV2;
class CigiHatHotRespV3;
class CigiHatHotXRespV3;
class CigiHatHotRespV3_2;
class CigiHatHotXRespV3_2;


class CIGI_SPEC CigiBaseHatHotResp : public CigiBasePacket
{

friend class CigiHatRespV1;
friend class CigiHatRespV2;
friend class CigiHotRespV2;
friend class CigiHatHotRespV3;
friend class CigiHatHotXRespV3;
friend class CigiHatHotRespV3_2;
friend class CigiHatHotXRespV3_2;

public:

   //=========================================================
   //! The enumeration for the CigiBaseHatHotResp Group
   //!
   enum ReqTypeGrp
   {
      HAT=0,
      HOT=1
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseHatHotResp() { };

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseHatHotResp() { };


   //==> Buffer Packing/Unpacking

   //=========================================================
   //! A pure virtual Pack function.  
   //! This function is not implemented in this class.
   //! \param Base - A pointer to the instance of the packet 
   //!          to be packed. (Downcast to CigiBasePacket)
   //! \param Buff - A pointer to the current pack point.
   //! \param Spec - A pointer to special data.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const=0;

   //=========================================================
   //! A pure virtual Unpack function.
   //! This function is not implemented in this class.
   //! \param Buff - A pointer to the current pack point.
   //! \param Swap - N/A for V1 & V2
   //! \param Spec - A pointer to special data.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec) =0;



   //==> Accessing Member Variable Values functions

   //+> HatHotID

   //=========================================================
   //! Sets the HatHotID with bound checking control
   //! \param HatHotIDIn - Hat/Hot Request/Response ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetHatHotID(const Cigi_uint16 HatHotIDIn, bool bndchk=true)
   {
      HatHotID = HatHotIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the HatHotID value.
   //! \return the current HatHotID.
   Cigi_uint16 GetHatHotID(void) const { return(HatHotID); }


   //+> Valid

   //=========================================================
   //! Sets the Valid with bound checking control
   //! \param ValidIn - Data is valid.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetValid(const bool ValidIn, bool bndchk=true)
   {
      Valid = ValidIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Valid value.
   //! \return the current Valid.
   bool GetValid(void) const { return(Valid); }



protected:

   //==> Member variables

   //=========================================================
   //! HatHotID<br>
   //! Hat/Hot Request/Response ID.
   //!
   Cigi_uint16 HatHotID;

   //=========================================================
   //! Valid<br>
   //! Data is valid.
   //!
   bool Valid;

   //=========================================================
   //! HostFrame<br>
   //! The least significant nibble of the Host frame number
   //!  when the hat/hot was calculated.
   //!
   Cigi_uint8 HostFrame;

   //=========================================================
   //! ReqType<br>
   //! The type of response data.
   //!  0-HAT<br>
   //!  1-HOT
   //!
   //!
   ReqTypeGrp ReqType;

   //=========================================================
   //! Hat<br>
   //! The Height Above Terrain result data.
   //!
   double Hat;

   //=========================================================
   //! Hot<br>
   //! The Height Of Terrain result data
   //!
   double Hot;

   //=========================================================
   //! Material<br>
   //! The material code of the specified location.
   //!
   Cigi_uint32 Material;

   //=========================================================
   //! NormAz<br>
   //! The Azimuth from true north of the normal vector of the
   //!   polygon which contains the test point.
   //!
   float NormAz;

   //=========================================================
   //! NormEl<br>
   //! The Elevation of the normal vector of the polygon
   //!   which contains the test point.
   //!
   //!
   float NormEl;

   //=========================================================
   //! VldHat<br>
   //! The Hat data is valid.
   //!
   bool VldHat;

   //=========================================================
   //! VldHot<br>
   //! The Hot data is valid
   //!
   bool VldHot;


};

#endif // #if !defined(_CIGI_BASE_HAT_HOT_RESP_INCLUDED_)
