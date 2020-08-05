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
 *  FILENAME:   CigiVersionID.h
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
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if !defined(_CIGI_VERSION_ID_INCLUDED_)
#define _CIGI_VERSION_ID_INCLUDED_


#include "CigiTypes.h"



class CIGI_SPEC CigiVersionID
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiVersionID(void)
      : CigiMajorVersion(0)
      , CigiMinorVersion(0)
   {
   }

   //=========================================================
   //! Initializing Constructor
   //!
   CigiVersionID(int CigiMajorVersionIn, int CigiMinorVersionIn)
      : CigiMajorVersion(CigiMajorVersionIn)
      , CigiMinorVersion(CigiMinorVersionIn)
   {
   }

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiVersionID(void)
   {
   }


   //==> Public Accessor functions

   //=========================================================
   //! Gets the value indicating if a valid IGCtrl or SOF 
   //!   packet has been packed for the message.
   //!
   //! \return This returns a value indicating if a valid 
   //!   IGCtrl or SOF packet has been packed
   //!
   void SetCigiVersion(int CigiMajorVersionIn, int CigiMinorVersionIn)
   {
      CigiMajorVersion = CigiMajorVersionIn;
      CigiMinorVersion = CigiMinorVersionIn;
   }


   //==> Processor functions

   //=========================================================
   //! Gets the value indicating if a valid IGCtrl or SOF 
   //!   packet has been packed for the message.
   //!
   //! \return This returns a value indicating if a valid 
   //!   IGCtrl or SOF packet has been packed
   //!
   int GetCombinedCigiVersion(void)
   {
      int CVer = ((CigiMajorVersion << 8) & 0x0000ff00) | 
                  (CigiMinorVersion & 0x000000ff);

      return(CVer);
   }


   //=========================================================
   //! Determines if the version of CIGI is a known version.
   //!
   //! \return This returns a value indicating if a know
   //!   version of CIGI is stored
   //!
   bool IsKnownCigiVersion(void)
   {
      bool KnownVer = false;
      if(CigiMajorVersion > 0)
      {
         if(CigiMajorVersion < 3)
         {
            if(CigiMinorVersion == 0)
               KnownVer = true;
         }
         else if(CigiMajorVersion == 3)
         {
            if((CigiMinorVersion >= 0) && (CigiMinorVersion <= 3))
               KnownVer = true;
         }
      }

      return(KnownVer);
   }


   //=========================================================
   //! Determines if the best match to a known version of CIGI
   //!
   void BestCigiVersion(void)
   {
      if((CigiMajorVersion == 1) || (CigiMajorVersion == 2))
         CigiMinorVersion = 0;
      else if(CigiMajorVersion == 3)
      {
         if(CigiMinorVersion > 3)
            CigiMinorVersion = 3;
         else if(CigiMinorVersion < 2)
            CigiMinorVersion = 0;
      }
      else
      {
         // Set the version to the most mature version of CIGI
         CigiMajorVersion = 3;
         CigiMinorVersion = 3;
      }
   }


   //==> Public operator functions

   //=========================================================
   //! Testing for equality
   //!
   //! \return This returns true if the versions are the same
   //!
   bool operator ==(const CigiVersionID &TestVersion)
   {
      return((CigiMajorVersion == TestVersion.CigiMajorVersion) &&
              (CigiMinorVersion == TestVersion.CigiMinorVersion));
   }

   //=========================================================
   //! Testing for Not Equal
   //!
   //! \return This returns true if the versions are not the same
   //!
   bool operator !=(const CigiVersionID &TestVersion)
   {
      return((CigiMajorVersion != TestVersion.CigiMajorVersion) ||
              (CigiMinorVersion != TestVersion.CigiMinorVersion));
   }

   //=========================================================
   //! Tests for greater than
   //!
   //! \return This returns true if the second version is
   //!   a more mature version.
   //!
   bool operator >(const CigiVersionID &TestVersion)
   {
      bool Rslt = false;
      if(CigiMajorVersion > TestVersion.CigiMajorVersion)
         Rslt = true;
      else if(CigiMajorVersion == TestVersion.CigiMajorVersion)
      {
         if(CigiMinorVersion > TestVersion.CigiMinorVersion)
            Rslt = true;
      }
      return(Rslt);
   }

   //=========================================================
   //! Tests for greater than or equal to
   //!
   //! \return This returns true if the second version is
   //!   the same or a more mature version.
   //!
   bool operator >=(const CigiVersionID &TestVersion)
   {
      bool Rslt = false;
      if(CigiMajorVersion > TestVersion.CigiMajorVersion)
         Rslt = true;
      else if(CigiMajorVersion == TestVersion.CigiMajorVersion)
      {
         if(CigiMinorVersion >= TestVersion.CigiMinorVersion)
            Rslt = true;
      }
      return(Rslt);
   }

   //=========================================================
   //! Tests for Less than
   //!
   //! \return This returns true if the second version is
   //!   a less mature version.
   //!
   bool operator <(const CigiVersionID &TestVersion)
   {
      bool Rslt = false;
      if(CigiMajorVersion < TestVersion.CigiMajorVersion)
         Rslt = true;
      else if(CigiMajorVersion == TestVersion.CigiMajorVersion)
      {
         if(CigiMinorVersion < TestVersion.CigiMinorVersion)
            Rslt = true;
      }
      return(Rslt);
   }

   //=========================================================
   //! Tests for less than or equal to
   //!
   //! \return This returns true if the second version is
   //!   the same or a less mature version.
   //!
   bool operator <=(const CigiVersionID &TestVersion)
   {
      bool Rslt = false;
      if(CigiMajorVersion < TestVersion.CigiMajorVersion)
         Rslt = true;
      else if(CigiMajorVersion == TestVersion.CigiMajorVersion)
      {
         if(CigiMinorVersion <= TestVersion.CigiMinorVersion)
            Rslt = true;
      }
      return(Rslt);
   }


   //==> Member variables

   //=========================================================
   //! Buffer Created<br>
   //! Specifies whether the buffer has been created.
   //!
   int CigiMajorVersion;

   //=========================================================
   //! Buffer Created<br>
   //! Specifies whether the buffer has been created.
   //!
   int CigiMinorVersion;

};

#endif   // _CIGI_VERSION_ID_INCLUDED_


