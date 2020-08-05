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
 *  FILENAME:   CigiBaseLosResp.h
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
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Changed native C++ types in function declarations to CCL-defined types.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added forward references and granted friend access to the 
 *  CigiLosRespV3_2 and CigiLosXRespV3_2 classes.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added a HostFrame member variable to track when the los was calculated.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Moved Packet information to base packet.
 *  
 * </pre>
 *  The Boeing Company
 *
 */


#if !defined(_CIGI_BASE_LOS_RESP_INCLUDED_)
#define _CIGI_BASE_LOS_RESP_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_LOS_RESP_PACKET_ID_V1 103
#define CIGI_LOS_RESP_PACKET_SIZE_V1 40

#define CIGI_LOS_RESP_PACKET_ID_V2 103
#define CIGI_LOS_RESP_PACKET_SIZE_V2 40

#define CIGI_LOS_RESP_PACKET_ID_V3 104
#define CIGI_LOS_RESP_PACKET_SIZE_V3 16

#define CIGI_LOS_XRESP_PACKET_ID_V3 105
#define CIGI_LOS_XRESP_PACKET_SIZE_V3 56

#define CIGI_LOS_RESP_PACKET_ID_V3_2 104
#define CIGI_LOS_RESP_PACKET_SIZE_V3_2 16

#define CIGI_LOS_XRESP_PACKET_ID_V3_2 105
#define CIGI_LOS_XRESP_PACKET_SIZE_V3_2 56


class CigiLosRespV1;
class CigiLosRespV2;
class CigiLosRespV3;
class CigiLosXRespV3;
class CigiLosRespV3_2;
class CigiLosXRespV3_2;


class CIGI_SPEC CigiBaseLosResp : public CigiBasePacket
{

friend class CigiLosRespV1;
friend class CigiLosRespV2;
friend class CigiLosRespV3;
friend class CigiLosXRespV3;
friend class CigiLosRespV3_2;
friend class CigiLosXRespV3_2;


public:

   //=========================================================
   //! The enumeration for the CigiBaseLosResp Group
   //!
   enum LOSCoordGrp
   {
      Geodetic=0,
      Entity=1
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseLosResp();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseLosResp();


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

   //+> LosID

   //=========================================================
   //! Sets the LosID with bound checking control
   //! \param LosIDIn - Line of Sight ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLosID(const Cigi_uint16 LosIDIn, bool bndchk=true)
   {
      LosID = LosIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the LosID value.
   //! \return the current LosID.
   Cigi_uint16 GetLosID(void) const { return(LosID); }


   //+> Valid

   //=========================================================
   //! Sets the Valid with bound checking control
   //! \param ValidIn - An intersection occurred and the data
   //!    is valid.
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


   //+> Visible

   //=========================================================
   //! Sets the Visible with bound checking control
   //! \param VisibleIn - The LOS Segment Destination or End Point
   //!    is visible From the Source or Start Point.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetVisible(const bool VisibleIn, bool bndchk=true)
   {
      Visible = VisibleIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Visible value.
   //! \return the current Visible.
   bool GetVisible(void) const { return(Visible); }


   //+> Range

   //=========================================================
   //! Sets the Range with bound checking control
   //! \param RangeIn - Range to the intersection.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRange(const double RangeIn, bool bndchk=true)
   {
      Range = RangeIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Range value.
   //! \return the current Range.
   double GetRange(void) const { return(Range); }


   //+> Latitude

   //=========================================================
   //! Sets the Latitude with bound checking control
   //! \param LatitudeIn - Latitude of the intersection.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLatitude(const double LatitudeIn, bool bndchk=true);

   //=========================================================
   //! Gets the Latitude value.
   //! \return the current Latitude.
   double GetLatitude(void) const { return(LatOrXoff); }


   //+> Longitude

   //=========================================================
   //! Sets the Longitude with bound checking control
   //! \param LongitudeIn - Longitude of the intersection
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLongitude(const double LongitudeIn, bool bndchk=true);

   //=========================================================
   //! Gets the Longitude value.
   //! \return the current Longitude.
   double GetLongitude(void) const { return(LonOrYoff); }


   //+> Altitude

   //=========================================================
   //! Sets the Altitude with bound checking control
   //! \param AltitudeIn - Altitude of the intersection
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetAltitude(const double AltitudeIn, bool bndchk=true)
   {
      AltOrZoff = AltitudeIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Altitude value.
   //! \return the current Altitude.
   double GetAltitude(void) const { return(AltOrZoff); }


   //+> ValidV1or2

   //=========================================================
   //! Gets the ValidV1or2 value.
   //! \return the current ValidV1or2.
   bool GetValidV1or2(void) const { return(ValidV1or2); }



protected:

   //==> Member variables

   //=========================================================
   //! LosID<br>
   //! The Line Of Sight ID
   //!
   Cigi_uint16 LosID;

   //=========================================================
   //! Valid<br>
   //! An intersection occurred and the data is valid.
   //!
   bool Valid;

   //=========================================================
   //! EntityIDValid<br>
   //! The LOS intersected an Entity and a valid Entity ID is
   //!   provided.
   //!
   bool EntityIDValid;

   //=========================================================
   //! RangeValid<br>
   //! An intersection occurred and the Range data is valid.
   //!
   bool RangeValid;

   //=========================================================
   //! Visible<br>
   //! The Destination or End Point is visible from the Source
   //!   or Start Point.
   //!
   bool Visible;

   //=========================================================
   //! HostFrame<br>
   //! The least significant nibble of the Host frame number
   //!  when the los was calculated.
   //!
   Cigi_uint8 HostFrame;

   //=========================================================
   //! IntersectionCoordSys<br>
   //!  0-Geodetic<br>
   //!  1-Entity
   //! What coordinate system the Intersection Point position
   //!   is presented.
   //!
   LOSCoordGrp IntersectionCoordSys;

   //=========================================================
   //! RespCount<br>
   //! The number of responses for this request.
   //!
   Cigi_uint8 RespCount;

   //=========================================================
   //! EntityID<br>
   //! The Entity ID of the entity with which the LOS intersected.
   //!
   Cigi_uint16 EntityID;

   //=========================================================
   //! Range<br>
   //! The range from the Source or Start Point to the intersection.
   //!
   double Range;

   //=========================================================
   //! Latitude<br>
   //! The latitude of the intersection.
   //!
   double LatOrXoff;

   //=========================================================
   //! Longitude<br>
   //! The longitude of the intersection.
   //!
   double LonOrYoff;

   //=========================================================
   //! Altitude<br>
   //! The altitude of the intersection
   //!
   double AltOrZoff;

   //=========================================================
   //! Red<br>
   //! The red color component of the surface at the point of
   //!   intersection.
   //!
   Cigi_uint8 Red;

   //=========================================================
   //! Green<br>
   //! The green color component of the surface at the point of
   //!   intersection.
   //!
   Cigi_uint8 Green;

   //=========================================================
   //! Blue<br>
   //! The blue color component of the surface at the point of
   //!   intersection.
   //!
   Cigi_uint8 Blue;

   //=========================================================
   //! Alpha<br>
   //! The alpha color component of the surface at the point of
   //!   intersection.
   //!
   Cigi_uint8 Alpha;

   //=========================================================
   //! Material<br>
   //! The material code of the surface at the point of intersection.
   //!
   Cigi_uint32 Material;

   //=========================================================
   //! NormalAz<br>
   //! The azimuth of the surface normal at the point of intersection.
   //!
   float NormalAz;

   //=========================================================
   //! NormalEl<br>
   //! The elevation of the surface normal at the point of intersection.
   //!
   float NormalEl;

   //=========================================================
   //! ValidV1or2<br>
   //! Data is valid for Cigi Version 1 and 2
   //!
   bool ValidV1or2;


};

#endif // #if !defined(_CIGI_BASE_LOS_RESP_INCLUDED_)
