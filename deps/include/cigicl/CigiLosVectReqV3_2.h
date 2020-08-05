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
 *  FILENAME:   CigiLosVectReqV3_2.h
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
 *  04/14/2006 Greg Basler                       Version 1.7
 *  Initial Release for CIGI 3.2 compatibility.
 *  Based on CigiLosVectReqV3.h"
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


#if !defined(_CIGI_LOS_VECT_REQ_V3_2_INCLUDED_)
#define _CIGI_LOS_VECT_REQ_V3_2_INCLUDED_

#include "CigiBaseLosVectReq.h"



class CIGI_SPEC CigiLosVectReqV3_2 : public CigiBaseLosVectReq
{

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiLosVectReqV3_2();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiLosVectReqV3_2();


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
   //! \param ReqTypeIn - Specifies which type of response
   //!    is desired.<br>
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
   //! \param SrcCoordSysIn - Specifies which coordinate system
   //!    is used to define the source or starting point.<br>
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


   //+> ResponseCoordSys

   //=========================================================
   //! Sets the ResponseCoordSys with bound checking control
   //! \param ResponseCoordSysIn - Specifies which coordinate system
   //!    is desired to define the intersection point.<br>
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
   //! \param AlphaThreshIn - The lowest alpha value that will cause
   //!    a report to be generated when intersected.
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
   //! \param EntityIDIn - The ID of the origin entity.
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


   //+> VectAz

   //=========================================================
   //! Sets the VectAz with bound checking control
   //! \param VectAzIn - The azimuth of the LOS vector
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetVectAz(const float VectAzIn, bool bndchk=true);

   //=========================================================
   //! Gets the VectAz value.
   //! \return the current VectAz.
   float GetVectAz(void)
   {
      if(VectAz > 180.0f)
         VectAz -= 360.0f;

      return(VectAz);
   }


   //+> MinRange

   //=========================================================
   //! Sets the MinRange with bound checking control
   //! \param MinRangeIn - The minimum range from the starting
   //!    or source point that will result in a report when intersected.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetMinRange(const float MinRangeIn, bool bndchk=true)
   {
      MinRange = MinRangeIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the MinRange value.
   //! \return the current MinRange.
   float GetMinRange(void) const { return(MinRange); }


   //+> SrcXoff

   //=========================================================
   //! Sets the SrcXoff with bound checking control
   //! \param SrcXoffIn - The offset of the starting or source point
   //!    along the X axis from the origin of the origin entity.
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
   //! \param SrcYoffIn - The offset of the starting or source point
   //!    along the Y axis from the origin of the origin entity.
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
   //! \param SrcZoffIn - The offset of the starting or source point
   //!    along the Z axis from the origin of the origin entity.
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


   //+> Mask

   //=========================================================
   //! Sets the Mask with bound checking control
   //! \param MaskIn - 
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


   //+> UpdatePeriod

   //=========================================================
   //! Sets the UpdatePeriod with bound checking control
   //! \param UpdatePeriodIn - The number of frames between each
   //!   LOS response from the IG.  If zero - the IG should
   //!   only respond with one hat/hot response.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetUpdatePeriod(const Cigi_uint8 UpdatePeriodIn, bool bndchk=true)
   {
      UpdatePeriod = UpdatePeriodIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Update Period value.
   //! \return The number of frames between each
   //!   hat/hot response from the IG.  If zero - the IG should
   //!   only respond with one hat/hot response.
   Cigi_uint8 GetUpdatePeriod(void) const { return(UpdatePeriod); }



};

#endif // #if !defined(_CIGI_LOS_VECT_REQ_V3_2_INCLUDED_)
