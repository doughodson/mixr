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
 *  FILENAME:   CigiBaseCollDetSegResp.h
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


#if !defined(_CIGI_BASE_COLL_DET_SEG_RESP_INCLUDED_)
#define _CIGI_BASE_COLL_DET_SEG_RESP_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_COLL_DET_SEG_RESP_PACKET_ID_V1 104
#define CIGI_COLL_DET_SEG_RESP_PACKET_SIZE_V1 24

#define CIGI_COLL_DET_SEG_RESP_PACKET_ID_V2 104
#define CIGI_COLL_DET_SEG_RESP_PACKET_SIZE_V2 24

#define CIGI_COLL_DET_SEG_RESP_PACKET_ID_V3 113
#define CIGI_COLL_DET_SEG_RESP_PACKET_SIZE_V3 16


class CigiCollDetSegRespV1;
class CigiCollDetSegRespV2;
class CigiCollDetSegRespV3;


class CIGI_SPEC CigiBaseCollDetSegResp : public CigiBasePacket
{

friend class CigiCollDetSegRespV1;
friend class CigiCollDetSegRespV2;
friend class CigiCollDetSegRespV3;

public:

   //=========================================================
   //! The enumeration for the CigiBaseCollDetSegResp Group
   //!
   enum CollTypeGrp
   {
      NonEntity=0,
      Entity=1
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseCollDetSegResp();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseCollDetSegResp();


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


   //+> Material

   //=========================================================
   //! Sets the Material with bound checking control
   //! \param MaterialIn - Material of the polygon collided with
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


   //+> IsValidV1or2

   //=========================================================
   //! Sets the IsValidV1or2 with bound checking control
   //! \param IsValidV1or2In - The data is valid with CIGI 1 and 2
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetIsValidV1or2(const bool IsValidV1or2In, bool bndchk=true)
   {
      IsValidV1or2 = IsValidV1or2In;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the IsValidV1or2 value.
   //! \return the current IsValidV1or2.
   bool GetIsValidV1or2(void) const { return(IsValidV1or2); }



protected:

   //==> Member variables

   //=========================================================
   //! EntityID<br>
   //! Entity ID
   //!
   Cigi_uint16 EntityID;

   //=========================================================
   //! SegID<br>
   //! Segment ID
   //!
   Cigi_uint8 SegID;

   //=========================================================
   //! CollType<br>
   //! Collision with a non-entity or an entity<br>
   //!  0-NonEntity<br>
   //!  1-Entity
   //!
   //!
   CollTypeGrp CollType;

   //=========================================================
   //! CollEntityID<br>
   //! The Entity ID of the entity collided with
   //!
   Cigi_uint16 CollEntityID;

   //=========================================================
   //! Material<br>
   //! The material code of the polygon collided with
   //!
   Cigi_uint32 Material;

   //=========================================================
   //! IntersectDist<br>
   //! The distance along the segment from the starting or
   //!   source point where the collision occured.
   //!
   float IntersectDist;

   //=========================================================
   //! X<br>
   //! The position of the collision along the X axis of the
   //!   defining entity's coordinate system.
   //!
   float X;

   //=========================================================
   //! Y<br>
   //! The position of the collision along the Y axis of the
   //!   defining entity's coordinate system.
   //!
   float Y;

   //=========================================================
   //! Z<br>
   //! The position of the collision along the Z axis of the
   //!   defining entity's coordinate system.
   //!
   float Z;

   //=========================================================
   //! IsValidV1or2<br>
   //! The data is Valid CIGI 1 or CIGI 2 data - true<br>
   //! OR the data is Valid CIGI 3 data - false.
   //!
   bool IsValidV1or2;


};

#endif // #if !defined(_CIGI_BASE_COLL_DET_SEG_RESP_INCLUDED_)
