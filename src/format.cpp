#include "format.h"

#include <string>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long elapsed_seconds) {
  // ref:
  // https://stackoverflow.com/questions/25696992/converting-seconds-to-hours-and-minutes-and-seconds/52444808
  //      https://knowledge.udacity.com/questions/155686
  int hours, minutes, seconds;
  minutes = elapsed_seconds / 60;
  hours = minutes / 60;
  seconds = elapsed_seconds % 60;

  std::string hour_str = std::to_string(hours);
  std::string minutes_str = std::to_string(minutes % 60);
  std::string seconds_str = std::to_string(seconds);

  // elapsed time in HH:MM::SS
  return hour_str.insert(0, 2 - hour_str.length(), '0') + ":" +
         minutes_str.insert(0, 2 - minutes_str.length(), '0') + ":" +
         seconds_str.insert(0, 2 - seconds_str.length(), '0');
}