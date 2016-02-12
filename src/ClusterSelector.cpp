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
    int iFirstEvt = aSetupUniformity.iEvt_First;
    int iNEvt = aSetupUniformity.iEvt_Total;
    
    Int_t iClustMulti;  //I cry a little inside because of this
    Int_t iClustPos_Y[3072];
	Int_t iClustSize[3072];
	Int_t iClustTimeBin[3072];
    
	Float_t fClustPos_X[3072];
    Float_t fClustADC[3072];
    
    //Int_t *iClustSize = NULL;//[55];
    //Int_t *iClustPos_Y = NULL;//[55];
    //Int_t *iClustTimeBin = NULL;//[55];

    //Float_t *fClustPos_X = NULL;//[55];
    //Float_t *fClustADC = NULL;//[55];

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
    tree_Clusters->SetBranchAddress("clustPos",&fClustPos_X);
    tree_Clusters->SetBranchAddress("clustSize",&iClustSize);
    tree_Clusters->SetBranchAddress("clustADCs",&fClustADC);
    tree_Clusters->SetBranchAddress("clustTimebin",&iClustTimeBin);
    tree_Clusters->SetBranchAddress("planeID",&iClustPos_Y);
    
    //Check Event Range
    //------------------------------------------------------
    /*if (aSetupUniformity.iEvt_First < 0) { //Case: Negative Event Number for First Event
        aSetupUniformity.iEvt_First = 0;
    } //End Case: Negative Event Number for First Event
    else if(aSetupUniformity.iEvt_First > tree_Clusters->GetEntries() ){ //Case: First Event Beyond Event Range
        printClassMethodMsg("ClusterSelector","setClusters", ("Error, First Event Requested as " + getString( aSetupUniformity.iEvt_First ) + " Greater Thant Total Number of Events " + getString( tree_Clusters->GetEntries() ) ).c_str() );
    }*/ //End Case: First Event Beyond Event Range
    
    if ( -1 == iNEvt ) { //Case: All Events
        iFirstEvt = 0;
        iNEvt = tree_Clusters->GetEntries();
    } //End Case: All Events
    else{ //Case: Event Range
        if ( iFirstEvt > tree_Clusters->GetEntries() ) { //Case: Incorrect Event Range, 1st Event Requested Beyond All Events
            printClassMethodMsg("ClusterSelector","setClusters", ("Error, First Event Requested as " + getString( aSetupUniformity.iEvt_First ) + " Greater Thant Total Number of Events " + getString( tree_Clusters->GetEntries() ) ).c_str() );
            printClassMethodMsg("ClusterSelector","setClusters", "Exiting!!!");
            return;
        } //End Case: Incorrect Event Range, 1st Event Requested Beyond All Events
        else if( (iFirstEvt + iNEvt) > tree_Clusters->GetEntries() ){
            iNEvt = tree_Clusters->GetEntries() - iFirstEvt;
        }
        else if( iFirstEvt < 0){
            iFirstEvt = 0;
        }
    } //End Case: Event Range
    
    //Get data event-by-event
    //------------------------------------------------------
    //for (int i=0; i < tree_Clusters->GetEntries(); ++i) {
    for (int i=iFirstEvt; i < iNEvt; ++i) {
        //Needed to implement a Hack
        //First check to make sure the cluster multiplicity is within the selection
        //Only then get the info on the clusters
        
        //Make sure we only read the number of clusters
        tree_Clusters->SetBranchStatus("*",0);
        tree_Clusters->SetBranchStatus("nclust",1);
        
        //Get the number of clusters
        tree_Clusters->GetEntry(i);

        //Allocate memory for this number of clusters
        //NOTE: OLD WAY; VERY SLOW
        //iClustPos_Y = new Int_t[iClustMulti];
        //iClustSize = new Int_t[iClustMulti];
    	//iClustTimeBin = new Int_t[iClustMulti];
    
    	//fClustPos_X = new Float_t[iClustMulti];
    	//fClustADC = new Float_t[iClustMulti];

        //These pointers are refreshed EVERY iteration; so we need to set the address every iteration
        //tree_Clusters->SetBranchAddress("clustPos",&fClustPos_X);
    	//tree_Clusters->SetBranchAddress("clustSize",&iClustSize);
    	//tree_Clusters->SetBranchAddress("clustADCs",&fClustADC);
    	//tree_Clusters->SetBranchAddress("clustTimebin",&iClustTimeBin);
    	//tree_Clusters->SetBranchAddress("planeID",&iClustPos_Y);
    
        //Okay make sure we can read all branches
            //tree_Clusters->SetBranchStatus("*",1);
        //tree_Clusters->SetBranchStatus("nclust",1);

        //Now get the data
        //tree_Clusters->GetEntry(i);

        //Output to the user some message that we are still running
        if (i % 1000 == 0) cout<<"Cluster Selection; " <<i<<" Events Analyzed\n";
        
        //If the event fails to pass the selection; skip it
        //---------------Event Selection---------------
        //Cut on number of clusters
        if ( !(aSetupUniformity.selClust.iCut_MultiMin < iClustMulti && iClustMulti < aSetupUniformity.selClust.iCut_MultiMax) ) continue;
        
        //Okay make sure we can read all branches
        tree_Clusters->SetBranchStatus("*",1);
        //tree_Clusters->SetBranchStatus("nclust",1);

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
            inputDet.setCluster(clust);
        } //End Loop Over Number of Clusters

        //Unallocate pointers
        //delete [] iClustPos_Y; iClustPos_Y = NULL;
        //delete [] iClustSize; iClustSize = NULL;
        //delete [] iClustTimeBin; iClustTimeBin = NULL;
        //delete [] fClustPos_X; fClustPos_X = NULL;
        //delete [] fClustADC; fClustADC = NULL;
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
