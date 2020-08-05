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
 *  FILENAME:   CigiProcessType.h
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
 *  2006.10.10 Greg Basler                       0.1
 *  Initial Release.
 *  
 *  05/14/2008 Greg Basler                       Version 2.2.0
 *  Fixed the conversion process.
 *  
 *  Author: The Boeing Company
 *
 * </pre>
 */

#if !defined(_CIGI_PROCESS_TYPE_INCLUDED_)
#define _CIGI_PROCESS_TYPE_INCLUDED_


//=========================================================
//! The enumeration or Table of Signal Types.
//!
namespace CigiProcessType
{
   typedef enum {
      ProcNone=0,
      ProcIGCtrl=1,
      ProcSOF=2,
      ProcStd=3,
      ProcEntity=4,
      ProcEnvCtrl=5,
      ProcVarSize=6,
      ProcShortArtPartToArtPart=7,
      TwoPassCnvtProcNone=8,
      TwoPassCnvtProcStd=9
   } Type;
}


#endif   // _CIGI_PROCESS_TYPE_INCLUDED_


