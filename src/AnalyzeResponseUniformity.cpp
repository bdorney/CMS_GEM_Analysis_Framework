//
//  AnalyzeResponseUniformity.cpp
//  
//
//  Created by Brian L Dorney on 28/01/16.
//
//

#include "DetectorMPGD.h"
#include "AnalyzeResponseUniformity.h"

using std::cout;
using std::endl;
using std::make_shared;
using std::map;
using std::shared_ptr;
using std::string;
using std::vector;

using QualityControl::Plotter::InfoFit;

using QualityControl::Timing::getString;
using QualityControl::Timing::printROOTFileStatus;
using QualityControl::Timing::HistoSetup;
using QualityControl::Timing::stofSafe;

using namespace QualityControl::Uniformity;

//Default Constructor
AnalyzeResponseUniformity::AnalyzeResponseUniformity(){
    strAnalysisName = "analysis";
} //End Default Constructor

//Set inputs at construction
AnalyzeResponseUniformity::AnalyzeResponseUniformity(AnalysisSetupUniformity inputSetup, DetectorMPGD & inputDet){
    strAnalysisName = "analysis";
    
    //Store Analysis Parameters
    aSetup = inputSetup;
    
    //Store Detector
    detMPGD = inputDet;
} //End Constructor

void AnalyzeResponseUniformity::calcStatistics(SummaryStatistics &inputStatObs, std::multiset<float> &mset_fInputObs, string strObsName){
    //Variable Declaration;
    std::vector<float> vec_fInputObsVariance;
    
    std::multiset<float>::iterator iterQ1 = mset_fInputObs.begin();
    std::multiset<float>::iterator iterQ2 = mset_fInputObs.begin();
    std::multiset<float>::iterator iterQ3 = mset_fInputObs.begin();
    
    //Determine max, min, & mean
    inputStatObs.fMax   = *mset_fInputObs.rbegin();   //Last member of the multiset
    inputStatObs.fMin   = *mset_fInputObs.begin();    //First member of the multiset
    inputStatObs.fMean  = std::accumulate( mset_fInputObs.begin(), mset_fInputObs.end(), 0. ) / mset_fInputObs.size();
    
    //Determine standard deviation
    float fAvg = inputStatObs.fMean;
    vec_fInputObsVariance.resize( mset_fInputObs.size() );
    std::transform(mset_fInputObs.begin(), mset_fInputObs.end(), vec_fInputObsVariance.begin(), [fAvg](float x) { return x - fAvg; });
    inputStatObs.fStdDev = std::sqrt( std::inner_product(vec_fInputObsVariance.begin(), vec_fInputObsVariance.end(), vec_fInputObsVariance.begin(), 0.0) / vec_fInputObsVariance.size() );
    
    //Determine Q1, Q2, & Q3
    std::advance(iterQ1, (int)std::ceil( mset_fInputObs.size() * 0.25 ) );  inputStatObs.fQ1 = *iterQ1;
    std::advance(iterQ2, (int)std::ceil( mset_fInputObs.size() * 0.50 ) );  inputStatObs.fQ2 = *iterQ2;
    std::advance(iterQ3, (int)std::ceil( mset_fInputObs.size() * 0.75 ) );  inputStatObs.fQ3 = *iterQ3;
    
    //Determine IQR
    inputStatObs.fIQR = inputStatObs.fQ3 - inputStatObs.fQ1;
    
    //Make distribution
    inputStatObs.hDist = std::make_shared<TH1F>( TH1F( getNameByIndex(-1, -1, -1, "h", strObsName + "Dataset" ).c_str(), "", 40, inputStatObs.fMean - 5. * inputStatObs.fStdDev, inputStatObs.fMean + 5. * inputStatObs.fStdDev) );
    inputStatObs.hDist->Sumw2();
    inputStatObs.hDist->GetXaxis()->SetTitle( strObsName.c_str() );
    inputStatObs.hDist->GetYaxis()->SetTitle( "N" );
    
    //Fill distribution & determine all outliers
    float fLowerBound = inputStatObs.fQ1 - 1.5 * inputStatObs.fIQR;
    float fUpperBound = inputStatObs.fQ3 + 1.5 * inputStatObs.fIQR;
    inRange inDataRange(fLowerBound, fUpperBound);
    
    inputStatObs.mset_fOutliers = mset_fInputObs;
    for (auto iterSet = inputStatObs.mset_fOutliers.begin(); iterSet != inputStatObs.mset_fOutliers.end(); ++iterSet) { //Loop Over input multiset
        inputStatObs.hDist->Fill( (*iterSet) );
        
        /*if (inDataRange( (*iterSet) ) ) {
         iterSet = inputStatObs.mset_fOutliers.erase(iterSet);
         }*/
    } //End Loop Over input multiset
    
    //Fit distribution
    //inputStatObs.fitDist = std::make_shared<TF1>( TF1( getNameByIndex(-1, -1, -1, "fit", strObsName + "Dataset" ).c_str(), "gaus(0)", inputStatObs.fMean - 5. * inputStatObs.fStdDev, inputStatObs.fMean + 5. * inputStatObs.fStdDev) );
    //inputStatObs.fitDist->SetParameter(0, inputStatObs.hDist->GetBinContent( inputStatObs.hDist->GetMaximumBin() ) );
    //inputStatObs.fitDist->SetParameter(1, inputStatObs.hDist->GetMean() );
    //inputStatObs.fitDist->SetParameter(2, inputStatObs.hDist->GetRMS() );
    
    //inputStatObs.hDist->Fit(inputStatObs.fitDist.get(),"QM","", inputStatObs.fMean - 5. * inputStatObs.fStdDev, inputStatObs.fMean + 5. * inputStatObs.fStdDev );
    
    shared_ptr<TF1> fitDist_Gaus = std::make_shared<TF1>( TF1( getNameByIndex(-1, -1, -1, "fit", strObsName + "Dataset" ).c_str(), "gaus(0)", inputStatObs.fMean - 5. * inputStatObs.fStdDev, inputStatObs.fMean + 5. * inputStatObs.fStdDev) );
    fitDist_Gaus->SetParameter(0, inputStatObs.hDist->GetBinContent( inputStatObs.hDist->GetMaximumBin() ) );
    fitDist_Gaus->SetParameter(1, inputStatObs.hDist->GetMean() );
    fitDist_Gaus->SetParameter(2, inputStatObs.hDist->GetRMS() );
    
    inputStatObs.hDist->Fit(fitDist_Gaus.get(),"QM","", inputStatObs.fMean - 5. * inputStatObs.fStdDev, inputStatObs.fMean + 5. * inputStatObs.fStdDev );
    
    shared_ptr<TF1> fitDist_Landau = std::make_shared<TF1>( TF1( getNameByIndex(-1, -1, -1, "fit", strObsName + "Dataset" ).c_str(), "landau(0)", inputStatObs.fMean - 5. * inputStatObs.fStdDev, inputStatObs.fMean + 5. * inputStatObs.fStdDev) );
    fitDist_Landau->SetParameter(0, inputStatObs.hDist->GetBinContent( inputStatObs.hDist->GetMaximumBin() ) );
    fitDist_Landau->SetParameter(1, inputStatObs.hDist->GetMean() );
    fitDist_Landau->SetParameter(2, inputStatObs.hDist->GetRMS() );
    
    inputStatObs.hDist->Fit(fitDist_Landau.get(),"QM","", inputStatObs.fMean - 5. * inputStatObs.fStdDev, inputStatObs.fMean + 5. * inputStatObs.fStdDev );
    
    float fNormChi2_Gaus = fitDist_Gaus->GetChisquare() / fitDist_Gaus->GetNDF();
    float fNormChi2_Landau = fitDist_Landau->GetChisquare() / fitDist_Landau->GetNDF();
    //bool bNormChi2Bad_Gaus = (std::isinf(fNormChi2_Gaus) || std::isnan(fNormChi2_Gaus) );
    //bool bNormChi2Bad_Landau = (std::isinf(fNormChi2_Landau) || std::isnan(fNormChi2_Landau) );
    
    if (fNormChi2_Gaus < fNormChi2_Landau) {
        inputStatObs.fitDist = fitDist_Gaus;
    }
    else{
        inputStatObs.fitDist = fitDist_Landau;
    }
    
    return;
} //End AnalyzeResponseUniformity::calcStatistics()

TF1 AnalyzeResponseUniformity::getFit(int iEta, int iPhi, int iSlice, InfoFit & setupFit, shared_ptr<TH1F> hInput, TSpectrum &specInput ){
    //Histogram Declaration
    TF1 ret_Func( Analyzer::getFit(setupFit, hInput, specInput) );
    
    //Set Name for this convention
    ret_Func.SetName( getNameByIndex(iEta, iPhi, iSlice, "fit", setupFit.m_strFit_Name).c_str() );
    
    //Return fit
    //------------------------------------------------------
    return ret_Func;
} //End AnalyzeResponseUniformity::getFit()

TGraphErrors AnalyzeResponseUniformity::getGraph(int iEta, int iPhi, HistoSetup & setupHisto){
    //Graph Declaration
    TGraphErrors ret_Graph( Analyzer::getGraph(setupHisto) );
    
    //Set Name for this convention
    ret_Graph.SetName( getNameByIndex(iEta, iPhi, -1, "g", setupHisto.strHisto_Name).c_str() );
    
    return ret_Graph;
} //End AnalyzeResponseUniformity::getGraph()

//Returns a histogram whose parmeters match those defined in hte input HistoSetup object
TH1F AnalyzeResponseUniformity::getHistogram(int iEta, int iPhi, HistoSetup &setupHisto){
    //Histo Declaration
    TH1F ret_Histo( Analyzer::getHistogram(setupHisto) );
    
    //Set Name for this convention
    ret_Histo.SetName( getNameByIndex(iEta, iPhi, -1, "h", setupHisto.strHisto_Name).c_str() );

    //Return Histogram
    return ret_Histo;
} //End AnalyzeResponseUniformity::getHistogram()

TH2F AnalyzeResponseUniformity::getHistogram2D(int iEta, int iPhi, HistoSetup &setupHisto_X, HistoSetup &setupHisto_Y){
    //Histo Declaration
    TH2F ret_Histo( Analyzer::getHistogram2D(setupHisto_X, setupHisto_Y ) );
                   
    //Set Name for this convention
    ret_Histo.SetName( getNameByIndex(iEta, iPhi, -1, "h", setupHisto_Y.strHisto_Name + "_v_" + setupHisto_X.strHisto_Name ).c_str() );
    
    //Return Histogram
    return ret_Histo;
} //End AnalyzeResponseUniformity::getHistogram2D

//Formats a given input string such that it follows the iEta, iPhi, iSlice naming convention
string AnalyzeResponseUniformity::getNameByIndex(int iEta, int iPhi, int iSlice, std::string & strInputPrefix, std::string & strInputName){
    //Variable Declaration
    string ret_Name = "";
    
    if (iSlice > -1) {
        ret_Name = strInputPrefix + "_iEta" + getString(iEta) + "iPhi" + getString(iPhi) + "Slice" + getString(iSlice) + "_" + strInputName;
    }
    else if (iPhi > -1){ //Case: Specific (iEta,iPhi) sector
        ret_Name = strInputPrefix + "_iEta" + getString(iEta) + "iPhi" + getString(iPhi) + "_" + strInputName;
    } //End Case: Specific (iEta,iPhi) sector
    else if (iEta > -1){
        ret_Name = strInputPrefix + "_iEta" + getString(iEta) + "_" + strInputName;
    }
    else{ //Case: iEta Sector, sum over sector's iPhi
        ret_Name = strInputPrefix + "_Summary_" + strInputName;
    } //End Case: iEta Sector, sum over sector's iPhi
    
    return ret_Name;
} //End AnalyzeResponseUniformity::getNameByIndex()

//Formats a given input string such that it follows the iEta, iPhi, iSlice naming convention
//Will take text written in quotes without requiring a variable
string AnalyzeResponseUniformity::getNameByIndex(int iEta, int iPhi, int iSlice, const std::string & strInputPrefix, const std::string & strInputName){
    //Variable Declaration
    string ret_Name = "";
    
    if (iSlice > -1) {
        ret_Name = strInputPrefix + "_iEta" + getString(iEta) + "iPhi" + getString(iPhi) + "Slice" + getString(iSlice) + "_" + strInputName;
    }
    else if (iPhi > -1){ //Case: Specific (iEta,iPhi) sector
        ret_Name = strInputPrefix + "_iEta" + getString(iEta) + "iPhi" + getString(iPhi) + "_" + strInputName;
    } //End Case: Specific (iEta,iPhi) sector
    else if (iEta > -1){
        ret_Name = strInputPrefix + "_iEta" + getString(iEta) + "_" + strInputName;
    }
    else{ //Case: iEta Sector, sum over sector's iPhi
        ret_Name = strInputPrefix + "_Summary_" + strInputName;
    } //End Case: iEta Sector, sum over sector's iPhi
    
    return ret_Name;
} //End AnalyzeResponseUniformity::getNameByIndex()