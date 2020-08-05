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
 *  FILENAME:   CigiLosXRespV3.h
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


#if !defined(_CIGI_LOS_XRESP_V3_INCLUDED_)
#define _CIGI_LOS_XRESP_V3_INCLUDED_

#include "CigiBaseLosResp.h"



class CIGI_SPEC CigiLosXRespV3 : public CigiBaseLosResp
{

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiLosXRespV3();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiLosXRespV3();


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

   //+> EntityIDValid

   //=========================================================
   //! Sets the EntityIDValid with bound checking control
   //! \param EntityIDValidIn - intersection with an entity and
   //!    the entity id is valid.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetEntityIDValid(const bool EntityIDValidIn, bool bndchk=true)
   {
      EntityIDValid = EntityIDValidIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the EntityIDValid value.
   //! \return the current EntityIDValid.
   bool GetEntityIDValid(void) const { return(EntityIDValid); }


   //+> RangeValid

   //=========================================================
   //! Sets the RangeValid with bound checking control
   //! \param RangeValidIn - There is an intersection and the
   //!    range data is valid.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRangeValid(const bool RangeValidIn, bool bndchk=true)
   {
      RangeValid = RangeValidIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the RangeValid value.
   //! \return the current RangeValid.
   bool GetRangeValid(void) const { return(RangeValid); }


   //+> IntersectionCoordSys

   //=========================================================
   //! Sets the IntersectionCoordSys with bound checking control
   //! \param IntersectionCoordSysIn - The intersection position<br>
   //!    data coordinate system.
   //!   Geodetic=0<br>
   //!   Entity=1<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetIntersectionCoordSys(const LOSCoordGrp IntersectionCoordSysIn, bool bndchk=true);

   //=========================================================
   //! Gets the IntersectionCoordSys value.
   //! \return the current IntersectionCoordSys.
   LOSCoordGrp GetIntersectionCoordSys(void) const { return(IntersectionCoordSys); }


   //+> RespCount

   //=========================================================
   //! Sets the RespCount with bound checking control
   //! \param RespCountIn - The number of responses for this LOS operation.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRespCount(const Cigi_uint8 RespCountIn, bool bndchk=true)
   {
      RespCount = RespCountIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the RespCount value.
   //! \return the current RespCount.
   Cigi_uint8 GetRespCount(void) const { return(RespCount); }


   //+> EntityID

   //=========================================================
   //! Sets the EntityID with bound checking control
   //! \param EntityIDIn - The ID of the entity intersected.
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


   //+> Xoff

   //=========================================================
   //! Sets the Xoff with bound checking control
   //! \param XoffIn - The offset along the X axis of the
   //!    intersected entity from its origin.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetXoff(const double XoffIn, bool bndchk=true)
   {
      LatOrXoff = XoffIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Xoff value.
   //! \return the current Xoff.
   double GetXoff(void) const { return(LatOrXoff); }


   //+> Yoff

   //=========================================================
   //! Sets the Yoff with bound checking control
   //! \param YoffIn - The offset along the Y axis of the
   //!    intersected entity from its origin.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetYoff(const double YoffIn, bool bndchk=true)
   {
      LonOrYoff = YoffIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Yoff value.
   //! \return the current Yoff.
   double GetYoff(void) const { return(LonOrYoff); }


   //+> Zoff

   //=========================================================
   //! Sets the Zoff with bound checking control
   //! \param ZoffIn - The offset along the Z axis of the
   //!    intersected entity from its origin.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetZoff(const double ZoffIn, bool bndchk=true)
   {
      AltOrZoff = ZoffIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Zoff value.
   //! \return the current Zoff.
   double GetZoff(void) const { return(AltOrZoff); }


   //+> Red

   //=========================================================
   //! Sets the Red with bound checking control
   //! \param RedIn - The red color component of the surface at 
   //!   the point ofintersection.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRed(const Cigi_uint8 RedIn, bool bndchk=true)
   {
      Red = RedIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Red value.
   //! \return the current Red.
   Cigi_uint8 GetRed(void) const { return(Red); }


   //+> Green

   //=========================================================
   //! Sets the Green with bound checking control
   //! \param GreenIn - The green color component of the surface at 
   //!   the point ofintersection.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetGreen(const Cigi_uint8 GreenIn, bool bndchk=true)
   {
      Green = GreenIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Green value.
   //! \return the current Green.
   Cigi_uint8 GetGreen(void) const { return(Green); }


   //+> Blue

   //=========================================================
   //! Sets the Blue with bound checking control
   //! \param BlueIn - The blue color component of the surface at 
   //!   the point ofintersection.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetBlue(const Cigi_uint8 BlueIn, bool bndchk=true)
   {
      Blue = BlueIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Blue value.
   //! \return the current Blue.
   Cigi_uint8 GetBlue(void) const { return(Blue); }


   //+> Alpha

   //=========================================================
   //! Sets the Alpha with bound checking control
   //! \param AlphaIn - The alpha color component of the surface at 
   //!   the point ofintersection.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetAlpha(const Cigi_uint8 AlphaIn, bool bndchk=true)
   {
      Alpha = AlphaIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Alpha value.
   //! \return the current Alpha.
   Cigi_uint8 GetAlpha(void) const { return(Alpha); }


   //+> Material

   //=========================================================
   //! Sets the Material with bound checking control
   //! \param MaterialIn - The material code of the intersected surface.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetMaterial(const Cigi_uint32 MaterialIn, bool bndchk=true)
   {
      Material = MaterialIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Material value.
   //! \return the current Material.
   Cigi_uint32 GetMaterial(void) const { return(Material); }


   //+> NormalAz

   //=========================================================
   //! Sets the NormalAz with bound checking control
   //! \param NormalAzIn - The azimuth of the surface normal at
   //!    the point of intersection.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetNormalAz(const float NormalAzIn, bool bndchk=true);

   //=========================================================
   //! Gets the NormalAz value.
   //! \return the current NormalAz.
   float GetNormalAz(void) const { return(NormalAz); }


   //+> NormalEl

   //=========================================================
   //! Sets the NormalEl with bound checking control
   //! \param NormalElIn - The elevation of the surface normal at 
   //!    the point of intersection.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetNormalEl(const float NormalElIn, bool bndchk=true);

   //=========================================================
   //! Gets the NormalEl value.
   //! \return the current NormalEl.
   float GetNormalEl(void) const { return(NormalEl); }



};

#endif // #if !defined(_CIGI_LOS_XRESP_V3_INCLUDED_)
