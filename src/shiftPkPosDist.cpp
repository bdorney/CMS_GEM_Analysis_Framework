//C++
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>

//Framework Includes
#include "TimingUtilityFunctions.h"

//ROOT
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH1F.h"
#include "TROOT.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

using QualityControl::Timing::getCharSeparatedList;

int main(){
    //Set file path
    string strPFP = "/afs/cern.ch/user/d/dorney/scratch0/CMS_GEM/CMS_GEM_Analysis_Framework/data/sliceTestAna";

    //Set file names
    vector<string> vec_strFilenames;
    vec_strFilenames.push_back("GE11-VII-L-CERN-0001_Summary_Physics_Optimized_RandTrig_XRay40kV100uA_588uA_TimeCorr_DPGGeo_AnaWithFits.root");
    vec_strFilenames.push_back("GE11-VII-L-CERN-0002_Summary_Physics_Optimized_RandTrig_XRay40kV100uA_580uA_TimeCorr_DPGGeo_AnaWithFits.root");
    vec_strFilenames.push_back("GE11-VII-L-CERN-0003_Summary_Physics_Optimized_RandTrig_XRay40kV100uA_600uA_10115kEvt_TimeCorr_DPGGeo_AnaWithFits.root");
    //vec_strFilenames.push_back("GE11-VII-L-CERN-0004_Summary_Physics_Optimized_RandTrig_XRay40kV100uA_585uA_7pt4mm_9786kEvt_TimeCorr_DPGGeo_AnaWithFits.root");
    vec_strFilenames.push_back("GE11-VII-L-CERN-0004_Summary_Physics_Optimized_RandTrig_XRay40kV100uA_589uA_7pt2mm_10771kEvt_TimeCorr_DPGGeo_AnaWithFits.root");
    //vec_strFilenames.push_back("GE11-VII-L-CERN-0004_Summary_Physics_Optimized_RandTrig_XRay40kV100uA_615uA_7pt0mm_10217kEvt_TimeCorr_DPGGeo_AnaWithFits.root");
    vec_strFilenames.push_back("GE11-VII-S-CERN-0002_Summary_Physics_Optimized_RandTrig_XRay40kV100uA_580uA_TimeCorr_DPGGeo_AnaWithFits.root");
    vec_strFilenames.push_back("GE11-VII-S-CERN-0003_Summary_Physics_Optimized_RandTrig_XRay40kV100uA_580uA_TimeCorr_DPGGeo_AnaWithFits.root");
    
    //TFile doesn't manage TH1F objects
    TH1::AddDirectory(kFALSE);
    
    for (int i=0; i < vec_strFilenames.size(); ++i) { //Loop Over vec_strFilenames
        //Open the input file
        TFile * file_Input = new TFile( ( strPFP + "/" + vec_strFilenames[i] ).c_str(),"READ");
        
        //Get the input histogram
        TH1F *hSummaryDist = (TH1F*)file_Input->Get("Summary/h_Summary_ResponseFitPkPosDataset");
        
        int iNbinsX = hSummaryDist->GetNbinsX();
        
        //cout<<"====================NEW FILE====================\n";
        //cout<<"Filename = " << vec_strFilenames[i].c_str() << endl;
        //cout<<"\t\tVAR_INDEP,VAR_INDEP_ERR,VAR_DEP,VAR_DEP_ERR"<<endl;
        
        //Create the new plot and set all points
        TGraphErrors *gSummaryDistShifted = new TGraphErrors( iNbinsX+2 );
        
        for (int i=1; i<=iNbinsX; ++i) {
            //Print to user
            //cout<<"\t\t";
            //cout<<hSummaryDist->GetBinCenter(i)- hDataset->GetMean()<<",";
            //cout<<hSummaryDist->GetBinWidth(i)*0.5<<",";
            //cout<<hSummaryDist->GetBinContent(i)<<",";
            //cout<<hSummaryDist->GetBinError(i)*0.5<<endl;
            
            //Set values
            gSummaryDistShifted->SetPoint( i-1, hSummaryDist->GetBinCenter(i) - hSummaryDist->GetMean(), hSummaryDist->GetBinContent(i) );
            gSummaryDistShifted->SetPointError( i-1, 0., hSummaryDist->GetBinError(i) );
            
        } //End Loop Over bins of hSummaryDist
        
        //Add two fake points so we can change the x-axis at will
        gSummaryDistShifted->SetPoint(iNbinsX, -2100, 0);
        gSummaryDistShifted->SetPoint(iNbinsX+1, 2100, 0);
        
        //Get the input fit
        TF1 *fit_Summary_ResponseFitPkPosDataset_shifted = (TF1*)file_Input->Get("Summary/fit_Summary_ResponseFitPkPosDataset");
        
        //Shift the parameters
        float fXmax= fit_Summary_ResponseFitPkPosDataset_shifted->GetXmax();
        float fXmin= fit_Summary_ResponseFitPkPosDataset_shifted->GetXmin();
        fit_Summary_ResponseFitPkPosDataset_shifted->SetRange(fXmin - hSummaryDist->GetMean(), fXmax - hSummaryDist->GetMean() );
        fit_Summary_ResponseFitPkPosDataset_shifted->SetParameter(1, fit_Summary_ResponseFitPkPosDataset_shifted->GetParameter(1) - hSummaryDist->GetMean() );
        
        //Create the output file name
        vector<string> vec_strParsedFileName = getCharSeparatedList( vec_strFilenames[i], '_' );
        string strDetSerialNo = vec_strParsedFileName[0];
        
        TFile *file_Output = new TFile( (strDetSerialNo + "_PkPosDistShifted.root" ).c_str()  , "RECREATE");
        TDirectory *dir_Summary =file_Output->mkdir("Summary");
        dir_Summary->cd();
        gSummaryDistShifted->Write();
        fit_Summary_ResponseFitPkPosDataset_shifted->Write();
        
        //Close everything
        file_Input->Close();
        delete hSummaryDist;
        delete file_Input;
        
        file_Output->Close();
        delete gSummaryDistShifted;
        delete fit_Summary_ResponseFitPkPosDataset_shifted;
        //delete dir_Summary;
        delete file_Output;
    } //End Loop Over vec_strFilenames
    
    cout<<"Finished\n";
    
    return 0;
} //End main()


	
