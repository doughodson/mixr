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
 *  FILENAME:   CigiTypes_Default.h
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
 *  09/09/2008 Greg Basler                       Version 2.2.1
 *  Initial Release.
 *  Split types file into specific architecture types files.,
 *
 * </pre>
 *  Author: The Boeing Company
 */


#if !defined(CIGI_TYPES_DEFAULT_H)
#define CIGI_TYPES_DEFAULT_H

#include <stdint.h>


typedef uint8_t Cigi_uint8;
typedef int8_t Cigi_int8;
typedef char Cigi_Ascii;

typedef uint16_t Cigi_uint16;
typedef int16_t Cigi_int16;

typedef uint32_t Cigi_uint32;
typedef int32_t Cigi_int32;

typedef uint64_t Cigi_uint64;
typedef int64_t Cigi_int64;

#define _I64_CONST_(CONST_ARG)  CONST_ARG ## LL

#define CIGI_SPEC



#endif // #if !defined(CIGI_TYPES_DEFAULT_H)

