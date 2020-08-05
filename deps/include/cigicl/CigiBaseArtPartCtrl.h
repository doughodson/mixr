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
 *  FILENAME:   CigiBaseArtPartCtrl.h
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


#if !defined(_CIGI_BASE_ART_PART_CTRL_INCLUDED_)
#define _CIGI_BASE_ART_PART_CTRL_INCLUDED_

#include "CigiBasePacket.h"


// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_ART_PART_CTRL_PACKET_ID_V1 4
#define CIGI_ART_PART_CTRL_PACKET_SIZE_V1 20

#define CIGI_ART_PART_CTRL_PACKET_ID_V2 4
#define CIGI_ART_PART_CTRL_PACKET_SIZE_V2 32

#define CIGI_ART_PART_CTRL_PACKET_ID_V3 6
#define CIGI_ART_PART_CTRL_PACKET_SIZE_V3 32


class CigiArtPartCtrlV1;
class CigiArtPartCtrlV2;
class CigiArtPartCtrlV3;


class CIGI_SPEC CigiBaseArtPartCtrl : public CigiBasePacket
{

friend class CigiArtPartCtrlV1;
friend class CigiArtPartCtrlV2;
friend class CigiArtPartCtrlV3;

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseArtPartCtrl();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseArtPartCtrl();


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


   //+> ArtPartEn

   //=========================================================
   //! Sets the ArtPartEn with bound checking control
   //! \param ArtPartEnIn - Specified articulated part enabled or disabled
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetArtPartEn(const bool ArtPartEnIn, bool bndchk=true)
   {
      ArtPartEn = ArtPartEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ArtPartEn value.
   //! \return the current ArtPartEn.
   bool GetArtPartEn(void) const { return(ArtPartEn); }


   //+> XOffEn

   //=========================================================
   //! Sets the XOffEn with bound checking control
   //! \param XOffEnIn - X offset value is valid for use.(true)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetXOffEn(const bool XOffEnIn, bool bndchk=true)
   {
      XOffEn = XOffEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the XOffEn value.
   //! \return the current XOffEn.
   bool GetXOffEn(void) const { return(XOffEn); }


   //+> YOffEn

   //=========================================================
   //! Sets the YOffEn with bound checking control
   //! \param YOffEnIn - Y offset value is valid for use.(true)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetYOffEn(const bool YOffEnIn, bool bndchk=true)
   {
      YOffEn = YOffEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the YOffEn value.
   //! \return the current YOffEn.
   bool GetYOffEn(void) const { return(YOffEn); }


   //+> ZOffEn

   //=========================================================
   //! Sets the ZOffEn with bound checking control
   //! \param ZOffEnIn - Z offset value is valid for use.(true)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetZOffEn(const bool ZOffEnIn, bool bndchk=true)
   {
      ZOffEn = ZOffEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ZOffEn value.
   //! \return the current ZOffEn.
   bool GetZOffEn(void) const { return(ZOffEn); }


   //+> RollEn

   //=========================================================
   //! Sets the RollEn with bound checking control
   //! \param RollEnIn - Roll value is valid for use.(true)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRollEn(const bool RollEnIn, bool bndchk=true)
   {
      RollEn = RollEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the RollEn value.
   //! \return the current RollEn.
   bool GetRollEn(void) const { return(RollEn); }


   //+> PitchEn

   //=========================================================
   //! Sets the PitchEn with bound checking control
   //! \param PitchEnIn - Pitch value is valid for use.(true)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetPitchEn(const bool PitchEnIn, bool bndchk=true)
   {
      PitchEn = PitchEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the PitchEn value.
   //! \return the current PitchEn.
   bool GetPitchEn(void) const { return(PitchEn); }


   //+> YawEn

   //=========================================================
   //! Sets the YawEn with bound checking control
   //! \param YawEnIn - Yaw value is valid for use.(true)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetYawEn(const bool YawEnIn, bool bndchk=true)
   {
      YawEn = YawEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the YawEn value.
   //! \return the current YawEn.
   bool GetYawEn(void) const { return(YawEn); }


   //+> XOff

   //=========================================================
   //! Sets the XOff with bound checking control
   //! \param XOffIn - The Offset from the specified entity's origin
   //!   along the entity's X axis.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetXOff(const float XOffIn, bool bndchk=true)
   {
      XOff = XOffIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the XOff value.
   //! \return the current XOff.
   float GetXOff(void) const { return(XOff); }


   //+> YOff

   //=========================================================
   //! Sets the YOff with bound checking control
   //! \param YOffIn - The Offset from the specified entity's origin
   //!   along the entity's Y axis.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetYOff(const float YOffIn, bool bndchk=true)
   {
      YOff = YOffIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the YOff value.
   //! \return the current YOff.
   float GetYOff(void) const { return(YOff); }


   //+> ZOff

   //=========================================================
   //! Sets the ZOff with bound checking control
   //! \param ZOffIn - The Offset from the specified entity's origin
   //!   along the entity's Z axis.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetZOff(const float ZOffIn, bool bndchk=true)
   {
      ZOff = ZOffIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ZOff value.
   //! \return the current ZOff.
   float GetZOff(void) const { return(ZOff); }


   //+> Roll

   //=========================================================
   //! Sets the Roll with bound checking control
   //! \param RollIn - The roll about the specified articulated part's
   //!   origin relative to the entity's body coordinate system.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRoll(const float RollIn, bool bndchk=true);

   //=========================================================
   //! Gets the Roll value.
   //! \return the current Roll.
   float GetRoll(void) const { return(Roll); }


   //+> Pitch

   //=========================================================
   //! Sets the Pitch with bound checking control
   //! \param PitchIn - The pitch about the specified articulated part's
   //!   origin relative to the entity's body coordinate system.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetPitch(const float PitchIn, bool bndchk=true);

   //=========================================================
   //! Gets the Pitch value.
   //! \return the current Pitch.
   float GetPitch(void) const { return(Pitch); }


   //+> Yaw

   //=========================================================
   //! Sets the Yaw with bound checking control
   //! \param YawIn - The yaw about the specified articulated part's
   //!   origin relative to the entity's body coordinate system.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetYaw(const float YawIn, bool bndchk=true);

   //=========================================================
   //! Gets the Yaw value.
   //! \return the current Yaw.
   float GetYaw(void) const { return(Yaw); }



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
   //!
   Cigi_int8 ArtPartIDV1;

   //=========================================================
   //! ArtPartIDV3<br>
   //! The Unique ID for an articulated part of a specific entity.<br>
   //!
   Cigi_uint8 ArtPartIDV3;

   //=========================================================
   //! ArtPartEn<br>
   //! Articulated part is enabled or disabled for use and display.
   //! true - valid - use and display.<br>
   //! false - not valid  - do not use and display.<br>
   //!
   bool ArtPartEn;

   //=========================================================
   //! XOffEn<br>
   //! The X offset data is valid for use.<br>
   //! true - valid and should be used.<br>
   //! false - not valid and should not be used.<br>
   //!
   bool XOffEn;

   //=========================================================
   //! YOffEn<br>
   //! The X offset data is valid for use.<br>
   //! true - valid and should be used.<br>
   //! false - not valid and should not be used.<br>
   //!
   bool YOffEn;

   //=========================================================
   //! ZOffEn<br>
   //! The X offset data is valid for use.<br>
   //! true - valid and should be used.<br>
   //! false - not valid and should not be used.<br>
   //!
   bool ZOffEn;

   //=========================================================
   //! RollEn<br>
   //! The roll data is valid for use.<br>
   //! true - valid and should be used.<br>
   //! false - not valid and should not be used.<br>
   //!
   bool RollEn;

   //=========================================================
   //! PitchEn<br>
   //! The pitch data is valid for use.<br>
   //! true - valid and should be used.<br>
   //! false - not valid and should not be used.<br>
   //!
   bool PitchEn;

   //=========================================================
   //! YawEn<br>
   //! The yaw data is valid for use.<br>
   //! true - valid and should be used.<br>
   //! false - not valid and should not be used.<br>
   //!
   bool YawEn;

   //=========================================================
   //! XOff<br>
   //! The Offset from the specified entity's origin
   //!   along the entity's Z axis in meters.
   //!
   float XOff;

   //=========================================================
   //! YOff<br>
   //! The Offset from the specified entity's origin
   //!   along the entity's Z axis in meters.
   //!
   float YOff;

   //=========================================================
   //! ZOff<br>
   //! The Offset from the specified entity's origin
   //!   along the entity's Z axis in meters.
   //!
   float ZOff;

   //=========================================================
   //! Roll<br>
   //! The roll about the specified articulated part's
   //!   origin relative to the entity's body coordinate system
   //!   in degrees.
   //!
   float Roll;

   //=========================================================
   //! Pitch<br>
   //! The pitch about the specified articulated part's
   //!   origin relative to the entity's body coordinate system
   //!   in degrees.
   //!
   float Pitch;

   //=========================================================
   //! Yaw<br>
   //! The yaw about the specified articulated part's
   //!   origin relative to the entity's body coordinate system
   //!   in degrees.
   //!
   float Yaw;


};

#endif // #if !defined(_CIGI_BASE_ART_PART_CTRL_INCLUDED_)
