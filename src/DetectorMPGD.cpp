//
//  DetectorMPGD.cpp
//  
//
//  Created by Brian L Dorney on 20/01/16.
//
//

#include "DetectorMPGD.h"

using std::cout;
using std::endl;
using std::map;
using std::multimap;
using std::vector;

using QualityControl::Timing::getString;

using namespace QualityControl::Uniformity;

//Default constructor
DetectorMPGD::DetectorMPGD(){
    strDetName = "Detector";
    strDetNameNoSpecChar	= "Detector";
} //End default constructor

//Constructor to use when supplying a vector of clusters
DetectorMPGD::DetectorMPGD(multimap<int, Cluster> map_inputClusters){
    strDetName = "Detector";
    strDetNameNoSpecChar	= "Detector";
    setClusters(map_inputClusters);
}

//Constructor to use when supplying a map of eta sectors
DetectorMPGD::DetectorMPGD(map<int,ReadoutSectorEta> map_inputSectors){
    //bAnaSetup = false;
    map_sectorsEta = map_inputSectors;
}

//Wipes all stored Clusters
void DetectorMPGD::resetClusters(){
    for (auto iterEta = map_sectorsEta.begin(); iterEta != map_sectorsEta.end(); ++iterEta) { //Loop Over Detector's ReadoutSectorEta Objects
        
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over Phi Sectors within an Eta Sector
            (*iterPhi).second.map_clusters.clear();
        } //End Loop Over Phi Sectors within an Eta Sector
    } //End Loop Over Detector's ReadoutSectorEta Objects
    
    return;
} //End DetectorMPGD::resetClusters()

//Wipes all stored Hits
void DetectorMPGD::resetHits(){
    for (auto iterEta = map_sectorsEta.begin(); iterEta != map_sectorsEta.end(); ++iterEta) { //Loop Over Detector's ReadoutSectorEta Objects
        
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over Phi Sectors within an Eta Sector
            (*iterPhi).second.map_hits.clear();
        } //End Loop Over Phi Sectors within an Eta Sector
    } //End Loop Over Detector's ReadoutSectorEta Objects
    
    return;
} //End DetectorMPGD::resetHits()

//Wipes all stored physics objects (Clusters and Hits)
void DetectorMPGD::resetPhysObj(){
    for (auto iterEta = map_sectorsEta.begin(); iterEta != map_sectorsEta.end(); ++iterEta) { //Loop Over Detector's ReadoutSectorEta Objects
        
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over Phi Sectors within an Eta Sector
            (*iterPhi).second.map_clusters.clear();
            (*iterPhi).second.map_hits.clear();
        } //End Loop Over Phi Sectors within an Eta Sector
    } //End Loop Over Detector's ReadoutSectorEta Objects
    
    return;
} //End DetectorMPGD::resetHits()

//Returns all clusters
multimap<int, Cluster> DetectorMPGD::getClusters(){
    //Variable Declaration
    multimap<int, Cluster> map_retClus;
    
    if (map_sectorsEta.size() > 0) { //Case: Eta Sectors Defined
        for (auto iterEta = map_sectorsEta.begin(); iterEta != map_sectorsEta.end(); ++iterEta) {
            for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) {
                map_retClus.insert( (*iterPhi).second.map_clusters.begin(), (*iterPhi).second.map_clusters.end() );
            } //End Loop Over Phi Sectors within an Eta Sector
        } //End Loop Over map_sectorsEta
    } //End Case: Eta Sectors Defined
    else{ //Case: No Sectors Defined
        printClassMethodMsg("DetectorMPGD","getClusters","Error: DetectorMPGD Has No Defined Eta Sectors");
        printClassMethodMsg("DetectorMPGD","getClusters","\tPlease Initialize Eta Sectors Before Trying to Set Clusters");
    } //End Case: No Sectors Defined
    
    return map_retClus;
} //End DetectorMPGD::getClusters()

//Returns clusters associated with a specific iEta value (all iPhi for this sector)
multimap<int, Cluster> DetectorMPGD::getClusters(int iEta){
    //Variable Declaration
    multimap<int, Cluster> map_retClus;
    
    if (map_sectorsEta.count(iEta) > 0 ) { //Case: Requested iEta Value Exists!!
        for (auto iterPhi = map_sectorsEta[iEta].map_sectorsPhi.begin(); iterPhi != map_sectorsEta[iEta].map_sectorsPhi.end(); ++iterPhi) {
            map_retClus.insert( (*iterPhi).second.map_clusters.begin(), (*iterPhi).second.map_clusters.end() );
        } //End Loop Over Phi Sectors within an Eta Sector
    } //End Case: Requested iEta Value Exists!!
    else{ //Case: Requested iEta Value doesn ot exist, return an empty vector
        printClassMethodMsg("DetectorMPGD","getClusters", ("Error: iEta " + getString(iEta) + " Does NOT Exists!!!" ).c_str() );
        printClassMethodMsg("DetectorMPGD","getClusters", "\tThe Returned vector is Empty!!!");
    } //End Case: Requested iEta Value doesn ot exist, return an empty vector
    return map_retClus;
} //End getClusters

//Returns cluster associated with a specific (iEta, iPhi) location
multimap<int, Cluster> DetectorMPGD::getClusters(int iEta, int iPhi){
    //Variable Declaration
    multimap<int, Cluster> map_retClus;
    
    if ( map_sectorsEta.count(iEta) > 0 ) { //Case: Requested iEta Value exists
        if ( map_sectorsEta[iEta].map_sectorsPhi.count(iPhi) > 0 ) { //Case: Requeste iPhi Value exists
            map_retClus = map_sectorsEta[iEta].map_sectorsPhi[iPhi].map_clusters;
        } //End Case: Requeste iPhi Value exists
        else{ //Case: Requested iPhi Value doesn ot exist, return an empty vector
            printClassMethodMsg("DetectorMPGD","getClusters", ("Error: (iEta,iPhi) = (" + getString(iEta) + "," + getString(iPhi) + ") Does NOT Exists!!!" ).c_str() );
            printClassMethodMsg("DetectorMPGD","getClusters", "\tThe Returned vector is Empty!!!");
        } //End Case: Requested iPhi Value doesn ot exist, return an empty vector
    } //End Case: Requested iEta Value exists
    else{ //Case: Requested iEta Value doesn ot exist, return an empty vector
        printClassMethodMsg("DetectorMPGD","getClusters", ("Error: iEta " + getString(iEta) + " Does NOT Exists!!!" ).c_str() );
        printClassMethodMsg("DetectorMPGD","getClusters", "\tThe Returned vector is Empty!!!");
    } //End Case: Requested iEta Value doesn ot exist, return an empty vector
    
    return map_retClus;
} //End getClusters

//Returns all hits
multimap<int, Hit> DetectorMPGD::getHits(){
    //Variable Declaration
    multimap<int, Hit> map_retHits;
    
    if (map_sectorsEta.size() > 0) { //Case: Eta Sectors Defined
        for (auto iterEta = map_sectorsEta.begin(); iterEta != map_sectorsEta.end(); ++iterEta) {
            for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) {
                map_retHits.insert( (*iterPhi).second.map_hits.begin(), (*iterPhi).second.map_hits.end() );
            } //End Loop Over Phi Sectors within an Eta Sector
        } //End Loop Over map_sectorsEta
    } //End Case: Eta Sectors Defined
    else{ //Case: No Sectors Defined
        printClassMethodMsg("DetectorMPGD","getHits","Error: DetectorMPGD Has No Defined Eta Sectors");
        printClassMethodMsg("DetectorMPGD","getHits","\tPlease Initialize Eta Sectors Before Trying to Set Clusters");
    } //End Case: No Sectors Defined
    
    return map_retHits;
} //End DetectorMPGD::getHits()

//Returns hits associated with a specific iEta value (all iPhi for this sector)
multimap<int, Hit> DetectorMPGD::getHits(int iEta){
    //Variable Declaration
    multimap<int, Hit> map_retHits;
    
    if (map_sectorsEta.count(iEta) > 0 ) { //Case: Requested iEta Value Exists!!
        for (auto iterPhi = map_sectorsEta[iEta].map_sectorsPhi.begin(); iterPhi != map_sectorsEta[iEta].map_sectorsPhi.end(); ++iterPhi) {
            map_retHits.insert( (*iterPhi).second.map_hits.begin(), (*iterPhi).second.map_hits.end() );
        } //End Loop Over Phi Sectors within an Eta Sector
    } //End Case: Requested iEta Value Exists!!
    else{ //Case: Requested iEta Value doesn ot exist, return an empty vector
        printClassMethodMsg("DetectorMPGD","getHits", ("Error: iEta " + getString(iEta) + " Does NOT Exists!!!" ).c_str() );
        printClassMethodMsg("DetectorMPGD","getHits", "\tThe Returned vector is Empty!!!");
    } //End Case: Requested iEta Value doesn ot exist, return an empty vector
    
    return map_retHits;
} //End DetectorMPGD::getHits()

//Returns cluster associated with a specific (iEta, iPhi) location
multimap<int, Hit> DetectorMPGD::getHits(int iEta, int iPhi){
    //Variable Declaration
    multimap<int, Hit> map_retHits;
    
    if ( map_sectorsEta.count(iEta) > 0 ) { //Case: Requested iEta Value exists
        if ( map_sectorsEta[iEta].map_sectorsPhi.count(iPhi) > 0 ) { //Case: Requeste iPhi Value exists
            map_retHits = map_sectorsEta[iEta].map_sectorsPhi[iPhi].map_hits;
        } //End Case: Requeste iPhi Value exists
        else{ //Case: Requested iPhi Value doesn ot exist, return an empty vector
            printClassMethodMsg("DetectorMPGD","getHits", ("Error: (iEta,iPhi) = (" + getString(iEta) + "," + getString(iPhi) + ") Does NOT Exists!!!" ).c_str() );
            printClassMethodMsg("DetectorMPGD","getHits", "\tThe Returned vector is Empty!!!");
        } //End Case: Requested iPhi Value doesn ot exist, return an empty vector
    } //End Case: Requested iEta Value exists
    else{ //Case: Requested iEta Value doesn ot exist, return an empty vector
        printClassMethodMsg("DetectorMPGD","getHits", ("Error: iEta " + getString(iEta) + " Does NOT Exists!!!" ).c_str() );
        printClassMethodMsg("DetectorMPGD","getHits", "\tThe Returned vector is Empty!!!");
    } //End Case: Requested iEta Value doesn ot exist, return an empty vector
    
    return map_retHits;
} //End DetectorMPGD::getHits()

//Returns the position of an iEta sector
float DetectorMPGD::getEtaPos(int iEta){
    if (map_sectorsEta.count(iEta)) { //Case: iEta Value Exists, return position
        return map_sectorsEta[iEta].fPos_Y;
    } //End Case: iEta Value Exists, return position
    else{ //Case: iEta Value does not exist
        return -1.;
    } //End Case: iEta Value does not exist
} //End DetectorMPGD::getEtaPos()

//Returns the width of an iEta sector
float DetectorMPGD::getEtaWidth(int iEta){
    if (map_sectorsEta.count(iEta)) { //Case: iEta Value Exists, return width
        return map_sectorsEta[iEta].fWidth;
    } //End Case: iEta Value Exists, return width
    else{ //Case: iEta Value does not exist
        return -1.;
    } //End Case: iEta Value does not exist
} //End DetectorMPGD::getEtaWidth()

//Returns the eta sector
ReadoutSectorEta DetectorMPGD::getEtaSector(int iEta){
    ReadoutSectorEta retSector;
    
    if ( map_sectorsEta.count(iEta) > 0 ) { //Case: Requested iEta Value exists
        retSector = map_sectorsEta[iEta];
    } //End Case: Requested iEta Value exists
    else{ //Case: Requested iEta Value doesn ot exist, return an empty sector
        printClassMethodMsg("DetectorMPGD","getEtaSector", ("Error: iEta " + getString(iEta) + " Does NOT Exists!!!" ).c_str() );
        printClassMethodMsg("DetectorMPGD","getEtaSector", "\tThe Returned sector is Empty!!!");
    } //End Case: Requested iEta Value doesn ot exist, return an empty sector
    
    return retSector;
} //End getEtaSector

//Returns the phi sector
ReadoutSectorPhi DetectorMPGD::getPhiSector(int iEta, int iPhi){
    ReadoutSectorPhi retSector;
    ReadoutSectorEta etaSector = getEtaSector(iEta);
    
    if ( etaSector.map_sectorsPhi.size() > 0 ) { //Case: Requested iEta Value exists
        if ( etaSector.map_sectorsPhi.count(iPhi) > 0 ) { //Case: Requested iPhi Value exists
            retSector = etaSector.map_sectorsPhi[iPhi];
        } //End Case: Requested iPhi Value exists
        else{ //Case: Requested iPhi Value doesn ot exist, return an empty vector
            printClassMethodMsg("DetectorMPGD","getPhiSector", ("Error: iPhi " + getString(iPhi) + " Does NOT Exists!!!" ).c_str() );
            printClassMethodMsg("DetectorMPGD","getPhiSector", "\tThe Returned sector is Empty!!!");
        } //End Case: Requested iPhi Value doesn ot exist, return an empty vector
    } //End Case: Requested iEta Value exists
    else{ //Case: Requested iEta Value doesn ot exist, return an empty vector
        printClassMethodMsg("DetectorMPGD","getPhiSector", ("Error: iEta " + getString(iEta) + " Does NOT Exists!!!" ).c_str() );
        printClassMethodMsg("DetectorMPGD","getPhiSector", "\tThe Returned sector is Empty!!!");
    } //End Case: Requested iEta Value doesn ot exist, return an empty vector
    
    return retSector;
} //End getPhiSector

//Sets a cluster
void DetectorMPGD::setCluster(int iNum_Evt, Cluster &inputCluster){
    //Check if the DetectorMPGD is initialized!
    if ( map_sectorsEta.size() > 0 ) { //Case: Eta Sectors Defined
        float fEtaLim_Low, fEtaLim_High;
        
        for (auto iterEta = map_sectorsEta.begin(); iterEta != map_sectorsEta.end(); ++iterEta) {
            //Setup Evaluation Limits
            if ( (*iterEta).second.fPos_Y > 0 ){ //Case: iEta Y-Pos > 0
                fEtaLim_Low = (*iterEta).second.fPos_Y - (0.1 * (*iterEta).second.fPos_Y);
                fEtaLim_High = (*iterEta).second.fPos_Y + (0.1 * (*iterEta).second.fPos_Y);
            } //End Case: iEta Y-Pos > 0
            else{ //Case: iEta Y-Pos <= 0
                fEtaLim_Low = (*iterEta).second.fPos_Y + (0.1 * (*iterEta).second.fPos_Y);
                fEtaLim_High = (*iterEta).second.fPos_Y - (0.1 * (*iterEta).second.fPos_Y);
            } //End Case: Case: iEta Y-Pos <= 0
           
		//Debugging
		//cout<<"===============================================\n";
		//cout<<"fEtaLim_Low = " << fEtaLim_Low << endl;
		//cout<<"fEtaLim_High = " << fEtaLim_High << endl;
		//cout<<"inputCluster.fPos_Y = " << inputCluster.fPos_Y << endl;
 
            //Find matching eta sector
            if ( fEtaLim_Low < inputCluster.fPos_Y && inputCluster.fPos_Y < fEtaLim_High ) { //Case: Matching Eta Sector Found!
                
                //Find matching phi sector within this eta sector
                for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over map_sectorsPhi
                    
                    if ( (*iterPhi).second.fPos_Xlow <= inputCluster.fPos_X && inputCluster.fPos_X < (*iterPhi).second.fPos_Xhigh ) { //Case: Matching Phi Sector Found!
                        (*iterPhi).second.map_clusters.insert( std::pair<int, Cluster>(iNum_Evt, inputCluster) );

			//Debugging
			//cout<<"Phi Selected\n";

                        break;
                    } //End Case: Matching Phi Sector Found!
                } //End Loop Over map_sectorsPhi
                
                break;
            } //End Case: Matching Eta Sector Found!
        } //End Loop Over map_sectorsEta
    } //End Case: Eta Sectors Defined
    else{ //Case: No Sectors Defined
        printClassMethodMsg("DetectorMPGD","setCluster","Error: DetectorMPGD Has No Defined Eta Sectors");
        printClassMethodMsg("DetectorMPGD","setCluster","\tPlease Initialize Eta Sectors Before Trying to Set Clusters");
        return;
    } //End Case: No Sectors Defined
    
    return;
} //End DetectorMPGD::setCluster

//Sets a hit
void DetectorMPGD::setHit(int iNum_Evt, Hit &inputHit){
    //Check if the DetectorMPGD is initialized!
    if ( map_sectorsEta.size() > 0 ) { //Case: Eta Sectors Defined
        int fEtaLim_Low, fEtaLim_High;
        
        for (auto iterEta = map_sectorsEta.begin(); iterEta != map_sectorsEta.end(); ++iterEta) {
            //Setup Evaluation Limits
            if ( (*iterEta).second.fPos_Y > 0 ){ //Case: iEta Y-Pos > 0
                fEtaLim_Low = (*iterEta).second.fPos_Y - (0.1 * (*iterEta).second.fPos_Y);
                fEtaLim_High = (*iterEta).second.fPos_Y + (0.1 * (*iterEta).second.fPos_Y);
            } //End Case: iEta Y-Pos > 0
            else{ //Case: iEta Y-Pos <= 0
                fEtaLim_Low = (*iterEta).second.fPos_Y + (0.1 * (*iterEta).second.fPos_Y);
                fEtaLim_High = (*iterEta).second.fPos_Y - (0.1 * (*iterEta).second.fPos_Y);
            } //End Case: Case: iEta Y-Pos <= 0
            
            //Find matching eta sector
            if ( fEtaLim_Low < inputHit.fPos_Y && inputHit.fPos_Y < fEtaLim_High ) { //Case: Matching Eta Sector Found!
                
                //Find matching phi sector within this eta sector
                for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over map_sectorsPhi
                    
                    if ( (*iterPhi).second.iStripNum_Min <= inputHit.iStripNum && inputHit.iStripNum < (*iterPhi).second.iStripNum_Max ) { //Case: Matching Phi Sector Found!
                        (*iterPhi).second.map_hits.insert( std::pair<int, Hit>(iNum_Evt, inputHit) );
                        break;
                    } //End Case: Matching Phi Sector Found!
                } //End Loop Over map_sectorsPhi
                
                break;
            } //End Case: Matching Eta Sector Found!
        } //End Loop Over map_sectorsEta
    } //End Case: Eta Sectors Defined
    else{ //Case: No Sectors Defined
        printClassMethodMsg("DetectorMPGD","setHit","Error: DetectorMPGD Has No Defined Eta Sectors");
        printClassMethodMsg("DetectorMPGD","setHit","\tPlease Initialize Eta Sectors Before Trying to Set Clusters");
        return;
    } //End Case: No Sectors Defined
    
    //If DetectorMPGD is initialized, loop through the eta sectors
    
    return;
} //End DetectorMPGD::setHit

//Defines an eta sector
void DetectorMPGD::setEtaSector(int iEta, float fInputPos_Y, float fInputWidth, int iNumPhiSector){
    //Check to see if this iEta sector already exists
    if (map_sectorsEta.count(iEta) > 0) { //Case: Eta Sector Exists; DO NOTHING
        printClassMethodMsg("DetectorMPGD","setEtaSector", ("Error: iEta " + getString(iEta) + " Already Exists!!!" ).c_str() );
        printClassMethodMsg("DetectorMPGD","setEtaSector", "\tSkipping!!! Previously Stored Data is not erased");
        
        return;
    } //End Case: Eta Sector Exists; DO NOTHING
    else{ //Case: Eta Sector does not exists; CREATE!
        //Variable Declaration
        ReadoutSectorEta etaSector;
        
        etaSector.fPos_Y = fInputPos_Y;
        etaSector.fWidth = fInputWidth;
        
        //Is this sector the widest sector?
        //if (fMaxSectorWidth < fInputWidth) fMaxSectorWidth = fInputWidth;
        
        //Make three Phi Sectors for this Eta Sector
        for (int i=1; i <= iNumPhiSector; ++i) {
            ReadoutSectorPhi phiSector;
            
            phiSector.fPos_Xlow = -0.5 * fInputWidth + (i-1) * fInputWidth / 3.;
            phiSector.fPos_Xhigh= -0.5 * fInputWidth + (i) * fInputWidth / 3.;
            
            phiSector.fWidth = fabs( phiSector.fPos_Xhigh - phiSector.fPos_Xlow );
            
            phiSector.iStripNum_Min = (i-1) * 128;
            phiSector.iStripNum_Max = i * 128;
            
            etaSector.map_sectorsPhi[i] = phiSector;
        } //End make three phi sectors for this Eta Sector
        
        //Store the eta sector in the DetectorMPGD's map
        map_sectorsEta[iEta] = etaSector;
        
        return;
    } //End Case: Eta Sector does not exists; CREATE!
} //End DetectorMPGD::setEtaSector

void DetectorMPGD::setEvent(Event &inputEvt){
    //Fill event level distributions
    hMulti_Clust->Fill( inputEvt.vec_clusters.size() );
    hMulti_Hit->Fill( inputEvt.vec_hits.size() );
    
    //Set Clusters
    for (int i=0; i < inputEvt.vec_clusters.size(); ++i) {
        setCluster(inputEvt.iNum_Evt, inputEvt.vec_clusters[i]);
    }
    
    //Set Hits
    for (int i=0; i < inputEvt.vec_hits.size(); ++i) {
        setHit(inputEvt.iNum_Evt, inputEvt.vec_hits[i]);
    }
    
    return;
} //End DetectorMPGD::setEvent()

void DetectorMPGD::setEventOnlyClusters(Event &inputEvt){
    //Fill event level distributions
    hMulti_Clust->Fill( inputEvt.vec_clusters.size() );
    
    //Set Clusters
    for (int i=0; i < inputEvt.vec_clusters.size(); ++i) {
        setCluster(inputEvt.iNum_Evt, inputEvt.vec_clusters[i]);
    }
    
    return;
} //End DetectorMPGD::setEvent()

void DetectorMPGD::setEventOnlyHits(Event &inputEvt){
    //Fill event level distributions
    hMulti_Hit->Fill( inputEvt.vec_hits.size() );
    
    //Set Hits
    for (int i=0; i < inputEvt.vec_hits.size(); ++i) {
        setHit(inputEvt.iNum_Evt, inputEvt.vec_hits[i]);
    }
    
    return;
} //End DetectorMPGD::setEventOnlyHits()


