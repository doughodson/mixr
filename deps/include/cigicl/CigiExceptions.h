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
 *  FILENAME:   CigiExceptions.h
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
 *  Added the CIGI_SPEC modifier to the class declarations for exporting the 
 *  classes in a Windows DLL.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 * </pre>
 *  Author: The Boeing Company
 *
 */


#if ! defined( _CIGI_EXCEPTIONS_INCLUDED_)
#define _CIGI_EXCEPTIONS_INCLUDED_
    
    
#include "CigiException.h"
#include <sstream>
#include <string.h>
#include <stdio.h>


//=========================================================
//! This is the exception thrown when a problem involving 
//!   a class, variable, memory, etc. allocatin failure 
//!   is encountered.
//!
class CIGI_SPEC CigiAllocFailedException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiAllocFailedException 
   //!   exception class
   //!
   CigiAllocFailedException(void)
   {
      Msg = "Allocation Failure\n";
      ErrorCode = CIGI_ERROR_ALLOC_FAILED;
   }

   //=========================================================
   //! General Destructor for the CigiAllocFailedException 
   //!   exception class
   //!
   virtual ~CigiAllocFailedException(void) throw() { };

protected:

};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   the current message buffer being overrun is encountered.
//!
class CIGI_SPEC CigiBufferOverrunException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiBufferOverrunException 
   //!   exception class
   //!
   CigiBufferOverrunException(void)
   {
      Msg = "Buffer Overrun Error\n";
      ErrorCode = CIGI_ERROR_BUFFER_OVERRUN;
   }

   //=========================================================
   //! General Destructor for the CigiBufferOverrunException 
   //!   exception class
   //!
   virtual ~CigiBufferOverrunException(void) throw() { };

protected:

};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   the buffer being to small to use is encountered.
//!
class CIGI_SPEC CigiBufferTooSmallException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiBufferTooSmallException 
   //!   exception class
   //!
   CigiBufferTooSmallException(void)
   {
      Msg = "Buffer Too Small Error\n";
      ErrorCode = CIGI_ERROR_BUFFER_TOO_SMALL;
   }


   //=========================================================
   //! General Destructor for the CigiBufferTooSmallException 
   //!   exception class
   //!
   virtual ~CigiBufferTooSmallException(void) throw() { };

protected:


};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   routines having a specific call sequence being called 
//!   out of that sequence is encountered.
//!
class CIGI_SPEC CigiCalledOutOfSequenceException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiCalledOutOfSequenceException 
   //!   exception class
   //!
   CigiCalledOutOfSequenceException(void)
   {
      Msg = "Routine Call Out Of Sequence Error\n";
      ErrorCode = CIGI_ERROR_CALLED_OUT_OF_SEQUENCE;
   }

   //=========================================================
   //! General Destructor for the CigiCalledOutOfSequenceException 
   //!   exception class
   //!
   virtual ~CigiCalledOutOfSequenceException(void) throw() { };

protected:


};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   the use of an invalid opcode/packet id is encountered.
//!
class CIGI_SPEC CigiInvalidOpcodeException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiInvalidOpcodeException 
   //!   exception class
   //!
   CigiInvalidOpcodeException(void)
   {
      Msg = "Invalid Packet ID\n";
      ErrorCode = CIGI_ERROR_INVALID_OPCODE;
   }

   //=========================================================
   //! General Destructor for the CigiInvalidOpcodeException 
   //!   exception class
   //!
   virtual ~CigiInvalidOpcodeException(void) throw() { };

protected:


};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   an invalid packet is encountered.
//!
class CIGI_SPEC CigiInvalidPacketException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiInvalidPacketException 
   //!   exception class
   //!
   CigiInvalidPacketException(void)
   {
      Msg = "Invalid Packet\n";
      ErrorCode = CIGI_ERROR_INVALID_PACKET;
   }

   //=========================================================
   //! General Destructor for the CigiInvalidPacketException 
   //!   exception class
   //!
   virtual ~CigiInvalidPacketException(void) throw() { };

protected:


};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   an incorrect packet size is encountered.
//!
class CIGI_SPEC CigiInvalidPacketSizeException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiInvalidPacketSizeException 
   //!   exception class
   //!
   CigiInvalidPacketSizeException(void)
   {
      Msg = "Invalid Packet Size\n";
      ErrorCode = CIGI_ERROR_INVALID_PACKET_SIZE;
   }

   //=========================================================
   //! General Destructor for the CigiInvalidPacketSizeException 
   //!   exception class
   //!
   virtual ~CigiInvalidPacketSizeException(void) throw() { };

protected:


};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   an invalid session being requested is encountered.
//!
class CIGI_SPEC CigiInvalidSessionException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiInvalidSessionException 
   //!   exception class
   //!
   CigiInvalidSessionException(void) 
   {
      Msg = "Invalid Session\n";
      ErrorCode = CIGI_ERROR_INVALID_SESSION;
   }

   //=========================================================
   //! General Destructor for the CigiInvalidSessionException 
   //!   exception class
   //!
   virtual ~CigiInvalidSessionException(void) throw() { };

protected:


};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   invalid seesion type being requested is encountered.
//!
class CIGI_SPEC CigiInvalidSessionTypeException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiInvalidSessionTypeException 
   //!   exception class
   //!
   CigiInvalidSessionTypeException(void) 
   {
      Msg = "Invalid Session Type\n";
      ErrorCode = CIGI_ERROR_INVALID_SESSION_TYPE;
   }

   //=========================================================
   //! General Destructor for the CigiInvalidSessionTypeException 
   //!   exception class
   //!
   virtual ~CigiInvalidSessionTypeException(void) throw() { };

protected:


};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   too many sessions being requested is encountered.
//!
class CIGI_SPEC CigiMaxSessionsException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiMaxSessionsException 
   //!   exception class
   //!
   CigiMaxSessionsException(void) 
   {
      Msg = "Requested Number Of Sessions Exceeds Maximum\n";
      ErrorCode = CIGI_ERROR_MAX_SESSIONS;
   }

   //=========================================================
   //! General Destructor for the CigiMaxSessionsException 
   //!   exception class
   //!
   virtual ~CigiMaxSessionsException(void) throw() { };

protected:


};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   the IG Control message not being created and used is encountered.
//!
class CIGI_SPEC CigiMissingIgControlException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiMissingIgControlException 
   //!   exception class
   //!
   CigiMissingIgControlException(void) 
   {
      Msg = "The Message Is Missing The IG Control Packet\n";
      ErrorCode = CIGI_ERROR_MISSING_IG_CONTROL_PACKET;
   }

   //=========================================================
   //! General Destructor for the CigiMissingIgControlException 
   //!   exception class
   //!
   virtual ~CigiMissingIgControlException(void) throw() { };

protected:


};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   the Start Of Frame message not being created and used 
//!   is encountered.
//!
class CIGI_SPEC CigiMissingStartOfFrameException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiMissingStartOfFrameException 
   //!   exception class
   //!
   CigiMissingStartOfFrameException(void) 
   {
      Msg = "The Message Is Missing The Start Of Frame Packet\n";
      ErrorCode = CIGI_ERROR_MISSING_SOF_PACKET;
   }

   //=========================================================
   //! General Destructor for the CigiMissingStartOfFrameException 
   //!   exception class
   //!
   virtual ~CigiMissingStartOfFrameException(void) throw() { };

protected:


};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   an Unimplemented function, packet, etc. is encountered.
//!
class CIGI_SPEC CigiNotImplementedException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiNotImplementedException 
   //!   exception class
   //!
   CigiNotImplementedException(void) 
   {
      Msg = "Not Implemented Error\n";
      ErrorCode = CIGI_ERROR_NOT_IMPLEMENTED;
   }

   //=========================================================
   //! General Destructor for the CigiNotImplementedException 
   //!   exception class
   //!
   virtual ~CigiNotImplementedException(void) throw() { };

protected:


};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   a null pointer being improperly used is encountered.
//!
class CIGI_SPEC CigiNullPointerException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiNullPointerException 
   //!   exception class
   //!
   CigiNullPointerException(void) 
   {
      Msg = "Null Pointer Error\n";
      ErrorCode = CIGI_ERROR_UNEXPECTED_NULL;
   }

   //=========================================================
   //! General Destructor for the CigiNullPointerException 
   //!   exception class
   //!
   virtual ~CigiNullPointerException(void) throw() { };

protected:


};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   a packet being too large is encountered.
//!
class CIGI_SPEC CigiPacketTooLargeException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiPacketTooLargeException 
   //!   exception class
   //!
   CigiPacketTooLargeException(void) 
   {
      Msg = "Packet Is Too Large\n";
      ErrorCode = CIGI_ERROR_PACKET_TOO_LARGE;
   }

   //=========================================================
   //! General Destructor for the CigiPacketTooLargeException 
   //!   exception class
   //!
   virtual ~CigiPacketTooLargeException(void) throw() { };

protected:


};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   a packet parameter being set to an invalid value 
//!   is encountered.
//!
class CIGI_SPEC CigiValueOutOfRangeException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiValueOutOfRangeException 
   //!   exception class
   //!
   CigiValueOutOfRangeException(void) 
   {
      Msg = "Value Out Of Range\n";
      ErrorCode = CIGI_ERROR_VALUE_OUT_OF_RANGE;
   }

   //=========================================================
   //! Integer Data Fill Constructor for the CigiValueOutOfRangeException 
   //!   exception class
   //!
   CigiValueOutOfRangeException(const char *ParameterName, int Value, int Low, int High) 
   {
      std::ostringstream os;
      os << "Parameter Out of Range: Parameter "
         << ParameterName << " is " << Value 
         << ". The valid range is from " << Low << " to " << High << std::endl;

      Msg = os.str();
      ErrorCode = CIGI_ERROR_VALUE_OUT_OF_RANGE;
   }

   //=========================================================
   //! Float/Double Data Fill Constructor for the CigiValueOutOfRangeException 
   //!   exception class
   //!
   CigiValueOutOfRangeException(const char *ParameterName, double Value, double Low, double High) 
   {
      std::ostringstream os;
      os << "Parameter Out of Range: Parameter "
         << ParameterName << " is " << Value 
         << ". The valid range is from " << Low << " to " << High << std::endl;

      Msg = os.str();
      ErrorCode = CIGI_ERROR_VALUE_OUT_OF_RANGE;
   }

   //=========================================================
   //! Float/Double Data Fill Constructor for the CigiValueOutOfRangeException 
   //!   exception class
   //!
   CigiValueOutOfRangeException(const char *ParameterName, double Value, const char *Mod, double Limit) 
   {
      std::ostringstream os;
      os << "Parameter Out of Range: Parameter "
         << ParameterName << " is " << Value 
         << ". The valid range is " << Mod << " " << Limit << std::endl;

      Msg = os.str();
      ErrorCode = CIGI_ERROR_VALUE_OUT_OF_RANGE;
   }

   //=========================================================
   //! General Destructor for the CigiValueOutOfRangeException 
   //!   exception class
   //!
   virtual ~CigiValueOutOfRangeException(void) throw() { };

protected:

};

//=========================================================
//! This is the exception thrown when a problem involving 
//!   an attempted use of the wrong CIGI version is encountered.
//!
class CIGI_SPEC CigiWrongVersionException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiWrongVersionException 
   //!   exception class
   //!
   CigiWrongVersionException(void) 
   {
      Msg = "Wrong Cigi Version Used\n";
      ErrorCode = CIGI_ERROR_WRONG_VERSION;
   }

   //=========================================================
   //! General Destructor for the CigiWrongVersionException 
   //!   exception class
   //!
   virtual ~CigiWrongVersionException(void) throw() { };

protected:


};


//=========================================================
//! This is the exception thrown when a problem involving 
//!   an attempted use of the wrong CIGI version is encountered.
//!
class CIGI_SPEC CigiInvalidUserPacketException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiInvalidUserPacketException 
   //!   exception class
   //!
   CigiInvalidUserPacketException(void) 
   {
      Msg = "Cigi User Defined Packets Must Have an ID above 199\n";
      ErrorCode = CIGI_ERROR_INVALID_USER_PACKET;
   }


   //=========================================================
   //! Constructor used to specify the invalid user packet id
   //!
   CigiInvalidUserPacketException(Cigi_uint8 PacketID)
   {
      std::ostringstream os;
      os << "Cigi User Defined Packets Must Have an ID above 199\nUsed ID was "
         << (int)PacketID << std::endl;

      Msg = os.str();
      ErrorCode = CIGI_ERROR_INVALID_USER_PACKET;
   }


   //=========================================================
   //! General Destructor for the CigiWrongVersionException 
   //!   exception class
   //!
   virtual ~CigiInvalidUserPacketException(void) throw() { };

protected:


};



//=========================================================
//! This is the exception thrown when a problem involving 
//!   an attempted use of the wrong CIGI version is encountered.
//!
class CIGI_SPEC CigiImproperPacketException : public CigiException
{

public:

   //=========================================================
   //! General Constructor for the CigiImproperPacketException 
   //!   exception class
   //!
   CigiImproperPacketException(void) 
   {
      Msg = "Improper packet for Session Type\n";
      ErrorCode = CIGI_ERROR_IMPROPER_PACKET;
   }


   //=========================================================
   //! General Destructor for the CigiWrongVersionException 
   //!   exception class
   //!
   virtual ~CigiImproperPacketException(void) throw() { };

protected:


};

#endif // #if ! defined(_CIGI_EXCEPTIONS_INCLUDED_)
