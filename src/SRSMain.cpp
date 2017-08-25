//Framework Includes
#include <queue>
#include "SRSMain.h"
#include "SRSConfiguration.h"
#include "SRSMapping.h"
#include "SRSHit.h"
#include "SRSAPVEvent.h"
#include "SRSFECDecoder.h"
#include "SRSEventBuilder.h"
#include "SRSOutputROOT.h"

//SRSMain* SRSMain::_repro = 0;

SRSMain::~SRSMain(){
  if (!_isClosed)
    this->Close();
};
 
/*SRSMain* 
SRSMain::Reprocessor(const std::string& rawfile, const std::string& config){
  if (_repro == 0) _repro = new SRSMain(rawfile,config);
  return _repro;
}*/


void 
SRSMain::Reprocess(){

  std::ifstream ifs(_rawfile.c_str(),std::ios::binary);
  unsigned int nMax = 5000;
  unsigned long buffer[nMax];
  unsigned long event[nMax];
  unsigned int iw = 7;
  unsigned int nw = 0;
  bool goaway = false;
  bool firstADCinFEC = true;
  std::queue<unsigned int>  adcheader;
  int nhead = 8;
  int nadc=0;
  bool fillFECdata = false;
  unsigned int nEvent=0;

  SRSEventBuilder* eventBuilder = 0;
  bool debug_s=false;
  unsigned int iFEC=99;
  int nFEC = _maps->GetNbOfFECs();


  do {
    unsigned int j  =0x0;
    ifs.read((char*)&j,4);
    if (!ifs.eof()){
      adcheader.push(j);
      if (adcheader.size() == 10)
        adcheader.pop();
      //      std::cout <<"  first in queie 0x" <<std::hex<<adcheader.back()<<"  queue size "<<std::dec<<adcheader.size()               
      //                <<" last 0x"<<std::hex<<adcheader.front()<<std::dec<<std::endl;                                                 
      if (firstADCinFEC && (( j >>8 ) & 0xffffff) == 0x41505a){
        firstADCinFEC = false;
        fillFECdata = true;

        if (adcheader.size() ==9 ){
          buffer[0]=adcheader.front();
          adcheader.pop();
          buffer[1]=adcheader.front();
          adcheader.pop();
          iFEC = adcheader.front() & 0xff;
          for (unsigned int i=2;i<8;i++){
            buffer[i] = adcheader.front();
            adcheader.pop();
          }
	  iw=7;
        } else {
	  std::cout<<"SRSMain::  +++ reprocess: Wrong Start of Event"<<std::endl;
	}
        nadc++;
        int adcId = j&0xff;//(( j >>8 ) & 0xffffffff);                                                                                  
	if (debug_s)std::cout <<"SRSMain::  +++ reprocess: "<<std::hex<<" ADC Id = 0x"<<adcId<<std::dec<<std::endl;
	//	goaway = (nadc == 35);
      } else if ((j&0xffffffff) == 0xfafafafa){
        firstADCinFEC = true;
        fillFECdata = false;
        iw++;
        buffer[iw]=j;
        if (iFEC == 1) {
	  eventBuilder = new SRSEventBuilder(nEvent,_conf->GetMaxClusterSize(), _conf->GetMinClusterSize(), _conf->GetZeroSupCut(),\
					     _conf->GetRunType(),
					     std::string(_conf->GetClusterPositionCorrectionFlag()).find("applyCorrections")!=std::\
					     string(_conf->GetClusterPositionCorrectionFlag()).npos);

          nEvent++;
	  if(debug_s)std::cout <<"New Event "<<nEvent<<std::endl;
        }
        if (iFEC == nFEC){
	  SRSFECDecoder dec(eventBuilder);
	  dec.decodeFEC(iw,buffer);
	  if (nEvent%1000==0) std::cout<<" SRSMain:: +++ reprocess: Event # "<<nEvent<<std::endl;
	  if (debug_s) std::cout <<" +++ reprocess: compute cluster"<<std::endl;
          eventBuilder->ComputeClustersInDetectorPlane();
          // Fill the trees                                                                                                             
          _root->FillRootFile(eventBuilder);
          eventBuilder=0;

        }else if (iFEC > 3){
	  std::cout <<" BAD FEC"<<std::endl;
        } else {
	  SRSFECDecoder dec(eventBuilder);
	  dec.decodeFEC(iw,buffer);
        }
	iw=0;
      } else if (fillFECdata){
        iw++;
        buffer[iw]=j;
      }
    }
  }while(!ifs.eof() && !goaway);
  ifs.close();
  if (eventBuilder != 0){
    //    delete eventBuilder;
  }
}



SRSMain::SRSMain(const std::string& rawfile, const std::string& config) : 
	_rawfile(rawfile), 
	_config(config),
	_conf(new SRSConfiguration),
	//_maps(new SRSMapping),
	_maps( SRSMapping::GetInstance() ),
	_root(new SRSOutputROOT),
	_isClosed(false)
  {
  this->Init();
}

void 
SRSMain::Init(){
 //_conf = new SRSConfiguration(_config.c_str());
 _conf->Init(_config.c_str());
 //_maps = SRSMapping::GetInstance();
 _maps->LoadDefaultMapping(_conf->GetMappingFile());
 _maps->LoadAPVtoPadMapping(_conf->GetPadMappingFile());
 _maps->PrintMapping();
 //_root = new SRSOutputROOT(_conf->GetZeroSupCut(), _conf->GetROOTDataType());
 _root->InitRootFile();
 _root->SetZeroSupCut(std::stoi(_conf->GetZeroSupCut()));
 _root->SetROOTDataType(_conf->GetROOTDataType());
}

void SRSMain::Close(){
  std::string strRunName = _rawfile;
  _isClosed = true;
  if( strRunName.find(".raw") != std::string::npos){
	strRunName.erase(strRunName.find(".raw"), strRunName.length() - strRunName.find(".raw") );
  }

  _root->SetRunName(strRunName);
  _root->WriteRootFile();
  //delete _root;
  _root.reset();
  //delete _conf;
  _conf.reset();
  _maps.reset();
}
