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
 *  FILENAME:   CigiBaseLosSegReq.h
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
 *  Added a forward reference and granted friend access to the 
 *  CigiLosSegReqV3_2 class.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added the DestEntityIDValid member variable to determine if a 
 *  detination entity's ID is valid or not.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added the UpdatePeriod member variable to specify the number of frames 
 *  between each LOS update from the IG.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added the DestEntityID member variable to specify the ID of the destination 
 *  entity, if there is one.
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
 *  The Boeing Company
 *
 */


#if !defined(_CIGI_BASE_LOS_SEG_REQ_INCLUDED_)
#define _CIGI_BASE_LOS_SEG_REQ_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_LOS_SEG_REQ_PACKET_ID_V1 42
#define CIGI_LOS_SEG_REQ_PACKET_SIZE_V1 48

#define CIGI_LOS_SEG_REQ_PACKET_ID_V2 42
#define CIGI_LOS_SEG_REQ_PACKET_SIZE_V2 56

#define CIGI_LOS_SEG_REQ_PACKET_ID_V3 25
#define CIGI_LOS_SEG_REQ_PACKET_SIZE_V3 64

#define CIGI_LOS_SEG_REQ_PACKET_ID_V3_2 25
#define CIGI_LOS_SEG_REQ_PACKET_SIZE_V3_2 64


class CigiLosSegReqV1;
class CigiLosSegReqV2;
class CigiLosSegReqV3;
class CigiLosSegReqV3_2;


class CIGI_SPEC CigiBaseLosSegReq : public CigiBasePacket
{

friend class CigiLosSegReqV1;
friend class CigiLosSegReqV2;
friend class CigiLosSegReqV3;
friend class CigiLosSegReqV3_2;

public:

   //=========================================================
   //! The enumeration for the CigiBaseLosSegReq Group
   //!
   enum ReqTypeGrp
   {
      Basic=0,
      Extended=1
   };

   //=========================================================
   //! The enumeration for the CigiBaseLosSegReq Group
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
   CigiBaseLosSegReq();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseLosSegReq();


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


   //+> SrcLat

   //=========================================================
   //! Sets the SrcLat with bound checking control
   //! \param SrcLatIn - Source Point Latitude
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
   //! \param SrcLonIn - Source Point Longitude
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
   //! \param SrcAltIn - Source Point Altitude
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


   //+> DstLat

   //=========================================================
   //! Sets the DstLat with bound checking control
   //! \param DstLatIn - Destination point Latitude
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDstLat(const double DstLatIn, bool bndchk=true);

   //=========================================================
   //! Gets the DstLat value.
   //! \return the current DstLat.
   double GetDstLat(void) const { return(DstXLat); }


   //+> DstLon

   //=========================================================
   //! Sets the DstLon with bound checking control
   //! \param DstLonIn - Destination point Longitude
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDstLon(const double DstLonIn, bool bndchk=true);

   //=========================================================
   //! Gets the DstLon value.
   //! \return the current DstLon.
   double GetDstLon(void) const { return(DstYLon); }


   //+> DstAlt

   //=========================================================
   //! Sets the DstAlt with bound checking control
   //! \param DstAltIn - Destination point Altitude
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDstAlt(const double DstAltIn, bool bndchk=true)
   {
      DstZAlt = DstAltIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the DstAlt value.
   //! \return the current DstAlt.
   double GetDstAlt(void) const { return(DstZAlt); }


   //+> Valid Data for Cigi version 1 or 2

   //=========================================================
   //! Specifies whether the currently stored data is valid for
   //!   CIGI version 1 or version 2.
   //! \return the current DstAlt.
   bool IsValidV1or2(void) const { return(ValidV1or2); }



protected:

   //==> Member variables

   //=========================================================
   //! LosID<br>
   //! Line Of Sight request ID
   //!
   Cigi_uint16 LosID;

   //=========================================================
   //! ReqType<br>
   //!  Specifies which type (Basic or Extended) of response
   //!    is wanted.<br>
   //!  0-Basic<br>
   //!  1-Extended
   //!
   //!
   ReqTypeGrp ReqType;

   //=========================================================
   //! SrcCoordSys<br>
   //!  Specifies in which coordinate system the source
   //!     point is presented.<br>
   //!  0-Geodetic<br>
   //!  1-Entity
   //!
   //!
   CoordSysGrp SrcCoordSys;

   //=========================================================
   //! DstCoordSys<br>
   //!  Specifies in which coordinate system the destination
   //!     point is presented.<br>
   //!  0-Geodetic<br>
   //!  1-Entity
   //!
   //!
   CoordSysGrp DstCoordSys;

   //=========================================================
   //! ResponseCoordSys<br>
   //!  Requests in which coordinate system the response
   //!     point should be presented.<br>
   //!  0-Geodetic<br>
   //!  1-Entity
   //!
   //!
   CoordSysGrp ResponseCoordSys;

   //=========================================================
   //! DestEntityIDValid<br>
   //! Specifies the destination entity's ID is valid or not.
   //!
   bool DestEntityIDValid;

   //=========================================================
   //! AlphaThresh<br>
   //!  The lowest alpha value of a polygon that will result
   //!    in an intersection response.
   //!
   Cigi_uint8 AlphaThresh;

   //=========================================================
   //! EntityID<br>
   //!  The ID of the entity from whose origin the offsets are
   //!    measured.
   //!
   Cigi_uint16 EntityID;

   //=========================================================
   //! SrcXLat<br>
   //!  The Latitude of the source or starting point OR<br>
   //!  The X axis offset of the source or starting point
   //!    from a specified entity's origin.
   //!
   double SrcXLat;

   //=========================================================
   //! SrcYLon<br>
   //!  The Longitude of the source or starting point OR<br>
   //!  The Y axis offset of the source or starting point
   //!    from a specified entity's origin.
   //!
   double SrcYLon;

   //=========================================================
   //! SrcZAlt<br>
   //!  The Altitude of the source or starting point OR<br>
   //!  The Z axis offset of the source or starting point
   //!    from a specified entity's origin.
   //!
   double SrcZAlt;

   //=========================================================
   //! DstXLat<br>
   //!  The Latitude of the destination or ending point OR<br>
   //!  The X axis offset of the destination or ending point
   //!    from a specified entity's origin.
   //!
   double DstXLat;

   //=========================================================
   //! DstYLon<br>
   //!  The Longitude of the destination or ending point OR<br>
   //!  The Y axis offset of the destination or ending point
   //!    from a specified entity's origin.
   //!
   double DstYLon;

   //=========================================================
   //! DstZAlt<br>
   //!  The Altitude of the destination or ending point OR<br>
   //!  The Z axis offset of the destination or ending point
   //!    from a specified entity's origin.
   //!
   double DstZAlt;

   //=========================================================
   //! Mask<br>
   //!  The material mask.
   //!
   Cigi_uint32 Mask;

   //=========================================================
   //! UpdatePeriod<br>
   //! The number of frames between each LOS update from
   //!  the IG.  If 0 - single shot
   //!
   Cigi_uint8 UpdatePeriod;

   //=========================================================
   //! DestEntityID<br>
   //! If there is a destination entity, this specifies the ID
   //!  of the destination entity.
   //!
   Cigi_uint16 DestEntityID;

   //=========================================================
   //! ValidV1or2<br>
   //!  Specifies whether the contained data is in a valid
   //!    format for CIGI 1 or CIGI 2.
   //!
   bool ValidV1or2;


};

#endif // #if !defined(_CIGI_BASE_LOS_SEG_REQ_INCLUDED_)
