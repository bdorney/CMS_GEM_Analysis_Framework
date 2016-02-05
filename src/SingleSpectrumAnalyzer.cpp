//
//  SingleSpectrumAnalyzer.cpp
//  
//
//  Created by Brian L Dorney on 05/02/16.
//
//

//C++ Includes
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

//Framework Includes
#include "TimingUtilityFunctions.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes
#include "TDirectory.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TSpectrum.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

using Timing::getString;
using Timing::stoiSafe;

using namespace ROOT;

/*struct FitResults{
    int iNDF;
    
    float fAmp, fMean, fSigma;
    float fChi2;
    
    //Initialization
    FitResults(){
        iNDF = 1;
        
        fAmp = fMean = fSigma = fChi2 = -1.;
    }
};*/

//0 -> Executable
//1 -> Input TFile
//2 -> Full Physical Name of Histo, e.g. "Folder1/Folder2/.../FolderN/Histo"
//3 -> Num Iterations for Background
int main( int argc_, char * argv_[] ){
    //Variable Declaration
    int iNumIter;               //Test 1->iNumIter iterations for background finding;
    int iNumPeak;               //Number of Peaks Found
    
    Double_t *dPeakPos;         //X-Position of Peaks Found
    Double_t *dPeakAmp;         //Y-Position of Peaks Found
    
    TFile *file_ROOT = NULL;    //Input ROOT File
    TFile *file_Output = NULL;  //Output ROOT File
    TH1F *hSpec = NULL;         //Histogram to be loaded from file_ROOT containing spectrum
    //TH1F *hBKG = NULL;          //Background found in hSpec;
    //TH1F *hSpecNoBKG = NULL;    //Spectrum After Background subtraction;
    
    TGraphErrors *gIterImpact_PeakPos;// = new TGraphErrors( );
    TGraphErrors *gIterImpact_NormChi2;//= new TGraphErrors( );
    
    TSpectrum specAnalyzer;
    
    vector<string> vec_strInputArgs;    //Container for input arguments
    
    //Transfer Input Arguments into vec_strInputArgs
    //------------------------------------------------------
    vec_strInputArgs.resize(argc_);
    std::copy(argv_, argv_ + argc_, vec_strInputArgs.begin() );

    //Open the input root file
    //------------------------------------------------------
    file_ROOT = new TFile(vec_strInputArgs[1].c_str(),"READ","",1);
    
    //Check to see if data file opened successfully, if so load the histogram
    //------------------------------------------------------
    if ( !file_ROOT->IsOpen() || file_ROOT->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("SingleSpectrumAnalyzer() - error while opening file: " + vec_strInputArgs[1] ).c_str() );
        Timing::printROOTFileStatus(file_ROOT);
        std::cout << "Exiting!!!\n";
        
        return -1;
    } //End Case: failed to load ROOT file
    
    hSpec = (TH1F*) file_ROOT->Get( vec_strInputArgs[2].c_str() );

    if ( nullptr == hSpec ) { //Case: failed to load TTree
        cout<<"error while fetching " << vec_strInputArgs[2].c_str() << " from " << vec_strInputArgs[1] << endl;
        cout<<"Exiting!!!\n";
        
        return -1;
    } //End Case: failed to load TTree
    
    //Find the peaks in spectrum
    //------------------------------------------------------
    iNumPeak = specAnalyzer.Search(hSpec, 2, "", 0.5 );
    dPeakPos = specAnalyzer.GetPositionX();
    
    if (iNumPeak != 1) { //Check On Number of Found Peaks
        cout<<"Error I am looking for only 1 Peak but Found: " << iNumPeak << endl;;
        cout<<"Cross-check specAnalyzer Parameters\n";
        
        return -2;
    } //End Check On Number of Found Peaks
    
    //Setup Output File
    //------------------------------------------------------
    file_Output = new TFile("singleSpecAna.root","RECREATE","",1 );
    //file_Output->cd();

    //Iteratively Find the Background And see Which is Best
    //------------------------------------------------------
    iNumIter = stoiSafe("INTERNAL",vec_strInputArgs[3]);
    
    gIterImpact_PeakPos = new TGraphErrors(iNumIter);   gIterImpact_PeakPos->SetName("gIterImpact_PeakPos");
    gIterImpact_NormChi2= new TGraphErrors(iNumIter);  gIterImpact_NormChi2->SetName("gIterImpact_NormChi2");

    for (int i=1; i <= iNumIter; ++i) { //Background Testing Loop
        //Get the Background
        TH1F *hBKG = (TH1F*) specAnalyzer.Background(hSpec,i,"");
        
        //Subtract the background from the spectrum
        TH1F *hSpecNoBKG = (TH1F*) hSpec->Clone("hSpecNoBKG");
        
        hSpecNoBKG->Add(hBKG,-1.);
        
        //Declare the Fit;
        TF1 *fit_SpecNoBKG = new TF1("fit_SpecNoBKG","[0]*exp(-0.5*((x-[1])/[2])**2)",hSpec->GetBinLowEdge(1), hSpec->GetBinLowEdge( hSpec->GetNbinsX() ) );
        
        //Set the initial Fit Parmaeters
        //fit_SpecNoBKG->SetParameter(0, dPeakAmp[0] );
        fit_SpecNoBKG->SetParameter(1, dPeakPos[0] );

        //Set bounds on the initial parmeters
        fit_SpecNoBKG->SetParLimits(0,0,1e12);
        fit_SpecNoBKG->SetParLimits(1,0,15000);
	
        //Perform Fit
        hSpecNoBKG->Fit(fit_SpecNoBKG,"QMR","",dPeakPos[0]-600, dPeakPos[0]+600);
        
        //Store
        gIterImpact_PeakPos->SetPoint(i, i, fit_SpecNoBKG->GetParameter(1) / dPeakPos[0] );
        gIterImpact_PeakPos->SetPointError(i, 0, fit_SpecNoBKG->GetParError(1) / dPeakPos[0] );
        
        gIterImpact_NormChi2->SetPoint(i, i, fit_SpecNoBKG->GetChisquare() / fit_SpecNoBKG->GetNDF() );

        cout<<i<<"\t"<<dPeakPos[0]<<"\t"<<fit_SpecNoBKG->GetParameter(1)<<"\t"<<"+/-"<<fit_SpecNoBKG->GetParameter(2)<<"\t"<<fit_SpecNoBKG->GetChisquare() / fit_SpecNoBKG->GetNDF()<<endl;

        TDirectory *dir_ThisIter = file_Output->mkdir( ("Iteration_" + getString(i) ).c_str() );
        dir_ThisIter->cd();
        hBKG->Write();
        hSpecNoBKG->Write();
        fit_SpecNoBKG->Write();
        
        //delete fit_SpecNoBKG;
    } //End Background Testing Loop
    
    file_Output->cd();
    hSpec->Write();
    gIterImpact_PeakPos->Write();
    gIterImpact_NormChi2->Write();
    
    file_Output->Close();
    file_ROOT->Close();
    
    return 1;
} //End main()
