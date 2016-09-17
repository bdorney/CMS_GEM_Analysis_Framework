//
//  SelectorCluster.cpp
//  
//
//  Created by Brian L Dorney on 28/01/16.
//
//

#include "SelectorCluster.h"

using std::cout;
using std::string;
using std::vector;

using namespace QualityControl::Uniformity;

//Default Constructor
SelectorCluster::SelectorCluster(){
    bVerboseMode = false;
} //End Default Constructor

//Filters an input vector<Uniformity::Cluster object based on the stored Uniformity::AnalysisSetupUniformity attribute
vector<Cluster> SelectorCluster::filterClusters(std::vector<Cluster> vec_inputClusts){
    //Variable Declaration
    vector<Cluster> vec_retClusts;
    
    //Could do this with vector::erase but believe that would be more inefficient (since container is re-organized everytime)
    for (auto iterClust = vec_inputClusts.begin(); iterClust != vec_inputClusts.end(); ++iterClust) { //Loop Over input Clusters
        if ( clusterPassesSelection( (*iterClust) ) ) { vec_retClusts.push_back( *iterClust ); }
    } //End Loop Over input Clusters
    
    return vec_retClusts;
} //End SelectorCluster::filterClusters()

//Given an output ROOT file from AMORE (ROOTDATATYPE = CLUSTERS)
//Applies the cluster selection and stores those selected clusters in inputDet
//Input is a std::string storing the physical filename
void SelectorCluster::setClusters(std::string &strInputRootFileName, Uniformity::DetectorMPGD &inputDet){
    //Variable Declaration
    TFile *file_ROOT = NULL;
    
    //Open this run's root file
    //------------------------------------------------------
    file_ROOT = new TFile(strInputRootFileName.c_str(),"READ","",1);
    
    //Check to see if data file opened successfully, if so load the tree
    //------------------------------------------------------
    if ( !file_ROOT->IsOpen() || file_ROOT->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("Uniformity::SelectorCluster::setClusters() - error while opening file: " + strInputRootFileName ).c_str() );
        Timing::printROOTFileStatus(file_ROOT);
        std::cout << "Exiting!!!\n";
        
        return;
    } //End Case: failed to load ROOT file
    
    //Calling setClusters
    //------------------------------------------------------
    setClusters(file_ROOT, inputDet);
    
    //Close the Input ROOT File
    //------------------------------------------------------
    file_ROOT->Close();
    
    return;
} //End SelectorCluster::setClusters()

//Given an output ROOT file from AMORE (ROOTDATATYPE = CLUSTERS)
//Applies the cluster selection and stores those selected clusters in inputDet
//Input is a TFile *
void SelectorCluster::setClusters(TFile * file_InputRootFile, Uniformity::DetectorMPGD &inputDet){
    //Variable Declaration
    //int iFirstEvt = aSetup.iEvt_First;
    //int iNEvt = aSetup.iEvt_Total;
    
    Int_t iClustMulti;  //I cry a little inside because of this
    Int_t iClustPos_Y[3072];
    Int_t iClustSize[3072];
    Int_t iClustTimeBin[3072];
    
    Float_t fClustPos_X[3072];
    Float_t fClustADC[3072];
    
    Cluster clust;
    
    std::pair<int,int> pair_iEvtRange;
    
    TTree *tree_Clusters = NULL;
    
    //Check to see if data file opened successfully, if so load the tree
    //------------------------------------------------------
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("Uniformity::SelectorCluster::setClusters() - error while opening file: " + (string) file_InputRootFile->GetName() ).c_str() );
        Timing::printROOTFileStatus(file_InputRootFile);
        std::cout << "Exiting!!!\n";
        
        return;
    } //End Case: failed to load ROOT file
    
    tree_Clusters = (TTree*) file_InputRootFile->Get("TCluster");
    
    if ( nullptr == tree_Clusters ) { //Case: failed to load TTree
        printClassMethodMsg("SelectorCluster","setClusters",("error while fetching: " + (string) file_InputRootFile->GetName() ).c_str() );
        printClassMethodMsg("SelectorCluster","setClusters","\tTree returns nullptr; Exiting!!!");
        
        return;
    } //End Case: failed to load TTree
    
    //Initialize Tree Branch Address to retrieve the cluster information
    //------------------------------------------------------
    tree_Clusters->SetBranchAddress("nclust", &iClustMulti);
    tree_Clusters->SetBranchAddress("clustPos",&fClustPos_X);
    tree_Clusters->SetBranchAddress("clustSize",&iClustSize);
    tree_Clusters->SetBranchAddress("clustADCs",&fClustADC);
    tree_Clusters->SetBranchAddress("clustTimebin",&iClustTimeBin);
    tree_Clusters->SetBranchAddress("planeID",&iClustPos_Y);
    
    //Determine Event Range
    //------------------------------------------------------
    pair_iEvtRange = getEventRange( aSetup.iEvt_First, aSetup.iEvt_Total, tree_Clusters->GetEntries() );
    
    //Get data event-by-event
    //------------------------------------------------------
    //for (int i=iFirstEvt; i < iNEvt; ++i) {
    for (int i=pair_iEvtRange.first; i < pair_iEvtRange.second; ++i) {
        //Needed to implement a Hack
        //First check to make sure the cluster multiplicity is within the selection
        //Only then get the info on the clusters
        
        //Make sure we only read the number of clusters
        tree_Clusters->SetBranchStatus("*",0);
        tree_Clusters->SetBranchStatus("nclust",1);
        
        //Get the number of clusters
        tree_Clusters->GetEntry(i);
        
        //Output to the user some message that we are still running
        if (i % 1000 == 0) cout<<"Cluster Selection; " <<i<<" Events Analyzed\n";
        
        //If the event fails to pass the selection; skip it
        //---------------Event Selection---------------
        //Cut on number of clusters
        if ( !(aSetup.selClust.iCut_MultiMin < iClustMulti && iClustMulti < aSetup.selClust.iCut_MultiMax) ) continue;
        
        //Okay make sure we can read all branches
        tree_Clusters->SetBranchStatus("*",1);
        
        //Now get the remaining data
        tree_Clusters->GetEntry(i);
        
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
            inputDet.setCluster(i, clust);
        } //End Loop Over Number of Clusters
    } //End Loop Over "Events"
    
    //Do not close the input TFile, it is used elsewhere
    
    return;
} //End SelectorCluster::setClusters()

//Check if Cluster Passes selection stored in aSetup? True -> Passes; False -> Fails
bool SelectorCluster::clusterPassesSelection(Cluster &inputClust){
    //Cluster Selection
    
    //Cluster with ADC below noise threshold?
	//cout<<"inputClust.fADC = " << inputClust.fADC << std::endl;
    if (inputClust.fADC < aSetup.selClust.iCut_ADCNoise){ return false; }
    
    //Cluster Size too small or too large?
    if (inputClust.iSize < aSetup.selClust.iCut_SizeMin){ return false; }
    if (inputClust.iSize > aSetup.selClust.iCut_SizeMax) {return false; }
    //if (inputClust.iSize <= aSetup.selClust.iCut_SizeMin){ return false; }
    //if (inputClust.iSize >= aSetup.selClust.iCut_SizeMax) {return false; }
    
    //Cluster Time too early or too late?
    if (inputClust.iTimeBin < aSetup.selClust.iCut_TimeMin){ return false; }
    if (inputClust.iTimeBin > aSetup.selClust.iCut_TimeMax) {return false; }
    //if (inputClust.iTimeBin <= aSetup.selClust.iCut_TimeMin){ return false; }
    //if (inputClust.iTimeBin >= aSetup.selClust.iCut_TimeMax) {return false; }
    
    //If we arrive here the cluster passes our selection; give true
    return true;
} //End clusterPassesSelection()
