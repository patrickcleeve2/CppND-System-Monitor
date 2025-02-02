#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// helper functions for parsing files
template <typename T>
T findValueByKey(std::string const &keyFilter, std::string const &filename) {
  std::string line, key;
  T value;

  std::ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  return value;
};

template <typename T>
T getValueOfFile(std::string const &filename) {
  std::string line;
  T value;

  std::ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
};

// Read the operating system name from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filterOperatingSystemName) {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read the kernel version from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal = findValueByKey<float>(filterMemTotalString, kMeminfoFilename);
  float memFree = findValueByKey<float>(filterMemFreeString, kMeminfoFilename);
  return (memTotal - memFree) / memTotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime, idle_uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle_uptime;
  }
  return uptime;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string value;
  vector<string> cpu;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        // get all cpus, first line is aggregate
        if ((key.find(filterCpu) != string::npos)) {
          cpu.push_back(line);
        }
      }
    }
  }

  return cpu;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return findValueByKey<float>(filterProcesses, kStatFilename);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return findValueByKey<float>(filterRunningProcesses, kStatFilename);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  return std::string(
      getValueOfFile<std::string>(std::to_string(pid) + kCmdlineFilename));
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  float ram = findValueByKey<float>(filterProcMem,
                                    std::to_string(pid) + kStatusFilename);
  // convert kB to MB
  return std::to_string(ram / 1024.0).substr(0, 5);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  return findValueByKey<string>(filterUID,
                                std::to_string(pid) + kStatusFilename);
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string line;

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      // parse all etc/passwd data for user and uid
      // ref: https://www.youtube.com/watch?v=q7jbVeAtdqg
      std::vector<std::string> result;
      std::istringstream sstream(line);
      std::string word;
      while (std::getline(sstream, word, ':')) {
        result.push_back(word);
      }

      // return username matching uid
      if (result[2] == uid) {
        return result[0].substr(0, 7);  // user
      }
    }
  }

  return uid;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long startime;  // clock ticks
  long uptime;    // sec
  string line;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
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

      // starttime (22) ref: https://man7.org/linux/man-pages/man5/proc.5.html
      startime = std::stoi(result[21]);
      uptime = LinuxParser::UpTime() - (startime / sysconf(_SC_CLK_TCK));
    }
  }

  return uptime;
}
