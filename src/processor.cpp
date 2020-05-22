#include<string.h>
#include<vector>
#include "processor.h"
#include "linux_parser.h"
using namespace std;
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<string> jiffy = LinuxParser::CpuUtilization();
    long int sum =0;
    for(auto i=0;i<jiffy.size();i++){
        sum += stol(jiffy[i]);
    }
    float utilize = 1-((float)stol(jiffy[3])/sum); 
      
    return utilize;
}