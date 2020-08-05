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
 *  FILENAME:   CigiBaseLosVectReq.h
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
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  Moved Packet information to base packet.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#if !defined(_CIGI_BASE_LOS_VECT_REQ_INCLUDED_)
#define _CIGI_BASE_LOS_VECT_REQ_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_LOS_VECT_REQ_PACKET_ID_V1 43
#define CIGI_LOS_VECT_REQ_PACKET_SIZE_V1 40

#define CIGI_LOS_VECT_REQ_PACKET_ID_V2 43
#define CIGI_LOS_VECT_REQ_PACKET_SIZE_V2 48

#define CIGI_LOS_VECT_REQ_PACKET_ID_V3 26
#define CIGI_LOS_VECT_REQ_PACKET_SIZE_V3 56

#define CIGI_LOS_VECT_REQ_PACKET_ID_V3_2 26
#define CIGI_LOS_VECT_REQ_PACKET_SIZE_V3_2 56


class CigiLosVectReqV1;
class CigiLosVectReqV2;
class CigiLosVectReqV3;
class CigiLosVectReqV3_2;


class CIGI_SPEC CigiBaseLosVectReq : public CigiBasePacket
{

friend class CigiLosVectReqV1;
friend class CigiLosVectReqV2;
friend class CigiLosVectReqV3;
friend class CigiLosVectReqV3_2;

public:

   //=========================================================
   //! The enumeration for the CigiBaseLosVectReq Group
   //!
   enum ReqTypeGrp
   {
      Basic=0,
      Extended=1
   };

   //=========================================================
   //! The enumeration for the CigiBaseLosVectReq Group
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
   CigiBaseLosVectReq();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseLosVectReq();


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

   //=========================================================
   //! A virtual Conversion Information function.
   //! This function provides conversion information for this
   //!  packet.
   //! \param CnvtVersion - The CIGI version to which this packet
   //!    is being converted.
   //! \param CnvtInfo - The information needed for conversion
   //!    
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	virtual int GetCnvt(CigiVersionID &CnvtVersion,
                       CigiCnvtInfoType::Type &CnvtInfo);



   //==> Accessing Member Variable Values functions

   //+> LosID

   //=========================================================
   //! Sets the LosID with bound checking control
   //! \param LosIDIn - Line Of Sight ID
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


   //+> VectEl

   //=========================================================
   //! Sets the VectEl with bound checking control
   //! \param VectElIn - Vector Elevation
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetVectEl(const float VectElIn, bool bndchk=true);

   //=========================================================
   //! Gets the VectEl value.
   //! \return the current VectEl.
   float GetVectEl(void) const { return(VectEl); }


   //+> MaxRange

   //=========================================================
   //! Sets the MaxRange with bound checking control
   //! \param MaxRangeIn - Maximum range of the vector
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetMaxRange(const float MaxRangeIn, bool bndchk=true)
   {
      MaxRange = MaxRangeIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the MaxRange value.
   //! \return the current MaxRange.
   float GetMaxRange(void) const { return(MaxRange); }


   //+> SrcLat

   //=========================================================
   //! Sets the SrcLat with bound checking control
   //! \param SrcLatIn - Source or Starting Point Latitude
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSrcLat(const double SrcLatIn, bool bndchk=true);

   //=========================================================
   //! Gets the SrcLat value.
   //! \return the current SrcLat.
   double GetSrcLat(void) const { return(SrcXLat); }


   //+> SrcLon

   //=========================================================
   //! Sets the SrcLon with bound checking control
   //! \param SrcLonIn - Source or Starting Point Longitude
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSrcLon(const double SrcLonIn, bool bndchk=true);

   //=========================================================
   //! Gets the SrcLon value.
   //! \return the current SrcLon.
   double GetSrcLon(void) const { return(SrcYLon); }


   //+> SrcAlt

   //=========================================================
   //! Sets the SrcAlt with bound checking control
   //! \param SrcAltIn - Source or Starting Point Altitude
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSrcAlt(const double SrcAltIn, bool bndchk=true)
   {
      SrcZAlt = SrcAltIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the SrcAlt value.
   //! \return the current SrcAlt.
   double GetSrcAlt(void) const { return(SrcZAlt); }



protected:

   //==> Member variables

   //=========================================================
   //! LosID<br>
   //! Line Of Sight ID
   //!
   Cigi_uint16 LosID;

   //=========================================================
   //! ReqType<br>
   //!  0-Basic<br>
   //!  1-Extended
   //! Requests a specific type of result
   //!
   ReqTypeGrp ReqType;

   //=========================================================
   //! SrcCoordSys<br>
   //!  0-Geodetic<br>
   //!  1-Entity
   //! The Coordinate system of the source or starting point.
   //!
   CoordSysGrp SrcCoordSys;

   //=========================================================
   //! ResponseCoordSys<br>
   //!  0-Geodetic<br>
   //!  1-Entity
   //! The Coordinate system of the response intersection point.
   //!
   CoordSysGrp ResponseCoordSys;

   //=========================================================
   //! AlphaThresh<br>
   //! The Lowest Alpha level that will cause a response
   //!
   Cigi_uint8 AlphaThresh;

   //=========================================================
   //! EntityID<br>
   //! The ID of the originating entity.
   //!
   Cigi_uint16 EntityID;

   //=========================================================
   //! VectAz<br>
   //! The Azimuth angle of the LOS vector
   //!
   float VectAz;

   //=========================================================
   //! VectEl<br>
   //! The Elevation angle of the LOS vector
   //!
   float VectEl;

   //=========================================================
   //! MinRange<br>
   //! The Minimum range of the LOS vector that will cause
   //!   an intersection response.
   //!
   float MinRange;

   //=========================================================
   //! MaxRange<br>
   //! The Maximum range of the LOS vector.
   //!
   float MaxRange;

   //=========================================================
   //! SrcLat<br>
   //! The latitude of the starting or source point.
   //! OR
   //! The offset of the starting or source point 
   //!   along the X axis of the originating entity.
   //!
   double SrcXLat;

   //=========================================================
   //! SrcLon<br>
   //! The longitude of the starting or source point.
   //! OR
   //! The offset of the starting or source point 
   //!   along the Y axis of the originating entity.
   //!
   double SrcYLon;

   //=========================================================
   //! SrcAlt<br>
   //! The altitude of the starting or source point
   //! OR
   //! The offset of the starting or source point 
   //!   along the Z axis of the originating entity.
   //!
   double SrcZAlt;

   //=========================================================
   //! Mask<br>
   //! The mask to specify which materials cause a response to
   //!   be generated when intersected.
   //!
   Cigi_uint32 Mask;

   //=========================================================
   //! UpdatePeriod<br>
   //! The number of frames between each LOS update from
   //!  the IG.  If 0 - single shot
   //!
   Cigi_uint8 UpdatePeriod;

   //=========================================================
   //! ValidV1or2<br>
   //! Flag specifying the data is valid for CIGI version 1 or 2
   //!
   bool ValidV1or2;

};

#endif // #if !defined(_CIGI_BASE_LOS_VECT_REQ_INCLUDED_)
