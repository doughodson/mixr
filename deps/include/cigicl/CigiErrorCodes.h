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
 *  FILENAME:   CigiErrorCodes.h
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
 *  09/15/2003 James Durtschy                    CIGI_CR_DR_1
 *  Initial Release.
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if ! defined( _CIGI_ERROR_CODES_INCLUDED_)
#define _CIGI_ERROR_CODES_INCLUDED_
    
    #define CIGI_SUCCESS                              0
    #define CIGI_ERROR_NONE                           0
    
    #define CIGI_ERROR_MISCELLANEOUS                 -1
    #define CIGI_ERROR_UNEXPECTED_NULL               -2
    #define CIGI_ERROR_BUFFER_OVERRUN                -3
    #define CIGI_ERROR_CALLED_OUT_OF_SEQUENCE        -4
    #define CIGI_ERROR_INVALID_PACKET                -5
    #define CIGI_ERROR_INVALID_OPCODE                -6
    #define CIGI_ERROR_ALLOC_FAILED                  -7
    #define CIGI_ERROR_BUFFER_TOO_SMALL              -8
    #define CIGI_ERROR_MAX_SESSIONS                  -9
    #define CIGI_ERROR_INVALID_SESSION              -10
    #define CIGI_ERROR_INVALID_SESSION_TYPE         -11
    #define CIGI_ERROR_MISSING_IG_CONTROL_PACKET    -12
    #define CIGI_ERROR_MISSING_SOF_PACKET           -13
    #define CIGI_ERROR_NOT_IMPLEMENTED              -14
    #define CIGI_ERROR_PACKET_TOO_LARGE             -15
    #define CIGI_ERROR_INVALID_PACKET_SIZE          -16
    #define CIGI_ERROR_VALUE_OUT_OF_RANGE           -17
    #define CIGI_ERROR_WRONG_VERSION                -18
    #define CIGI_ERROR_INVALID_USER_PACKET          -19
    #define CIGI_ERROR_IMPROPER_PACKET              -20


#endif  // _CIGI_ERROR_CODES_INCLUDED_








































































