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
 *  FILENAME:   CigiTypes.h
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
 *  Added the CIGI_SPEC preprocessor definition for building the project as a
 *  Windows DLL.
 *
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Added the PackPointer union type declaration.
 *
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Defined Cigi_uint16, Cigi_uint32, and Cigi_uint64 for various processors.
 *
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and
 *  Cigi_uint8.
 *
 *  07/15/2008 Greg Basler                       Version 2.2.1
 *  Removed the CCL library version from all files and created
 *  defines in this file for the current CCL library version.
 *
 *  09/09/2008 Greg Basler                       Version 2.2.1
 *  Initial Release.
 *  Split types file into specific architecture types files.,
 *
 * </pre>
 *  Author: The Boeing Company
 */


#if !defined(CIGI_TYPES_H)
#define CIGI_TYPES_H


#define CCL_MAJOR_VERSION 3
#define CCL_MINOR_VERSION 3
#define CCL_PATCH_LEVEL   0


// Due to type problems between compilers

#if defined(WIN32) && !(defined(__CYGWIN__) || defined(__MINGW32__))
   #include "CigiTypes_MSVS_Win32.h"
#else
   #include "CigiTypes_Default.h"
#endif



typedef union PackPointerUnion
{
   Cigi_uint8 *c;
   Cigi_int8 *b;
   Cigi_uint16 *s;
   Cigi_int16 *h;
   Cigi_uint32 *l;
   float *f;
   double *d;
} PackPointer;


#endif // #if !defined(CIGI_TYPES_H)





