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
 *  FILENAME:   CigiBaseRateCtrl.h
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
 *  CigiRateCtrlV3_2 class.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added the CoordSys member variable to indicate which coordinate system 
 *  rates are applied to.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added the CoordSysGrp enumeration for the CigiBaseLosSegReq group.
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


#if !defined(_CIGI_BASE_RATE_CTRL_INCLUDED_)
#define _CIGI_BASE_RATE_CTRL_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_RATE_CTRL_PACKET_ID_V1 5
#define CIGI_RATE_CTRL_PACKET_SIZE_V1 32

#define CIGI_RATE_CTRL_PACKET_ID_V2 5
#define CIGI_RATE_CTRL_PACKET_SIZE_V2 32

#define CIGI_RATE_CTRL_PACKET_ID_V3 8
#define CIGI_RATE_CTRL_PACKET_SIZE_V3 32

#define CIGI_RATE_CTRL_PACKET_ID_V3_2 8
#define CIGI_RATE_CTRL_PACKET_SIZE_V3_2 32


class CigiRateCtrlV1;
class CigiRateCtrlV2;
class CigiRateCtrlV3;
class CigiRateCtrlV3_2;


class CIGI_SPEC CigiBaseRateCtrl : public CigiBasePacket
{

friend class CigiRateCtrlV1;
friend class CigiRateCtrlV2;
friend class CigiRateCtrlV3;
friend class CigiRateCtrlV3_2;

public:


   //=========================================================
   //! The enumeration for the CigiBaseLosSegReq Group
   //!
   enum CoordSysGrp
   {
      World=0,
      Parent=0,
      Local=1
   };


   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseRateCtrl() { };

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseRateCtrl() { };


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
                       CigiCnvtInfoType::Type &CnvtInfo)
   {
      CnvtInfo.ProcID = CigiProcessType::ProcStd;

      // V1 & V2 use the same packet id number
      if(CnvtVersion.CigiMajorVersion < 3)
         CnvtInfo.CnvtPacketID = CIGI_RATE_CTRL_PACKET_ID_V2;
      else
         CnvtInfo.CnvtPacketID = CIGI_RATE_CTRL_PACKET_ID_V3;

      return(CIGI_SUCCESS);
   }



   //==> Accessing Member Variable Values functions

   //+> EntityID

   //=========================================================
   //! Sets the EntityID with bound checking control
   //! \param EntityIDIn - Specifies the entity to which this
   //!   packet's data is applied.
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


   //+> XRate

   //=========================================================
   //! Sets the XRate with bound checking control
   //! \param XRateIn - The entity's or art part's X velocity.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetXRate(const float XRateIn, bool bndchk=true)
   {
      XRate = XRateIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the XRate value.
   //! \return the current XRate.
   float GetXRate(void) const { return(XRate); }


   //+> YRate

   //=========================================================
   //! Sets the YRate with bound checking control
   //! \param YRateIn - The entity's or art part's Y velocity.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetYRate(const float YRateIn, bool bndchk=true)
   {
      YRate = YRateIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the YRate value.
   //! \return the current YRate.
   float GetYRate(void) const { return(YRate); }


   //+> ZRate

   //=========================================================
   //! Sets the ZRate with bound checking control
   //! \param ZRateIn - The entity's or art part's Z velocity.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetZRate(const float ZRateIn, bool bndchk=true)
   {
      ZRate = ZRateIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ZRate value.
   //! \return the current ZRate.
   float GetZRate(void) const { return(ZRate); }


   //+> RollRate

   //=========================================================
   //! Sets the RollRate with bound checking control
   //! \param RollRateIn - The entity's or art part's roll rate.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRollRate(const float RollRateIn, bool bndchk=true)
   {
      RollRate = RollRateIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the RollRate value.
   //! \return the current RollRate.
   float GetRollRate(void) const { return(RollRate); }


   //+> PitchRate

   //=========================================================
   //! Sets the PitchRate with bound checking control
   //! \param PitchRateIn - The entity's or art part's pitch rate.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetPitchRate(const float PitchRateIn, bool bndchk=true)
   {
      PitchRate = PitchRateIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the PitchRate value.
   //! \return the current PitchRate.
   float GetPitchRate(void) const { return(PitchRate); }


   //+> YawRate

   //=========================================================
   //! Sets the YawRate with bound checking control
   //! \param YawRateIn - The entity's or art part's yaw rate.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetYawRate(const float YawRateIn, bool bndchk=true)
   {
      YawRate = YawRateIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the YawRate value.
   //! \return the current YawRate.
   float GetYawRate(void) const { return(YawRate); }



protected:

   //==> Member variables

   //=========================================================
   //! EntityID<br>
   //! Uniquely identifies the specific entity.
   //!
   Cigi_uint16 EntityID;

   //=========================================================
   //! ArtPartIDV1<br>
   //! The Unique ID for an articulated part of a specific entity.<br>
   //! If -1 - This data is applied to the Entity.<br>
   //! If any other value - This data is applied to the articulated part.
   //!
   Cigi_int8 ArtPartIDV1;

   //=========================================================
   //! ArtPartIDV3<br>
   //! The Unique ID for an articulated part of a specific entity.<br>
   //!
   Cigi_uint8 ArtPartIDV3;

   //=========================================================
   //! ApplyToArtPart<br>
   //! Flag specifying whether this is applied to an entity
   //!  or an articulated part.<br>
   //! true - Applied to the articulated part.
   //! false - Applied to the entity.
   //!
   bool ApplyToArtPart;

   //=========================================================
   //! ArtPartIDV3<br>
   //! The Coordinate system in which the rates are applied.<br>
   //! 0-World/Parent<br>
   //! 1-Local
   //!
   CoordSysGrp CoordSys;

   //=========================================================
   //! XRate<br>
   //! The meters per second velocity along the X axis.<br>
   //!  For an entity - The Geodetic surface NED X axis.<br>
   //!  For an art part - The host entity's body X axis.
   //!
   float XRate;

   //=========================================================
   //! YRate<br>
   //! The meters per second velocity along the Y axis.<br>
   //!  For an entity - The Geodetic surface NED Y axis.<br>
   //!  For an art part - The host entity's body Y axis.
   //!
   float YRate;

   //=========================================================
   //! ZRate<br>
   //! The meters per second velocity along the Z axis.<br>
   //!  For an entity - The Geodetic surface NED Z axis.<br>
   //!  For an art part - The host entity's body Z axis.
   //!
   float ZRate;

   //=========================================================
   //! RollRate<br>
   //! The degrees per second velocity about the X axis.<br>
   //!  For an entity - The Geodetic surface NED pitch and
   //!    yaw rotated X axis.<br>
   //!  For an art part - The host entity's body X axis.
   //!
   float RollRate;

   //=========================================================
   //! PitchRate<br>
   //! The degrees per second velocity about the Y axis.<br>
   //!  For an entity - The Geodetic surface NED yaw rotated Y axis.<br>
   //!  For an art part - The host entity's body Y axis.
   //!
   float PitchRate;

   //=========================================================
   //! YawRate<br>
   //! The degrees per second velocity about the Z axis.<br>
   //!  For an entity - The Geodetic surface NED Z axis.<br>
   //!  For an art part - The host entity's body Z axis.
   //!
   float YawRate;


};

#endif // #if !defined(_CIGI_BASE_RATE_CTRL_INCLUDED_)
