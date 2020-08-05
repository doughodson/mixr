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
 *  FILENAME:   CigiBaseCollDetVolResp.h
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


#if !defined(_CIGI_BASE_COLL_DET_VOL_RESP_INCLUDED_)
#define _CIGI_BASE_COLL_DET_VOL_RESP_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_COLL_DET_VOL_RESP_PACKET_ID_V2 107
#define CIGI_COLL_DET_VOL_RESP_PACKET_SIZE_V2 8

#define CIGI_COLL_DET_VOL_RESP_PACKET_ID_V3 114
#define CIGI_COLL_DET_VOL_RESP_PACKET_SIZE_V3 16


class CigiCollDetVolRespV2;
class CigiCollDetVolRespV3;


class CIGI_SPEC CigiBaseCollDetVolResp : public CigiBasePacket
{

friend class CigiCollDetVolRespV2;
friend class CigiCollDetVolRespV3;

public:

   //=========================================================
   //! The enumeration for the CigiBaseCollDetVolResp Group
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
   CigiBaseCollDetVolResp();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseCollDetVolResp();


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


   //+> CollType

   //=========================================================
   //! Sets the CollType with bound checking control
   //! \param CollTypeIn - Collision type<br>
   //!   NonEntity=0<br>
   //!   Entity=1<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCollType(const CollTypeGrp CollTypeIn, bool bndchk=true);

   //=========================================================
   //! Gets the CollType value.
   //! \return the current CollType.
   CollTypeGrp GetCollType(void) const { return(CollType); }


   //+> CollEntityID

   //=========================================================
   //! Sets the CollEntityID with bound checking control
   //! \param CollEntityIDIn - Entity Collided with
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCollEntityID(const Cigi_uint16 CollEntityIDIn, bool bndchk=true)
   {
      CollEntityID = CollEntityIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the CollEntityID value.
   //! \return the current CollEntityID.
   Cigi_uint16 GetCollEntityID(void) const { return(CollEntityID); }



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
   //! CollType<br>
   //! Collision Type<br>
   //!  0-NonEntity<br>
   //!  1-Entity
   //!
   //!
   CollTypeGrp CollType;

   //=========================================================
   //! CollEntityID<br>
   //! Entity with which the referance entity collided
   //!
   Cigi_uint16 CollEntityID;

   //=========================================================
   //! CollVolID<br>
   //! Volume ID with which the referance volume collided
   //!
   Cigi_uint8 CollVolID;


};

#endif // #if !defined(_CIGI_BASE_COLL_DET_VOL_RESP_INCLUDED_)
