#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {   int hours, minutes;
  minutes = seconds / 60;
  hours = minutes / 60;

  // elapsed time in HH:MM::SS
  return std::to_string(hours) + ":" + std::to_string(minutes % 60) + ":" +
         std::to_string(seconds % 60); 
         
}
// ref:
// https://stackoverflow.com/questions/25696992/converting-seconds-to-hours-and-minutes-and-seconds/52444808