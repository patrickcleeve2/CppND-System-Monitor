#include <vector>
#include <string>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {     
    vector<string> cpu = LinuxParser::CpuUtilization();
    return 0.8;  }