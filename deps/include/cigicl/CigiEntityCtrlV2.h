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
 *  FILENAME:   CigiEntityCtrlV2.h
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
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  04/24/2007 Greg Basler                       Version 1.7.5
 *  Changed Input altitude to a double as required in the CIGI
 *  version 2.1 ICD.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Moved Packet information to base packet.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#if !defined(_CIGI_ENTITY_CONTROL_V2_INCLUDED_)
#define _CIGI_ENTITY_CONTROL_V2_INCLUDED_



#include "CigiBaseEntityCtrl.h"





//=========================================================
//! The class for the Entity Control packet for CIGI version 2
//!
class CIGI_SPEC CigiEntityCtrlV2 : public CigiBaseEntityCtrl  
{

public:

   //=========================================================
   //! The enumeration for the Entity State flag for CIGI 2
   //!
   enum EntityStV2Grp
   {
      LoadHideV2=0,
      LoadShowV2=1,
      UnloadV2=2
   };

   //=========================================================
   //! The enumeration for the Effect State flag for CIGI 2
   //!
   enum EffectStV2Grp
   {
      StopV2=0,
      PlayV2=1,
      RestartV2=2,
   };


   //==> Management

   //=========================================================
   //! General Constructor
   //!
	CigiEntityCtrlV2();

   //=========================================================
   //! General Destructor
   //!
	virtual ~CigiEntityCtrlV2();



   //==> Buffer Packing/Unpacking

   //=========================================================
   //! The virtual Pack function for CIGI 1
   //! \param Base - A pointer to the instance of the packet 
   //!          to be packed. (Downcast to CigiBasePacket)
   //! \param Buff - A pointer to the current pack point.
   //! \param Spec - A pointer to special data -
   //!          This is not used in this class.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	virtual int Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const;

   //=========================================================
   //! The virtual Unpack function for CIGI 1
   //! \param Buff - A pointer to the current pack point.
   //! \param Swap - N/A for V1 & V2
   //! \param Spec - A pointer to special data -
   //!          This is not used in this class.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	virtual int Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec);


   //==> Accessing Member Variable Values functions


   //+> Internal Temperature

   //=========================================================
   //! Sets the Opacity with bound checking control
   //! \param OpacityIn - The percent opacity of the entity<br>
   //!   0.0 - transparent
   //!   100.0 - fully visible
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   int SetOpacity(const float OpacityIn, bool bndchk=true);

   //=========================================================
   //! Gets the Opacity value.
   //! \return The percent opacity of the entity<br>
   //!   0.0 - transparent
   //!   100.0 - fully visible
   //!
   float GetOpacity(void) const { return(Opacity); }

   //=========================================================
   //! Sets the Temperature with bound checking control
   //! \param Temp - The internal temperature of the entity 
   //!   in degrees C.
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   int SetTemp(const float Temp, bool bndchk=true)
   {
      Temperature = Temp;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Temperature value
   //! \return The internal temperature of the entity 
   //!   in degrees C.
   //!
   float GetTemp(void) const { return(Temperature); }


   //+> Heading

   //=========================================================
   //! Sets the heading with bound checking control
   //! \param Heading - The heading of the entity in degrees.<br>
   //!   (0 - 360.0)
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetHeading(const float Heading, bool bndchk=true);

   //=========================================================
   //! Gets the heading value
   //! \return The heading of the entity in degrees.<br>
   //!   (0 to 360.0)
   //!
   float GetHeading(void) const { return(Yaw); }


   //+> Altitude

   //=========================================================
   //! Sets the altitude with bound checking control
   //! \param Alt - The altitude above MSL of the entity in meters.
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetAlt(const double Alt, bool bndchk=true)
   {
      AltOrZoff = Alt;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the altitude value
   //! \return The altitude above MSL of the entity in meters.
   //!
   double GetAlt(void) const { return(AltOrZoff); }


   //+> Z Offset

   //=========================================================
   //! Sets the Z Offset with bound checking control
   //! \param Zoff - The Z offset from the origin of the parent 
   //!   entity in meters.
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetZoff(const double Zoff, bool bndchk=true)
   {
      AltOrZoff = Zoff;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Z Offset value
   //! \return The Z offset from the origin of the parent 
   //!   entity in meters.
   //!
	double GetZoff(void) const { return(AltOrZoff); }


   //+> Entity State

   //=========================================================
   //! Sets the Entity State with bound checking control
   //! \param EntityStateIn - The current state of this entity.<br>
   //!   LoadHide=0, <br>
   //!   LoadShow=1, <br>
   //!   Unload=2
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetEntityState(const EntityStV2Grp EntityStateIn, bool bndchk=true);

   //=========================================================
   //! Gets the Entity State value
   //! \return The current state of this entity.<br>
   //!   LoadHide=0, <br>
   //!   LoadShow=1, <br>
   //!   Unload=2
   //!
   EntityStV2Grp GetEntityState(void) const { return((EntityStV2Grp)EntityState); }


   //+> Effect State

   //=========================================================
   //! Sets the Effect State with bound checking control
   //! \param EffectStateIn - The current state of this entity's 
   //!   special effect.<br>
   //!   Stop=0, <br>
   //!   Play=1, <br>
   //!   Restart=2,
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetEffectState(const EffectStV2Grp EffectStateIn, bool bndchk=true);

   //=========================================================
   //! Gets the Effect State value
   //! \return The current state of this entity's 
   //!   special effect.<br>
   //!   Stop=0, <br>
   //!   Play=1, <br>
   //!   Restart=2,
   //!
	EffectStV2Grp GetEffectState(void) const;


protected:

   static const EffectStV2Grp V3toV2[4];
   static const AnimationStateGrp V2toV3[3];

};

#endif // #if !defined(_CIGI_ENTITY_CONTROL_V2_INCLUDED_)
