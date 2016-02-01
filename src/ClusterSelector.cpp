//
//  ClusterSelector.cpp
//  
//
//  Created by Brian L Dorney on 28/01/16.
//
//

#include "ClusterSelector.h"

using std::cout;

using namespace Uniformity;

//Default Constructor
ClusterSelector::ClusterSelector(){
    
} //End Default Constructor

//Given an output ROOT file from AMORE (ROOTDATATYPE = CLUSTERS)
//Applies the cluster selection and stores those selected clusters in inputDet
void ClusterSelector::setClusters(std::string &strInputRootFileName, Uniformity::DetectorMPGD &inputDet){
    //Variable Declaration
    Int_t iClustMulti;  //I cry a little inside because of this
    Int_t iClustPos_Y[55];
    Int_t iClustSize[55];
    Int_t iClustTimeBin[55];
    
    Float_t fClustPos_X[55];
    Float_t fClustADC[55];
    
    Cluster clust;
    
    TFile *file_ROOT = NULL;
    TTree *tree_Clusters = NULL;
    
    //Open this run's root file
    //------------------------------------------------------
    file_ROOT = new TFile(strInputRootFileName.c_str(),"READ","",1);
    
    //Check to see if data file opened successfully, if so load the tree
    //------------------------------------------------------
    if ( !file_ROOT->IsOpen() || file_ROOT->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("Uniformity::ClusterSelector::setClusters() - error while opening file: " + strInputRootFileName ).c_str() );
        Timing::printROOTFileStatus(file_ROOT);
        std::cout << "Exiting!!!\n";
        
        return;
    } //End Case: failed to load ROOT file
    
    tree_Clusters = (TTree*) file_ROOT->Get("TCluster");
    
    if ( nullptr == tree_Clusters ) { //Case: failed to load TTree
        printClassMethodMsg("ClusterSelector","setClusters",("error while fetching: " + strInputRootFileName ).c_str() );
        printClassMethodMsg("ClusterSelector","setClusters","\tTree returns nullptr; Exiting!!!");
    } //End Case: failed to load TTree
    
    //Initialize Tree Branch Address to retrieve the cluster information
    //------------------------------------------------------
    tree_Clusters->SetBranchAddress("nclust", &iClustMulti);
    //tree_Clusters->SetBranchAddress("clustPos", &clust.fPos_X);
    tree_Clusters->SetBranchAddress("clustPos",&fClustPos_X);
    //tree_Clusters->SetBranchAddress("clustSize", &clust.fSize);
    tree_Clusters->SetBranchAddress("clustSize",&iClustSize);
    //tree_Clusters->SetBranchAddress("clustADCs", &clust.fADC);
    tree_Clusters->SetBranchAddress("clustADCs",&fClustADC);
    //tree_Clusters->SetBranchAddress("clustTimebin", &clust.fTimeBin);
    tree_Clusters->SetBranchAddress("clustTimebin",&iClustTimeBin);
    //tree_Clusters->SetBranchAddress("planeID", &clust.fPos_Y);
    tree_Clusters->SetBranchAddress("planeID",&iClustPos_Y);
    
    //Get data event-by-event
    //------------------------------------------------------
    for (int i=0; i < tree_Clusters->GetEntries(); ++i) {
        //Get Event
        tree_Clusters->GetEntry(i);
        
        //Output to the user some message that we are still running
        if (i % 1000 == 0) cout<<"Cluster Selection; " <<i<<" Events Analyzed\n";
        
        //If the event fails to pass the selection; skip it
        //---------------Event Selection---------------
        //if ( iClustMulti > aSetupUniformity.selClust.iCut_NClust ) continue;
        if ( !(aSetupUniformity.selClust.iCut_MultiMin < iClustMulti && iClustMulti < aSetupUniformity.selClust.iCut_MultiMax) ) continue;
        
        //Loop Over the elements of the cluster array (yes it must be done like this due to how hte NTuple from AMORE is created)
        //For each element create a cluster, and check if it passes the selection
        for (int j=0; j < iClustMulti; ++j) { //Loop Over Number of Clusters
            //Set the cluster info
            clust.iPos_Y = iClustPos_Y[j];
            clust.fPos_X = fClustPos_X[j];
            
            clust.fADC = fClustADC[j];
            
            clust.iSize = iClustSize[j];
            
            clust.iTimeBin = iClustTimeBin[j];
            
            //If the cluster fails to pass the selection; skip it
            //---------------Cluster Selection---------------
            if ( !clusterPassesSelection(clust) ) continue;
            
            //If a cluster makes it here, store it in the detector
            inputDet.setCluster(clust);
        } //End Loop Over Number of Clusters
    } //End Loop Over "Events"
    
    return;
} //End ClusterSelector::setClusters()

//Check if Cluster Passes selection stored in aSetupUniformity? True -> Passes; False -> Fails
bool ClusterSelector::clusterPassesSelection(Cluster &inputClust){
    //Cluster Selection
    
    //Cluster with ADC below noise threshold?
	//cout<<"inputClust.fADC = " << inputClust.fADC << std::endl;
    if (inputClust.fADC < aSetupUniformity.selClust.iCut_ADCNoise){ return false; }
    
    //Cluster Size too small or too large?
    if (inputClust.iSize < aSetupUniformity.selClust.iCut_SizeMin){ return false; }
    if (inputClust.iSize > aSetupUniformity.selClust.iCut_SizeMax) {return false; }
    
    //Cluster Time too early or too late?
    if (inputClust.iTimeBin < aSetupUniformity.selClust.iCut_TimeMin){ return false; }
    if (inputClust.iTimeBin > aSetupUniformity.selClust.iCut_TimeMax) {return false; }
    
    //If we arrive here the cluster passes our selection; give true
    return true;
} //End clusterPassesSelection()
