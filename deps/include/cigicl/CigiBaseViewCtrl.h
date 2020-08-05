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
 *  FILENAME:   CigiBaseViewCtrl.h
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


#if !defined(_CIGI_BASE_VIEW_CTRL_INCLUDED_)
#define _CIGI_BASE_VIEW_CTRL_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_VIEW_CTRL_PACKET_ID_V1 8
#define CIGI_VIEW_CTRL_PACKET_SIZE_V1 32

#define CIGI_VIEW_CTRL_PACKET_ID_V2 8
#define CIGI_VIEW_CTRL_PACKET_SIZE_V2 32

#define CIGI_VIEW_CTRL_PACKET_ID_V3 16
#define CIGI_VIEW_CTRL_PACKET_SIZE_V3 32


class CigiViewCtrlV1;
class CigiViewCtrlV2;
class CigiViewCtrlV3;


class CIGI_SPEC CigiBaseViewCtrl : public CigiBasePacket
{

friend class CigiViewCtrlV1;
friend class CigiViewCtrlV2;
friend class CigiViewCtrlV3;

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseViewCtrl();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseViewCtrl();


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
   //! \param EntityIDIn - Identifies the entity which the view
   //!   or view group should be attached.
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
   //! \param RollIn - The roll about the specified entity's
   //!   body coordinate system.
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
   //! \param PitchIn - The pitch about the specified entity's
   //!   body coordinate system.
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
   //! \param YawIn - The yaw about the specified entity's
   //!   body coordinate system.
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
   //! Uniquely identifies the specific entity<br>
   //!
   Cigi_uint16 EntityID;

   //=========================================================
   //! ViewID<br>
   //! Specifies the view that this packet controls.<br>
   //!  If The Group ID is not 0, ViewID is ignored.
   //!
   Cigi_uint16 ViewID;

   //=========================================================
   //! GroupID<br>
   //! Specifies the view group that this packet controls.
   //!
   Cigi_uint8 GroupID;

   //=========================================================
   //! XOffEn<br>
   //! X Offset data valid and to be used.
   //!
   bool XOffEn;

   //=========================================================
   //! YOffEn<br>
   //! Y Offset data valid and to be used.
   //!
   bool YOffEn;

   //=========================================================
   //! ZOffEn<br>
   //! Z Offset data valid and to be used.
   //!
   bool ZOffEn;

   //=========================================================
   //! RollEn<br>
   //! Roll data valid and to be used.
   //!
   bool RollEn;

   //=========================================================
   //! PitchEn<br>
   //! Pitch  data valid and to be used.
   //!
   bool PitchEn;

   //=========================================================
   //! YawEn<br>
   //! Yaw  data valid and to be used.
   //!
   bool YawEn;

   //=========================================================
   //! XOff<br>
   //! The Offset from the specified entity's origin
   //!   along the entity's X axis.
   //!
   float XOff;

   //=========================================================
   //! YOff<br>
   //! The Offset from the specified entity's origin
   //!   along the entity's Y axis.
   //!
   float YOff;

   //=========================================================
   //! ZOff<br>
   //! The Offset from the specified entity's origin
   //!   along the entity's Z axis.
   //!
   float ZOff;

   //=========================================================
   //! Roll<br>
   //! The roll about the specified entity's body coordinate system.
   //!
   float Roll;

   //=========================================================
   //! Pitch<br>
   //! The pitch about the specified entity's body coordinate system.
   //!
   float Pitch;

   //=========================================================
   //! Yaw<br>
   //! The yaw about the specified entity's body coordinate system.
   //!
   float Yaw;


};

#endif // #if !defined(_CIGI_BASE_VIEW_CTRL_INCLUDED_)
