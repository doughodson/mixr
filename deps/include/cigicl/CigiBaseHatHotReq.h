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
 *  FILENAME:   CigiBaseHatHotReq.h
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
 *  Added a forward reference and granted friend access to the 
 *  CigiHatHotReqV3_2 class.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added the UpdatePeriod variable to set the number of frames between each 
 *  hat/hot update from the IG.
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


#if !defined(_CIGI_BASE_HAT_HOT_REQ_INCLUDED_)
#define _CIGI_BASE_HAT_HOT_REQ_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_HAT_REQ_PACKET_ID_V1 41
#define CIGI_HAT_REQ_PACKET_SIZE_V1 24

#define CIGI_HAT_REQ_PACKET_ID_V2 41
#define CIGI_HAT_REQ_PACKET_SIZE_V2 32

#define CIGI_HOT_REQ_PACKET_ID_V2 44
#define CIGI_HOT_REQ_PACKET_SIZE_V2 24

#define CIGI_HAT_HOT_REQ_PACKET_ID_V3 24
#define CIGI_HAT_HOT_REQ_PACKET_SIZE_V3 32

#define CIGI_HAT_HOT_REQ_PACKET_ID_V3_2 24
#define CIGI_HAT_HOT_REQ_PACKET_SIZE_V3_2 32



class CigiHatReqV1;
class CigiHatReqV2;
class CigiHotReqV2;
class CigiHatHotReqV3;
class CigiHatHotReqV3_2;


class CIGI_SPEC CigiBaseHatHotReq : public CigiBasePacket
{

friend class CigiHatReqV1;
friend class CigiHatReqV2;
friend class CigiHotReqV2;
friend class CigiHatHotReqV3;
friend class CigiHatHotReqV3_2;

public:

   //=========================================================
   //! The enumeration for the CigiBaseHatHotReq Group
   //!
   enum ReqTypeGrp
   {
      HAT=0,
      HOT=1,
      Extended=2
   };

   //=========================================================
   //! The enumeration for the CigiBaseHatHotReq Group
   //!
   enum CoordSysGrp
   {
      Geodetic=0,
      Entity=1
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseHatHotReq();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseHatHotReq();


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
   //! \param HatHotIDIn - HAT/HOT Request ID.
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


   //+> Lat

   //=========================================================
   //! Sets the Lat with bound checking control
   //! \param LatIn - The Latitude of the location to check.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLat(const double LatIn, bool bndchk=true);

   //=========================================================
   //! Gets the Lat value.
   //! \return the current Lat.
   double GetLat(void) const { return(LatOrXoff); }


   //+> Lon

   //=========================================================
   //! Sets the Lon with bound checking control
   //! \param LonIn - The Longitude of the location to check.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLon(const double LonIn, bool bndchk=true);

   //=========================================================
   //! Gets the Lon value.
   //! \return the current Lon.
   double GetLon(void) const { return(LonOrYoff); }



protected:

   //==> Member variables

   //=========================================================
   //! HatHotID<br>
   //! HAT/HOT Request ID.
   //!
   Cigi_uint16 HatHotID;

   //=========================================================
   //! ReqType<br>
   //! The type of request and response desired<br>
   //!  0-HAT<br>
   //!  1-HOT<br>
   //!  2-Extended
   //!
   ReqTypeGrp ReqType;

   //=========================================================
   //! SrcCoordSys<br>
   //! The Coordinate system of the point data.
   //!  0-Geodetic<br>
   //!  1-Entity
   //!
   //!
   CoordSysGrp SrcCoordSys;

   //=========================================================
   //! UpdatePeriod<br>
   //! The number of frames between each hat/hot update from
   //!  the IG.  If 0 - single shot
   //!
   Cigi_uint8 UpdatePeriod;

   //=========================================================
   //! EntityID<br>
   //! The ID of the Origin Entity
   //!
   Cigi_uint16 EntityID;

   //=========================================================
   //! LatOrXoff<br>
   //! The position of the request point specified as latitude or 
   //!   an offset along the X axis of the origin entity 
   //!   measured from the origin of that entity.
   //!
   double LatOrXoff;

   //=========================================================
   //! LonOrYoff<br>
   //! The position of the request point specified as longitude or 
   //!   an offset along the Y axis of the origin entity 
   //!   measured from the origin of that entity.
   //!
   double LonOrYoff;

   //=========================================================
   //! AltOrZoff<br>
   //! The position of the request point specified as altitude or 
   //!   an offset along the Z axis of the origin entity 
   //!   measured from the origin of that entity.
   //!
   double AltOrZoff;

   //=========================================================
   //! IsValidV1or2<br>
   //! Whether the data is valid for CIGI 1 and/or 2
   //!
   bool IsValidV1or2;


};

#endif // #if !defined(_CIGI_BASE_HAT_HOT_REQ_INCLUDED_)
