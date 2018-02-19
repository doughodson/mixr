
#ifndef __mixr_base_Statistic_H__
#define __mixr_base_Statistic_H__

#include "mixr/base/Object.hpp"

#include <cmath>
#include <limits>

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Statistic
// Description:  General statistics class: acts like a statistical calculator:
//               Computes the mean, absolute mean, the variance, standard
//               deviation, RMS, maximum and minimum values of all data points
//               added to the statistic.  Use sigma() to add points and clear()
//               to restart (or clear) the statistic.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Statistic
// Slots: none
//------------------------------------------------------------------------------
class Statistic : public Object
{
    DECLARE_SUBCLASS(Statistic, Object)

public:
   Statistic();

   void sigma(const double value);                          // Adds a data point
   void sigma(const double* const values, const int size);  // Adds an array of data points
   void sigma(const float* const values, const int size);   // Adds an array of data points

   int getN() const                  { return n; }       // Returns the number of data points
   double mean() const;                                  // Returns the mean of the data (or zero if no data points)
   double absMean() const;                               // Returns the mean of the abs values (or zero)
   double variance() const;                              // Returns the variance (or zero)
   double stdDev() const;                                // Returns the standard deviation (or zero)
   // returns the max data point (or the negative of max value for double if there were no data points)
   double maxValue() const           { return maximum; }
   // returns the min data point (or max value for double if there were no data points)
   double minValue() const           { return minimum; }
   double rms() const;                                    // Returns the Root-Mean-Squared
   // returns the confidence interval for the statistic given Z
   double ci(const double) const;
   double value() const              { return value1; }   // Returns the last values added by sigma()

   // clears the statistics; it is now ready for new data points
   void clear();

private:
   int n{};               // number of values
   double maximum{-std::numeric_limits<double>::max()};  // max value
   double minimum{std::numeric_limits<double>::max()};   // min value
   double sum{};          // sum of values
   double absSum{};       // sum of abs values
   double sumSq{};        // sum of the squares
   double value1{};       // last value added
} ;

// adds a data point
inline void Statistic::sigma(const double value)
{
   // save the value
   value1 = value;

   // Abs value
   double avalue{value};
   if (avalue < 0.0) avalue = -avalue;

   // Max/Min value
   if (value > maximum) maximum = value;
   if (value < minimum) minimum = value;

   // Sum of values
   sum = sum + value;

   // Sum of abs values
   absSum = absSum + avalue;

   // Sum of squares
   sumSq = sumSq + (value * value);

   // Number of values
   n++;
}

// adds an array of 'double' data points
inline void Statistic::sigma(const double* const values, const int size)
{
   if (values != nullptr && size > 0) {
      for (int i = 0; i < size; i++) {
         sigma( values[i] );
      }
   }
}

// adds an array of 'double' data points
inline void Statistic::sigma(const float* const values, const int size)
{
   if (values != nullptr && size > 0) {
      for (int i = 0; i < size; i++) {
         sigma( static_cast<double>(values[i]) );
      }
   }
}

// returns the mean of the data points
inline double Statistic::mean() const
{
   if (n != 0)
      return (sum / static_cast<double>(n));
   else
      return 0.0;
}

// returns the mean of the abs of the data points
inline double Statistic::absMean() const
{
   if (n != 0)
      return (absSum / static_cast<double>(n));
   else
      return 0.0;
}

// returns the variance
inline double Statistic::variance() const
{
   double var{};
   int n1{n - 1};
   double sqTheSum {sum * sum};
   double numer{(n * (sumSq )) - sqTheSum};
   // due to round-off error, numerator can end up less than zero!
   // if so, variance is very, very small, therefore set to zero.
   if (numer < 0.0) {
     numer = 0.0;
   }
   const auto denom = static_cast<double>(n) * static_cast<double>(n1);
   if (denom != 0.0) {
      var = (numer / denom);
   }
   return var;
}

// returns the standard deviation
inline double Statistic::stdDev() const
{
   return (std::sqrt(variance()));
}

// returns the RMS value
inline double Statistic::rms() const
{
   double rmsValue {};
   if (n != 0.0) {
      rmsValue = std::sqrt( sumSq / static_cast<double>(n) );
   }
   return rmsValue;
}

// returns the confidence interval
inline double Statistic::ci(const double Z) const
{
  if (n != 0) {
    return (Z*stdDev()) / std::sqrt(static_cast<double>(n));
  }
  return 0.0;
}

}
}

#endif
