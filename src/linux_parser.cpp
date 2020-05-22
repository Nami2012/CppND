#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);  
    std::istringstream linestream(line);
    linestream >> os >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float MemTotal,MemFree;
  string key,value,kb,line;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      while(linestream>>key>>value>>kb){
        if(key == "MemTotal:")
           { MemTotal = std::stof(value);}
        if(key == "MemFree:"){
          MemFree = std::stof(value);
        }
      }
    }
  }
  return (MemTotal-MemFree)/MemTotal;
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime_long;
  std::string uptime,idletime,line;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      while(linestream>>uptime>>idletime);
      uptime_long = std::stol(uptime);
    }
  }
  
  return uptime_long; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::string line;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    if(std::getline(filestream,line)){
      std::istringstream linestream(line);
      std::string cpu;
      long user,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice;
        linestream>>user>>nice>>system>>idle>>iowait>>irq>>softirq>>steal>>guest>>guest_nice;
        long totalusertime = user -guest;
        long totalnicetime = nice - guest_nice;
        long totalidletime = idle + iowait;
        long totalsystemtime = system + irq + softirq;
        long totalvirtualtime = guest + guest_nice;
        return totalusertime + totalnicetime+totalidletime+ totalsystemtime + totalvirtualtime +steal;        

    }
    
     }
  return 0;
 }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string value,line;
  vector<string> stat;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if(filestream.is_open()){
    if(std::getline(filestream,line)){
      std::istringstream linestream(line);
      while(linestream>>value){
        stat.push_back(value);
        }
       long utime{std::stol(stat[13])};
       long stime{std::stol(stat[14])};
       long cutime{std::stol(stat[15])};
       long cstime{std::stol(stat[16])};
       return (utime + stime + cutime + cstime);}

    }
    return 0;
  }
  
  

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  long upTime = 0;
  long idleTime;
  if (filestream.is_open()) {
      std::string line;
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> upTime >> idleTime;
  } 
  return upTime;
   }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::string line;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if(filestream.is_open()){
    if(std::getline(filestream,line)){
      std::istringstream linestream(line);
      std::string cpu;
      long user,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice;
        linestream>>user>>nice>>system>>idle>>iowait>>irq>>softirq>>steal>>guest>>guest_nice;
        long totalidletime = idle + iowait;
        return totalidletime;        

    }
    
     }
  return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string value, line, key;
  vector<string> jiffies_list;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  	std::istringstream linestream(line);
   	while (linestream >> key){
    	if (key != "cpu"){
          jiffies_list.push_back(key);}
    }
    }
  return jiffies_list;
   }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::ifstream filestream(kProcDirectory + kStatFilename);
   long totalProcesses = 0;
  if (filestream.is_open()) {
      std::string line;
      while (std::getline(filestream, line) ) {
        std::istringstream linestream(line);
        std::string key;
        linestream >> key;
        if (key == "processes")
        {          
            linestream >> totalProcesses;
            return totalProcesses;
        }
      }

  }
  return 0;  }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::ifstream filestream(kProcDirectory + kStatFilename);
   long Procs_running = 0;
  if (filestream.is_open()) {
      std::string line;
      while (std::getline(filestream, line) ) {
        std::istringstream linestream(line);
        std::string key;
        linestream >> key;
        if (key == "procs_running")
        {          
            linestream >> Procs_running;
            return Procs_running; 
            }
      }

  }
  return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
     std::string cmdline;
     std::ifstream filestream(kProcDirectory +std::to_string(pid)+kCmdlineFilename);
      if (filestream.is_open()) {
        if(std::getline(filestream, cmdline) ) {
            return cmdline;
            }
      }
 
  
  return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::string key,value,kb,line;
  int VmSize;
   std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while (std::getline(filestream,line))
    { std::istringstream linestream(line);
      linestream>>key>>value>>kb;
      
      if(key == "VmSize:"){
       
        VmSize = std::stoi(value);
        VmSize /=1000;
        return std::to_string(VmSize);
      }
    } 
  }
   return "0"; }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string key, value, uid, user, line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
  		std::istringstream linestream(line);
    	while (linestream >> key >> value) {
        	if (key == "Uid:") { 
              	uid = value;
               }
        	}
      	}
    }
  return uid;
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::string user, pwd,uid;
  std::string Uid_ = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if(filestream.is_open()){
    string line;
    while(std::getline(filestream,line)){
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream linestream(line);
      while(linestream>>user>>pwd>>uid){
        if(uid== Uid_)
          return user;
      }
    }
  }
  
  return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
    string value, line;
  long int starttime, uptime;
  vector<string> stat_list;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
			stat_list.push_back(value);
        }
    }
  starttime = std::stol(stat_list[21])/sysconf(_SC_CLK_TCK);
  uptime =  LinuxParser::UpTime() - starttime;
  return uptime;
   }

   float LinuxParser::CpuUtilization(int pid){
       std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
       long seconds,totalTime;
    if (filestream.is_open()) {
        std::string line;
        std::getline(filestream, line);
        std::istringstream linestream(line);
        std::string ignore;
        long utime;
        long stime;
        long cutime;
        long cstime;
        long starttime;
        for(int i = 0; i < 13; i++) linestream >> ignore;
        linestream >> utime >> stime >> cutime >> cstime ;
        for(int i = 0; i < 4; i++) linestream >> ignore;
        linestream >> starttime;
        seconds = LinuxParser::UpTime() - (starttime/sysconf(_SC_CLK_TCK));
        totalTime = (utime + stime + cutime + cstime)/sysconf(_SC_CLK_TCK);
    }
     return totalTime*1.0/seconds;//secondsd;
      
    }

  