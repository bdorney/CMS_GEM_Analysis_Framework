#ifndef _SRS_PUBLISHERSRSPUBLISHER_H
#define _SRS_PUBLISHERSRSPUBLISHER_H

/*******************************************************************************
 *   SRS - SRS                                                         *
 *  SRSPublisher                                                                *
 *  SRS Module Class                                                            *
 *  Author: Kondo GNANVO 18/08/2010                                             *
 *  Modified by Stefano Colafranceschi on 22/08/2015 to include ZS rawfiles     *
 *******************************************************************************/
#include <map>
#include <list>
#include <fstream>
#include <iostream>
#include <string>
//#include <MonitorObject.h>
//#include <PublisherModule.h>

//#include <Event.h>
//#include <TDATEEventParser.h>

//#include "TFile.h"

//#include "SRSTrack.h"
//#include "SRSCommon.h"
#include "SRSMapping.h"
//#include "SRSPedestal.h"
//#include "SRSAlignment.h"
//#include "SRSOutputROOT.h"
//#include "SRSRawPedestal.h"
//#include "SRSHistoManager.h"
#include "SRSConfiguration.h"
//#include "SRSFECEventDecoder.h"
//#include "SRSFECPedestalDecoder.h"
//#include "SRSPositionCorrection.h"

//class amore::core::Event;
//using namespace std;

            
class SRSPublisher{
  
 public:
  
  SRSPublisher();
  ~SRSPublisher();
  void Init(SRSConfiguration* conf);
  void ParseEvent();
  int LoadPedestalRootFileIfNecessary(unsigned int * buffer) ;
                
 private:
  int atoi(std::string& s);
  int atof(std::string& s);
  int fEvent;
  int fEventType;
  int fEventRunNb;
  int fEquipmentSize;
  int sigmaCut;
  std::string fRunType, fCFGFile, fCycleWait, fStartEventNumber, fEventFrequencyNumber, fAmoreAgentID;
  SRSMapping       *fMapping;
  SRSConfiguration *fSRSConf;
  int fMinNbOfEquipments ;
  int fMaxNbOfEquipments ;
  std::map <int, int > fApvNoFromApvIDMap ;
  std::map <int, float > fApvGainFromApvNoMap ;
  std::map <int, std::string > fApvNameFromIDMap ;
};
#endif
