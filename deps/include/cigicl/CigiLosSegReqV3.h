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
 *  FILENAME:   CigiLosSegReqV3.h
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
 *  Moved Packet information to base packet.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#if !defined(_CIGI_LOS_SEG_REQ_V3_INCLUDED_)
#define _CIGI_LOS_SEG_REQ_V3_INCLUDED_

#include "CigiBaseLosSegReq.h"



class CIGI_SPEC CigiLosSegReqV3 : public CigiBaseLosSegReq
{

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiLosSegReqV3();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiLosSegReqV3();


   //==> Buffer Packing/Unpacking

   //=========================================================
   //! The virtual Pack function for CIGI 3
   //! \param Base - A pointer to the instance of the packet 
   //!          to be packed. (Downcast to CigiBasePacket)
   //! \param Buff - A pointer to the current pack point.
   //! \param Spec - A pointer to special data -
   //!          This is not used in this class.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const;

   //=========================================================
   //! The virtual Unpack function for CIGI 3
   //! \param Buff - A pointer to the current pack point.
   //! \param Swap - N/A for V1 & V2
   //! \param Spec - A pointer to special data -
   //!          This is not used in this class.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec);



   //==> Accessing Member Variable Values functions

   //+> ReqType

   //=========================================================
   //! Sets the ReqType with bound checking control
   //! \param ReqTypeIn - Request type<br>
   //!   This specifies whether the response should be basic or extended.<br>
   //!   Basic=0<br>
   //!   Extended=1<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetReqType(const ReqTypeGrp ReqTypeIn, bool bndchk=true);

   //=========================================================
   //! Gets the ReqType value.
   //! \return the current ReqType.
   ReqTypeGrp GetReqType(void) const { return(ReqType); }


   //+> SrcCoordSys

   //=========================================================
   //! Sets the SrcCoordSys with bound checking control
   //! \param SrcCoordSysIn - Specifies in which coordinate system
   //!   the starting point is specified.<br>
   //!   Geodetic=0<br>
   //!   Entity=1<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSrcCoordSys(const CoordSysGrp SrcCoordSysIn, bool bndchk=true);

   //=========================================================
   //! Gets the SrcCoordSys value.
   //! \return the current SrcCoordSys.
   CoordSysGrp GetSrcCoordSys(void) const { return(SrcCoordSys); }


   //+> DstCoordSys

   //=========================================================
   //! Sets the DstCoordSys with bound checking control
   //! \param DstCoordSysIn - Specifies in which coordinate system
   //!   the ending point is specified.<br>
   //!   Geodetic=0<br>
   //!   Entity=1<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDstCoordSys(const CoordSysGrp DstCoordSysIn, bool bndchk=true);

   //=========================================================
   //! Gets the DstCoordSys value.
   //! \return the current DstCoordSys.
   CoordSysGrp GetDstCoordSys(void) const { return(DstCoordSys); }


   //+> ResponseCoordSys

   //=========================================================
   //! Sets the ResponseCoordSys with bound checking control
   //! \param ResponseCoordSysIn - Specifies in which coordinate system
   //!   the response intersection point is specified.<br>
   //!   Geodetic=0<br>
   //!   Entity=1<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetResponseCoordSys(const CoordSysGrp ResponseCoordSysIn, bool bndchk=true);

   //=========================================================
   //! Gets the ResponseCoordSys value.
   //! \return the current ResponseCoordSys.
   CoordSysGrp GetResponseCoordSys(void) const { return(ResponseCoordSys); }


   //+> AlphaThresh

   //=========================================================
   //! Sets the AlphaThresh with bound checking control
   //! \param AlphaThreshIn - The IG will only register an intersection
   //!   when the intersected polygon has an alpha of this value or higher.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetAlphaThresh(const Cigi_uint8 AlphaThreshIn, bool bndchk=true)
   {
      AlphaThresh = AlphaThreshIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the AlphaThresh value.
   //! \return the current AlphaThresh.
   Cigi_uint8 GetAlphaThresh(void) const { return(AlphaThresh); }


   //+> EntityID

   //=========================================================
   //! Sets the EntityID with bound checking control
   //! \param EntityIDIn - The Entity ID that the source and/or destination
   //!   point is measured from when the coordinate system for that point
   //!   is specified as "Entity"
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetEntityID(const Cigi_uint16 EntityIDIn, bool bndchk=true)
   {
      EntityID = EntityIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the EntityID value.
   //! \return the current EntityID.
   Cigi_uint16 GetEntityID(void) const { return(EntityID); }


   //+> SrcXoff

   //=========================================================
   //! Sets the SrcXoff with bound checking control
   //! \param SrcXoffIn - The source point X axis offset in meters from
   //!   the specified entity's origin.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSrcXoff(const double SrcXoffIn, bool bndchk=true)
   {
      SrcXLat = SrcXoffIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the SrcXoff value.
   //! \return the current SrcXoff.
   double GetSrcXoff(void) const { return(SrcXLat); }


   //+> SrcYoff

   //=========================================================
   //! Sets the SrcYoff with bound checking control
   //! \param SrcYoffIn - The source point Y axis offset in meters from
   //!   the specified entity's origin.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSrcYoff(const double SrcYoffIn, bool bndchk=true)
   {
      SrcYLon = SrcYoffIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the SrcYoff value.
   //! \return the current SrcYoff.
   double GetSrcYoff(void) const { return(SrcYLon); }


   //+> SrcZoff

   //=========================================================
   //! Sets the SrcZoff with bound checking control
   //! \param SrcZoffIn - The source point Z axis offset in meters from
   //!   the specified entity's origin.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSrcZoff(const double SrcZoffIn, bool bndchk=true)
   {
      SrcZAlt = SrcZoffIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the SrcZoff value.
   //! \return the current SrcZoff.
   double GetSrcZoff(void) const { return(SrcZAlt); }


   //+> DstXoff

   //=========================================================
   //! Sets the DstXoff with bound checking control
   //! \param DstXoffIn - The destination point X axis offset in meters from
   //!   the specified entity's origin.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDstXoff(const double DstXoffIn, bool bndchk=true)
   {
      DstXLat = DstXoffIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the DstXoff value.
   //! \return the current DstXoff.
   double GetDstXoff(void) const { return(DstXLat); }


   //+> DstYoff

   //=========================================================
   //! Sets the DstYoff with bound checking control
   //! \param DstYoffIn - The destination point Y axis offset in meters from
   //!   the specified entity's origin.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDstYoff(const double DstYoffIn, bool bndchk=true)
   {
      DstYLon = DstYoffIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the DstYoff value.
   //! \return the current DstYoff.
   double GetDstYoff(void) const { return(DstYLon); }


   //+> DstZoff

   //=========================================================
   //! Sets the DstZoff with bound checking control
   //! \param DstZoffIn - The destination point Z axis offset in meters from
   //!   the specified entity's origin.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDstZoff(const double DstZoffIn, bool bndchk=true)
   {
      DstZAlt = DstZoffIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the DstZoff value.
   //! \return the current DstZoff.
   double GetDstZoff(void) const { return(DstZAlt); }


   //+> Mask

   //=========================================================
   //! Sets the Mask with bound checking control
   //! \param MaskIn - The material mask to turn on or off registration
   //!   of intersections with polygons having specific material properties.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetMask(const Cigi_uint32 MaskIn, bool bndchk=true)
   {
      Mask = MaskIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Mask value.
   //! \return the current Mask.
   Cigi_uint32 GetMask(void) const { return(Mask); }



};

#endif // #if !defined(_CIGI_LOS_SEG_REQ_V3_INCLUDED_)
