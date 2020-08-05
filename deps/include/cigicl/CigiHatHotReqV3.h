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
 *  FILENAME:   CigiHatHotReqV3.h
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


#if !defined(_CIGI_HAT_HOT_REQ_V3_INCLUDED_)
#define _CIGI_HAT_HOT_REQ_V3_INCLUDED_

#include "CigiBaseHatHotReq.h"



class CIGI_SPEC CigiHatHotReqV3 : public CigiBaseHatHotReq
{

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiHatHotReqV3();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiHatHotReqV3();


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

   //+> ReqType

   //=========================================================
   //! Sets the ReqType with bound checking control
   //! \param ReqTypeIn - The type of request and response desired<br>
   //!   HAT=0<br>
   //!   HOT=1<br>
   //!   Extended=2<br>
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
   //! \param SrcCoordSysIn - The Coordinate system of the point data<br>
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


   //+> EntityID

   //=========================================================
   //! Sets the EntityID with bound checking control
   //! \param EntityIDIn - The ID of the Origin Entity
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
   //! \param XoffIn - The position of the request point 
   //!   specified as an offset along the X axis of the origin  
   //!   entity and measured from the origin of that entity.
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
   //! \param YoffIn - The position of the request point 
   //!   specified as an offset along the Y axis of the origin  
   //!   entity and measured from the origin of that entity.
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


   //+> Alt

   //=========================================================
   //! Sets the Alt with bound checking control
   //! \param AltIn - The altitude of the request point
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetAlt(const double AltIn, bool bndchk=true)
   {
      AltOrZoff = AltIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Alt value.
   //! \return the current Alt.
   double GetAlt(void) const { return(AltOrZoff); }


   //+> Zoff

   //=========================================================
   //! Sets the Zoff with bound checking control
   //! \param ZoffIn - The position of the request point 
   //!   specified as an offset along the Z axis of the origin  
   //!   entity and measured from the origin of that entity.
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



};

#endif // #if !defined(_CIGI_HAT_HOT_REQ_V3_INCLUDED_)
