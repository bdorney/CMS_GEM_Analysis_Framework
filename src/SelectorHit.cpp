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

using std::array;
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
    
    //vector<array<Short_t, 3072> > vec_sHitADC;

    //Hit hitStrip;
    
	//I cry a lot more because of this...
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
    //for (int i=aSetupUniformity.selHit.iCut_TimeMin; i<=aSetupUniformity.selHit.iCut_TimeMax; ++i) { //Set Relevant Time Bins
    //for (int i=0; i<= 29; ++i) { //Set All Time Bins
        //array<Short_t, 3072> arr_sIthTimeBin;        

        //vec_sHitADC.push_back(arr_sIthTimeBin);
        //tree_Hits->SetBranchAddress( ("adc" + Timing::getString(i) ).c_str(), &vec_sHitADC[i]);
    //} //End Set All Time Bins
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
		//Define the Hit
		Hit hitStrip;

            //Set the Hit info
            hitStrip.iPos_Y     = iHitPos_Y[j];
            hitStrip.iStripNum  = iHitStrip[j];
            hitStrip.iTimeBin   = iHitTimeBin[j];
            
            //Fill the ADC for this Hit
            //hitStrip.vec_sADC   = vec_sHitADC;
            //std::copy(std::begin(sHitADC), std::end(sHitADC), hitStrip.vec_sADC.begin() );
            //for (auto iterADCTimeBin = vec_sHitADC.begin(); iterADCTimeBin != vec_sHitADC.end(); ++iterADCTimeBin) { //Loop Over APV Time Bins
                //hitStrip.vec_sADC.push_back( (*iterADCTimeBin)[j] );
            //} //End Loop Over APV Time Bins
            
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
            
		//Debugging
		//cout<<"======================NEW HIT======================\n";
		//cout<<"k\tADC_Val\n";
		//for(int k=0; k<hitStrip.vec_sADC.size(); ++k){
			//cout<<k<<"\t"<<hitStrip.vec_sADC[k]<<endl;
		//}
		//cout<<"hitStrip.sADCIntegral = " << hitStrip.sADCIntegral << endl;

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
