#ifndef THSTATUS_H
#define THSTATUS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <TMath.h>
#include <stdint.h>

using namespace std;

class THStatus
{
public:
  /// Constructor
  THStatus(int nevt);
  /// print status
  void print_status(int ievt);  
  void get_vmem(bool make);
  
private:
  int plot_status;
  int nevt_;
};


#endif
