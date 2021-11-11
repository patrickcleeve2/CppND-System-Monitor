#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() const { return Process::pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const {
  // The last one!

  float cpu_utilisation;
  string line;

  std::ifstream filestream(LinuxParser::kProcDirectory +
                           std::to_string(Process::Pid()) +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      // parse all data
      // ref: https://www.youtube.com/watch?v=q7jbVeAtdqg
      std::vector<std::string> result;
      std::istringstream sstream(line);
      std::string word;
      while (std::getline(sstream, word, ' ')) {
        result.push_back(word);
      }

      // calculate cpu utilisation
      // utime, stime, cutime, cstime, uptime
      // 13, 14, 15, 16, 21
      // ref:
      // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599

      long utime = std::stoi(result[13]);
      long stime = std::stoi(result[14]);
      long cutime = std::stoi(result[15]);
      long cstime = std::stoi(result[16]);
      long starttime = std::stoi(result[21]);

      float total_time = utime + stime + cutime + cstime;  // include children
      float seconds =
          LinuxParser::UpTime() - (starttime / sysconf(_SC_CLK_TCK));
      float cpu_utilisation = (total_time / sysconf(_SC_CLK_TCK)) / seconds;
    }
  }

  return cpu_utilisation;
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Process::Pid()); }

// Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(Process::Pid()); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Process::Pid()); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Process::Pid()); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return Process::CpuUtilization() < a.CpuUtilization() ? true : false;
  //   return std::stoi(Process::Ram()) < std::stoi(a.Ram()) ? true : false;
}

// Overload the "greater than" comparison operator for Process objects
bool Process::operator>(Process const& a) const {
  return Process::CpuUtilization() < a.CpuUtilization() ? true : false;
  //   return std::stoi(Process::Ram()) > std::stoi(a.Ram()) ? true : false;
}

// Set the process pid
void Process::setPid(int pid) { Process::pid_ = pid; }