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
 *  FILENAME:   CigiIGSession.h
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
 *  Added the CIGI_SPEC modifier to the class declaration for exporting the 
 *  class in a Windows DLL.
 * </pre>
 * @author The Boeing Company
 *
 */

#if !defined(_CIGI_IG_SESSION_INCLUDED_)
#define _CIGI_IG_SESSION_INCLUDED_


#include "CigiSession.h"


//=========================================================
//! The class for the IG Session
//!
class CIGI_SPEC CigiIGSession : public CigiSession  
{
public:

   //==> Management

   //=========================================================
   //! Main Constructor
   //!
   CigiIGSession(const int NumInBuf = 2, const int InBufLen = 16384,
                 const int NumOutBuf = 2, const int OutBufLen = 16384);

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiIGSession();

};

#endif // !defined(_CIGI_IG_SESSION_INCLUDED_)
