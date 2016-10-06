// Mapping class is implemented as a singleton

#ifndef __SRSMAPPING__
#define __SRSMAPPING__
/*******************************************************************************
*  AMORE FOR SRS - SRS                                                         *
*  SRSMapping                                                                  *
*  SRS Module Class                                                            *
*  Author: Kondo GNANVO 18/08/2010                                             *
*******************************************************************************/
#include <sstream>
#include <map>
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

class SRSMapping{ 
  
 public:
  //SRSMapping() {fNbOfAPVs = 0;}
  ~SRSMapping();
  /*~SRSMapping(){
    Clear() ;
    ClearMapOfList(fDetectorListFromDetectorTypeMap) ;
    ClearMapOfList(fDetectorListFromReadoutBoardMap) ;
    ClearMapOfList(fDetectorPlaneListFromDetectorMap) ;
    ClearMapOfList(fFECIDListFromDetectorPlaneMap) ;
    ClearMapOfList(fAPVIDListFromFECIDMap) ;
    ClearMapOfList(fAPVIDListFromDetectorPlaneMap) ;
    ClearMapOfList(fAPVIDListFromDetectorMap) ;
    ClearMapOfList(fAPVToPadChannelMap) ;
    ClearMapOfList(fPadDetectorMap) ;
    ClearMapOfList(f1DStripsPlaneMap) ;
    ClearMapOfList(fCartesianPlaneMap) ;
    ClearMapOfList(fCMSGEMDetectorMap) ;
    ClearMapOfList(fUVangleReadoutMap);
  }*/

  template <typename M> void ClearMapOfList( M & amap ) ;

  static SRSMapping * GetInstance() {
    if (instance == 0) instance = new SRSMapping() ;
    return instance;
  }

  void PrintMapping();
  void SaveMapping(const char * mappingCfgFilename);

  void LoadDefaultMapping(const char * mappingCfgFilename);
  void LoadAPVtoPadMapping(const char * mappingCfgFilename);

  void SetAPVMap(std::string detPlane, int fecId, int adcCh, int apvNo, int apvOrient, int apvIndex, int apvHdrLevel, int stripmapping);
  void SetAPVtoPadMapping(int fecId, int adcCh, int padId, int apvCh) ;

  void Set1DStripsReadoutMap(std::string readoutBoard, std::string detType, std::string detName, int detID, std::string plane,  float size, int connectors, int orient) ;
  void SetCartesianStripsReadoutMap(std::string readoutBoard, std::string detectorType, std::string detector, int detID, std::string planeX,  float sizeX, int connectorsX, int orientX, std::string planeY,  float sizeY, int connectorsY, int orientY) ;
  void SetUVStripsReadoutMap(std::string readoutBoard, std::string detType, std::string detector, int detID,  float length,  float outerR,  float innerR, std::string planeTop, int conectTop, int orientTop, std::string planeBot, int connectBot, int orientBot) ;
  void SetPadsReadoutMap(std::string readoutBoard, std::string detType, std::string detName, int detID, std::string padPlane, float sizeX,  float sizeY, float nbOfPadX, float nbOfPadY, float nbOfConnectors) ;
  void SetCMSGEMReadoutMap(std::string readoutBoard, std::string detectorType,  std::string detector, int detID, std::string EtaSector, float etaSectorPos, float etaSectorSize, float nbOfSectorConnectors, int apvOrientOnEtaSector) ;

  void Clear();

  std::map<int, int>  GetAPVNoFromIDMap()            {return fAPVNoFromIDMap ;}
  std::map<int, int>  GetAPVIDFromAPVNoMap()         {return fAPVIDFromAPVNoMap ;}
  std::map<int, int>  GetAPVGainFromIDMap()          {return fAPVGainFromIDMap ;}
  std::map<int, int>  GetAPVOrientationFromIDMap()   {return fAPVOrientationFromIDMap;}
  std::map<int, int>  GetAPVstripmappingFromIDMap()   {return fAPVstripmappingFromIDMap;}
  std::map<int, int>  GetAPVHeaderLevelFromIDMap()   {return fAPVHeaderLevelFromIDMap;}
  std::map<int, int>  GetAPVIndexOnPlaneFromIDMap()  {return fAPVIndexOnPlaneFromIDMap;}

  std::map<int, std::string>   GetAPVFromIDMap()           {return fAPVFromIDMap ;} 
  std::map<int, std::string>   GetDetectorFromIDMap()      {return fDetectorFromIDMap ;}

  std::map<std::string, float>   GetPlaneIDFromPlaneMap()      {return fPlaneIDFromPlaneMap ;}

  std::map<std::string, std::vector<float> > GetPadDetectorMap ()   {return fPadDetectorMap ;}
  std::map<std::string, std::vector<float> > GetCartesianPlaneMap() {return fCartesianPlaneMap ; }

  std::map<std::string, std::list<int> > GetAPVIDListFromDetectorMap()      {return fAPVIDListFromDetectorMap;}
  std::map<std::string, std::list<int> > GetAPVIDListFromDetectorPlaneMap() {return fAPVIDListFromDetectorPlaneMap;}

  std::list<int> GetAPVIDListFromDetector(std::string detName) {
    fAPVIDListFromDetectorMap[detName].unique() ;
    return fAPVIDListFromDetectorMap[detName];
  }

  std::list<int> GetAPVIDListFromDetectorPlane(std::string planeName) {
    fAPVIDListFromDetectorPlaneMap[planeName].unique() ;
    return fAPVIDListFromDetectorPlaneMap[planeName];
  }

  std::list<int> GetFECIDListFromDetectorPlane(std::string planeName) {
    fFECIDListFromDetectorPlaneMap[planeName].unique();
    return fFECIDListFromDetectorPlaneMap[planeName];
  }

  std::list<int> GetAPVIDListFromFECID(int fecId) {
    fAPVIDListFromFECIDMap[fecId].unique();
    return fAPVIDListFromFECIDMap[fecId];
  }

  std::list<std::string> GetDetectorPlaneListFromDetector(std::string detName) {
    fDetectorPlaneListFromDetectorMap[detName].unique();
    return  fDetectorPlaneListFromDetectorMap[detName] ;
  }

  int GetFECIDFromAPVID(int apvID)       {return (apvID >> 4 ) & 0xF;}
  int GetADCChannelFromAPVID(int apvID)  {return apvID & 0xF;}

  int GetAPVNoFromID(int apvID)          {return fAPVNoFromIDMap[apvID];}
  int GetAPVIDFromAPVNo(int apvID)       {return fAPVIDFromAPVNoMap[apvID];}
  int GetAPVIndexOnPlane(int apvID)      {return fAPVIndexOnPlaneFromIDMap[apvID];}
  int GetAPVOrientation(int apvID)       {return fAPVOrientationFromIDMap[apvID];}
  int GetAPVstripmapping(int apvID)       {return fAPVstripmappingFromIDMap[apvID];}
  int GetAPVHeaderLevelFromID(int apvID) {return fAPVHeaderLevelFromIDMap[apvID];}
  int GetAPVIDFromName(std::string apvName)    {return fAPVIDFromNameMap[apvName];}

  int GetDetectorIDFromDetector(std::string detName) {return fDetectorIDFromDetectorMap[detName];}
  std::string GetDetectorFromID(int detID)   {return fDetectorFromIDMap[detID];}

  std::string GetDetectorTypeFromDetector(std::string detectorName)   {return fDetectorTypeFromDetectorMap[detectorName];}
  std::string GetReadoutBoardFromDetector(std::string detectorName)   {return fReadoutBoardFromDetectorMap[detectorName];}
  std::string GetDetectorFromPlane(std::string planeName)             {return fDetectorFromPlaneMap[planeName];}

  std::vector< float > GetPadDetectorMap(std::string detector)     {return fPadDetectorMap[detector] ;}
  std::vector< float > Get1DStripsReadoutMap(std::string plane)    {return f1DStripsPlaneMap[plane] ;}
  std::vector< float > GetCMSGEMReadoutMap(std::string etaSector)  {return fCMSGEMDetectorMap[etaSector] ;}
  std::vector< float > GetCartesianReadoutMap(std::string plane)   {return fCartesianPlaneMap[plane] ;}
  std::vector< float > GetUVangleReadoutMap(std::string plane)     {return fUVangleReadoutMap[plane] ;}

  float GetPlaneIDorEtaSector(std::string planeName) {
    std::string readoutType = GetReadoutBoardFromDetector(GetDetectorFromPlane(planeName))  ;
    float planeIDorEtaSector=0;
    
    if(readoutType == "CARTESIAN")	planeIDorEtaSector = (fCartesianPlaneMap[planeName])[0];
    if(readoutType == "1DSTRIPS") planeIDorEtaSector = (f1DStripsPlaneMap[planeName])[0];
    if(readoutType == "CMSGEM")  {
      planeIDorEtaSector = (fCMSGEMDetectorMap[planeName])[0];
    }
    if(readoutType == "UV_ANGLE") planeIDorEtaSector = (fUVangleReadoutMap[planeName])[0] ;
    return planeIDorEtaSector ;
  }

  float GetSizeOfPlane(std::string planeName) {
    std::string readoutType = GetReadoutBoardFromDetector(GetDetectorFromPlane(planeName))  ;
    float planeSize = 0;
    if(readoutType == "CARTESIAN") planeSize = (fCartesianPlaneMap[planeName])[1] ;
    if(readoutType == "1DSTRIPS") planeSize = (f1DStripsPlaneMap[planeName])[1] ;
    if(readoutType == "CMSGEM")   planeSize = (fCMSGEMDetectorMap[planeName])[1];
    return planeSize ;
  }

  int GetNbOfAPVs(std::string planeName)  {
    std::string readoutType = GetReadoutBoardFromDetector(GetDetectorFromPlane(planeName))  ;
    std::string detectorType = GetDetectorTypeFromDetector(GetDetectorFromPlane(planeName))  ;
    int nbOfAPVs=0;
    if(readoutType == "CARTESIAN") nbOfAPVs = (int) (fCartesianPlaneMap[planeName])[2];
    if(readoutType == "1DSTRIPS") nbOfAPVs = (int) (f1DStripsPlaneMap[planeName])[2];
    if(readoutType == "CMSGEM")   nbOfAPVs = (int) (fCMSGEMDetectorMap[planeName])[2];
    if ((readoutType == "UV_ANGLE") &&  (detectorType == "EICPROTO1"))  nbOfAPVs =  (int) (((fUVangleReadoutMap[planeName])[2]) /2 ); ;
    return nbOfAPVs ;
  } 

  int GetPlaneOrientation(std::string planeName)  {
    std::string readoutType = GetReadoutBoardFromDetector(GetDetectorFromPlane(planeName))  ;
    int orient=-1;
    if(readoutType == "CARTESIAN") orient = (int) (fCartesianPlaneMap[planeName])[3];
    if(readoutType == "1DSTRIPS") orient = (int) (f1DStripsPlaneMap[planeName])[3];
    if(readoutType == "CMSGEM")   orient = (int) (fCMSGEMDetectorMap[planeName])[3];
    if(readoutType == "UV_ANGLE") orient = (int) (fUVangleReadoutMap[planeName])[2];
    return orient ;
  } 
 
  std::string GetDetectorPlaneFromAPVID(int apvID) {return fDetectorPlaneFromAPVIDMap[apvID];}

  std::string GetAPVFromID(int apvID) {return fAPVFromIDMap[apvID];}
  std::string GetAPV(std::string detPlane, int fecId, int adcChannel, int apvNo, int apvIndex, int apvID) ;

  std::vector<int> GetPadChannelsMapping(int apvID) {return fAPVToPadChannelMap[apvID]; }
  std::map< int, std::vector<int> > GetPadChannelsMapping() {return fAPVToPadChannelMap;}

  int GetNbOfAPVs() {return fAPVNoFromIDMap.size();}
  int GetNbOfFECs() {return fAPVIDListFromFECIDMap.size();}
  int GetNbOfDetectorPlane() {return fDetectorFromPlaneMap.size();}
  int GetNbOfDetectors() {return fReadoutBoardFromDetectorMap.size();}

  /**
  std::vector<int> GetListOfAPVIDs(){
   std::vector<int> list;
   std::map<int, int>::const_iterator itr;
   for (itr = fAPVNoFromIDMap.begin(); itr != fAPVNoFromIDMap.end(); itr++){
      list.push_back((*itr).first);
   }
   return list;
  }
  */

  /**
  std::vector<int> GetListOfDetectors(){
   std::vector<int> list;
   std::map<int, int>::const_iterator itr;
   for (itr = fDetectorFromIDMap.begin(); itr != fDetectorFromIDMap.end(); itr++){
      list.push_back((*itr).first);
   }
   return list;
  }
  */


  bool IsAPVIDMapped(int apvID) {
    std::map<int, std::string>::iterator itr ;
    itr = fAPVFromIDMap.find(apvID) ;
    if(itr != fAPVFromIDMap.end())
      return true ;
    else
      return false ;
  }

private:

  SRSMapping() {fNbOfAPVs = 0;}
  std::list<std::string> tokenize(const std::string& line, const char * p);
  float atof(const std::string& s);
  int   atoi(const std::string& s);

  static SRSMapping * instance;
  int fNbOfAPVs ;

  std::map<int, int>   fAPVHeaderLevelFromIDMap;
  std::map<int, int>   fAPVNoFromIDMap, fAPVIDFromAPVNoMap, fAPVIndexOnPlaneFromIDMap,fAPVOrientationFromIDMap, fAPVstripmappingFromIDMap;
  std::map<std::string, int> fNbOfAPVsFromDetectorMap ;          

  std::map<int, int>      fAPVGainFromIDMap ; 
  std::map<std::string, int>    fAPVIDFromNameMap ; 
  std::map<int, std::string>    fAPVFromIDMap ;  

  std::map<std::string, float>  fPlaneIDFromPlaneMap ;
  std::map<int, std::string>    fDetectorPlaneFromAPVIDMap;

  std::map<int, std::string>    fReadoutBoardFromIDMap ;  

  std::map<std::string, std::string>  fDetectorTypeFromDetectorMap ;
  std::map<std::string, std::string>  fReadoutBoardFromDetectorMap ;
  std::map<std::string, int>    fDetectorIDFromDetectorMap ;  
  std::map<int, std::string>    fDetectorFromIDMap ;   
  std::map<int, std::string>    fDetectorFromAPVIDMap; 
  std::map<std::string, std::string > fDetectorFromPlaneMap ;

  std::map<int, std::list<int> >   fAPVIDListFromFECIDMap;       

  std::map<std::string, std::list<int> > fFECIDListFromDetectorPlaneMap; 
  std::map<std::string, std::list<int> > fAPVIDListFromDetectorMap;     
  std::map<std::string, std::list<int> > fAPVIDListFromDetectorPlaneMap;

  std::map<std::string, std::list<std::string> > fDetectorListFromDetectorTypeMap ;
  std::map<std::string, std::list<std::string> > fDetectorListFromReadoutBoardMap;
  std::map<std::string, std::list<std::string> > fDetectorPlaneListFromDetectorMap ;

  std::map<int, std::vector<int> > fAPVToPadChannelMap; 

  std::map<std::string, std::vector<float> > fPadDetectorMap ;
  std::map<std::string, std::vector<float> > fUVangleReadoutMap;
  std::map<std::string, std::vector<float> > f1DStripsPlaneMap ;
  std::map<std::string, std::vector<float> > fCartesianPlaneMap ;
  std::map<std::string, std::vector<float> > fCMSGEMDetectorMap ;

};

template <typename M> void SRSMapping::ClearMapOfList( M & amap ) {
    for (auto iterList = amap.begin(); iterList != amap.end(); ++iterList ) {
        ((*iterList).second).clear();
    }
    amap.clear() ;

    return;
}

#endif
