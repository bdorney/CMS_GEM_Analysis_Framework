#include <iostream>
#include <iomanip>
#include "SRSConfiguration.h"
#include "SRSMapping.h"
#include "SRSHit.h"
#include "SRSAPVEvent.h"
#include "SRSFECDecoder.h"
#include "SRSEventBuilder.h"
#include "SRSOutputROOT.h"
#include "TFile.h"


void help(const std::string& msg1, const std::string& msg2){
  std::cout << msg1 <<":  "<<msg2<<std::endl;
  exit(-1);
}

int main(int arcg, char** argv){
  if (arcg!=2) {
    help("Usage: ",std::string(argv[0])+" [file]");
  }
  std::ifstream ifs(argv[1],std::ios::binary);
  bool EndOfEvent=false;
  bool debug_s = true;
  bool debug = true;

  unsigned int nMax = 5000;
  unsigned long buffer[nMax];
  unsigned long event[nMax];
  unsigned long preve[nMax];
  unsigned int nw = 0;

  bool newevent=true;

  SRSConfiguration* fSRSConf = new SRSConfiguration("amore.cfg");
  SRSMapping* fMapping;
  fMapping = SRSMapping::GetInstance();
  fMapping->LoadDefaultMapping(fSRSConf->GetMappingFile());
  fMapping->LoadAPVtoPadMapping(fSRSConf->GetPadMappingFile());
  fMapping->PrintMapping() ;
  std::map<int,int>         fApvNoFromApvIDMap = fMapping->GetAPVNoFromIDMap();
  std::map<int,std::string>       fApvNameFromIDMap  = fMapping->GetAPVFromIDMap();

  SRSOutputROOT* fROOT = new SRSOutputROOT(fSRSConf->GetZeroSupCut(), fSRSConf->GetROOTDataType());
  fROOT->InitRootFile();
  int start_eventRecord=0;
  int end_eventRecord=0;

  //  unsigned int lb=0;
  unsigned int nEvent = 0;
  unsigned int iFEC=0,nFEC = 2;
  unsigned int lr=0;
  SRSEventBuilder* eventBuilder = 0;
  do {
    if (newevent){
      newevent = false;
      nEvent++;
      if (nEvent%1000==0) std::cout<<" +++ reprocess: Event # "<<nEvent<<std::endl;
      eventBuilder = new SRSEventBuilder(nEvent,fSRSConf->GetMaxClusterSize(), fSRSConf->GetMinClusterSize(), fSRSConf->GetZeroSupCut(), fSRSConf->GetRunType(), std::string(fSRSConf->GetClusterPositionCorrectionFlag()).find("applyCorrections")!=std::string(fSRSConf->GetClusterPositionCorrectionFlag()).npos);	  
      

    }
    bool eventok = false;
    int nendofmark = 0;
    unsigned int j  =0x0;
    ifs.read((char*)&j,4);
    if (debug_s) std::cout <<" j  "<<std::hex<<j<<std::endl;
    if (!ifs.eof()){
      buffer[nw]=j&0xffffffff;
      nw++;
      if (nw >= nMax) {
	std::cout<<" +++ reprocess: Number of long words on this event exceed the maximum "<<nMax<<" please change nMax"<<std::endl;
	exit(-1);
      }
      // Each FEC data finish with 0xfafafafa and one events is componsed by NFEC fragments. The event builder collect the hit of the 
      // fragments and make the clusters.
      if ((j&0xffffffff) == 0xfafafafa){
	nendofmark++;
	if (debug) std::cout <<std::dec<<" end of data fragment record mark "<<std::endl;
	// realign the words skiping event header, the event start 8 words before the first adc mark (0x41505a)
	bool first = true;
	int nadc = 0;
        for (unsigned int ir=0;ir<nw;ir++){
          if( ((buffer[ir] >> 8) & 0xffffff) == 0x41505a) {
	    if (first) {
	      start_eventRecord = ir - 8;
	      if (start_eventRecord < 0) {
		std::cout <<"  +++ reprocess: Wrong Start of Event"<<std::endl;
	      }              
	      first = false;
	    }
	    nadc+;
	    // break;
          }
        }
	if (debug) std::cout <<" +++ reprocess: Number of ADC "<<nadc<<" Event "<<nEvent<<" FEC# "<<iFEC+1<<std::endl;
        end_eventRecord=nw-start_eventRecord;
	if (debug_s) {
	  std::cout <<std::dec<< " +++ reprocess: number of words "<<nw<<" start of event record "<<start_eventRecord<<" end "<<end_eventRecord<<std::endl;
	  std::cout <<" +++ reprocess: filling the event buffer "<<std::endl;
	}
	if (debug_s) {
	  for (unsigned int ib=0;ib<nw;ib++){
	    std::cout <<" 0x"<<std::setw(8)<<std::setfill('0')<<std::hex<<buffer[ib]<<std::dec;
	    if ((ib+1)%8 == 0) {
	      std::cout <<std::dec<<std::endl;
	    }
	  }
	  std::cout <<std::endl;
	}
	// the event fragment  is stored for deconding and hit building 
	for (unsigned int ir=0;ir<end_eventRecord;ir++){
          event[ir]=buffer[start_eventRecord+ir];
        }

	
	if (debug) std::cout <<" +++ reprocess: event # "<<nEvent<<" FEC "<<iFEC+1<<std::endl;
	SRSFECDecoder dec(eventBuilder,debug_s);
	dec.decodeFEC(end_eventRecord,event);

	iFEC++;
	if (iFEC == nFEC){
	  // The event is now complete and we can form the cluster
	  if (debug) std::cout <<" +++ reprocess: compute cluster"<<std::endl;
	  eventBuilder->ComputeClustersInDetectorPlane();
	  // Fill the trees
	  fROOT->FillRootFile(eventBuilder);

	  newevent = true;
	  //  delete eventBuilder;
	  eventBuilder=0;
	  iFEC = 0;
	}
	nw = 0;
      } //end of good fragment control
      } // end of Mark control
    } // good file control...
  }while(!ifs.eof()||EndOfEvent);
  fROOT->WriteRootFile();
  if (eventBuilder != 0){
    //    delete eventBuilder;
  }
  delete fROOT;
  std::cout <<std::endl;
  ifs.close();
  delete fSRSConf;
}
