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
 *  FILENAME:   CigiTypes_MSVS_Win32.h
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


#if !defined(CIGI_TYPES_MSVS_WIN32_H)
#define CIGI_TYPES_MSVS_WIN32_H


typedef unsigned __int8 Cigi_uint8;
typedef __int8 Cigi_int8;
typedef char Cigi_Ascii;

typedef unsigned __int16 Cigi_uint16;
typedef __int16 Cigi_int16;

typedef unsigned __int32 Cigi_uint32;
typedef __int32 Cigi_int32;

typedef unsigned __int64 Cigi_uint64;
typedef __int64 Cigi_int64;


#define _I64_CONST_(CONST_ARG)  CONST_ARG ## i64



#ifdef CCL_DLL

   #ifdef _EXPORT_CCL_
      #define CIGI_SPEC __declspec(dllexport)
   #else
      #define CIGI_SPEC __declspec(dllimport)
   #endif

#else

   #define CIGI_SPEC

#endif


#endif // #if !defined(CIGI_TYPES_MSVC_WIN32_H)





