#include "processor.h"

#include <iostream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::vector;

// Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> cpu_string = LinuxParser::CpuUtilization();

  // only do the aggregated cpu data for now
  string cpu = cpu_string[0];

  std::istringstream sstream(cpu);
  string word;
  vector<int> cpu_data;

  // extract proc data from string
  while (sstream >> word) {
    if (word != "cpu") {
      cpu_data.push_back(std::stoi(word));
    }
  }

  // cpu usage calculation (ref:
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux)
  // user, nice, system, idle, iowait, irq, softirq, steal, guest, guestnice, ?
  // 0,    1,    2,      3,    4,      5,   6,       7,     8,     9,         10
  float idle = cpu_data[3] + cpu_data[4];
  float non_idle = cpu_data[0] + cpu_data[1] + cpu_data[2] + cpu_data[5] +
                   cpu_data[6] + cpu_data[7];

  return non_idle / (idle + non_idle);
}