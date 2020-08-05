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
 *  FILENAME:   CigiSwapping.cpp
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
 *  Removed the inline defnitions for the PackedPointer union and included the 
 *  CigiTypes.h header.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Added modifiers to export the CigiSwap2, CigiSwap4, and CigiSwap8 functions 
 *  when built as a Windows DLL.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Changed native C++ types to use CCL-defined types instead where appropriate.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Changed CigiCopy8 to make sure that it works for big endian and non-intel
 *  machines.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiTypes.h"

union _SWAP_UNION
{
    Cigi_uint32 i;
    Cigi_uint8 b[4];
};
typedef union _SWAP_UNION SWAP_UNION;


// ====================================================================
// CigiSwap2
// ====================================================================
CIGI_SPEC void CigiSwap2( void *dest, const void *src)
{
    static Cigi_uint16 d, s;
    
    s = *( ( Cigi_uint16 *)src);
    
    d = s << 8;
    d |= s >> 8;
    
    *( ( Cigi_uint16 *)dest) = d;
}

// ====================================================================
// CigiSwap4
// ====================================================================
CIGI_SPEC void CigiSwap4( void *dest, const void *src)
{
    static SWAP_UNION d, s;
    
    s.i = *( ( Cigi_uint32 *)src);
    
    d.b[0] = s.b[3];
    d.b[1] = s.b[2];
    d.b[2] = s.b[1];
    d.b[3] = s.b[0];
    
    *( ( Cigi_uint32 *)dest) = d.i;
}

// ====================================================================
// CigiSwap8
// ====================================================================
CIGI_SPEC void CigiSwap8( void *dest, const void *src)
{
    static SWAP_UNION d1, s1;
    static SWAP_UNION d2, s2;
    
    /* Do each word separately.  Each pair of variables should be enclosed within 
     * braces to minimize the number of variables declared as "register."
     */    
    
    s1.i = *( ( Cigi_uint32 *)src);
    s2.i = *( ( Cigi_uint32 *)src + 1);
    
    d1.b[0] = s1.b[3];
    d1.b[1] = s1.b[2];
    d1.b[2] = s1.b[1];
    d1.b[3] = s1.b[0];     
    
    d2.b[0] = s2.b[3];
    d2.b[1] = s2.b[2];
    d2.b[2] = s2.b[1];
    d2.b[3] = s2.b[0];
    
    *( ( Cigi_uint32 *)dest) = d2.i;
    *( ( Cigi_uint32 *)dest + 1) = d1.i;
}

// ====================================================================
// CigiSwap8
// ====================================================================
CIGI_SPEC void CigiCopy8( void *dest, const void *src)
{
   Cigi_uint32 *tDest = (Cigi_uint32 *)dest;
   Cigi_uint32 *tSrc = (Cigi_uint32 *)src;

   *tDest++ = *tSrc++;
   *tDest = *tSrc;
}

