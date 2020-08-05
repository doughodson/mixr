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
 *  FILENAME:   CigiAnimationTable.cpp
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
 *  Defined _EXPORT_CCL_ for exporting the class in a Windows DLL.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Changed native C++ types to use CCL-defined types instead where appropriate.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 * </pre>
 *  Author: The Boeing Company
 *
 */


#define _EXPORT_CCL_

#include "CigiAnimationTable.h"
#include "CigiErrorCodes.h"

#include <string.h>


// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiAnimationTable
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiAnimationTable::CigiAnimationTable()
{

   memset(AnimationTable,0,16384);

}

// ================================================
// ~CigiAnimationTable
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiAnimationTable::~CigiAnimationTable()
{

}

// ====================================================================
// Accessors
// ====================================================================

// ================================================
// GetPastAnimation
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
Cigi_uint32 CigiAnimationTable::GetPastAnimation(const Cigi_uint16 EntityID)
{

   int ndx;
   int shift;

   shift = EntityID & 0x3;

   ndx = EntityID >> 2;

   return((Cigi_uint32)(AnimationTable[ndx] >> shift) & 0x03);

}



// ================================================
// SetPastAnimation
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiAnimationTable::SetPastAnimation(const Cigi_uint16 EntityID, const Cigi_uint32 State)
{

   int ndx;
   int shift;
   Cigi_uint8 mask;

   shift = EntityID & 0x3;

   ndx = EntityID >> 2;

   mask = 0x03 << shift;
   AnimationTable[ndx] = (AnimationTable[ndx] & (~mask)) |
                         ((Cigi_uint8)(State & 0x03) << shift);

   return(CIGI_SUCCESS);

}
