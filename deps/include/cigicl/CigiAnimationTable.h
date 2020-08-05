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
 *  FILENAME:   CigiAnimationTable.h
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
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if !defined(_CIGI_ANIMATION_TABLE_INCLUDED_)
#define _CIGI_ANIMATION_TABLE_INCLUDED_

#include "CigiTypes.h"

//=========================================================
//! The class to hold and work the Animation table<br>
//! Because the Conversions between the Entity Control
//!   animation state Versions 1 and 3, we needed to create
//!   a table to determine the corrent state for each frame.
//!
class CIGI_SPEC CigiAnimationTable  
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
	CigiAnimationTable();

   //=========================================================
   //! General Destructor
   //!
	virtual ~CigiAnimationTable();



   //==> Accessing The Correct Table Values functions

   //=========================================================
   //! Gets the last frame's animation state for this Entity ID.
   //! \param EntityID - Uniquely identifies the specific entity
   //!   0 - The ownship
   //!   All other values - An Application defined entity
   //!
   //! \return This returns the last frame's animation state for this Entity ID.
   //!
   Cigi_uint32 GetPastAnimation(const Cigi_uint16 EntityID);

   //=========================================================
   //! Sets the last frame's animation state for this Entity ID.
   //! \param EntityID - Uniquely identifies the specific entity
   //!   0 - The ownship
   //!   All other values - An Application defined entity
   //! \param State - This frame's animation state for this Entity ID
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetPastAnimation(const Cigi_uint16 EntityID, const Cigi_uint32 State);

protected:

   //==> Member variables

   //=========================================================
   //! AnimationTable[16384]<br>
   //! A table to store the last frame's animation state for
   //!   each valid Entity ID<br>
   //! Each 2 bits holds an Entity's past animation state.
   //! 0 Stop<br>
   //! 1 Pause<br>
   //! 2 Play<br>
   //! 3 Continue<br>
   //!
	Cigi_uint8 AnimationTable[16384];

};

#endif // !defined(_CIGI_ANIMATION_TABLE_INCLUDED_)
