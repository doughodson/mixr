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
 *  FILENAME:   CigiBasePositionReq.h
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


#if !defined(_CIGI_BASE_POSITION_REQ_INCLUDED_)
#define _CIGI_BASE_POSITION_REQ_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_POSITION_REQ_PACKET_ID_V3 27
#define CIGI_POSITION_REQ_PACKET_SIZE_V3 8


class CigiPositionReqV1;
class CigiPositionReqV2;
class CigiPositionReqV3;


class CIGI_SPEC CigiBasePositionReq : public CigiBasePacket
{

friend class CigiPositionReqV1;
friend class CigiPositionReqV2;
friend class CigiPositionReqV3;

public:

   //=========================================================
   //! The enumeration for the CigiBasePositionReq Group
   //!
   enum UpdateModeGrp
   {
      OneShot=0,
      Continuous=1
   };

   //=========================================================
   //! The enumeration for the CigiBasePositionReq Group
   //!
   enum ObjectClassGrp
   {
      Entity=0,
      ArtPart=1,
      View=2,
      ViewGrp=3,
      MotionTracker=4
   };

   //=========================================================
   //! The enumeration for the CigiBasePositionReq Group
   //!
   enum CoordSysGrp
   {
      Geodetic=0,
      ParentEntity=1,
      Submodel=2
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBasePositionReq();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBasePositionReq();


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

   //+> ObjectID

   //=========================================================
   //! Sets the ObjectID with bound checking control
   //! \param ObjectIDIn - Object ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetObjectID(const Cigi_uint16 ObjectIDIn, bool bndchk=true)
   {
      ObjectID = ObjectIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ObjectID value.
   //! \return the current ObjectID.
   Cigi_uint16 GetObjectID(void) const { return(ObjectID); }


   //+> ArtPartID

   //=========================================================
   //! Sets the ArtPartID with bound checking control
   //! \param ArtPartIDIn - Articulated Part ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetArtPartID(const Cigi_uint8 ArtPartIDIn, bool bndchk=true)
   {
      ArtPartID = ArtPartIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ArtPartID value.
   //! \return the current ArtPartID.
   Cigi_uint8 GetArtPartID(void) const { return(ArtPartID); }


   //+> UpdateMode

   //=========================================================
   //! Sets the UpdateMode with bound checking control
   //! \param UpdateModeIn - Update mode<br>
   //!   OneShot=0<br>
   //!   Continuous=1<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetUpdateMode(const UpdateModeGrp UpdateModeIn, bool bndchk=true);

   //=========================================================
   //! Gets the UpdateMode value.
   //! \return the current UpdateMode.
   UpdateModeGrp GetUpdateMode(void) const { return(UpdateMode); }


   //+> ObjectClass

   //=========================================================
   //! Sets the ObjectClass with bound checking control
   //! \param ObjectClassIn - Type of Object<br>
   //!   Entity=0<br>
   //!   ArtPart=1<br>
   //!   View=3<br>
   //!   ViewGrp=4<br>
   //!   MotionTracker=5<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetObjectClass(const ObjectClassGrp ObjectClassIn, bool bndchk=true);

   //=========================================================
   //! Gets the ObjectClass value.
   //! \return the current ObjectClass.
   ObjectClassGrp GetObjectClass(void) const { return(ObjectClass); }


   //+> CoordSys

   //=========================================================
   //! Sets the CoordSys with bound checking control
   //! \param CoordSysIn - Coordinate system used<br>
   //!   Geodetic=0<br>
   //!   ParentEntity=1<br>
   //!   Submodel=2<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCoordSys(const CoordSysGrp CoordSysIn, bool bndchk=true);

   //=========================================================
   //! Gets the CoordSys value.
   //! \return the current CoordSys.
   CoordSysGrp GetCoordSys(void) const { return(CoordSys); }



protected:

   //==> Member variables

   //=========================================================
   //! ObjectID<br>
   //! Object ID
   //!
   Cigi_uint16 ObjectID;

   //=========================================================
   //! ArtPartID<br>
   //! Articulated Part ID
   //!
   Cigi_uint8 ArtPartID;

   //=========================================================
   //! UpdateMode<br>
   //! Update mode<br>
   //!  0-OneShot<br>
   //!  1-Continuous
   //!
   //!
   UpdateModeGrp UpdateMode;

   //=========================================================
   //! ObjectClass<br>
   //! Type of object<br>
   //!  0-Entity<br>
   //!  1-ArtPart<br>
   //!  3-View<br>
   //!  4-ViewGrp<br>
   //!  5-MotionTracker
   //!
   //!
   ObjectClassGrp ObjectClass;

   //=========================================================
   //! CoordSys<br>
   //! Coordinate System used.
   //!  0-Geodetic<br>
   //!  1-ParentEntity<br>
   //!  2-Submodel
   //!
   //!
   CoordSysGrp CoordSys;


};

#endif // #if !defined(_CIGI_BASE_POSITION_REQ_INCLUDED_)
