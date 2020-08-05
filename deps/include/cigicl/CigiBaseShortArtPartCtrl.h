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
 *  FILENAME:   CigiBaseShortArtPartCtrl.h
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


#if !defined(_CIGI_BASE_SHORT_ART_PART_CTRL_INCLUDED_)
#define _CIGI_BASE_SHORT_ART_PART_CTRL_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_SHORT_ART_PART_CTRL_PACKET_ID_V3 7
#define CIGI_SHORT_ART_PART_CTRL_PACKET_SIZE_V3 16


class CigiShortArtPartCtrlV3;
class CigiArtPartCtrlV3;


class CIGI_SPEC CigiBaseShortArtPartCtrl : public CigiBasePacket
{

friend class CigiShortArtPartCtrlV3;

public:

   //=========================================================
   //! The enumeration for the CigiBaseShortArtPartCtrl Group
   //!
   enum DofSelectGrp
   {
      NotUsed=0,
      Xoff=1,
      Yoff=2,
      Zoff=3,
      Yaw=4,
      Pitch=5,
      Roll=6
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseShortArtPartCtrl();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseShortArtPartCtrl();


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

   //=========================================================
   //! A virtual Conversion Information function.
   //! This function provides conversion information for this
   //!  packet.
   //! \param CnvtVersion - The CIGI version to which this packet
   //!    is being converted.
   //! \param ArtPartID - The ID of the Articulated Part to
   //!    process.
   //! \param ArtPart - The Articulated Part Control Packet
   //!    object to fill.
   //!    
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SpecialConversion(CigiVersionID &CnvtVersion,
                         Cigi_uint8 ArtPartID,
                         CigiArtPartCtrlV3 *ArtPart);



   //==> Accessing Member Variable Values functions

   //+> EntityID

   //=========================================================
   //! Sets the EntityID with bound checking control
   //! \param EntityIDIn - Enitity ID
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


   //+> ArtPart1

   //=========================================================
   //! Sets the ArtPart1 with bound checking control
   //! \param ArtPart1In - Articulated Part 1 ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetArtPart1(const Cigi_uint8 ArtPart1In, bool bndchk=true)
   {
      ArtPart1 = ArtPart1In;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ArtPart1 value.
   //! \return the current ArtPart1.
   Cigi_uint8 GetArtPart1(void) const { return(ArtPart1); }


   //+> ArtPart2

   //=========================================================
   //! Sets the ArtPart2 with bound checking control
   //! \param ArtPart2In - Articulated Part 2 ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetArtPart2(const Cigi_uint8 ArtPart2In, bool bndchk=true)
   {
      ArtPart2 = ArtPart2In;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ArtPart2 value.
   //! \return the current ArtPart2.
   Cigi_uint8 GetArtPart2(void) const { return(ArtPart2); }


   //+> DofSelect1

   //=========================================================
   //! Sets the DofSelect1 with bound checking control
   //! \param DofSelect1In - Dof selection for articulated part 1<br>
   //!   NotUsed=0<br>
   //!   Xoff=1<br>
   //!   Yoff=2<br>
   //!   Zoff=3<br>
   //!   Yaw=4<br>
   //!   Pitch=5<br>
   //!   Roll=6<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDofSelect1(const DofSelectGrp DofSelect1In, bool bndchk=true);

   //=========================================================
   //! Gets the DofSelect1 value.
   //! \return the current DofSelect1.
   DofSelectGrp GetDofSelect1(void) const { return(DofSelect1); }


   //+> DofSelect2

   //=========================================================
   //! Sets the DofSelect2 with bound checking control
   //! \param DofSelect2In - Dof selection for articulated part 2<br>
   //!   NotUsed=0<br>
   //!   Xoff=1<br>
   //!   Yoff=2<br>
   //!   Zoff=3<br>
   //!   Yaw=4<br>
   //!   Pitch=5<br>
   //!   Roll=6<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDofSelect2(const DofSelectGrp DofSelect2In, bool bndchk=true);

   //=========================================================
   //! Gets the DofSelect2 value.
   //! \return the current DofSelect2.
   DofSelectGrp GetDofSelect2(void) const { return(DofSelect2); }


   //+> ArtPart1En

   //=========================================================
   //! Sets the ArtPart1En with bound checking control
   //! \param ArtPart1EnIn - Articulated part 1
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetArtPart1En(const bool ArtPart1EnIn, bool bndchk=true)
   {
      ArtPart1En = ArtPart1EnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ArtPart1En value.
   //! \return the current ArtPart1En.
   bool GetArtPart1En(void) const { return(ArtPart1En); }


   //+> ArtPart2En

   //=========================================================
   //! Sets the ArtPart2En with bound checking control
   //! \param ArtPart2EnIn - Articulated part 2
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetArtPart2En(const bool ArtPart2EnIn, bool bndchk=true)
   {
      ArtPart2En = ArtPart2EnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ArtPart2En value.
   //! \return the current ArtPart2En.
   bool GetArtPart2En(void) const { return(ArtPart2En); }


   //+> Dof1

   //=========================================================
   //! Sets the Dof1 with bound checking control
   //! \param Dof1In - Dof value for Articulated part 1
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDof1(const float Dof1In, bool bndchk=true)
   {
      Dof1 = Dof1In;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Dof1 value.
   //! \return the current Dof1.
   float GetDof1(void) const { return(Dof1); }


   //+> Dof2

   //=========================================================
   //! Sets the Dof2 with bound checking control
   //! \param Dof2In - Dof value for Articulated part 2
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDof2(const float Dof2In, bool bndchk=true)
   {
      Dof2 = Dof2In;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Dof2 value.
   //! \return the current Dof2.
   float GetDof2(void) const { return(Dof2); }



protected:

   //==> Member variables

   //=========================================================
   //! EntityID<br>
   //! Entity ID
   //!
   Cigi_uint16 EntityID;

   //=========================================================
   //! ArtPart1<br>
   //! Articulated part 1 ID
   //!
   Cigi_uint8 ArtPart1;

   //=========================================================
   //! ArtPart2<br>
   //! Articulated part 2 ID
   //!
   Cigi_uint8 ArtPart2;

   //=========================================================
   //! DofSelect1<br>
   //! Dof selection for articulated part 1<br>
   //!  0-NotUsed<br>
   //!  1-Xoff<br>
   //!  2-Yoff<br>
   //!  3-Zoff<br>
   //!  4-Yaw<br>
   //!  5-Pitch<br>
   //!  6-Roll
   //!
   //!
   DofSelectGrp DofSelect1;

   //=========================================================
   //! DofSelect2<br>
   //! Dof selection for articulated part 2<br>
   //!  0-NotUsed<br>
   //!  1-Xoff<br>
   //!  2-Yoff<br>
   //!  3-Zoff<br>
   //!  4-Yaw<br>
   //!  5-Pitch<br>
   //!  6-Roll
   //!
   //!
   DofSelectGrp DofSelect2;

   //=========================================================
   //! ArtPart1En<br>
   //! Articulated part 1 enable
   //!
   bool ArtPart1En;

   //=========================================================
   //! ArtPart2En<br>
   //! Articulated part 2 enable
   //!
   bool ArtPart2En;

   //=========================================================
   //! Dof1<br>
   //! Position value for the selected DOF of articulated part 1
   //!
   float Dof1;

   //=========================================================
   //! Dof2<br>
   //! Position value for the selected DOF of articulated part 2
   //!
   float Dof2;


};

#endif // #if !defined(_CIGI_BASE_SHORT_ART_PART_CTRL_INCLUDED_)
