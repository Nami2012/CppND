#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  
  Process(int pid);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  bool operator>(Process const& a) const; 
  void CpuUtilization(long process_jiffy, long system_jiffy);
  
  // TODO: Declare any necessary private members
 private:
 int pid_;
 float CpuUtilization_;
 long cached_process_jiffy_=0;
 long cached_system_jiffy_=0;
};

#endif