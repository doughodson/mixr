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
 *  FILENAME:   CigiEntityCtrlV1.h
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
 *  Added function modifiers to export the CigiSwap2, CigiSwap4, and CigiSwap8 
 *  functions when built as a Windows DLL.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Changed the implementations of the CigiCopy2, CigiCopy4, CIGI_FLOAT_TO_B6, 
 *  and CIGI_FLOAT_TO_BA to use CIGI-defined types instead of native C++ types.
 *  
 *  04/06/2005 Greg Basler                       Version 1.5.2
 *  Included the CigiTypes.h header file. Modified the CigiCopy2 and CigiCopy4 
 *  macros to use the cigi-defined data types instead of native types.
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


#if !defined(_CIGI_SWAPPING_INCLUDE_)
#define _CIGI_SWAPPING_INCLUDE_

#include "CigiTypes.h"

//=========================================================
//! Byte swaps the Cigi_int16 pointed to by src
//!  and places the result into the Cigi_int16 pointed to by dest.
//! \param dest - Specifies where the byte swapped Cigi_int16 
//!   should be placed
//! \param src - Specifies the source Cigi_int16 to byte swap.
//!
void CIGI_SPEC CigiSwap2( void *dest, const void *src);

//=========================================================
//! Byte swaps the word pointed to by src
//!  and places the result into the word pointed to by dest.
//! \param dest - Specifies where the byte swapped word 
//!   should be placed
//! \param src - Specifies the source word to byte swap.
//!
void CIGI_SPEC CigiSwap4( void *dest, const void *src);

//=========================================================
//! Byte swaps the double word pointed to by src<br>
//!  and places the result into the double word pointed to by dest.
//! \param dest - Specifies where the byte swapped double word 
//!   should be placed
//! \param src - Specifies the source double word to byte swap.
//!
void CIGI_SPEC CigiSwap8( void *dest, const void *src);

//=========================================================
//! Copies the double word pointed to by src into the <br>
//!   double word pointed to by dest.
//! \param dest - Specifies where the double word should be placed
//! \param src - Specifies the source double word to copy.
//!
void CIGI_SPEC CigiCopy8( void *dest, const void *src);


/** Macros for copying bytes of data from one address to another.
  */

//=========================================================
//! Copies the byte pointed to by src into the byte <br>
//!   pointed to by dest.
//! \param _dest_ - Specifies where the byte should be placed
//! \param _src_ - Specifies the source byte to copy.
//!
#define CigiCopy1( _dest_, _src_)                           \
    ( *( ( Cigi_int8*)( _dest_)) = *( ( Cigi_int8*)( _src_)) )

//=========================================================
//! Copies the Cigi_int16 pointed to by src into the short <br>
//!   pointed to by dest.
//! \param _dest_ - Specifies where the Cigi_int16 should be placed
//! \param _src_ - Specifies the source Cigi_int16 to copy.
//!
#define CigiCopy2( _dest_, _src_)                           \
    ( *( ( Cigi_int16*)( _dest_)) = *( ( Cigi_int16*)( _src_)) )

//=========================================================
//! Copies the word pointed to by src into the word <br>
//!   pointed to by dest.
//! \param _dest_ - Specifies where the word should be placed
//! \param _src_ - Specifies the source word to copy.
//!
#define CigiCopy4( _dest_, _src_)                           \
    ( *( ( Cigi_int32*)( _dest_)) = *( ( Cigi_int32*)( _src_)) )

//=========================================================
//! Formats a double floating-point number (double) as a type B6 number.<br>
//! note: Percision of up to 0.015625 may be lost in this conversion.
//! \param _fval - Specifies the double floating-point number to convert.
//!
//! \return the B6 converted number.
//!
#define CIGI_FLOAT_TO_B6( _fval) ( (Cigi_int16)( ( _fval) * 64.0))

/** Formats a number of type B6 as a floating-point number (double).
  */
//=========================================================
//! Formats a type B6 number as a double floating-point number (double).
//! \param _b6val - Specifies the B6 format number to convert.
//!
//! \return the double converted number.
//!
#define CIGI_B6_TO_FLOAT( _b6val) ( ( (double) _b6val) / 64.0)

//=========================================================
//! Formats a double floating-point number (double) as a type B-angle number.<br>
//! note: Percision of up to 0.0054931640625 may be lost in this conversion.
//! \param _fval - Specifies the double floating-point number to convert.
//!
//! \return the B6 converted number.
//!
#define CIGI_FLOAT_TO_BA( _fval) ( (Cigi_int32)( ( _fval) * 182.0444))

/** Formats a number of type B6 as a floating-point number (double).
  */
//=========================================================
//! Formats a type B-angle number as a double floating-point number (double).
//! \param _baval - Specifies the B-angle format number to convert.
//!
//! \return the double converted number.
//!
#define CIGI_BA_TO_FLOAT( _baval) ( ( (double) _baval) / 182.0444)



#endif // #if !defined(_CIGI_SWAPPING_INCLUDE_)
