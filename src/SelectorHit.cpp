//
//  SelectorHit.cpp
//  
//
//  Created by Brian L Dorney on 11/03/16.
//
//

//C++ Includes
#include "SelectorHit.h"

//Framework Includes

//ROOT Includes

using std::cout;
using std::endl;
using std::string;
using std::vector;

using namespace Uniformity;
using namespace Timing;

//Default Constructor
SelectorHit::SelectorHit(){
    
} //End Default Constructor

//Given an output ROOT file from amoreSRS with hits
//Applies the hit selection and stores those selected hits in inputDet
//Right now ADC information of hits is not yet supported
//Input is a std::string storing the physical filename
void SelectorHit::setHits(std::string &strInputRootFileName, Uniformity::DetectorMPGD &inputDet){
    //Variable Declaration
    //int iFirstEvt = aSetupUniformity.iEvt_First;
    //int iNEvt = aSetupUniformity.iEvt_Total;
    
    //Int_t iHitMulti;  //I cry a little inside because of this
    //Int_t iHitPos_Y[3072];
    //Int_t iHitStrip[3072];
    //Int_t iHitTimeBin[3072];
    
    //Short_t sHitADC[30] = {0};
    
    //Hit hitStrip;
    
    TFile *file_ROOT = NULL;
    //TTree *tree_Hits = NULL;
    
    //Open this run's root file
    //------------------------------------------------------
    file_ROOT = new TFile(strInputRootFileName.c_str(),"READ","",1);
    
    //Check to see if data file opened successfully, if so load the tree
    //------------------------------------------------------
    if ( !file_ROOT->IsOpen() || file_ROOT->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("Uniformity::SelectorHit::setHits() - error while opening file: " + strInputRootFileName ).c_str() );
        Timing::printROOTFileStatus(file_ROOT);
        std::cout << "Exiting!!!\n";
        
        return;
    } //End Case: failed to load ROOT file
    
    //Simplified to just call the method below
    /*tree_Hits = (TTree*) file_ROOT->Get("THit");
    
    if ( nullptr == tree_Hits ) { //Case: failed to load TTree
        printClassMethodMsg("SelectorHit","setHits",("error while fetching: " + strInputRootFileName ).c_str() );
        printClassMethodMsg("SelectorHit","setHits","\tTree returns nullptr; Exiting!!!");

	return;
    } //End Case: failed to load TTree
    
    //Initialize Tree Branch Address to retrieve the hit information (ADC values are done separately below)
    //------------------------------------------------------
    for (int i=aSetupUniformity.selHit.iCut_TimeMin; i<=aSetupUniformity.selHit.iCut_TimeMax; ++i) { //Set Relevant Time Bins
        tree_Hits->SetBranchAddress( ("adc" + Timing::getString(i) ).c_str(), &sHitADC[i]);
    } //End Set Relevant Time Bins
    tree_Hits->SetBranchAddress("hitTimebin",&iHitTimeBin);
    tree_Hits->SetBranchAddress("nch", &iHitMulti);
    tree_Hits->SetBranchAddress("planeID",&iHitPos_Y);
    tree_Hits->SetBranchAddress("strip",iHitStrip);
    
    //Determine Event Range
    //------------------------------------------------------
    if ( -1 == iNEvt ) { //Case: All Events
        iFirstEvt = 0;
        iNEvt = tree_Hits->GetEntries();
    } //End Case: All Events
    else{ //Case: Event Range
        if ( iFirstEvt > tree_Hits->GetEntries() ) { //Case: Incorrect Event Range, 1st Event Requested Beyond All Events
            printClassMethodMsg("SelectorHit","setHits", ("Error, First Event Requested as " + Timing::getString( aSetupUniformity.iEvt_First ) + " Greater Thant Total Number of Events " + Timing::getString( tree_Hits->GetEntries() ) ).c_str() );
            printClassMethodMsg("SelectorHit","setHits", "Exiting!!!");
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
        if ( !(aSetupUniformity.selHit.iCut_MultiMin <= iHitMulti && iHitMulti <= aSetupUniformity.selHit.iCut_MultiMax) ) continue;
        
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
            //hitStrip.vec_sADC   = vec_sHitADC;
            
            std::copy(std::begin(sHitADC), std::end(sHitADC), hitStrip.vec_sADC.begin() );

            //If the hit fails to pass the selection; skip it
            //---------------Hit Selection---------------
            if ( !hitPassesSelection(hitStrip) ) continue;
            
            //If a hit makes it here, store it in the detector
            inputDet.setHit(hitStrip);
        } //End Loop Over Number of hits
    } //End Loop Over "Events"
    */
     
	//Clear stl containers? (Not doing this seems to cause some pointer to be freed)
    //------------------------------------------------------
    //  Placeholder
    
    //Call setHits()
    //------------------------------------------------------
    setHits(file_ROOT, inputDet);
    
	//Close the Input ROOT File
    //------------------------------------------------------
	file_ROOT->Close();

    return;
} //End SelectorHit::setHits()

//Given an output ROOT file from amoreSRS with hits
//Applies the hit selection and stores those selected hits in inputDet
//Right now ADC information of hits is not yet supported
//Input is a TFile *
void SelectorHit::setHits(TFile * file_InputRootFile, Uniformity::DetectorMPGD &inputDet){
    //Variable Declaration
    int iFirstEvt = aSetupUniformity.iEvt_First;
    int iNEvt = aSetupUniformity.iEvt_Total;
    
    Int_t iHitMulti;  //I cry a little inside because of this
    Int_t iHitPos_Y[3072];
    Int_t iHitStrip[3072];
    Int_t iHitTimeBin[3072];
    
    Short_t sHitADC[30] = {0};
    
    Hit hitStrip;
    
    TTree *tree_Hits = NULL;
    
    //Check to see if data file opened successfully, if so load the tree
    //------------------------------------------------------
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("Uniformity::SelectorHit::setHits() - error while opening file: " + (string)file_InputRootFile->GetName() ).c_str() );
        Timing::printROOTFileStatus(file_InputRootFile);
        std::cout << "Exiting!!!\n";
        
        return;
    } //End Case: failed to load ROOT file
    
    tree_Hits = (TTree*) file_InputRootFile->Get("THit");
    
    if ( nullptr == tree_Hits ) { //Case: failed to load TTree
        printClassMethodMsg("SelectorHit","setHits",("error while fetching: " + (string) file_InputRootFile->GetName() ).c_str() );
        printClassMethodMsg("SelectorHit","setHits","\tTree returns nullptr; Exiting!!!");
        
        return;
    } //End Case: failed to load TTree
    
    //Initialize Tree Branch Address to retrieve the hit information (ADC values are done separately below)
    //------------------------------------------------------
    for (int i=aSetupUniformity.selHit.iCut_TimeMin; i<=aSetupUniformity.selHit.iCut_TimeMax; ++i) { //Set Relevant Time Bins
        //NOTE: It looks like this is incorrectly setting the ADC,  sHitADC needs to be some how 2 dimensional
        //Right now sHitADC is being stored with the first hit of the event, and this being given to ALL hits in the event, then it's being updated on the next event
        //i.e. it is wrong.
        
        tree_Hits->SetBranchAddress( ("adc" + Timing::getString(i) ).c_str(), &sHitADC[i]);
    } //End Set Relevant Time Bins
    tree_Hits->SetBranchAddress("hitTimebin",&iHitTimeBin);
    tree_Hits->SetBranchAddress("nch", &iHitMulti);
    tree_Hits->SetBranchAddress("planeID",&iHitPos_Y);
    tree_Hits->SetBranchAddress("strip",iHitStrip);
    
    //Determine Event Range
    //------------------------------------------------------
    if ( -1 == iNEvt ) { //Case: All Events
        iFirstEvt = 0;
        iNEvt = tree_Hits->GetEntries();
    } //End Case: All Events
    else{ //Case: Event Range
        if ( iFirstEvt > tree_Hits->GetEntries() ) { //Case: Incorrect Event Range, 1st Event Requested Beyond All Events
            printClassMethodMsg("SelectorHit","setHits", ("Error, First Event Requested as " + Timing::getString( aSetupUniformity.iEvt_First ) + " Greater Thant Total Number of Events " + Timing::getString( tree_Hits->GetEntries() ) ).c_str() );
            printClassMethodMsg("SelectorHit","setHits", "Exiting!!!");
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
        if ( !(aSetupUniformity.selHit.iCut_MultiMin <= iHitMulti && iHitMulti <= aSetupUniformity.selHit.iCut_MultiMax) ) continue;
        
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
            //hitStrip.vec_sADC   = vec_sHitADC;
            
            std::copy(std::begin(sHitADC), std::end(sHitADC), hitStrip.vec_sADC.begin() );
            
            //If the hit fails to pass the selection; skip it
            //---------------Hit Selection---------------
            if ( !hitPassesSelection(hitStrip) ) continue;
            
            //If a hit makes it here, store it in the detector
            inputDet.setHit(hitStrip);
        } //End Loop Over Number of hits
    } //End Loop Over "Events"
    
    //Clear stl containers? (Not doing this seems to cause some pointer to be freed)
    //------------------------------------------------------
    //  Placeholder
    
    //Do not close the input TFile, it is used elsewhere
    
    return;
} //End SelectorHit::setHits()

//Check if Hit Passes selection stored in aSetupUniformity? True -> Passes; False -> Fails
bool SelectorHit::hitPassesSelection(Uniformity::Hit &inputHit){
    //Hit Selection
    
    //Hit Time too early or too late?
    if (inputHit.iTimeBin < aSetupUniformity.selHit.iCut_TimeMin){ return false; }
    if (inputHit.iTimeBin > aSetupUniformity.selHit.iCut_TimeMax) {return false; }
    
    //Hit with ADC below noise threshold?
    if (inputHit.vec_sADC[inputHit.iTimeBin] < aSetupUniformity.selHit.iCut_ADCNoise){ return false; }
    
    //Hit with ADC above saturation threshold?
    if (inputHit.vec_sADC[inputHit.iTimeBin] > aSetupUniformity.selHit.iCut_ADCSat){ return false; }
    
    //If we arrive here the hit passes our selection; give true
    return true;
} //End SelectorHit::hitPassesSelection()
