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
 *  FILENAME:   CigiException.h
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
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Added the CIGI_SPEC modifier to the class declaration for exporting the 
 *  class in a Windows DLL.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Included the CigiTypes.h header file.
 * </pre>
 *  Author: The Boeing Company
 *
 */


class CigiException;


#if ! defined( _CIGI_EXCEPTION_INCLUDED_)
#define _CIGI_EXCEPTION_INCLUDED_


// -------- include files -----------------------------------------

#include <string>
#include <string.h>

#include "CigiErrorCodes.h"

#include <exception>

#include "CigiTypes.h"

// -------- preprocessor definitions ------------------------------





// -------- class definition --------------------------------------

//=========================================================
//! The general base exception class for the CIGI API
//!
class CIGI_SPEC CigiException : public std::exception
{

public:


   //==> Management

   //=========================================================
   //! The general constructor for the base CIGI exception class
   //!
   CigiException( void) { };


   //=========================================================
   //! The general destructor for the base CIGI exception class
   //!
   virtual ~CigiException( void) throw() { };



   //==> Accessing Member Variable Values functions


   //=========================================================
   //! Returns whether the exception has a message.
   //! \return true for having a message and false for no msg..
   //!
   bool HasMessage( void) const
   {
      return !Msg.empty();
   }

   //=========================================================
   //! Gets the error code for the exception.
   //! \return The error code for this exception.
   //!
   int GetErrorCode( void) const { return(ErrorCode); }

   //=========================================================
   //! Returns whether this exception has a set error code.
   //! \return True if this exception has an error code set.
   //!
   bool HasErrorCode( void) const
   {
      return(ErrorCode != CIGI_ERROR_NONE); // same as CIGI_SUCCESS
   }


   //=========================================================
   //! The virtual implemented what funtion.
   //! \return The message for this exception.
   //!
   virtual const char *what() const throw() { return Msg.c_str(); }


protected:

   //=========================================================
   //! The message for this exception.
   //!
   std::string Msg;

   //=========================================================
   //! The error code for this exception.
   //!
   int ErrorCode;


};
    
#endif  // _CIGI_EXCEPTION_INCLUDED_
