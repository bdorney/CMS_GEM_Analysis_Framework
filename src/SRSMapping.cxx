#include "SRSMapping.h"

SRSMapping * SRSMapping::instance = 0 ;

//======================================================================================================================================
/**
 void SRSMapping::Set2DStripsDetectorMap(std::string readoutBoard, std::string detectorType, std::string detector, int detID, std::string planeX,  Float_t sizeX, int connectorsX, int orientX, std::string planeY,  Float_t sizeY, int connectorsY, int orientY) {
 
 printf("   SRSMapping::SetDetectorMap() => readout=%s, detType=%s, det=%s, detID=%d, planeX=%s, SizeX=%f, connectorsX=%d, orientationX=%d, planeY=%s, SizeY=%f, connectorsY=%d, orientationY=%d \n", readoutBoard.c_str(), detectorType.c_str(), detector.c_str(), detID, planeX.c_str(), sizeX, connectorsX, orientX, planeY.c_str(), sizeY, connectorsY, orientY) ;
 
 fDetectorFromIDMap[detID]     = detector ;
 fReadoutBoardFromIDMap[detID] = readoutBoard ;
 
 fDetectorIDFromDetectorMap[detector] = detID ;
 fReadoutBoardFromDetectorMap[detector] = readoutBoard ;
 fDetectorTypeFromDetectorMap[detector] = detectorType ;
 
 fDetectorListFromDetectorTypeMap[detectorType].push_back(detector) ;
 fDetectorListFromReadoutBoardMap[readoutBoard].push_back(detector) ;
 
 fPlaneIDFromPlaneMap[planeX] = 0 ;
 fPlaneIDFromPlaneMap[planeY] = 1 ;
 
 fDetectorFromPlaneMap[planeX] = detector ;
 fDetectorFromPlaneMap[planeY] = detector ;
 
 fDetectorPlaneListFromDetectorMap[detector].push_back(planeX) ;
 fDetectorPlaneListFromDetectorMap[detector].push_back(planeY) ;
 
 fCartesianPlaneMap[planeX].push_back(0) ;
 fCartesianPlaneMap[planeX].push_back(sizeX) ;
 fCartesianPlaneMap[planeX].push_back(connectorsX) ;
 fCartesianPlaneMap[planeX].push_back(orientX) ;
 fCartesianPlaneMap[planeY].push_back(1) ;
 fCartesianPlaneMap[planeY].push_back(sizeY) ;
 fCartesianPlaneMap[planeY].push_back(connectorsY) ;
 fCartesianPlaneMap[planeY].push_back(orientY) ;
 }
 */

//======================================================================================================================================
void SRSMapping::SetCartesianStripsReadoutMap(std::string readoutBoard, std::string detectorType, std::string detector, int detID, std::string planeX,  float sizeX, int connectorsX, int orientX, std::string planeY,  float sizeY, int connectorsY, int orientY) {
    
    //printf("   SRSMapping::SetDetectorMap() => readout=%s, detType=%s, det=%s, detID=%d, planeX=%s, SizeX=%f, connectorsX=%d, orientationX=%d, planeY=%s, SizeY=%f, connectorsY=%d\n", readoutBoard.c_str(), detectorType.c_str(), detector.c_str(), detID, planeX.c_str(), sizeX, connectorsX, orientX, planeY.c_str(), sizeY, connectorsY, orientY) ;
    
    fDetectorFromIDMap[detID]     = detector ;
    fReadoutBoardFromIDMap[detID] = readoutBoard ;
    
    fDetectorIDFromDetectorMap[detector] = detID ;
    fReadoutBoardFromDetectorMap[detector] = readoutBoard ;
    fDetectorTypeFromDetectorMap[detector] = detectorType ;
    
    fDetectorListFromDetectorTypeMap[detectorType].push_back(detector) ;
    fDetectorListFromReadoutBoardMap[readoutBoard].push_back(detector) ;
    
    fPlaneIDFromPlaneMap[planeX] = 0 ;
    fPlaneIDFromPlaneMap[planeY] = 1 ;
    
    fDetectorFromPlaneMap[planeX] = detector ;
    fDetectorFromPlaneMap[planeY] = detector ;
    
    fDetectorPlaneListFromDetectorMap[detector].push_back(planeX) ;
    fDetectorPlaneListFromDetectorMap[detector].push_back(planeY) ;
    
    fCartesianPlaneMap[planeX].push_back(0) ;
    fCartesianPlaneMap[planeX].push_back(sizeX) ;
    fCartesianPlaneMap[planeX].push_back(connectorsX) ;
    fCartesianPlaneMap[planeX].push_back(orientX) ;
    fCartesianPlaneMap[planeY].push_back(1) ;
    fCartesianPlaneMap[planeY].push_back(sizeY) ;
    fCartesianPlaneMap[planeY].push_back(connectorsY) ;
    fCartesianPlaneMap[planeY].push_back(orientY) ;
}

//======================================================================================================================================
void SRSMapping::SetUVStripsReadoutMap(std::string readoutBoard, std::string detectorType, std::string detector, int detID,  float length,  float innerR,  float outerR, std::string planeTop, int conectTop, int orientTop, std::string planeBot, int connectBot, int orientBot) {
    
    //  printf("   SRSMapping::SetDetectorMap() => readout=%s, detType=%s, det=%s, detID=%d, planeX=%s, SizeX=%f, connectorsX=%d, orientationX=%d, planeY=%s, SizeY=%f, connectorsY=%d, orientationY=%d \n", readoutBoard.c_str(), detectorType.c_str(), detector.c_str(), detID, planeX.c_str(), sizeX, connectorsX, orientX, planeY.c_str(), sizeY, connectorsY, orientY) ;
    
    fDetectorFromIDMap[detID]     = detector ;
    fReadoutBoardFromIDMap[detID] = readoutBoard ;
    
    fDetectorIDFromDetectorMap[detector] = detID ;
    fReadoutBoardFromDetectorMap[detector] = readoutBoard ;
    fDetectorTypeFromDetectorMap[detector] = detectorType ;
    
    fDetectorListFromDetectorTypeMap[detectorType].push_back(detector) ;
    fDetectorListFromReadoutBoardMap[readoutBoard].push_back(detector) ;
    
    fPlaneIDFromPlaneMap[planeTop] = 0 ;
    fPlaneIDFromPlaneMap[planeBot] = 1 ;
    
    fDetectorFromPlaneMap[planeTop] = detector ;
    fDetectorFromPlaneMap[planeBot] = detector ;
    
    fDetectorPlaneListFromDetectorMap[detector].push_back(planeTop) ;
    fDetectorPlaneListFromDetectorMap[detector].push_back(planeBot) ;
    
    fUVangleReadoutMap[detector].push_back(length) ;
    fUVangleReadoutMap[detector].push_back(innerR) ;
    fUVangleReadoutMap[detector].push_back(outerR) ;
    
    fUVangleReadoutMap[planeTop].push_back(0) ;
    fUVangleReadoutMap[planeTop].push_back(conectTop) ;
    fUVangleReadoutMap[planeTop].push_back(orientTop) ;
    fUVangleReadoutMap[planeBot].push_back(1) ;
    fUVangleReadoutMap[planeBot].push_back(connectBot) ;
    fUVangleReadoutMap[planeBot].push_back(orientBot) ;
    
}

//======================================================================================================================================
void SRSMapping::Set1DStripsReadoutMap(std::string readoutBoard, std::string detectorType, std::string detector, int detID, std::string plane,  float size, int connectors, int orient) {
    
    printf("   SRSMapping::SetDetectorMap() => readout=%s, detType=%s, det=%s, detID=%d, plane=%s, Size=%f, connectors=%d, orientation=%d \n", readoutBoard.c_str(), detectorType.c_str(), detector.c_str(), detID, plane.c_str(), size, connectors, orient) ;
    
    fDetectorFromIDMap[detID]     = detector ;
    fReadoutBoardFromIDMap[detID] = readoutBoard ;
    
    fDetectorIDFromDetectorMap[detector] = detID ;
    fReadoutBoardFromDetectorMap[detector] = readoutBoard ;
    fDetectorTypeFromDetectorMap[detector] = detectorType ;
    
    fDetectorListFromDetectorTypeMap[detectorType].push_back(detector) ;
    fDetectorListFromReadoutBoardMap[readoutBoard].push_back(detector) ;
    
    fPlaneIDFromPlaneMap[plane] = 0 ;
    fDetectorFromPlaneMap[plane] = detector ;
    fDetectorPlaneListFromDetectorMap[detector].push_back(plane) ;
    
    f1DStripsPlaneMap[plane].push_back(0) ;
    f1DStripsPlaneMap[plane].push_back(size) ;
    f1DStripsPlaneMap[plane].push_back(connectors) ;
    f1DStripsPlaneMap[plane].push_back(orient) ;
}

//======================================================================================================================================
void SRSMapping::SetCMSGEMReadoutMap(std::string readoutBoard, std::string detectorType,  std::string detector, int detID, std::string EtaSector,  float etaSectorPos, float etaSectorSize, float nbOfSectorConnectors, int apvOrientOnEtaSector) {
    printf("   SRSMapping::SetDetectorMap() =>readout=%s, detType=%s, det=%s, detID=%d, EtaSector=%s, etaSectorSize=%f, nbOSectorfConnectors=%f, apvOrientOnEtaSector=%d\n", readoutBoard.c_str(), detectorType.c_str(), detector.c_str(), detID, EtaSector.c_str(), etaSectorSize, nbOfSectorConnectors, apvOrientOnEtaSector) ;
    
    fDetectorFromIDMap[detID]     = detector ;
    fReadoutBoardFromIDMap[detID] = readoutBoard ;
    
    fDetectorIDFromDetectorMap[detector] = detID ;
    fReadoutBoardFromDetectorMap[detector] = readoutBoard ;
    fDetectorTypeFromDetectorMap[detector] = detectorType ;
    fDetectorListFromDetectorTypeMap[detectorType].push_back(detector) ;
    fDetectorListFromReadoutBoardMap[readoutBoard].push_back(detector) ;
    
    fDetectorFromPlaneMap[EtaSector] = detector ;
    fDetectorPlaneListFromDetectorMap[detector].push_back(EtaSector) ;
    
    fCMSGEMDetectorMap[EtaSector].push_back(etaSectorPos) ;
    fCMSGEMDetectorMap[EtaSector].push_back(etaSectorSize) ;
    fCMSGEMDetectorMap[EtaSector].push_back(nbOfSectorConnectors) ;
    fCMSGEMDetectorMap[EtaSector].push_back(apvOrientOnEtaSector) ;
}

//======================================================================================================================================
void SRSMapping::SetPadsReadoutMap(std::string readoutBoard, std::string detectorType,  std::string detector, int detID, std::string padPlane, float padSizeX,  float padSizeY, float nbOfPadX, float nbOfPadY, float nbOfConnectors) {
    printf("   SRSMapping::SetDetectorMap() =>readout=%s, detType=%s, det=%s, detID=%d, padPlane=%s, nbOfPadX=%f, padSizeX=%f, nbOfPadY=%f, padSizeY=%f, nbOfConnectors=%f \n", readoutBoard.c_str(), detectorType.c_str(), detector.c_str(), detID, padPlane.c_str(), nbOfPadX, padSizeX, nbOfPadY, padSizeY, nbOfConnectors) ;
    
    fDetectorFromIDMap[detID]     = detector ;
    fReadoutBoardFromIDMap[detID] = readoutBoard ;
    
    fDetectorIDFromDetectorMap[detector] = detID ;
    fReadoutBoardFromDetectorMap[detector] = readoutBoard ;
    fDetectorTypeFromDetectorMap[detector] = detectorType ;
    fDetectorListFromDetectorTypeMap[detectorType].push_back(detector) ;
    fDetectorListFromReadoutBoardMap[readoutBoard].push_back(detector) ;
    
    fDetectorFromPlaneMap[padPlane] = detector ;
    fDetectorPlaneListFromDetectorMap[detector].push_back(padPlane) ;
    fPlaneIDFromPlaneMap[padPlane]  = 0 ;
    
    fPadDetectorMap[detector].push_back(padSizeX) ;
    fPadDetectorMap[detector].push_back(padSizeY) ;
    fPadDetectorMap[detector].push_back(nbOfPadX) ;
    fPadDetectorMap[detector].push_back(nbOfPadY) ;
    fPadDetectorMap[detector].push_back(nbOfConnectors) ;
}

//=========================================================================================================================
std::string SRSMapping::GetAPV(std::string detPlane, int fecId, int adcCh, int apvNo, int apvIndex, int apvID) {
    std::stringstream out ;
    out << apvID ;
    std::string apvIDStr = out.str();
    out.str("") ;
    out << fecId ;
    std::string fecIDStr = out.str();
    out.str("") ;
    out <<  adcCh;
    std::string adcChStr = out.str();
    out.str("") ;
    out <<  apvNo ;
    std::string apvNoStr = out.str();
    out.str("") ;
    out <<  apvIndex ;
    std::string apvIndexStr = out.str();
    out.str("") ;
    std::string apvName = "apv" + apvNoStr + "_Id" + apvIDStr + "_" + detPlane + "_adcCh" + adcChStr + "_FecId" + fecIDStr ;
    return apvName ;
}

//======================================================================================================================================
void  SRSMapping::SetAPVMap(std::string detPlane, int fecId, int adcCh, int apvNo, int apvOrient, int apvIndex, int apvHdr, int stripmapping) {
    
    int apvID = (fecId << 4) | adcCh ;
    
    std::string apvName = this->GetAPV(detPlane, fecId, adcCh, apvNo, apvIndex, apvID) ;
    
    fAPVNoFromIDMap[apvID]           = apvNo ;
    fAPVIDFromAPVNoMap[apvNo]        = apvID ;
    fAPVFromIDMap[apvID]             = apvName ;
    fAPVHeaderLevelFromIDMap[apvID]  = apvHdr ;
    fAPVOrientationFromIDMap[apvID]  = apvOrient ;
    fAPVstripmappingFromIDMap[apvID]  = stripmapping ;
    
    fAPVIndexOnPlaneFromIDMap[apvID] = apvIndex ;
    
    fAPVIDFromNameMap[apvName]       = apvID ;
    fDetectorPlaneFromAPVIDMap[apvID] = detPlane ;
    
    fAPVIDListFromFECIDMap[fecId].push_back(apvID);
    fFECIDListFromDetectorPlaneMap[detPlane].push_back(fecId);
    fAPVIDListFromDetectorPlaneMap[detPlane].push_back(apvID);
    
    std::string detector = GetDetectorFromPlane(detPlane) ;
    fAPVIDListFromDetectorMap[detector].push_back(apvID);
}

//======================================================================================================================================
void  SRSMapping::SetAPVtoPadMapping(int fecId, int adcCh, int padId, int apvCh) {
    int apvID      = (fecId << 4) | adcCh ;
    int apvChPadCh = (padId << 8) | apvCh ;
    fAPVToPadChannelMap[apvID].push_back(apvChPadCh) ;
}

//======================================================================================================================================
void SRSMapping::PrintMapping() {
    
  std::map<std::string, std::list<std::string> >::const_iterator det_itr ;
    for(det_itr = fDetectorPlaneListFromDetectorMap.begin(); det_itr != fDetectorPlaneListFromDetectorMap.end(); ++det_itr) {
        std::string detector = det_itr->first ;
        printf("  ======================================================================================================================\n") ;
        printf("  SRSMapping::PrintMapping() ==> Detector = %s \n",detector.c_str()) ;
        
        std::list<std::string> detPlaneList = det_itr->second ;
        std::list<std::string>::const_iterator plane_itr ;
        for(plane_itr = detPlaneList.begin(); plane_itr != detPlaneList.end(); ++plane_itr) {
            std::string detPlane = * plane_itr ;
            std::list<int> fecList = GetFECIDListFromDetectorPlane(detPlane) ;
            std::list<int>::const_iterator fec_itr ;
            for(fec_itr = fecList.begin(); fec_itr != fecList.end(); ++fec_itr) {
                int fecId = * fec_itr ;
                printf("  SRSMapping::PrintMapping()     Plane = %s,        FEC = %d \n", detPlane.c_str(), fecId) ;
                
                std::list<int> apvList = GetAPVIDListFromDetectorPlane(detPlane) ;
                std::list<int>::const_iterator apv_itr ;
                for(apv_itr = apvList.begin(); apv_itr != apvList.end(); ++apv_itr) {
                    int apvID       = * apv_itr ;
                    int apvNo       = this->GetAPVNoFromID(apvID);
                    int apvIndex    = this->GetAPVIndexOnPlane(apvID);
                    int apvOrient   = this->GetAPVOrientation(apvID);
                    int stripmapping = this->GetAPVstripmapping(apvID);
                    int fecID       = this->GetFECIDFromAPVID(apvID);
                    int adcCh       = this->GetADCChannelFromAPVID(apvID);
                    int apvHdrLevel = this->GetAPVHeaderLevelFromID(apvID);
                    std::string  apvName  = GetAPVFromID(apvID) ;
                    if(fecID == fecId) printf("  SRSMapping::PrintMapping() ==> adcCh=%d,  apvName=%s,  apvID=%d, apvNo=%d,  apvIndex=%d,  apvOrientation=%d,  apvHdr=%d \n", adcCh, apvName.c_str(), apvID, apvNo, apvIndex, apvOrient, apvHdrLevel) ;
                }
            }
            printf("\n") ;
        }
    }
    printf("======================================================================================================================\n") ;
    printf("  SRSMapping::PrintMapping() ==> Mapping of %d detectors, %d planes, %d FECs, %d APVs\n", this->GetNbOfDetectors(), this->GetNbOfDetectorPlane(), this->GetNbOfFECs(), this->GetNbOfAPVs());
    printf("======================================================================================================================\n") ;
}

//======================================================================================================================================
void SRSMapping::SaveMapping(const char * file) {
    printf("  SRSMapping::SaveMapping() ==> Saving SRS Mapping to file [%s],\n", file) ;
    FILE * f = fopen(file, "w");
    
    fprintf(f,"#################################################################################################\n") ;
    fprintf(f,"         readoutType  Detector    Plane  DetNo   Plane   size (mm)  connectors  orientation\n");
    fprintf(f,"#################################################################################################\n") ;
    
    std::map<std::string, std::list<std::string> >::const_iterator det_itr ;
    for(det_itr = fDetectorPlaneListFromDetectorMap.begin(); det_itr != fDetectorPlaneListFromDetectorMap.end(); ++det_itr) {
        std::string detector      = det_itr->first ;
        std::string readoutBoard = this->GetReadoutBoardFromDetector(detector) ;
        std::string detectorType = this->GetDetectorTypeFromDetector(detector) ;
        
        if ( (readoutBoard == "CARTESIAN") ||  (readoutBoard == "UV_ANGLE_OLD") ){
            std::list<std::string> detPlaneList = det_itr->second ;
            std::string planeX    = detPlaneList.front() ;
            std::vector <float> cartesianPlaneX = this->GetCartesianReadoutMap(planeX) ;
            float sizeX     = cartesianPlaneX[1] ;
            int connectorsX = (int) (cartesianPlaneX[2]) ;
            int orientX     = (int) (cartesianPlaneX[3]) ;
            
            std::string planeY    = detPlaneList.back() ;
            std::vector <float> cartesianPlaneY = GetCartesianReadoutMap(planeY) ;
            float sizeY     = cartesianPlaneY[1] ;
            int connectorsY = (int) (cartesianPlaneY[2]) ;
            int orientY     = (int) (cartesianPlaneY[3]) ;
            fprintf(f,"DET,  %s,   %s,   %s,   %s,  %f,   %d,   %d,   %s,   %f,   %d,   %d \n", readoutBoard.c_str(), detectorType.c_str(), detector.c_str(), planeX.c_str(), sizeX, connectorsX, orientX, planeY.c_str(), sizeY, connectorsY, orientY ) ;
        }
        else if (readoutBoard == "UV_ANGLE") {
            std::list<std::string> detPlaneList = det_itr->second ;
            std::string planeX    = detPlaneList.front() ;
            std::vector <float> cartesianPlaneX = GetCartesianReadoutMap(planeX) ;
            float sizeX     = cartesianPlaneX[1] ;
            int connectorsX = (int) (cartesianPlaneX[2]) ;
            int orientX     = (int) (cartesianPlaneX[3]) ;
            
            std::string planeY    = detPlaneList.back() ;
            std::vector <float> cartesianPlaneY = GetCartesianReadoutMap(planeY) ;
            float sizeY     = cartesianPlaneY[1] ;
            int connectorsY = (int) (cartesianPlaneY[2]) ;
            int orientY     = (int) (cartesianPlaneY[3]) ;
            fprintf(f,"DET,  %s,   %s,   %s,   %s,  %f,   %d,   %d,   %s,   %f,   %d,   %d \n", readoutBoard.c_str(), detectorType.c_str(), detector.c_str(), planeX.c_str(), sizeX, connectorsX, orientX, planeY.c_str(), sizeY, connectorsY, orientY ) ;
        }
        
        else if (readoutBoard == "PADPLANE") {
            std::list<std::string> detPlaneList = det_itr->second ;
            std::string padPlane = detPlaneList.back() ;
            float padSizeX = GetPadDetectorMap(detector) [0];
            float padSizeY = GetPadDetectorMap(detector) [1];
            float nbOfPadX = GetPadDetectorMap(detector) [2];
            float nbOfPadY = GetPadDetectorMap(detector) [3];
            float nbOfCons = GetPadDetectorMap(detector) [4];
            fprintf(f,"DET,  %s,   %s,   %s,  %s,  %f,   %f,   %f,   %f,  %f\n", readoutBoard.c_str(), detectorType.c_str(), detector.c_str(), padPlane.c_str(), padSizeX, nbOfPadX,  padSizeY, nbOfPadY, nbOfCons) ;
        }
        
        else if (readoutBoard == "CMSGEM") {
            std::list<std::string> detPlaneList = det_itr->second ;
        }
        
        else {
            printf("  SRSMapping::SaveMapping() ==> detector readout board type %s is not yet implemented ==> PLEASE MOVE ON \n", readoutBoard.c_str()) ;
            continue ;
        }
    }
    
    fprintf(f,"###############################################################\n") ;
    fprintf(f,"#     fecId   adcCh   detPlane  apvOrient  apvIndex    apvHdr #\n");
    fprintf(f,"###############################################################\n") ;
    std::map<int, std::string>::const_iterator apv_itr;
    for(apv_itr = fAPVFromIDMap.begin(); apv_itr != fAPVFromIDMap.end(); ++apv_itr){
        int apvID      = apv_itr->first;
        int fecId      = GetFECIDFromAPVID(apvID);
        int adcCh      = GetADCChannelFromAPVID(apvID);
        std::string detPlane = GetDetectorPlaneFromAPVID(apvID) ;
        int apvOrient  = GetAPVOrientation(apvID);
        int stripmapping   = GetAPVstripmapping(apvID);
        int apvIndex   = GetAPVIndexOnPlane(apvID);
        int apvHdr     = GetAPVHeaderLevelFromID(apvID);
        fprintf(f,"APV,   %d,     %d,     %s,     %d,    %d,   %d\n", fecId, adcCh, detPlane.c_str(), apvOrient, apvIndex, apvHdr);
    }
    fclose(f);
}

//============================================================================================
void SRSMapping::LoadDefaultMapping(const char * mappingCfgFilename) {
    
  this->Clear() ;
    printf("  SRSMapping::LoadDefaultMapping() ==> Loading Mapping from %s \n", mappingCfgFilename) ;
    int apvNo = 0 ;
    int detID = 0 ;
    
    std::ifstream file(mappingCfgFilename, std::ifstream::in);
    char raw[1000];
    std::string line="";
    do {
      file.getline(raw,1000);
      line = std::string(raw);
      if (line.find("#") < 10) continue;      
      
      //=== Create an array of the tokens separated by "," in the line;
      std::list<std::string> tokens = this->tokenize(line,",");

        //=== iterator on the tokens array

      for (std::list<std::string>::iterator i=tokens.begin();i!=tokens.end();i++){      

	std::string s=*i;
	//      printf("    SRSMapping::LoadDefaultMapping() ==> Data ==> %s\n",s.c_str()) ;	
	if(s == "DET") {
	  
	  std::string readoutBoard = *(++i);
	  std::string detectorType = *(++i);
	  std::string detector     = *(++i);
	  if (readoutBoard == "CARTESIAN")  {
	    std::string planeX     = *(++i);
	    float sizeX            = this->atof(*(++i));
	    int   nbOfConnectorsX  = this->atoi(*(++i));
	    int   orientationX     = this->atoi(*(++i));
	    std::string planeY     = *(++i);
	    float sizeY            = this->atof(*(++i));
	    int   nbOfConnectorsY  = this->atoi(*(++i));;
	    int   orientationY      =this->atoi(*(++i));
	    this->SetCartesianStripsReadoutMap(readoutBoard, detectorType, detector, detID, planeX, sizeX, nbOfConnectorsX, orientationX, planeY, sizeY, nbOfConnectorsY, orientationY) ;
	  }
                
	  else if (readoutBoard == "1DSTRIPS")  {
	    std::string plane           = *(++i);
	    float size            = 0;
	    int   nbOfConnectors  = 0;
	    int   orientation     = 0;
	    this->Set1DStripsReadoutMap(readoutBoard, detectorType, detector, detID, plane, size, nbOfConnectors, orientation);
	  }                
	  else if (readoutBoard == "UV_ANGLE_OLD") {
	    std::string planeX           = *(++i);
	    float sizeX            = 0.;
	    int   nbOfConnectorsX  = 0;
	    int   orientationX  =0;
	    std::string planeY       = *(++i);
	    float sizeY            = 0;
	    int   nbOfConnectorsY  = 0;
	    int   orientationY  =0;
	    this->SetCartesianStripsReadoutMap(readoutBoard, detectorType, detector, detID, planeX, sizeX, nbOfConnectorsX, orientationX, planeY, sizeY, nbOfConnectorsY, orientationY) ;
	  }
                
	  else if (readoutBoard == "UV_ANGLE") {
	    float length           = 0;
	    float outerRadius      = 0;
	    float innerRadius      = 0;
	    std::string planeTop           = *(++i);
	    int   nbOfConnectorsTop  = 0;
	    int   orientationTop     = 0;
	    std::string planeBot           = *(++i);
	    int   nbOfConnectorsBot  = 0;
	    int   orientationBot     = 0;
	    this->SetUVStripsReadoutMap(readoutBoard, detectorType, detector, detID, length, innerRadius, outerRadius, planeTop, nbOfConnectorsTop, orientationTop, planeBot, nbOfConnectorsBot, orientationBot) ;
	  }
                
	  else if (readoutBoard == "PADPLANE") {
	    std::string padPlane     = *(++i);
	    float padSizeX     = 0;
	    float nbPadX       = 0;
	    float padSizeY     = 0;
	    float nbPadY       = 0;
	    float nbConnectors = 0;
	    this->SetPadsReadoutMap(readoutBoard, detectorType, detector, detID, padPlane, padSizeX, padSizeY, nbPadX, nbPadY, nbConnectors) ;
	  }
                
	  else if (readoutBoard == "CMSGEM") {
	    std::string etaSector     = *(++i);
	    float etaSectorPos        = this->atof(*(++i));
	    float etaSectorSize       = this->atof(*(++i));
	    float nbConnectors        = this->atof(*(++i));
	    int orientation           = this->atoi(*(++i));
	    std::cout <<" Readout Mapping for CMSGEM : "<<detID<<std::endl;
	    this->SetCMSGEMReadoutMap(readoutBoard, detectorType, detector, detID, etaSector, etaSectorPos, etaSectorSize, nbConnectors, orientation) ;
	  }
	  
	  else {
	    printf("XXXXXXX SRSMapping::LoadDefaultMapping()==> detector with this readout board type %s is not yet implemented ==> PLEASE MOVE ON XXXXXXXXXXX \n", readoutBoard.c_str()) ;
	    continue ;
	  }
	  detID++ ;
	}
        
	else if(s == "APV") {
	  int   fecId     = this->atoi(*(++i));
	  int   adcCh     = this->atoi(*(++i));
	  std::string detPlane  = *(++i);
	  int   apvOrient = this->atoi(*(++i));
	  int   apvIndex  = this->atoi(*(++i));
	  int   apvheader = this->atoi(*(++i));
	  int   stripmapping = this->atoi(*(++i));
	  if (detPlane == "NULL") continue ;	  
	  this->SetAPVMap(detPlane, fecId, adcCh, apvNo, apvOrient, apvIndex, apvheader, stripmapping) ;
	  apvNo++ ;
	}
      }
    }while(!file.eof());
    printf("\n  ======================================================================================================================\n") ;
}

//============================================================================================
void SRSMapping::LoadAPVtoPadMapping(const char * mappingCfgFilename) {
    //  Clear() ;
  /**
    printf("  SRSMapping::LoadAPVtoPadMapping() ==> Loading Mapping from %s \n", mappingCfgFilename) ;
    std::ifstream filestream (mappingCfgFilename, std::ifstream::in); 
    std::string line;
    while (line.ReadLine(filestream)) {
        
        line.Remove(std::string::kLeading, ' ');   // strip leading spaces
        if (line.BeginsWith("#")) continue ;   // and skip comments
        //    printf("   SRSMapping::LoadAPVtoPadMapping ==> Scanning the mapping cfg file %s\n",line.c_str()) ;
        
        //=== Create an array of the tokens separated by "," in the line;
        TObjArray * tokens = line.Tokenize(","); 
        
        //=== iterator on the tokens array
        TIter myiter(tokens); 
        while (TObjString * st = (TObjString*) myiter.Next()) {
            
            //== Remove leading and trailer spaces
            std::string s = st->GetString().Remove(std::string::kLeading, ' ' );
            s.Remove(std::string::kTrailing, ' ' );                         
            if(s == "PAD") {
                int apvCh      = (((TObjString*) myiter.Next())->GetString().Remove(std::string::kLeading, ' ' )).Atoi();
                int padId      = (((TObjString*) myiter.Next())->GetString().Remove(std::string::kLeading, ' ' )) .Atoi();;
                int fecId      = (((TObjString*) myiter.Next())->GetString().Remove(std::string::kLeading, ' ' )).Atoi();
                int adcCh      = (((TObjString*) myiter.Next())->GetString().Remove(std::string::kLeading, ' ' )).Atoi();
                SetAPVtoPadMapping(fecId, adcCh, padId, apvCh) ;
            }
        }
        tokens->Delete();
    }
    printf("\n  ======================================================================================================================\n") ;
  **/
}

//============================================================================================
void SRSMapping::Clear() {
    printf("  SRSMapping::Clear() ==> Clearing Previous Mapping \n") ;
    
    fAPVIDFromAPVNoMap.clear() ; 
    fAPVIDFromNameMap.clear() ; 
    fAPVIDListFromDetectorMap.clear() ; 
    fAPVIDListFromDetectorPlaneMap.clear() ; 
    fAPVNoFromIDMap.clear() ; 
    fAPVFromIDMap.clear() ; 
    fAPVHeaderLevelFromIDMap.clear() ;
    
    fPlaneIDFromPlaneMap.clear() ;
    fDetectorIDFromDetectorMap.clear() ; 
    fDetectorFromIDMap.clear() ; 
    fDetectorFromAPVIDMap.clear() ;  
    fDetectorFromPlaneMap.clear() ;
    
    fDetectorPlaneFromAPVIDMap.clear() ;
    
    fReadoutBoardFromIDMap.clear() ;
    fReadoutBoardFromDetectorMap.clear() ;
    
    fNbOfAPVsFromDetectorMap.clear() ;
    
    fAPVOrientationFromIDMap.clear() ;
    fAPVstripmappingFromIDMap.clear() ;
    
    fAPVIndexOnPlaneFromIDMap.clear() ;
    
    printf("  SRSMapping::Clear() ==> Previous Mapping cleared \n") ;
}

//============================================================================================
template <typename M> void ClearMapOfList( M & amap ) {
    for ( typename M::iterator it = amap.begin(); it != amap.end(); ++it ) {
        ((*it).second).clear();
    }
    amap.clear() ;
}

std::list<std::string> 
SRSMapping::tokenize(const std::string& line, const char * p){
  std::list<std::string> pzs;
  std::string pz="";
  std::string buf=line;
  do{
    int s=buf.find(p);
    std::string cz;
    if (s == buf.npos) {
      cz = buf;
      buf="";
    } else {
      cz = buf.substr(0,s);
      buf=buf.substr(s+1,buf.npos);
    }
    std::stringstream os;
    os << cz;
    os >> pz;
    pzs.push_back(pz);
  }while(buf.size()>0);
  return pzs;
}

float SRSMapping::atof(const std::string& s){
  float f;
  std::stringstream os;
  os<<s;
  os>>f;
  return f;
}
int SRSMapping::atoi(const std::string& s){
  int f;
  std::stringstream os;
  os<<s;
  os>>f;
  return f;
}
