#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid() const;
  std::string User();
  std::string Command();
  float CpuUtilization() const;
  std::string Ram() const;
  long int UpTime();
  bool operator<(Process const& a) const;  
  bool operator>(Process const& a) const;  

  void setPid(int pid);

  // TODO: Declare any necessary private members
 private:
  int pid_;
};

#endif