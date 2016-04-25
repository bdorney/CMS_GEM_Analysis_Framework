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
using std::vector;

using Timing::getString;

using namespace Uniformity;

//Default constructor
DetectorMPGD::DetectorMPGD(){
    //bAnaSetup = false;
} //End default constructor

//Constructor to use when supplying a vector of clusters
DetectorMPGD::DetectorMPGD(vector<Cluster> vec_inputClusters){
    //bAnaSetup = false;
    setCluster(vec_inputClusters);
}

//Constructor to use when supplying a map of eta sectors
DetectorMPGD::DetectorMPGD(map<int,SectorEta> map_inputSectors){
    //bAnaSetup = false;
    map_sectorsEta = map_inputSectors;
}

//Initializes TObjects for hits
/*void DetectorMPGD::initializeDistsHits(){

}*/ //End DetectorMPGD::initializeDistsHits()

//Returns all clusters
vector<Cluster> DetectorMPGD::getClusters(){
    //Variable Declaration
    vector<Cluster> vec_retClus;
    
    if (map_sectorsEta.size() > 0) { //Case: Eta Sectors Defined
        for (auto iterEta = map_sectorsEta.begin(); iterEta != map_sectorsEta.end(); ++iterEta) {
            for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) {
                vec_retClus.insert(vec_retClus.end(), (*iterPhi).second.vec_clusters.begin(), (*iterPhi).second.vec_clusters.end() );
            } //End Loop Over Phi Sectors within an Eta Sector
        } //End Loop Over map_sectorsEta
    } //End Case: Eta Sectors Defined
    else{ //Case: No Sectors Defined
        printClassMethodMsg("DetectorMPGD","getClusters","Error: DetectorMPGD Has No Defined Eta Sectors");
        printClassMethodMsg("DetectorMPGD","getClusters","\tPlease Initialize Eta Sectors Before Trying to Set Clusters");
    } //End Case: No Sectors Defined
    
    return vec_retClus;
} //End DetectorMPGD::getClusters()

//Returns clusters associated with a specific iEta value (all iPhi for this sector)
vector<Cluster> DetectorMPGD::getClusters(int iEta){
    //Variable Declaration
    vector<Cluster> vec_retClus;
    
    if (map_sectorsEta.count(iEta) > 0 ) { //Case: Requested iEta Value Exists!!
        //return map_sectorsEta[iEta].vec_clusters;
        
        for (auto iterPhi = map_sectorsEta[iEta].map_sectorsPhi.begin(); iterPhi != map_sectorsEta[iEta].map_sectorsPhi.end(); ++iterPhi) {
            vec_retClus.insert(vec_retClus.end(), (*iterPhi).second.vec_clusters.begin(), (*iterPhi).second.vec_clusters.end() );
        } //End Loop Over Phi Sectors within an Eta Sector
    } //End Case: Requested iEta Value Exists!!
    else{ //Case: Requested iEta Value doesn ot exist, return an empty vector
        printClassMethodMsg("DetectorMPGD","getClusters", ("Error: iEta " + getString(iEta) + " Does NOT Exists!!!" ).c_str() );
        printClassMethodMsg("DetectorMPGD","getClusters", "\tThe Returned vector is Empty!!!");
    } //End Case: Requested iEta Value doesn ot exist, return an empty vector
    return vec_retClus;
} //End getClusters

//Returns cluster associated with a specific (iEta, iPhi) location
vector<Cluster> DetectorMPGD::getClusters(int iEta, int iPhi){
    //Variable Declaration
    vector<Cluster> vec_retClus;
    
    if ( map_sectorsEta.count(iEta) > 0 ) { //Case: Requested iEta Value exists
        if ( map_sectorsEta[iEta].map_sectorsPhi.count(iPhi) > 0 ) { //Case: Requeste iPhi Value exists
            vec_retClus = map_sectorsEta[iEta].map_sectorsPhi[iPhi].vec_clusters;
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
    
    return vec_retClus;
} //End getClusters

//Returns all hits
vector<Hit> DetectorMPGD::getHits(){
    //Variable Declaration
    vector<Hit> vec_retHits;
    
    if (map_sectorsEta.size() > 0) { //Case: Eta Sectors Defined
        for (auto iterEta = map_sectorsEta.begin(); iterEta != map_sectorsEta.end(); ++iterEta) {
            for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) {
                vec_retHits.insert(vec_retHits.end(), (*iterPhi).second.vec_hits.begin(), (*iterPhi).second.vec_hits.end() );
            } //End Loop Over Phi Sectors within an Eta Sector
        } //End Loop Over map_sectorsEta
    } //End Case: Eta Sectors Defined
    else{ //Case: No Sectors Defined
        printClassMethodMsg("DetectorMPGD","getHits","Error: DetectorMPGD Has No Defined Eta Sectors");
        printClassMethodMsg("DetectorMPGD","getHits","\tPlease Initialize Eta Sectors Before Trying to Set Clusters");
    } //End Case: No Sectors Defined
    
    return vec_retHits;
} //End DetectorMPGD::getHits()

//Returns hits associated with a specific iEta value (all iPhi for this sector)
vector<Hit> DetectorMPGD::getHits(int iEta){
    //Variable Declaration
    vector<Hit> vec_retHits;
    
    if (map_sectorsEta.count(iEta) > 0 ) { //Case: Requested iEta Value Exists!!
        //return map_sectorsEta[iEta].vec_clusters;
        
        for (auto iterPhi = map_sectorsEta[iEta].map_sectorsPhi.begin(); iterPhi != map_sectorsEta[iEta].map_sectorsPhi.end(); ++iterPhi) {
            vec_retHits.insert(vec_retHits.end(), (*iterPhi).second.vec_hits.begin(), (*iterPhi).second.vec_hits.end() );
        } //End Loop Over Phi Sectors within an Eta Sector
    } //End Case: Requested iEta Value Exists!!
    else{ //Case: Requested iEta Value doesn ot exist, return an empty vector
        printClassMethodMsg("DetectorMPGD","getHits", ("Error: iEta " + getString(iEta) + " Does NOT Exists!!!" ).c_str() );
        printClassMethodMsg("DetectorMPGD","getHits", "\tThe Returned vector is Empty!!!");
    } //End Case: Requested iEta Value doesn ot exist, return an empty vector
    
    return vec_retHits;
} //End DetectorMPGD::getHits()

//Returns cluster associated with a specific (iEta, iPhi) location
vector<Hit> DetectorMPGD::getHits(int iEta, int iPhi){
    //Variable Declaration
    vector<Hit> vec_retHits;
    
    if ( map_sectorsEta.count(iEta) > 0 ) { //Case: Requested iEta Value exists
        if ( map_sectorsEta[iEta].map_sectorsPhi.count(iPhi) > 0 ) { //Case: Requeste iPhi Value exists
            vec_retHits = map_sectorsEta[iEta].map_sectorsPhi[iPhi].vec_hits;
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
    
    return vec_retHits;
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
SectorEta DetectorMPGD::getEtaSector(int iEta){
    SectorEta retSector;
    
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
SectorPhi DetectorMPGD::getPhiSector(int iEta, int iPhi){
    SectorPhi retSector;
    SectorEta etaSector = getEtaSector(iEta);
    
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
void DetectorMPGD::setCluster(Cluster &inputCluster){
    //Check if the DetectorMPGD is initialized!
    if ( map_sectorsEta.size() > 0 ) { //Case: Eta Sectors Defined
        for (auto iterEta = map_sectorsEta.begin(); iterEta != map_sectorsEta.end(); ++iterEta) {
            //Find matching eta sector
            if ( ( (*iterEta).second.fPos_Y - 0.1 * (*iterEta).second.fPos_Y ) < inputCluster.iPos_Y && inputCluster.iPos_Y < ( (*iterEta).second.fPos_Y + 0.1 * (*iterEta).second.fPos_Y ) ) { //Case: Matching Eta Sector Found!
                
                //Find matching phi sector within this eta sector
                for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over map_sectorsPhi
                    
                    if ( (*iterPhi).second.fPos_Xlow <= inputCluster.fPos_X && inputCluster.fPos_X < (*iterPhi).second.fPos_Xhigh ) { //Case: Matching Phi Sector Found!
                        (*iterPhi).second.vec_clusters.push_back(inputCluster);
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
    
    //If DetectorMPGD is initialized, loop through the eta sectors
    
    return;
} //End DetectorMPGD::setCluster

//Sets a hit
void DetectorMPGD::setHit(Hit &inputHit){
    //Check if the DetectorMPGD is initialized!
    if ( map_sectorsEta.size() > 0 ) { //Case: Eta Sectors Defined
        for (auto iterEta = map_sectorsEta.begin(); iterEta != map_sectorsEta.end(); ++iterEta) {
            //Find matching eta sector
            if ( ( (*iterEta).second.fPos_Y - 0.1 * (*iterEta).second.fPos_Y ) < inputHit.iPos_Y && inputHit.iPos_Y < ( (*iterEta).second.fPos_Y + 0.1 * (*iterEta).second.fPos_Y ) ) { //Case: Matching Eta Sector Found!
                
                //Find matching phi sector within this eta sector
                for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over map_sectorsPhi
                    
                    if ( (*iterPhi).second.iStripNum_Min <= inputHit.iStripNum && inputHit.iStripNum < (*iterPhi).second.iStripNum_Max ) { //Case: Matching Phi Sector Found!
                        (*iterPhi).second.vec_hits.push_back(inputHit);
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
        SectorEta etaSector;
        
        etaSector.fPos_Y = fInputPos_Y;
        etaSector.fWidth = fInputWidth;
        
        //Is this sector the widest sector?
        //if (fMaxSectorWidth < fInputWidth) fMaxSectorWidth = fInputWidth;
        
        //Make three Phi Sectors for this Eta Sector
        for (int i=1; i <= iNumPhiSector; ++i) {
            SectorPhi phiSector;
            
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
