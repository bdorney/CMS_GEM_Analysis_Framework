//
//  HitSelector.cpp
//  
//
//  Created by Brian L Dorney on 11/03/16.
//
//

//C++ Includes
#include "HitSelector.h"

//Framework Includes

//ROOT Includes

using std::cout;

using namespace Uniformity;

//Default Constructor
HitSelector::HitSelector(){
    
} //End Default Constructor

//Given an output ROOT file from amoreSRS with hits
//Applies the hit selection and stores those selected hits in inputDet
//Right now ADC information of hits is not yet supported
void HitSelector::setHits(std::string &strInputRootFileName, Uniformity::DetectorMPGD &inputDet){
    //Variable Declaration
    int iFirstEvt = aSetupUniformity.iEvt_First;
    int iNEvt = aSetupUniformity.iEvt_Total;
    
    Int_t iHitMulti;  //I cry a little inside because of this
    Int_t iHitPos_Y[3072];
    Int_t iHitStrip[3072];
    Int_t iHitTimeBin[3072];
    
    //Float_t fHitADC[3072];
    
    Hit hitStrip;
    
    TFile *file_ROOT = NULL;
    TTree *tree_Hits = NULL;
    
    //Open this run's root file
    //------------------------------------------------------
    file_ROOT = new TFile(strInputRootFileName.c_str(),"READ","",1);
    
    //Check to see if data file opened successfully, if so load the tree
    //------------------------------------------------------
    if ( !file_ROOT->IsOpen() || file_ROOT->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("Uniformity::HitSelector::setHits() - error while opening file: " + strInputRootFileName ).c_str() );
        Timing::printROOTFileStatus(file_ROOT);
        std::cout << "Exiting!!!\n";
        
        return;
    } //End Case: failed to load ROOT file
    
    tree_Hits = (TTree*) file_ROOT->Get("THit");
    
    if ( nullptr == tree_Hits ) { //Case: failed to load TTree
        printClassMethodMsg("HitSelector","setHits",("error while fetching: " + strInputRootFileName ).c_str() );
        printClassMethodMsg("HitSelector","setHits","\tTree returns nullptr; Exiting!!!");
    } //End Case: failed to load TTree
    
    //Initialize Tree Branch Address to retrieve the hit information
    //------------------------------------------------------
    tree_Hits->SetBranchAddress("nch", &iHitMulti);
    //tree_Hits->SetBranchAddress("adc?",&fHitADC); //Need some work
    tree_Hits->SetBranchAddress("planeID",&iHitPos_Y);
    tree_Hits->SetBranchAddress("strip",iHitStrip);
    tree_Hits->SetBranchAddress("hitTimebin",&iHitTimeBin);
    
    if ( -1 == iNEvt ) { //Case: All Events
        iFirstEvt = 0;
        iNEvt = tree_Hits->GetEntries();
    } //End Case: All Events
    else{ //Case: Event Range
        if ( iFirstEvt > tree_Hits->GetEntries() ) { //Case: Incorrect Event Range, 1st Event Requested Beyond All Events
            printClassMethodMsg("HitSelector","setHits", ("Error, First Event Requested as " + Timing::getString( aSetupUniformity.iEvt_First ) + " Greater Thant Total Number of Events " + Timing::getString( tree_Hits->GetEntries() ) ).c_str() );
            printClassMethodMsg("HitSelector","setHits", "Exiting!!!");
            return;
        } //End Case: Incorrect Event Range, 1st Event Requested Beyond All Events
        else if( (iFirstEvt + iNEvt) > tree_Hits->GetEntries() ){
            iNEvt = tree_Hits->GetEntries() - iFirstEvt;
        }
        else if( iFirstEvt < 0){
            iFirstEvt = 0;
        }
    } //End Case: Event Range
    
    //Get data event-by-event
    //------------------------------------------------------
    //for (int i=0; i < tree_Hits->GetEntries(); ++i) {
    for (int i=iFirstEvt; i < iNEvt; ++i) {
        //Needed to implement a Hack
        //First check to make sure the hit multiplicity is within the selection
        //Only then get the info on the hits
        
        //Make sure we only read the number of hits
        tree_Hits->SetBranchStatus("*",0);
        tree_Hits->SetBranchStatus("nch",1);
        
        //Get the number of hits
        tree_Hits->GetEntry(i);
        
        //Output to the user some message that we are still running
        if (i % 1000 == 0) cout<<"Hit Selection; " <<i<<" Events Analyzed\n";
        
        //If the event fails to pass the selection; skip it
        //---------------Event Selection---------------
        //Cut on number of hits
        if ( !(aSetupUniformity.selHit.iCut_MultiMin < iHitMulti && iHitMulti < aSetupUniformity.selHit.iCut_MultiMax) ) continue;
        
        //Okay make sure we can read all branches
        tree_Hits->SetBranchStatus("*",1);
        
        //Now get the remaining data
        tree_Hits->GetEntry(i);
        
        //Loop Over the elements of the hit array (yes it must be done like this due to how hte NTuple from AMORE is created)
        //For each element create a hit, and check if it passes the selection
        for (int j=0; j < iHitMulti; ++j) { //Loop Over Number of Hits
            //Set the Hit info
            hitStrip.iPos_Y     = iHitPos_Y[j];
            hitStrip.iStripNum  = iHitStrip[j];
            hitStrip.iTimeBin   = iHitTimeBin[j];
            
            //hitStrip.fADC = fHitADC[j];
            
            //If the hit fails to pass the selection; skip it
            //---------------Hit Selection---------------
            if ( !hitPassesSelection(hitStrip) ) continue;
            
            //If a hit makes it here, store it in the detector
            inputDet.setHit(hitStrip);
        } //End Loop Over Number of hits
    } //End Loop Over "Events"
    
    return;
} //End HitSelector::setHits()

//Check if Hit Passes selection stored in aSetupUniformity? True -> Passes; False -> Fails
bool HitSelector::hitPassesSelection(Uniformity::Hit &inputHit){
    //Hit Selection
    
    //Hit with ADC below noise threshold?
    //Not yet implemented
    //if (inputHit.fADC < aSetupUniformity.selHit.iCut_ADCNoise){ return false; }
    
    //Hit Time too early or too late?
    if (inputHit.iTimeBin < aSetupUniformity.selHit.iCut_TimeMin){ return false; }
    if (inputHit.iTimeBin > aSetupUniformity.selHit.iCut_TimeMax) {return false; }
    
    //If we arrive here the hit passes our selection; give true
    return true;
} //End HitSelector::hitPassesSelection()
