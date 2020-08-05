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
 *  FILENAME:   CigiBaseCollDetSegDef.h
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


#if !defined(_CIGI_BASE_COLL_DET_SEG_DEF_INCLUDED_)
#define _CIGI_BASE_COLL_DET_SEG_DEF_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_COLL_DET_SEG_DEF_PACKET_ID_V1 24
#define CIGI_COLL_DET_SEG_DEF_PACKET_SIZE_V1 24

#define CIGI_COLL_DET_SEG_DEF_PACKET_ID_V2 24
#define CIGI_COLL_DET_SEG_DEF_PACKET_SIZE_V2 24

#define CIGI_COLL_DET_SEG_DEF_PACKET_ID_V3 22
#define CIGI_COLL_DET_SEG_DEF_PACKET_SIZE_V3 40


class CigiCollDetSegDefV1;
class CigiCollDetSegDefV2;
class CigiCollDetSegDefV3;


class CIGI_SPEC CigiBaseCollDetSegDef : public CigiBasePacket
{

friend class CigiCollDetSegDefV1;
friend class CigiCollDetSegDefV2;
friend class CigiCollDetSegDefV3;

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseCollDetSegDef() { };

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseCollDetSegDef() { };


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

      // Note: V1 & V2 are the same
      if(CnvtVersion.CigiMajorVersion < 3)
         CnvtInfo.CnvtPacketID = CIGI_COLL_DET_SEG_DEF_PACKET_ID_V2;
      else
         CnvtInfo.CnvtPacketID = CIGI_COLL_DET_SEG_DEF_PACKET_ID_V3;

      return(CIGI_SUCCESS);
   }



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


   //+> SegmentEn

   //=========================================================
   //! Sets the SegmentEn with bound checking control
   //! \param SegmentEnIn - Segment ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSegmentEn(const bool SegmentEnIn, bool bndchk=true)
   {
      SegmentEn = SegmentEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the SegmentEn value.
   //! \return the current SegmentEn.
   bool GetSegmentEn(void) const { return(SegmentEn); }


   //+> X1

   //=========================================================
   //! Sets the X1 with bound checking control
   //! \param X1In - Segment starting position along the X axis.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetX1(const float X1In, bool bndchk=true)
   {
      X1 = X1In;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the X1 value.
   //! \return the current X1.
   float GetX1(void) const { return(X1); }


   //+> Y1

   //=========================================================
   //! Sets the Y1 with bound checking control
   //! \param Y1In - Segment starting position along the Y axis.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetY1(const float Y1In, bool bndchk=true)
   {
      Y1 = Y1In;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Y1 value.
   //! \return the current Y1.
   float GetY1(void) const { return(Y1); }


   //+> Z1

   //=========================================================
   //! Sets the Z1 with bound checking control
   //! \param Z1In - Segment starting position along the Z axis.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetZ1(const float Z1In, bool bndchk=true)
   {
      Z1 = Z1In;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Z1 value.
   //! \return the current Z1.
   float GetZ1(void) const { return(Z1); }


   //+> X2

   //=========================================================
   //! Sets the X2 with bound checking control
   //! \param X2In - Segment ending position along the X axis.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetX2(const float X2In, bool bndchk=true)
   {
      X2 = X2In;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the X2 value.
   //! \return the current X2.
   float GetX2(void) const { return(X2); }


   //+> Y2

   //=========================================================
   //! Sets the Y2 with bound checking control
   //! \param Y2In - Segment ending position along the Y axis.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetY2(const float Y2In, bool bndchk=true)
   {
      Y2 = Y2In;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Y2 value.
   //! \return the current Y2.
   float GetY2(void) const { return(Y2); }


   //+> Z2

   //=========================================================
   //! Sets the Z2 with bound checking control
   //! \param Z2In - Segment ending position along the Z axis.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetZ2(const float Z2In, bool bndchk=true)
   {
      Z2 = Z2In;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Z2 value.
   //! \return the current Z2.
   float GetZ2(void) const { return(Z2); }


   //+> Mask

   //=========================================================
   //! Sets the Mask with bound checking control
   //! \param MaskIn - Mask specifying which materials will result
   //!    in a report if collided with.
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



protected:

   //==> Member variables

   //=========================================================
   //! EntityID<br>
   //! Entity ID
   //!
   Cigi_uint16 EntityID;

   //=========================================================
   //! SegmentID<br>
   //! Segment ID
   //!
   Cigi_uint8 SegmentID;

   //=========================================================
   //! SegmentEn<br>
   //! Segment Enable
   //!
   bool SegmentEn;

   //=========================================================
   //! X1<br>
   //! Segment Starting position along the X axis offset from
   //!   the entity's origin.
   //!
   float X1;

   //=========================================================
   //! Y1<br>
   //! Segment starting position along the Y axis offset from
   //!   the entity's origin.
   //!
   float Y1;

   //=========================================================
   //! Z1<br>
   //! Segment starting position along the Z axis offset from
   //!   the entity's origin.
   //!
   float Z1;

   //=========================================================
   //! X2<br>
   //! Segment ending position along the X axis offset from
   //!   the entity's origin.
   //!
   float X2;

   //=========================================================
   //! Y2<br>
   //! Segment ending position along the Y axis offset from
   //!   the entity's origin.
   //!
   float Y2;

   //=========================================================
   //! Z2<br>
   //! Segment ending position along the Z axis offset from
   //!   the entity's origin.
   //!
   float Z2;

   //=========================================================
   //! Mask<br>
   //! A mask specifying collisions with which materials
   //!   will result in a response.
   //!
   Cigi_uint32 Mask;


};

#endif // #if !defined(_CIGI_BASE_COLL_DET_SEG_DEF_INCLUDED_)
