//
//  InterfaceRun.cpp
//  
//
//  Created by Brian L Dorney on 04/07/16.
//
//

#include "InterfaceRun.h"

using std::cout;
using std::endl;
using std::pair;
using std::string;

using namespace QualityControl::Uniformity;

//Default Constructor
InterfaceRun::InterfaceRun(){
    //bInitialized = false;
    bVerboseMode = false;
} //End Default Constructor

void InterfaceRun::analyzeInputAmoreSRS(std::string strInputRun){
    
    if (rSetup.bAnaStep_Hits && rSetup.bAnaStep_Clusters && !rSetup.bAnaStep_Reco) {
        analyzeAmoreSRSRunFullNoReReco(strInputRun);
    } //End Case: Full analysis w/o re-reco
    else if(rSetup.bAnaStep_Hits && rSetup.bAnaStep_Clusters && rSetup.bAnaStep_Reco){
        analyzeAmoreSRSRunFull(strInputRun);
    } //End Case: Full analysis re-reco clusters from hits
    else if(!rSetup.bAnaStep_Hits && rSetup.bAnaStep_Clusters && !rSetup.bAnaStep_Reco){
        analyzeAmoreSRSRunClustersOnly(strInputRun);
    } //End Case: Only Cluster Analysis (no re-reco)
    else if(rSetup.bAnaStep_Hits && !rSetup.bAnaStep_Clusters){
        analyzeAmoreSRSRunHitsOnly(strInputRun);
    } //End Case: Only Hit Analysis
    else{
        cout<<"QualityControl::Uniformity::InterfaceRun::analyzeInputAmoreSRS() - Unrecognized input configuration!!!\n";
        cout<<"\tAna_Hits = " << rSetup.bAnaStep_Hits << endl;
        cout<<"\tAna_Reco_Clusters = " << rSetup.bAnaStep_Reco << endl;
        cout<<"\tAna_Clusters = " << rSetup.bAnaStep_Clusters << endl;
        cout<<"\tPlease cross-check input run config file!!!\n";
    } //End Case: Unrecognized input configuration
    
    return;
} //End InterfaceRun::analyzeInputAmoreSRS()

//Analyze Run - Clusters Only
void InterfaceRun::analyzeAmoreSRSRunClustersOnly(string strInputRun){
    //Variable Declaration
    //bool bFrmwrkTree;
    
    Event evt;
    
    std::pair<int,int> pair_iEvtRange;
    
    TFile *file_ROOT = NULL;
    
    TTree *tree_Clusters = NULL;
    
    //For TCluster in amoreSRS
    Int_t iClustMulti;  //I cry a little inside because of this
    Int_t iClustPos_Y[3072];
    Int_t iClustSize[3072];
    Int_t iClustTimeBin[3072];
    
    Float_t fClustPos_X[3072];
    Float_t fClustADC[3072];
    
    //Open this run's root file
    //------------------------------------------------------
    file_ROOT = new TFile(strInputRun.c_str(),"READ","",1);
    
    //Check to see if data file opened successfully
    //------------------------------------------------------
    if ( !file_ROOT->IsOpen() || file_ROOT->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("Uniformity::SelectorCluster::setClusters() - error while opening file: " + strInputRun ).c_str() );
        Timing::printROOTFileStatus(file_ROOT);
        std::cout << "Exiting!!!\n";
        
        return;
    } //End Case: failed to load ROOT file
    
    //Load the TTree(s)
    //------------------------------------------------------
    tree_Clusters = (TTree*) file_ROOT->Get("TCluster");
    
    if ( nullptr == tree_Clusters ) { //Case: failed to load TTree
        printClassMethodMsg("InterfaceRun","analyzeEventFullNoReco",("error while fetching: " + (string) file_ROOT->GetName() ).c_str() );
        printClassMethodMsg("InterfaceRun","analyzeEventFullNoReco","\tCluster tree returns nullptr; Exiting!!!");
        
        return;
    } //End Case: failed to load TTree
    
    //Determine Event Range for Analysis
    //------------------------------------------------------
    pair_iEvtRange = getEventRange( aSetup.iEvt_First, aSetup.iEvt_Total, tree_Clusters->GetEntries() );
    
    //Set Branch Addresses - Clusters
    //------------------------------------------------------
    tree_Clusters->SetBranchAddress("nclust", &iClustMulti);
    tree_Clusters->SetBranchAddress("clustPos",&fClustPos_X);
    tree_Clusters->SetBranchAddress("clustSize",&iClustSize);
    tree_Clusters->SetBranchAddress("clustADCs",&fClustADC);
    tree_Clusters->SetBranchAddress("clustTimebin",&iClustTimeBin);
    tree_Clusters->SetBranchAddress("planeID",&iClustPos_Y);
    
    //Loop Over Events
    //------------------------------------------------------
    for (int i=pair_iEvtRange.first; i < pair_iEvtRange.second; ++i) { //Loop Over Events
        //Call reset/clear methods at the start of this iteration
        evt.clear();
        detMPGD.resetPhysObj();
        
        //Set the event number & run number
        //------------------------------------------------------
        evt.iNum_Evt = i;
        //evt.iNum_Run =
        
        //Determine Number of Clusters
        //------------------------------------------------------
        //Make sure we only read the number of clusters
        tree_Clusters->SetBranchStatus("*",0);
        tree_Clusters->SetBranchStatus("nclust",1);
        
        //Get the number of clusters
        tree_Clusters->GetEntry(i);
        
        //Output to the user some message that we are still running
        if (i % 1000 == 0) cout<< i <<" Events Analyzed\n";
        
        //Event Level Selection
        //------------------------------------------------------
        if ( !(aSetup.selClust.iCut_MultiMin < iClustMulti && iClustMulti < aSetup.selClust.iCut_MultiMax) ) continue;
        
        //Get & Select Physics Objects - Clusters
        //------------------------------------------------------
        //Okay make sure we can read all branches
        tree_Clusters->SetBranchStatus("*",1);
        
        //Now get the remaining data
        tree_Clusters->GetEntry(i);
        
        for (int j=0; j < iClustMulti; ++j) { //Loop Over Number of Clusters
            //Define the Cluster
            Cluster clust;
            
            //Set the cluster info
            clust.iPos_Y = iClustPos_Y[j];
            clust.fPos_X = fClustPos_X[j];
            
            clust.fADC = fClustADC[j];
            
            clust.iSize = iClustSize[j];
            
            clust.iTimeBin = iClustTimeBin[j];
            
            //If the cluster fails to pass the selection; skip it
            //---------------Cluster Selection---------------
            if ( !clustSelector.clusterPassesSelection(clust) ) continue;
            
            //If a cluster makes it here, store it in the detector
            evt.vec_clusters.push_back(clust);
        } //End Loop Over Number of Clusters
        
        //Give the Event to detMPG to store the Physics Objects
        //------------------------------------------------------
        detMPGD.setEventOnlyClusters(evt);
        
        //Fill Distributions & Update Detector - Clusters
        //------------------------------------------------------
        clustAnalyzer.setDetector(detMPGD);
        clustAnalyzer.fillHistos();
        
        detMPGD = clustAnalyzer.getDetector();
    } //End Loop Over Events
    
    //Close the Input ROOT File
    //------------------------------------------------------
    file_ROOT->Close();
    
    return;
} //End InterfaceRun::analyzeAmoreSRSRunClustersOnly()

//Analyze Run - Hits Only
void InterfaceRun::analyzeAmoreSRSRunHitsOnly(string strInputRun){
    //Variable Declaration
    //bool bFrmwrkTree;
    
    Event evt;
    
    std::pair<int,int> pair_iEvtRange;
    
    TFile *file_ROOT = NULL;
    
    //TTree *tree_Clusters = NULL, tree_Events = NULL, tree_Hits = NULL;
    TTree *tree_Hits = NULL;
    
    //For THit in amoreSRS
    Int_t iHitMulti;  //I cry a little inside because of this
    Int_t iHitPos_Y[3072];
    Int_t iHitStrip[3072];
    Int_t iHitTimeBin[3072];
    
    Short_t sHitADC_Bin0[3072];
    Short_t sHitADC_Bin1[3072];
    Short_t sHitADC_Bin2[3072];
    Short_t sHitADC_Bin3[3072];
    Short_t sHitADC_Bin4[3072];
    Short_t sHitADC_Bin5[3072];
    Short_t sHitADC_Bin6[3072];
    Short_t sHitADC_Bin7[3072];
    Short_t sHitADC_Bin8[3072];
    Short_t sHitADC_Bin9[3072];
    Short_t sHitADC_Bin10[3072];
    Short_t sHitADC_Bin11[3072];
    Short_t sHitADC_Bin12[3072];
    Short_t sHitADC_Bin13[3072];
    Short_t sHitADC_Bin14[3072];
    Short_t sHitADC_Bin15[3072];
    Short_t sHitADC_Bin16[3072];
    Short_t sHitADC_Bin17[3072];
    Short_t sHitADC_Bin18[3072];
    Short_t sHitADC_Bin19[3072];
    Short_t sHitADC_Bin20[3072];
    Short_t sHitADC_Bin21[3072];
    Short_t sHitADC_Bin22[3072];
    Short_t sHitADC_Bin23[3072];
    Short_t sHitADC_Bin24[3072];
    Short_t sHitADC_Bin25[3072];
    Short_t sHitADC_Bin26[3072];
    Short_t sHitADC_Bin27[3072];
    Short_t sHitADC_Bin28[3072];
    Short_t sHitADC_Bin29[3072];
    
    //Open this run's root file
    //------------------------------------------------------
    file_ROOT = new TFile(strInputRun.c_str(),"READ","",1);
    
    //Check to see if data file opened successfully
    //------------------------------------------------------
    if ( !file_ROOT->IsOpen() || file_ROOT->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("Uniformity::SelectorCluster::setClusters() - error while opening file: " + strInputRun ).c_str() );
        Timing::printROOTFileStatus(file_ROOT);
        std::cout << "Exiting!!!\n";
        
        return;
    } //End Case: failed to load ROOT file
    
    //Load the TTree(s)
    //------------------------------------------------------
    tree_Hits = (TTree*) file_ROOT->Get("THit");
    
    if ( nullptr == tree_Hits ) { //Case: failed to load TTree
        printClassMethodMsg("InterfaceRun","analyzeEventFullNoReco",("error while fetching: " + (string) file_ROOT->GetName() ).c_str() );
        printClassMethodMsg("InterfaceRun","analyzeEventFullNoReco","\tHit tree returns nullptr; Exiting!!!");
        
        return;
    } //End Case: failed to load TTree
    
    //Determine Event Range for Analysis
    //------------------------------------------------------
    pair_iEvtRange = getEventRange( aSetup.iEvt_First, aSetup.iEvt_Total, tree_Hits->GetEntries() );
    
    //Set Branch Addresses - Hits
    //------------------------------------------------------
    tree_Hits->SetBranchAddress("hitTimebin",&iHitTimeBin);
    tree_Hits->SetBranchAddress("nch", &iHitMulti);
    tree_Hits->SetBranchAddress("planeID",&iHitPos_Y);
    tree_Hits->SetBranchAddress("strip",&iHitStrip);
    
    //ADC
    tree_Hits->SetBranchAddress("adc0",&sHitADC_Bin0);
    tree_Hits->SetBranchAddress("adc1",&sHitADC_Bin1);
   	tree_Hits->SetBranchAddress("adc2",&sHitADC_Bin2);
   	tree_Hits->SetBranchAddress("adc3",&sHitADC_Bin3);
   	tree_Hits->SetBranchAddress("adc4",&sHitADC_Bin4);
   	tree_Hits->SetBranchAddress("adc5",&sHitADC_Bin5);
   	tree_Hits->SetBranchAddress("adc6",&sHitADC_Bin6);
   	tree_Hits->SetBranchAddress("adc7",&sHitADC_Bin7);
   	tree_Hits->SetBranchAddress("adc8",&sHitADC_Bin8);
   	tree_Hits->SetBranchAddress("adc9",&sHitADC_Bin9);
   	tree_Hits->SetBranchAddress("adc10",&sHitADC_Bin10);
   	tree_Hits->SetBranchAddress("adc11",&sHitADC_Bin11);
   	tree_Hits->SetBranchAddress("adc12",&sHitADC_Bin12);
   	tree_Hits->SetBranchAddress("adc13",&sHitADC_Bin13);
   	tree_Hits->SetBranchAddress("adc14",&sHitADC_Bin14);
   	tree_Hits->SetBranchAddress("adc15",&sHitADC_Bin15);
   	tree_Hits->SetBranchAddress("adc16",&sHitADC_Bin16);
   	tree_Hits->SetBranchAddress("adc17",&sHitADC_Bin17);
   	tree_Hits->SetBranchAddress("adc18",&sHitADC_Bin18);
   	tree_Hits->SetBranchAddress("adc19",&sHitADC_Bin19);
   	tree_Hits->SetBranchAddress("adc20",&sHitADC_Bin20);
   	tree_Hits->SetBranchAddress("adc21",&sHitADC_Bin21);
   	tree_Hits->SetBranchAddress("adc22",&sHitADC_Bin22);
   	tree_Hits->SetBranchAddress("adc23",&sHitADC_Bin23);
   	tree_Hits->SetBranchAddress("adc24",&sHitADC_Bin24);
   	tree_Hits->SetBranchAddress("adc25",&sHitADC_Bin25);
   	tree_Hits->SetBranchAddress("adc26",&sHitADC_Bin26);
   	tree_Hits->SetBranchAddress("adc27",&sHitADC_Bin27);
    tree_Hits->SetBranchAddress("adc28",&sHitADC_Bin28);
    tree_Hits->SetBranchAddress("adc29",&sHitADC_Bin29);
    
    //Loop Over Events
    //------------------------------------------------------
    for (int i=pair_iEvtRange.first; i < pair_iEvtRange.second; ++i) { //Loop Over Events
        //Call reset/clear methods at the start of this iteration
        evt.clear();
        detMPGD.resetPhysObj();
        
        //Set the event number & run number
        //------------------------------------------------------
        evt.iNum_Evt = i;
        //evt.iNum_Run =
        
        //Determine Number of Hits
        //------------------------------------------------------
        //Make sure we only read the number of hits
        tree_Hits->SetBranchStatus("*",0);
        tree_Hits->SetBranchStatus("nch",1);
        
        //Get the number of hits
        tree_Hits->GetEntry(i);
        
        //Output to the user some message that we are still running
        if (i % 1000 == 0) cout<< i <<" Events Analyzed\n";
        
        //Event Level Selection
        //------------------------------------------------------
        if ( !(aSetup.selHit.iCut_MultiMin <= iHitMulti && iHitMulti <= aSetup.selHit.iCut_MultiMax) ) continue;
        
        //Get & Select Physics Objects - Hits
        //------------------------------------------------------
        tree_Hits->SetBranchStatus("*",1);
        
        //Now get the remaining data
        tree_Hits->GetEntry(i);
        
        //Loop Over the elements of the hit array (yes it must be done like this due to how hte NTuple from AMORE is created)
        //For each element create a hit, and check if it passes the selection
        for (int j=0; j < iHitMulti; ++j) { //Loop Over Number of Hits
            //Define the Hit
            Hit hitStrip;
            
            //Set the Hit info
            hitStrip.iPos_Y     = iHitPos_Y[j];
            hitStrip.iStripNum  = iHitStrip[j];
            hitStrip.iTimeBin   = iHitTimeBin[j];
            
            //I really cry inside because of this
            hitStrip.vec_sADC[0] = sHitADC_Bin0[j];
            hitStrip.vec_sADC[1] = sHitADC_Bin1[j];
            hitStrip.vec_sADC[2] = sHitADC_Bin2[j];
            hitStrip.vec_sADC[3] = sHitADC_Bin3[j];
            hitStrip.vec_sADC[4] = sHitADC_Bin4[j];
            hitStrip.vec_sADC[5] = sHitADC_Bin5[j];
            hitStrip.vec_sADC[6] = sHitADC_Bin6[j];
            hitStrip.vec_sADC[7] = sHitADC_Bin7[j];
            hitStrip.vec_sADC[8] = sHitADC_Bin8[j];
            hitStrip.vec_sADC[9] = sHitADC_Bin9[j];
            hitStrip.vec_sADC[10] = sHitADC_Bin10[j];
            hitStrip.vec_sADC[11] = sHitADC_Bin11[j];
            hitStrip.vec_sADC[12] = sHitADC_Bin12[j];
            hitStrip.vec_sADC[13] = sHitADC_Bin13[j];
            hitStrip.vec_sADC[14] = sHitADC_Bin14[j];
            hitStrip.vec_sADC[15] = sHitADC_Bin15[j];
            hitStrip.vec_sADC[16] = sHitADC_Bin16[j];
            hitStrip.vec_sADC[17] = sHitADC_Bin17[j];
            hitStrip.vec_sADC[18] = sHitADC_Bin18[j];
            hitStrip.vec_sADC[19] = sHitADC_Bin19[j];
            hitStrip.vec_sADC[20] = sHitADC_Bin20[j];
            hitStrip.vec_sADC[21] = sHitADC_Bin21[j];
            hitStrip.vec_sADC[22] = sHitADC_Bin22[j];
            hitStrip.vec_sADC[23] = sHitADC_Bin23[j];
            hitStrip.vec_sADC[24] = sHitADC_Bin24[j];
            hitStrip.vec_sADC[25] = sHitADC_Bin25[j];
            hitStrip.vec_sADC[26] = sHitADC_Bin26[j];
            hitStrip.vec_sADC[27] = sHitADC_Bin27[j];
            hitStrip.vec_sADC[28] = sHitADC_Bin28[j];
            hitStrip.vec_sADC[29] = sHitADC_Bin29[j];
            
            //Add the ADC bins up
            hitStrip.sADCIntegral = std::accumulate(hitStrip.vec_sADC.begin(), hitStrip.vec_sADC.end(), 0);
            
            //If the hit fails to pass the selection; skip it
            //---------------Hit Selection---------------
            if ( !hitSelector.hitPassesSelection(hitStrip) ) continue;
            
            //If a hit makes it here, store it in the detector
            evt.vec_hits.push_back(hitStrip);
        } //End Loop Over Number of hits
        
        //Give the Event to detMPG to store the Physics Objects
        //------------------------------------------------------
        detMPGD.setEventOnlyHits(evt);
        
        //Fill Distributions & Update Detector - Hits
        //------------------------------------------------------
        hitAnalyzer.setDetector(detMPGD);
        hitAnalyzer.fillHistos();
        
        detMPGD = hitAnalyzer.getDetector();
    } //End Loop Over Events
    
    //Close the Input ROOT File
    //------------------------------------------------------
    file_ROOT->Close();
    
    return;
} //End InterfaceRun::analyzeAmoreSRSRunHitsOnly()

//Analyze Run - Hits & Clusters, Re-reco Clusters
void InterfaceRun::analyzeAmoreSRSRunFull(string strInputRun){
    
    //Placeholder for now
    
} //End InterfaceRun::analyzeAmoreSRSRunFull()

//Analyze Run - Hits & Clusters
void InterfaceRun::analyzeAmoreSRSRunFullNoReReco(string strInputRun){
    //Variable Declaration
    //bool bFrmwrkTree;

    Event evt;
    
    std::pair<int,int> pair_iEvtRange;
    
    TFile *file_ROOT = NULL;
    
    //TTree *tree_Clusters = NULL, tree_Events = NULL, tree_Hits = NULL;
    TTree *tree_Clusters = NULL, *tree_Hits = NULL;
    
    //For TCluster in amoreSRS
    Int_t iClustMulti;  //I cry a little inside because of this
    Int_t iClustPos_Y[3072];
    Int_t iClustSize[3072];
    Int_t iClustTimeBin[3072];
    
    Float_t fClustPos_X[3072];
    Float_t fClustADC[3072];
    
    //For THit in amoreSRS
    Int_t iHitMulti;  //I cry a little inside because of this
    Int_t iHitPos_Y[3072];
    Int_t iHitStrip[3072];
    Int_t iHitTimeBin[3072];
    
    Short_t sHitADC_Bin0[3072];
    Short_t sHitADC_Bin1[3072];
    Short_t sHitADC_Bin2[3072];
    Short_t sHitADC_Bin3[3072];
    Short_t sHitADC_Bin4[3072];
    Short_t sHitADC_Bin5[3072];
    Short_t sHitADC_Bin6[3072];
    Short_t sHitADC_Bin7[3072];
    Short_t sHitADC_Bin8[3072];
    Short_t sHitADC_Bin9[3072];
    Short_t sHitADC_Bin10[3072];
    Short_t sHitADC_Bin11[3072];
    Short_t sHitADC_Bin12[3072];
    Short_t sHitADC_Bin13[3072];
    Short_t sHitADC_Bin14[3072];
    Short_t sHitADC_Bin15[3072];
    Short_t sHitADC_Bin16[3072];
    Short_t sHitADC_Bin17[3072];
    Short_t sHitADC_Bin18[3072];
    Short_t sHitADC_Bin19[3072];
    Short_t sHitADC_Bin20[3072];
    Short_t sHitADC_Bin21[3072];
    Short_t sHitADC_Bin22[3072];
    Short_t sHitADC_Bin23[3072];
    Short_t sHitADC_Bin24[3072];
    Short_t sHitADC_Bin25[3072];
    Short_t sHitADC_Bin26[3072];
    Short_t sHitADC_Bin27[3072];
    Short_t sHitADC_Bin28[3072];
    Short_t sHitADC_Bin29[3072];
    
    //Open this run's root file
    //------------------------------------------------------
    file_ROOT = new TFile(strInputRun.c_str(),"READ","",1);
    
    //Check to see if data file opened successfully
    //------------------------------------------------------
    if ( !file_ROOT->IsOpen() || file_ROOT->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("Uniformity::SelectorCluster::setClusters() - error while opening file: " + strInputRun ).c_str() );
        Timing::printROOTFileStatus(file_ROOT);
        std::cout << "Exiting!!!\n";
        
        return;
    } //End Case: failed to load ROOT file
    
    //Load the TTree(s)
    //------------------------------------------------------
    tree_Hits = (TTree*) file_ROOT->Get("THit");
    tree_Clusters = (TTree*) file_ROOT->Get("TCluster");
    
    if ( nullptr == tree_Clusters ) { //Case: failed to load TTree
        printClassMethodMsg("InterfaceRun","analyzeEventFullNoReco",("error while fetching: " + (string) file_ROOT->GetName() ).c_str() );
        printClassMethodMsg("InterfaceRun","analyzeEventFullNoReco","\tCluster tree returns nullptr; Exiting!!!");
        
        return;
    } //End Case: failed to load TTree
    else if ( nullptr == tree_Hits ) { //Case: failed to load TTree
        printClassMethodMsg("InterfaceRun","analyzeEventFullNoReco",("error while fetching: " + (string) file_ROOT->GetName() ).c_str() );
        printClassMethodMsg("InterfaceRun","analyzeEventFullNoReco","\tHit tree returns nullptr; Exiting!!!");
        
        return;
    } //End Case: failed to load TTree
    else if( tree_Clusters->GetEntries() != tree_Hits->GetEntries() ){ //Case: Different Number of Events, Problem!
        printClassMethodMsg("InterfaceRun","analyzeEventFullNoReco",("error while fetching: " + (string) file_ROOT->GetName() ).c_str() );
        printClassMethodMsg("InterfaceRun","analyzeEventFullNoReco","\tEntries in cluster tree != entries in hit tree; Exiting!!!");
        
        return;
    } //End Case: Different Number of Events, Problem!
    
    //Determine Event Range for Analysis
    //------------------------------------------------------
    pair_iEvtRange = getEventRange( aSetup.iEvt_First, aSetup.iEvt_Total, tree_Clusters->GetEntries() );
    
    //Set Branch Addresses - Hits
    //------------------------------------------------------
    tree_Hits->SetBranchAddress("hitTimebin",&iHitTimeBin);
    tree_Hits->SetBranchAddress("nch", &iHitMulti);
    tree_Hits->SetBranchAddress("planeID",&iHitPos_Y);
    tree_Hits->SetBranchAddress("strip",&iHitStrip);
    
    //ADC
    tree_Hits->SetBranchAddress("adc0",&sHitADC_Bin0);
    tree_Hits->SetBranchAddress("adc1",&sHitADC_Bin1);
   	tree_Hits->SetBranchAddress("adc2",&sHitADC_Bin2);
   	tree_Hits->SetBranchAddress("adc3",&sHitADC_Bin3);
   	tree_Hits->SetBranchAddress("adc4",&sHitADC_Bin4);
   	tree_Hits->SetBranchAddress("adc5",&sHitADC_Bin5);
   	tree_Hits->SetBranchAddress("adc6",&sHitADC_Bin6);
   	tree_Hits->SetBranchAddress("adc7",&sHitADC_Bin7);
   	tree_Hits->SetBranchAddress("adc8",&sHitADC_Bin8);
   	tree_Hits->SetBranchAddress("adc9",&sHitADC_Bin9);
   	tree_Hits->SetBranchAddress("adc10",&sHitADC_Bin10);
   	tree_Hits->SetBranchAddress("adc11",&sHitADC_Bin11);
   	tree_Hits->SetBranchAddress("adc12",&sHitADC_Bin12);
   	tree_Hits->SetBranchAddress("adc13",&sHitADC_Bin13);
   	tree_Hits->SetBranchAddress("adc14",&sHitADC_Bin14);
   	tree_Hits->SetBranchAddress("adc15",&sHitADC_Bin15);
   	tree_Hits->SetBranchAddress("adc16",&sHitADC_Bin16);
   	tree_Hits->SetBranchAddress("adc17",&sHitADC_Bin17);
   	tree_Hits->SetBranchAddress("adc18",&sHitADC_Bin18);
   	tree_Hits->SetBranchAddress("adc19",&sHitADC_Bin19);
   	tree_Hits->SetBranchAddress("adc20",&sHitADC_Bin20);
   	tree_Hits->SetBranchAddress("adc21",&sHitADC_Bin21);
   	tree_Hits->SetBranchAddress("adc22",&sHitADC_Bin22);
   	tree_Hits->SetBranchAddress("adc23",&sHitADC_Bin23);
   	tree_Hits->SetBranchAddress("adc24",&sHitADC_Bin24);
   	tree_Hits->SetBranchAddress("adc25",&sHitADC_Bin25);
   	tree_Hits->SetBranchAddress("adc26",&sHitADC_Bin26);
   	tree_Hits->SetBranchAddress("adc27",&sHitADC_Bin27);
    tree_Hits->SetBranchAddress("adc28",&sHitADC_Bin28);
    tree_Hits->SetBranchAddress("adc29",&sHitADC_Bin29);
    
    //Set Branch Addresses - Clusters
    //------------------------------------------------------
    tree_Clusters->SetBranchAddress("nclust", &iClustMulti);
    tree_Clusters->SetBranchAddress("clustPos",&fClustPos_X);
    tree_Clusters->SetBranchAddress("clustSize",&iClustSize);
    tree_Clusters->SetBranchAddress("clustADCs",&fClustADC);
    tree_Clusters->SetBranchAddress("clustTimebin",&iClustTimeBin);
    tree_Clusters->SetBranchAddress("planeID",&iClustPos_Y);
    
    //Loop Over Events
    //------------------------------------------------------
    for (int i=pair_iEvtRange.first; i < pair_iEvtRange.second; ++i) { //Loop Over Events
        //Call reset/clear methods at the start of this iteration
        evt.clear();
        detMPGD.resetPhysObj();
        
        //Set the event number & run number
        //------------------------------------------------------
        evt.iNum_Evt = i;
        //evt.iNum_Run =
        
        //Determine Number of Hits & Clusters
        //------------------------------------------------------
        //Make sure we only read the number of hits
        tree_Hits->SetBranchStatus("*",0);
        tree_Hits->SetBranchStatus("nch",1);
        
        //Get the number of hits
        tree_Hits->GetEntry(i);
        
        //Make sure we only read the number of clusters
        tree_Clusters->SetBranchStatus("*",0);
        tree_Clusters->SetBranchStatus("nclust",1);
        
        //Get the number of clusters
        tree_Clusters->GetEntry(i);
        
        //Output to the user some message that we are still running
        if (i % 1000 == 0) cout<< i <<" Events Analyzed\n";
        
        //Event Level Selection
        //------------------------------------------------------
        if ( !(aSetup.selHit.iCut_MultiMin <= iHitMulti && iHitMulti <= aSetup.selHit.iCut_MultiMax) ) continue;
        if ( !(aSetup.selClust.iCut_MultiMin < iClustMulti && iClustMulti < aSetup.selClust.iCut_MultiMax) ) continue;
        
        //Get & Select Physics Objects - Hits
        //------------------------------------------------------
        tree_Hits->SetBranchStatus("*",1);

        //Now get the remaining data
        tree_Hits->GetEntry(i);
        
        //Loop Over the elements of the hit array (yes it must be done like this due to how hte NTuple from AMORE is created)
        //For each element create a hit, and check if it passes the selection
        for (int j=0; j < iHitMulti; ++j) { //Loop Over Number of Hits
            //Define the Hit
            Hit hitStrip;
            
            //Set the Hit info
            hitStrip.iPos_Y     = iHitPos_Y[j];
            hitStrip.iStripNum  = iHitStrip[j];
            hitStrip.iTimeBin   = iHitTimeBin[j];
            
            //I really cry inside because of this
            hitStrip.vec_sADC[0] = sHitADC_Bin0[j];
            hitStrip.vec_sADC[1] = sHitADC_Bin1[j];
            hitStrip.vec_sADC[2] = sHitADC_Bin2[j];
            hitStrip.vec_sADC[3] = sHitADC_Bin3[j];
            hitStrip.vec_sADC[4] = sHitADC_Bin4[j];
            hitStrip.vec_sADC[5] = sHitADC_Bin5[j];
            hitStrip.vec_sADC[6] = sHitADC_Bin6[j];
            hitStrip.vec_sADC[7] = sHitADC_Bin7[j];
            hitStrip.vec_sADC[8] = sHitADC_Bin8[j];
            hitStrip.vec_sADC[9] = sHitADC_Bin9[j];
            hitStrip.vec_sADC[10] = sHitADC_Bin10[j];
            hitStrip.vec_sADC[11] = sHitADC_Bin11[j];
            hitStrip.vec_sADC[12] = sHitADC_Bin12[j];
            hitStrip.vec_sADC[13] = sHitADC_Bin13[j];
            hitStrip.vec_sADC[14] = sHitADC_Bin14[j];
            hitStrip.vec_sADC[15] = sHitADC_Bin15[j];
            hitStrip.vec_sADC[16] = sHitADC_Bin16[j];
            hitStrip.vec_sADC[17] = sHitADC_Bin17[j];
            hitStrip.vec_sADC[18] = sHitADC_Bin18[j];
            hitStrip.vec_sADC[19] = sHitADC_Bin19[j];
            hitStrip.vec_sADC[20] = sHitADC_Bin20[j];
            hitStrip.vec_sADC[21] = sHitADC_Bin21[j];
            hitStrip.vec_sADC[22] = sHitADC_Bin22[j];
            hitStrip.vec_sADC[23] = sHitADC_Bin23[j];
            hitStrip.vec_sADC[24] = sHitADC_Bin24[j];
            hitStrip.vec_sADC[25] = sHitADC_Bin25[j];
            hitStrip.vec_sADC[26] = sHitADC_Bin26[j];
            hitStrip.vec_sADC[27] = sHitADC_Bin27[j];
            hitStrip.vec_sADC[28] = sHitADC_Bin28[j];
            hitStrip.vec_sADC[29] = sHitADC_Bin29[j];
            
            //Add the ADC bins up
            hitStrip.sADCIntegral = std::accumulate(hitStrip.vec_sADC.begin(), hitStrip.vec_sADC.end(), 0);
            
            //If the hit fails to pass the selection; skip it
            //---------------Hit Selection---------------
            if ( !hitSelector.hitPassesSelection(hitStrip) ) continue;
            
            //If a hit makes it here, store it in the detector
            evt.vec_hits.push_back(hitStrip);
        } //End Loop Over Number of hits
        
        //Get & Select Physics Objects - Clusters
        //------------------------------------------------------
        //Okay make sure we can read all branches
        tree_Clusters->SetBranchStatus("*",1);
        
        //Now get the remaining data
        tree_Clusters->GetEntry(i);
        
        for (int j=0; j < iClustMulti; ++j) { //Loop Over Number of Clusters
            //Define the Cluster
            Cluster clust;
            
            //Set the cluster info
            clust.iPos_Y = iClustPos_Y[j];
            clust.fPos_X = fClustPos_X[j];
            
            clust.fADC = fClustADC[j];
            
            clust.iSize = iClustSize[j];
            
            clust.iTimeBin = iClustTimeBin[j];
            
            //If the cluster fails to pass the selection; skip it
            //---------------Cluster Selection---------------
            if ( !clustSelector.clusterPassesSelection(clust) ) continue;
            
            //If a cluster makes it here, store it in the detector
            evt.vec_clusters.push_back(clust);
        } //End Loop Over Number of Clusters
        
        //Give the Event to detMPG to store the Physics Objects
        //------------------------------------------------------
        detMPGD.setEvent(evt);
        
        //Fill Distributions & Update Detector - Hits
        //------------------------------------------------------
        hitAnalyzer.setDetector(detMPGD);
        hitAnalyzer.fillHistos();
        
        detMPGD = hitAnalyzer.getDetector();
        
        //Fill Distributions & Update Detector - Clusters
        //------------------------------------------------------
        clustAnalyzer.setDetector(detMPGD);
        clustAnalyzer.fillHistos();
        
        detMPGD = clustAnalyzer.getDetector();
    } //End Loop Over Events

    //Close the Input ROOT File
    //------------------------------------------------------
    file_ROOT->Close();
    
    return;
} //End InterfaceRun::analyzeAmoreSRSRunFullNoReReco()

