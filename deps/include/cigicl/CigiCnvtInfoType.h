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
 *  FILENAME:   CigiCnvtInfoType.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface (CIGI) Class Library
 *
 *  PROGRAM DESCRIPTION:
 *  ...
 *
 *  MODIFICATION NOTES:
 *  DATE     NAME                                Project version
 *  DESCRIPTION OF CHANGE........................
 *
 *  03/11/2008 Greg Basler                       2.0.0
 *  Initial Release.
 *
 *  Author: The Boeing Company
 *
 * </pre>
 */

#if !defined(_CIGI_CONVERTION_INFORMATION_TYPE_INCLUDED_)
#define _CIGI_CONVERTION_INFORMATION_TYPE_INCLUDED_

#include "CigiProcessType.h"

//=========================================================
//! The information needed to convert between the versions
//!  of CIGI.
//!
namespace CigiCnvtInfoType
{
   typedef struct CigiCnvtInfoStruct {

      //============================
      //! The ID of the processing
      //!  method that this packet uses
      CigiProcessType::Type ProcID;

      //============================
      //! The packet ID of the packet
      //!  to which this packet will
      //!  be converted.
      Cigi_uint8 CnvtPacketID;

   } Type;
}


#endif   // _CIGI_CONVERTION_INFORMATION_TYPE_INCLUDED_


