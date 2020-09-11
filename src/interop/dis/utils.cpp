
#include "mixr/interop/dis/utils.hpp"

#include "mixr/base/util/str_utils.hpp"

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

}
}

