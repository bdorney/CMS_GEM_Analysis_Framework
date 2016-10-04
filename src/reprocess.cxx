#include <iostream>
#include <iomanip>
#include "SRSMain.h"


void help(const std::string& msg1, const std::string& msg2){
  std::cout << msg1 <<":  "<<msg2<<std::endl;
  exit(-1);
}

int main(int arcg, char** argv){

  if (arcg!=2) {
    help("Usage: ",std::string(argv[0])+" [file]");
  }
  std::string rawfile(argv[1]);
  std::cout <<"Opening raw data file "<<rawfile<<std::endl;
  // Init
  SRSMain* repro=SRSMain::Reprocessor(rawfile,"amore.cfg");

  // reprocessing and store on root
  repro->Reprocess();
  delete repro;
}
