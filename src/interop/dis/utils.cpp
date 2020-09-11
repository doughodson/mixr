
#include "mixr/interop/dis/utils.hpp"

#include "mixr/base/util/str_utils.hpp"

#include <cctype>

namespace mixr {
namespace dis {

//------------------------------------------------------------------------------
// Generate a federate name from the site and application numbers:
//  "SnnAmm" -- where nn and mm are the site and app numbers.
//------------------------------------------------------------------------------
bool makeFederateName(char* const fedName, const unsigned int len, const unsigned short site, const unsigned short app)
{
   static const int p10[5] = { 10000, 1000, 100, 10, 1 };

   bool ok{};
   if (fedName != nullptr && len > 0 && site > 0 && app > 0) {
      char cbuff[64]{};
      unsigned int idx{};

      // First the site number: Snnn
      cbuff[idx++] = 'S';
      {
         int tmp{ site };
         bool digits{};
         for (unsigned int i = 0; i < 5; i++) {
            int rr{ tmp / p10[i] };
            if (rr > 0 || digits) {
               cbuff[idx++] = '0' + char(rr);
               digits = true;
            }
            tmp -= (rr * p10[i]);
         }
      }

      // then the application number: Annn
      cbuff[idx++] = 'A';
      {
         int tmp{ app };
         bool digits{};
         for (unsigned int i = 0; i < 5; i++) {
            int rr{ tmp / p10[i] };
            if (rr > 0 || digits) {
               cbuff[idx++] = '0' + char(rr);
               digits = true;
            }
            tmp -= (rr * p10[i]);
         }
      }

      cbuff[idx++] = 0;

      if (idx <= len) {
         base::utStrcpy(fedName, len, cbuff);
         ok = true;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Generate a federation name from the exercise numbers:
//  "Ennn" -- where nnn is the exercise number, which must be greater than zero
//------------------------------------------------------------------------------
bool makeFederationName(char* const fedName, const unsigned int len, const unsigned short exercise)
{
   static const int p10[5] = { 10000, 1000, 100, 10, 1 };

   bool ok{};
   if (fedName != nullptr && len > 0 && exercise > 0) {
      char cbuff[64]{};
      unsigned int idx{};

      cbuff[idx++] = 'E';
      {
         int tmp{ exercise };
         bool digits{};
         for (unsigned int i = 0; i < 5; i++) {
            int rr{ tmp / p10[i] };
            if (rr > 0 || digits) {
               cbuff[idx++] = '0' + char(rr);
               digits = true;
            }
            tmp -= (rr * p10[i]);
         }
      }

      cbuff[idx++] = 0;

      if (idx <= len) {
         base::utStrcpy(fedName, len, cbuff);
         ok = true;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Parse federate name for the site and application numbers
//  (We're expecting "SnnAmm" where nn and mm are the site and app numbers.)
//------------------------------------------------------------------------------
bool parseFederateName(unsigned short* const site, unsigned short* const app, const char* const fedName)
{
   bool ok{};
   if (site != nullptr && app != nullptr && fedName != nullptr) {
      unsigned short tSite{};
      unsigned short tApp{};
      unsigned int idx{};

      // First check and convert site number
      ok = (std::toupper(fedName[idx++]) == 'S');
      if (ok) {
         unsigned int tmp{};
         unsigned int cnt{};
         while (cnt < 5 && std::isdigit(fedName[idx])) {
            tmp *= 10;
            tmp += static_cast<int>(fedName[idx++] - '0');
            cnt++;
         }
         ok = (tmp > 0) && (tmp <= 0xFFFF);
         if (ok) tSite = static_cast<unsigned short>(tmp);
      }

      // Next check and convert application number
      ok = (std::toupper(fedName[idx++]) == 'A');
      if (ok) {
         unsigned int tmp{};
         unsigned int cnt{};
         while (cnt < 5 && std::isdigit(fedName[idx])) {
            tmp *= 10;
            tmp += static_cast<int>(fedName[idx++] - '0');
            cnt++;
         }
         ok = (tmp > 0) && (tmp <= 0xFFFF);
         if (ok) tApp = static_cast<unsigned short>(tmp);
      }

      // If all is well, send the values back to the user.
      if (ok) {
         *site = tSite;
         *app = tApp;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Parse federation name for the exercise number
//  (We're expecting "Ennn" where nnn is the exercise.)
//------------------------------------------------------------------------------
bool parseFederationName(unsigned short* const exercise, const char* const fedName)
{
   bool ok{};
   if (exercise != nullptr && fedName != nullptr) {
      unsigned short tExercise{};
      unsigned int idx{};

      ok = (std::toupper(fedName[idx++]) == 'E');
      if (ok) {
         unsigned int tmp{};
         unsigned int cnt{};
         while (cnt < 5 && std::isdigit(fedName[idx])) {
            tmp *= 10;
            tmp += static_cast<int>(fedName[idx++] - '0');
            cnt++;
         }
         ok = (tmp > 0) && (tmp <= 0xFFFF);
         if (ok) tExercise = static_cast<unsigned short>(tmp);
      }

      // If all is well, send the value back to the user.
      if (ok) {
         *exercise = tExercise;
      }
   }
   return ok;
}

}
}

