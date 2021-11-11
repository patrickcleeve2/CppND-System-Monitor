#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() const { return Process::pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { 
    
    // The last one!
    
    
    
    
    return 0.2; }

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