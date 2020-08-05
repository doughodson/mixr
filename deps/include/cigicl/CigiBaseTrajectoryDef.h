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
 *  FILENAME:   CigiBaseTrajectoryDef.h
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


#if !defined(_CIGI_BASE_TRAJECTORY_DEF_INCLUDED_)
#define _CIGI_BASE_TRAJECTORY_DEF_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_TRAJECTORY_DEF_PACKET_ID_V1 21
#define CIGI_TRAJECTORY_DEF_PACKET_SIZE_V1 28

#define CIGI_TRAJECTORY_DEF_PACKET_ID_V2 21
#define CIGI_TRAJECTORY_DEF_PACKET_SIZE_V2 16

#define CIGI_TRAJECTORY_DEF_PACKET_ID_V3 20
#define CIGI_TRAJECTORY_DEF_PACKET_SIZE_V3 24


class CigiTrajectoryDefV1;
class CigiTrajectoryDefV2;
class CigiTrajectoryDefV3;


class CIGI_SPEC CigiBaseTrajectoryDef : public CigiBasePacket
{

friend class CigiTrajectoryDefV1;
friend class CigiTrajectoryDefV2;
friend class CigiTrajectoryDefV3;

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseTrajectoryDef();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseTrajectoryDef();


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
   //! \param Spec - A pointer to special data -
   //!          This is not used in this class.
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
   //! \param EntityIDIn - The Entity ID.
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
   //! Gets the EntityIDIn value.
   //! \return the current EntityIDIn.
   Cigi_uint16 GetEntityID(void) const { return(EntityID); }


   //+> RetardationRate

   //=========================================================
   //! Sets the RetardationRate with bound checking control
   //! \param RetardationRateIn - Deceleration usually due to drag.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRetardationRate(const float RetardationRateIn, bool bndchk=true)
   {
      RetardationRate = RetardationRateIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the RetardationRate value.
   //! \return the current RetardationRate.
   float GetRetardationRate(void) const { return(RetardationRate); }


   //+> TermVel

   //=========================================================
   //! Sets the TermVel with bound checking control
   //! \param TermVelIn - The terminal velocity.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetTermVel(const float TermVelIn, bool bndchk=true)
   {
      TermVel = TermVelIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the TermVel value.
   //! \return the current TermVel.
   float GetTermVel(void) const { return(TermVel); }



protected:

   //==> Member variables

   //=========================================================
   //! EntityID<br>
   //! Uniquely identifies the entity to which this packet 
   //!   is applied.
   //!
   Cigi_uint16 EntityID;

   //=========================================================
   //! Accel<br>
   //! Acceleration (usually due to gravity)
   //!
   float Accel;

   //=========================================================
   //! RetardationRate<br>
   //! Deceleration (usually due to drag)
   //!
   float RetardationRate;

   //=========================================================
   //! TermVel<br>
   //! Terminal Velocity
   //!
   float TermVel;

   //=========================================================
   //! Xoff<br>
   //! The offset along the entity's X axis from the entity's reference
   //!   point from which the trajectory is started.
   //!
   float Xoff;

   //=========================================================
   //! Yoff<br>
   //! The offset along the entity's Y axis from the entity's reference
   //!   point from which the trajectory is started.
   //!
   float Yoff;

   //=========================================================
   //! Zoff<br>
   //! The offset along the entity's Z axis from the entity's reference
   //!   point from which the trajectory is started.
   //!
   float Zoff;

   //=========================================================
   //! AccelX<br>
   //! The acceleration along the X (North) axis
   //!
   float AccelX;

   //=========================================================
   //! AccelY<br>
   //! The acceleration along the Y (East) axis
   //!
   float AccelY;

   //=========================================================
   //! AccelZ<br>
   //! The acceleration along the Z (Down) axis
   //!
   float AccelZ;



};

#endif // #if !defined(_CIGI_BASE_TRAJECTORY_DEF_INCLUDED_)
