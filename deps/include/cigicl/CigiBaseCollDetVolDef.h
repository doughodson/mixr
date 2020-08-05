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
 *  FILENAME:   CigiBaseCollDetVolDef.h
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


#if !defined(_CIGI_BASE_COLL_DET_VOL_DEF_INCLUDED_)
#define _CIGI_BASE_COLL_DET_VOL_DEF_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_COLL_DET_VOL_DEF_PACKET_ID_V2 25
#define CIGI_COLL_DET_VOL_DEF_PACKET_SIZE_V2 20

#define CIGI_COLL_DET_VOL_DEF_PACKET_ID_V3 23
#define CIGI_COLL_DET_VOL_DEF_PACKET_SIZE_V3 48


class CigiCollDetVolDefV2;
class CigiCollDetVolDefV3;


class CIGI_SPEC CigiBaseCollDetVolDef : public CigiBasePacket
{

friend class CigiCollDetVolDefV2;
friend class CigiCollDetVolDefV3;

public:

   //=========================================================
   //! The enumeration for the CigiBaseCollDetVolDef Group
   //!
   enum VolTypeGrp
   {
      Sphere=0,
      Cuboid=1
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseCollDetVolDef();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseCollDetVolDef();


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
   //! \param EntityIDIn - Entity ID
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


   //+> VolID

   //=========================================================
   //! Sets the VolID with bound checking control
   //! \param VolIDIn - Volume ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetVolID(const Cigi_uint8 VolIDIn, bool bndchk=true)
   {
      VolID = VolIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the VolID value.
   //! \return the current VolID.
   Cigi_uint8 GetVolID(void) const { return(VolID); }


   //+> VolEn

   //=========================================================
   //! Sets the VolEn with bound checking control
   //! \param VolEnIn - Volume enable
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetVolEn(const bool VolEnIn, bool bndchk=true)
   {
      VolEn = VolEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the VolEn value.
   //! \return the current VolEn.
   bool GetVolEn(void) const { return(VolEn); }


   //+> Xoff

   //=========================================================
   //! Sets the Xoff with bound checking control
   //! \param XoffIn - Offset along the X axis of the referance 
   //!     entity from its origin.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetXoff(const float XoffIn, bool bndchk=true)
   {
      Xoff = XoffIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Xoff value.
   //! \return the current Xoff.
   float GetXoff(void) const { return(Xoff); }


   //+> Yoff

   //=========================================================
   //! Sets the Yoff with bound checking control
   //! \param YoffIn - Offset along the Y axis of the referance 
   //!     entity from its origin.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetYoff(const float YoffIn, bool bndchk=true)
   {
      Yoff = YoffIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Yoff value.
   //! \return the current Yoff.
   float GetYoff(void) const { return(Yoff); }


   //+> Zoff

   //=========================================================
   //! Sets the Zoff with bound checking control
   //! \param ZoffIn - Offset along the Z axis of the referance 
   //!     entity from its origin.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetZoff(const float ZoffIn, bool bndchk=true)
   {
      Zoff = ZoffIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Zoff value.
   //! \return the current Zoff.
   float GetZoff(void) const { return(Zoff); }


   //+> HeightOrRadius

   //=========================================================
   //! Sets the HeightOrRadius with bound checking control
   //! \param HeightOrRadiusIn - The height or radius of the volume.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetHeightOrRadius(const float HeightOrRadiusIn, bool bndchk=true)
   {
      HeightOrRadius = HeightOrRadiusIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the HeightOrRadius value.
   //! \return the current HeightOrRadius.
   float GetHeightOrRadius(void) const { return(HeightOrRadius); }


   //+> Width

   //=========================================================
   //! Sets the Width with bound checking control
   //! \param WidthIn - The width of the volume.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetWidth(const float WidthIn, bool bndchk=true)
   {
      Width = WidthIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Width value.
   //! \return the current Width.
   float GetWidth(void) const { return(Width); }


   //+> Depth

   //=========================================================
   //! Sets the Depth with bound checking control
   //! \param DepthIn - The depth of the volume.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDepth(const float DepthIn, bool bndchk=true)
   {
      Depth = DepthIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Depth value.
   //! \return the current Depth.
   float GetDepth(void) const { return(Depth); }



protected:

   //==> Member variables

   //=========================================================
   //! EntityID<br>
   //! Entity ID
   //!
   Cigi_uint16 EntityID;

   //=========================================================
   //! VolID<br>
   //! Volume ID
   //!
   Cigi_uint8 VolID;

   //=========================================================
   //! VolEn<br>
   //! Volume enable
   //!
   bool VolEn;

   //=========================================================
   //! VolType<br>
   //! Volume type.<br>
   //!  0-Sphere<br>
   //!  1-Cuboid
   //!
   VolTypeGrp VolType;

   //=========================================================
   //! Xoff<br>
   //! Offset along the X axis of the referance entity from its origin.
   //!
   float Xoff;

   //=========================================================
   //! Yoff<br>
   //! Offset along the Y axis of the referance entity from its origin.
   //!
   float Yoff;

   //=========================================================
   //! Zoff<br>
   //! Offset along the Z axis of the referance entity from its origin.
   //!
   float Zoff;

   //=========================================================
   //! HeightOrRadius<br>
   //! The volume's height or radius
   //!
   float HeightOrRadius;

   //=========================================================
   //! Width<br>
   //! The volume's width
   //!
   float Width;

   //=========================================================
   //! Depth<br>
   //! The volume's depth
   //!
   float Depth;

   //=========================================================
   //! Roll<br>
   //! The volume's roll relative to the referance entity.
   //!
   float Roll;

   //=========================================================
   //! Pitch<br>
   //! The volume's pitch relative to the referance entity.
   //!
   float Pitch;

   //=========================================================
   //! Yaw<br>
   //! The volume's yaw relative to the referance entity.
   //!
   float Yaw;


};

#endif // #if !defined(_CIGI_BASE_COLL_DET_VOL_DEF_INCLUDED_)
