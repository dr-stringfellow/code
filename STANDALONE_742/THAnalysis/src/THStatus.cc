////////////////////////////////////////////////////////////////////////////////
//
// THStatus
// --------
//
//            09/04/2014 Christian Boeser <christian.boser@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "../interface/THStatus.h"

using namespace std;

THStatus::THStatus(int nevt){
  plot_status=-1;
  nevt_=nevt;
};

void THStatus::print_status(int ievt){
  ostringstream stream;
  stream << "\r[" ;
  float progress = (float)(1. + ievt)/(1.0*nevt_); // true progression in percent                                                                                                                                               
  int status_iter = TMath::Nint(progress*20.0); // scale to 20, and produce nearest integer                                                                                                                                    
  if( plot_status < status_iter){
    for(Int_t i=0;i<status_iter;i++){
      if(i>20) break;
      if(status_iter<6){
	stream << "\033[1;31m%\033[0m";
      }
      else if(status_iter<16){
	stream << "\033[1;33m%\033[0m";
      }
      else {
	stream << "\033[1;32m%\033[0m";
	}
    }
    
    for(Int_t i=0;i<(20-status_iter);i++){
      if(i>20) break;
      stream << "-" ;
    }
    
    stream <<  "] ";
    
    if(status_iter<6){
      stream << "\033[1;31m" << 5*status_iter << "%\033[0m";
    }
    else if(status_iter<16){
      stream << "\033[1;33m" << 5*status_iter << "%\033[0m";
    }
    else {
      stream << "\033[1;32m" << 5*status_iter << "%\033[0m";
    }
    
    stream << " completed.  ";
    cout << stream.str() << flush;
    //std::cout.flush();                                                                                                                                                                                                       
    plot_status=status_iter;
  }
}

// Memory information
void THStatus::get_vmem(bool make){
  if(make){
    ostringstream stream;
    static int pagesize = getpagesize();
    ifstream in("/proc/self/statm");
    if(!in) cout << "none" << endl;
    uint64_t n_pages;
    in >> n_pages;
    uint64_t myresult = (n_pages * pagesize) >> 20;
    cout << endl;
    cout << " Use " << myresult << " M of memory";
  }
}
