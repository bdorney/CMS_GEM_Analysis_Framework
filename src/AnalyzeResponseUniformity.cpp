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

using Timing::getString;
using Timing::printROOTFileStatus;
using Timing::HistoSetup;
using Timing::stofSafe;

using namespace Uniformity;

//Default Constructor
AnalyzeResponseUniformity::AnalyzeResponseUniformity(){
    strAnalysisName = "analysis";
} //End Default Constructor

//Set inputs at construction
AnalyzeResponseUniformity::AnalyzeResponseUniformity(AnalysisSetupUniformity inputSetup, DetectorMPGD inputDet){
    strAnalysisName = "analysis";
    
    //Store Analysis Parameters
    aSetup = inputSetup;
    
    //Store Detector
    detMPGD = inputDet;
} //End Constructor

//Checks to see if the detector's uniformity is within requested amount
void AnalyzeResponseUniformity::checkUniformity(){
    //Check to make sure at least one peak exists
    if(detMPGD.vec_allADCPeaks.size() == 0){
	cout<<"============Analysis Summary============\n";
    	cout<<"No Stored ADC Peaks; All Fits Failed!?\n";
	cout<<"You should Investigate the output data file\n";
	return;
    }

    //Variable Declaration
    float fResponse_Max = *(std::max_element(detMPGD.vec_allADCPeaks.begin(), detMPGD.vec_allADCPeaks.end() ) );
    float fResponse_Min = *(std::min_element(detMPGD.vec_allADCPeaks.begin(), detMPGD.vec_allADCPeaks.end() ) );
    float fResponse_Var = (fResponse_Max - fResponse_Min) / fResponse_Max;
    
    //Output Above Info to User
    cout<<"============Analysis Summary============\n";
    cout<<"Detector's Minimum Response: " << fResponse_Min << endl;
    cout<<"Detector's Maximum Response: " << fResponse_Max << endl;
    cout<<"Calculated Difference: " << fResponse_Var << endl;

    //Check if Detector is within tolerance
    if ( fabs(fResponse_Var) <= aSetup.fUniformityTolerance ) { //Case: Detector Within Tolerance
        cout<<"This is found to be within Tolerance (" << aSetup.fUniformityTolerance << "); CHAMBER PASSES!!!\n";
        cout<<"You should Investigate the output data file\n";
    } //End Case: Detector Within Tolerance
    else{ //Case: Detector OUTSIDE Tolerance
        cout<<"This is found to be OUTSIDE Tolerance (" << aSetup.fUniformityTolerance << "); CHAMBER FAILS!!!\n";
        cout<<"You should Investigate the output data file\n";
    } //End Case: Detector OUTSIDE Tolerance
    
    return;
} //check AnalyzeResponseUniformity::Uniformity

//Loads a ROOT file previously created by an instance of AnalyzeResponseUniformity
//Loads all TObjects found in the input ROOT file into detMPGD;
//Any previously stored information in detMPGD is lost.
/*void AnalyzeResponseUniformity::loadHistosFromFile( std::string & strInputMappingFileName, std::string & strInputROOTFileName ){
    //Variable Declaration
    TFile *ptr_fileInput = nullptr;
    
    //This method will be called when the user wants to re-run the fitting on a previously created batch of histograms
    //The user will directly supply an AMORE mapping file, this will make the DetectorMPGD structure so that it matches the one created when the histograms where first booked
    //The user will indirectly supply an analysis config file because they want to re-run the fits and need to give new information
    //Use previously existing framework code to setup the detector MPGD, then this method behaves as the reverse of storeHistos()
    
    //Setup the MPGD object
    //------------------------------------------------------
    ParameterLoaderAmoreSRS amoreLoader;
    amoreLoader.loadAmoreMapping(strInputMappingFileName);
    detMPGD = amoreLoader.getDetector();
    
    //Open the requested ROOT file
    //------------------------------------------------------
    ptr_fileInput = new TFile(strInputROOTFileName.c_str(),"READ","",1);
    
    //Check to see if data file opened successfully, if so load the tree
    //------------------------------------------------------
    if ( !ptr_fileInput->IsOpen() || ptr_fileInput->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("Uniformity::AnalyzeResponseUniformity::loadHistosFromFile() - error while opening file: " + strInputROOTFileName ).c_str() );
        Timing::printROOTFileStatus(ptr_fileInput);
        std::cout << "Exiting!!!\n";
        
        return;
    } //End Case: failed to load ROOT file
    
    //Loop Through the file and load all stored TObjects
    //------------------------------------------------------
    //Loop Over Stored iEta Sectors
    for (auto iterEta = detMPGD.map_sectorsEta.begin(); iterEta != detMPGD.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        
        //Get Directory
        //-------------------------------------
        //Check to see if the directory exists already
        TDirectory *dir_SectorEta = ptr_fileInput->GetDirectory( ( "SectorEta" + getString( (*iterEta).first ) ).c_str(), false, "GetDirectory" );
        
        //If the above pointer is null the directory does NOT exist, skip this Eta Sector
        if (dir_SectorEta == nullptr) continue;
        
        //Debugging
        //cout<<"dir_SectorEta->GetName() = " << dir_SectorEta->GetName()<<endl;
        
        //Load Histograms - SectorEta Level
        //-------------------------------------
        dir_SectorEta->cd();
        //Placeholder No fits performed on these histos for now
        
        //Loop Over Stored iPhi Sectors within this iEta Sector
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over Stored iPhi Sectors
            //Get Directory
            //-------------------------------------
            //Check to see if the directory exists already
            TDirectory *dir_SectorPhi = dir_SectorEta->GetDirectory( ( "SectorPhi" + getString( (*iterPhi).first ) ).c_str(), false, "GetDirectory"  );
            
            //If the above pointer is null the directory does NOT exist, skip this Phi Sector
            if (dir_SectorPhi == nullptr) continue;
            
            //Debugging
            //cout<<"dir_SectorPhi->GetName() = " << dir_SectorPhi->GetName()<<endl;
            
            //Load Histograms - SectorPhi Level
            //-------------------------------------
            dir_SectorPhi->cd();
            (*iterPhi).second.clustHistos.hADC_v_Pos = make_shared<TH2F>( *((TH2F*) dir_SectorPhi->Get( ("hiEta" + getString( (*iterEta).first ) + "iPhi" + getString( (*iterPhi).first ) + "_ClustADC_v_ClustPos").c_str() ) ) );
            
            //Check to see if 2D histo retrieved successfully
            if ( (*iterPhi).second.clustHistos.hADC_v_Pos == nullptr) continue;
            
            //Set to Global Directory - SectorPhi Level
            //-------------------------------------
            //Prevents seg faulting when closing ptr_fileInput
            (*iterPhi).second.clustHistos.hADC_v_Pos->SetDirectory(gROOT);
            
            //Slices
            //Trickery, detMPGD now has eta and phi sectors setup.
            //Slices are NOT setup
            //Here loop from 1 to aSetup.iUniformityGranularity and either:
            //      Option 1: Load slices from the file (slower?)
            //      Option 2: project out from SectorPhi::clustHistos::hADC_v_Pos
            //Implemented Option 2; faster many large number of I/O operations??
            for (int i=1; i <= aSetup.iUniformityGranularity; ++i ) { //Loop Over Slices
                //Set Histograms - Slice Level
                //-------------------------------------
                //Creat the slice
                SectorSlice slice;
                
                slice.hSlice_ClustADC = make_shared<TH1F>( *( (TH1F*) (*iterPhi).second.clustHistos.hADC_v_Pos->ProjectionY( ("hiEta" + getString( (*iterEta).first ) + "iPhi" + getString( (*iterPhi).first ) + "Slice" + getString(i) + "_ClustADC").c_str(),i,i,"") ) );
                
                //Make sure to set this histo to the global directory
                slice.hSlice_ClustADC->SetDirectory(gROOT);
                
                //Store position information for this slice
                slice.fPos_Center = (*iterPhi).second.clustHistos.hADC_v_Pos->GetXaxis()->GetBinCenter(i);
                slice.fWidth = (*iterPhi).second.clustHistos.hADC_v_Pos->GetXaxis()->GetBinWidth(i);
                
                //Store the slice
                (*iterPhi).second.map_slices[i] = slice;
            } //End Loop Over Slices
        } //End Loop Over Stored iPhi Sectors
    } //End Loop Over Stored iEta Sectors

    //Close the file
    //------------------------------------------------------
    ptr_fileInput->Close();
    
    return;
}*/ //End AnalyzeResponseUniformity::loadHistosFromFile()

void AnalyzeResponseUniformity::calcStatistics(SummaryStatistics &inputStatObs, std::multiset<float> &mset_fInputObs){
    //Variable Declaration;
    std::multiset<float>::iterator iterQ1 = mset_fInputObs.begin();
    std::multiset<float>::iterator iterQ2 = mset_fInputObs.begin();
    std::multiset<float>::iterator iterQ3 = mset_fInputObs.begin();

	cout<<"mset_fInputObs.size() = " << mset_fInputObs.size() << endl;
    
    //Determine max, min, & mean
    inputStatObs.fMax   = *mset_fInputObs.rbegin();   //Last member of the multiset
    inputStatObs.fMin   = *mset_fInputObs.begin();    //First member of the multiset
    inputStatObs.fMean  = std::accumulate( mset_fInputObs.begin(), mset_fInputObs.end(), 0. ) / mset_fInputObs.size();
    
    //Determine Q1, Q2, & Q3
    std::advance(iterQ1, (int)std::ceil( mset_fInputObs.size() * 0.25 ) );  inputStatObs.fQ1 = *iterQ1;
    std::advance(iterQ2, (int)std::ceil( mset_fInputObs.size() * 0.50 ) );  inputStatObs.fQ2 = *iterQ2;
    std::advance(iterQ3, (int)std::ceil( mset_fInputObs.size() * 0.75 ) );  inputStatObs.fQ3 = *iterQ3;
    
    //if (iterQ1 != nullptr && iterQ1 != mset_fInputObs.end() )
    //if (iterQ2 != nullptr && iterQ2 != mset_fInputObs.end() )
    //if (iterQ3 != nullptr && iterQ3 != mset_fInputObs.end() )
    
    //Determine IQR
    inputStatObs.fIQR = inputStatObs.fQ3 - inputStatObs.fQ1;
    
    //Determine all outliers
    float fLowerBound = inputStatObs.fQ1 - 1.5 * inputStatObs.fIQR;
    float fUpperBound = inputStatObs.fQ3 + 1.5 * inputStatObs.fIQR;
    inRange inDataRange(fLowerBound, fUpperBound);
    
    inputStatObs.mset_fOutliers = mset_fInputObs;
    for (auto iterSet = inputStatObs.mset_fOutliers.begin(); iterSet != inputStatObs.mset_fOutliers.end(); ++iterSet) { //Loop Over input multiset
        
        if (inDataRange( (*iterSet) ) ) {
            iterSet = inputStatObs.mset_fOutliers.erase(iterSet);
        }
        
    } //End Loop Over input multiset
    
    return;
} //End AnalyzeResponseUniformity::calcStatistics()

TF1 AnalyzeResponseUniformity::getFit(int iEta, int iPhi, int iSlice, HistoSetup & setupHisto, shared_ptr<TH1F> hInput, TSpectrum &specInput ){
    //Variable Declaration
    float fLimit_Max = setupHisto.fHisto_xUpper, fLimit_Min = setupHisto.fHisto_xLower;
    
    vector<string>::const_iterator iterVec_IGuess; //Iterator to use for setting initial guess of fit
    vector<float> vec_fFitRange;
    
    for (auto iterRange = aSetup.histoSetup_clustADC.vec_strFit_Range.begin(); iterRange != aSetup.histoSetup_clustADC.vec_strFit_Range.end(); ++iterRange) { //Loop Over Fit Range
        vec_fFitRange.push_back( getFitBoundary( (*iterRange), hInput, specInput ) );
    } //End Loop Over Fit Range
    
    if (vec_fFitRange.size() > 1) {
        fLimit_Min = (*std::min_element(vec_fFitRange.begin(), vec_fFitRange.end() ) );
        fLimit_Max = (*std::max_element(vec_fFitRange.begin(), vec_fFitRange.end() ) );
    }
    
    TF1 ret_Func( getNameByIndex(iEta, iPhi, iSlice, "fit", setupHisto.strHisto_Name).c_str(), setupHisto.strFit_Formula.c_str(), fLimit_Min, fLimit_Max);
    
    //Check to see if the number of parameters in the TF1 meets the expectation
    if ( ret_Func.GetNpar() < setupHisto.vec_strFit_ParamIGuess.size() || ret_Func.GetNpar() < setupHisto.vec_strFit_ParamLimit_Min.size() || ret_Func.GetNpar() < setupHisto.vec_strFit_ParamLimit_Max.size() ) { //Case: Set points for initial parameters do not meet expectations
        
        printClassMethodMsg("AnalyzeResponseUniformity","getFit","Error! Number of Parameters in Function Less Than Requested Initial Guess Parameters!");
        printClassMethodMsg("AnalyzeResponseUniformity","getFit", ("\tNum Parameter: " + getString( ret_Func.GetNpar() ) ).c_str() );
        printClassMethodMsg("AnalyzeResponseUniformity","getFit", ("\tNum Initial Guesses: " + getString( setupHisto.vec_strFit_ParamIGuess.size() ) ).c_str() );
        printClassMethodMsg("AnalyzeResponseUniformity","getFit", ("\tNum Initial Guess Limits (Min): " + getString( setupHisto.vec_strFit_ParamLimit_Min.size() ) ).c_str() );
        printClassMethodMsg("AnalyzeResponseUniformity","getFit", ("\tNum Initial Guess Limits (Max): " + getString( setupHisto.vec_strFit_ParamLimit_Max.size() ) ).c_str() );
        printClassMethodMsg("AnalyzeResponseUniformity","getFit", "No Initial Parameters Have Been Set! Please Cross-Check Input Analysis Config File" );
        
        return ret_Func;
    } //End Case: Set points for initial parameters do not meet expectations
    
    //Set Fit Parameters - Initial Value
    //------------------------------------------------------
    //Keywords are AMPLITUDE, MEAN, PEAK, SIGMA
    for (int i=0; i<setupHisto.vec_strFit_ParamIGuess.size(); ++i) { //Loop over parameters - Initial Guess
        iterVec_IGuess = std::find(vec_strSupportedKeywords.begin(), vec_strSupportedKeywords.end(), setupHisto.vec_strFit_ParamIGuess[i]);
        
        if ( iterVec_IGuess == vec_strSupportedKeywords.end() ) { //Case: No Keyword Found; Try to set a Numeric Value
            ret_Func.SetParameter(i, stofSafe( setupHisto.vec_strFit_ParamIGuess[i] ) );
        } //End Case: No Keyword Found; Try to set a Numeric Value
        else{ //Case: Keyword Found; Set Value based on Keyword
            ret_Func.SetParameter(i, getValByKeyword( (*iterVec_IGuess), hInput, specInput ) );
        } //End Case: Keyword Found; Set Value based on Keyword
    } //End Loop over parameters - Initial Guess
    
    //Set Fit Parameters - Boundaries
    //------------------------------------------------------
    if (setupHisto.vec_strFit_ParamLimit_Min.size() == setupHisto.vec_strFit_ParamLimit_Max.size() ) { //Check: Stored Parameter Limits Match

        //Here we use vec_strFit_ParamLimit_Min but we know it has the same number of parameters as vec_strFit_ParamLimit_Max
        //For each fit parameter, set the boundary
        for (int i=0; i<setupHisto.vec_strFit_ParamLimit_Min.size(); ++i) { //Loop over boundary parameters
            fLimit_Min = getFitBoundary(setupHisto.vec_strFit_ParamLimit_Min[i], hInput, specInput);
            fLimit_Max = getFitBoundary(setupHisto.vec_strFit_ParamLimit_Max[i], hInput, specInput);
            
		//cout<<"(fLimit_Min, fLimit_Max) = (" << fLimit_Min << ", " << fLimit_Max << ")\n";

            (fLimit_Max > fLimit_Min) ? ret_Func.SetParLimits(i, fLimit_Min, fLimit_Max ) : ret_Func.SetParLimits(i, fLimit_Max, fLimit_Min );
        } //End Loop over boundary parameters
    } //End Check: Stored Parameter Limits Match
    
    //Set Fit Parameters - Fixed?
    //------------------------------------------------------
    
    //Placeholder; maybe we add functionality in the future
    
    //Set Other Fit Data Members
    //------------------------------------------------------
    ret_Func.SetLineColor(kRed);
    ret_Func.SetLineWidth(3);
    
    //Return fit
    //------------------------------------------------------
    return ret_Func;
} //End AnalyzeResponseUniformity::getFit()

float AnalyzeResponseUniformity::getFitBoundary(std::string &strInputExp, std::shared_ptr<TH1F> hInput, TSpectrum &specInput){
    //Variable Declaration
    map<string, float> map_key2Val;
    
    //Search the input expression for each of the supported keywords
    //Store these Keywords with their values
    for (int i=0; i < vec_strSupportedKeywords.size(); ++i) { //Loop Through Supported Keywords
        if ( strInputExp.find( vec_strSupportedKeywords[i] ) != std::string::npos ) { //Case: Keyword Found!
            map_key2Val[vec_strSupportedKeywords[i]] = getValByKeyword( vec_strSupportedKeywords[i], hInput, specInput );
        } //End Case: Keyword Found!
    } //End Loop Through Supported Keywords
    
    //Check if map_key2Val has any entries, if so user requested complex expression; parse!
    //If map_key2Val is empty, user has a numeric input; convert to float!
    if (map_key2Val.size() > 0) { //Case: Complex Expression!
        //Setup the expression parser
        symbol_table_t symbol_table;    //Stores the variables in expression and maps them to C++ objects
        expression_t expression;        //Stores the actual expression & the symbol table
        parser_t parser;                //Parses the information for evaluation
        
        //Load all found keywords into the symbol table
        for (auto iterMap = map_key2Val.begin(); iterMap != map_key2Val.end(); ++iterMap) {
            symbol_table.add_variable( (*iterMap).first, (*iterMap).second);
        }
        
        //Give the expression the variables it should have
        expression.register_symbol_table(symbol_table);
        
        //Compile the parsing
        parser.compile(strInputExp, expression);
        
        //Return value to the user
        return expression.value();
    } //End Case: Complex Expression!
    else{ //Case: Numeric Input
        return stofSafe( strInputExp );
    } //End Case: Numeric Input
} //End AnalyzeResponseUniformity::getFitBoundary()

TGraphErrors AnalyzeResponseUniformity::getGraph(int iEta, int iPhi, HistoSetup & setupHisto){
    //Variable Declaration
    string strPrefix = "g";
    string strName = getNameByIndex(iEta, iPhi, -1, strPrefix, setupHisto.strHisto_Name);
    
    //Graph Declaration
    TGraphErrors ret_Graph;
    
    //Set Histo Data Members
    ret_Graph.GetXaxis()->SetTitle( setupHisto.strHisto_Title_X.c_str() );
    
    return ret_Graph;
} //End AnalyzeResponseUniformity::getGraph()

//Returns a histogram whose parmeters match those defined in hte input HistoSetup object
TH1F AnalyzeResponseUniformity::getHistogram(int iEta, int iPhi, HistoSetup &setupHisto){
    //Variable Declaration
    string strPrefix = "h";
    string strName = getNameByIndex(iEta, iPhi, -1, strPrefix, setupHisto.strHisto_Name);
    
    //Histo Declaration
    TH1F ret_Histo(strName.c_str(), "", setupHisto.iHisto_nBins, setupHisto.fHisto_xLower, setupHisto.fHisto_xUpper );

    //Set Histo Data Members
    ret_Histo.SetXTitle( setupHisto.strHisto_Title_X.c_str() );
    ret_Histo.SetYTitle( setupHisto.strHisto_Title_Y.c_str() );
    
    ret_Histo.Sumw2();
    
    //Return Histogram
    return ret_Histo;
} //End AnalyzeResponseUniformity::getHistogram()

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
    else{ //Case: iEta Sector, sum over sector's iPhi
        ret_Name = strInputPrefix + "_iEta" + getString(iEta) + "_" + strInputName;
    } //End Case: iEta Sector, sum over sector's iPhi

    return ret_Name;
} //End AnalyzeResponseUniformity::getNameByIndex()

float AnalyzeResponseUniformity::getPeakPos( shared_ptr<TF1> fitInput, HistoSetup & setupHisto ){
    //Search the peak parameter meaning vector for "PEAK"
    //If found return this parameter
    
    //If not found, return -1;
    //warn the user
    
    //Variable Declaration
    int iParamPos = -1;
    
    float ret_Val = -1;
    
    vector<string>::iterator iterParamMeaning = std::find(setupHisto.vec_strFit_ParamMeaning.begin(), setupHisto.vec_strFit_ParamMeaning.end(), "PEAK");
    
    if ( iterParamMeaning != setupHisto.vec_strFit_ParamMeaning.end() ) { //Case: Parameter Found!!!
        
        iParamPos = std::distance(setupHisto.vec_strFit_ParamMeaning.begin(), iterParamMeaning);
        
        ret_Val = fitInput->GetParameter(iParamPos);
    } //End Case: Parameter Found!!!
    else{ //Case: Parameter NOT Found
        printClassMethodMsg("AnalyzeResponseUniformity","getPeakPos","Error! - I Do not know which parameter in your fit function represents the peak!\n");
        printClassMethodMsg("AnalyzeResponseUniformity","getPeakPos","\tPlease Cross-check input analysis config file.\n");
        printClassMethodMsg("AnalyzeResponseUniformity","getPeakPos","\tEnsure the field 'Fit_Param_Map' has a value 'PEAK' and the posi 'PEAK' matches\n");
        printClassMethodMsg("AnalyzeResponseUniformity","getPeakPos","\tThe position of 'PEAK' in the list must match the numeric index of the parameter\n");
        printClassMethodMsg("AnalyzeResponseUniformity","getPeakPos","\te.g. if Parameter [2] represents the spectrum peak than 'PEAK' should be the third member in the list given to 'Fit_Param_Map'\n");
    } //End Case: Parameter NOT Foun
    
    return ret_Val;
} //End AnalyzeResponseUniformity::getPeakPos

float AnalyzeResponseUniformity::getPeakPosError( shared_ptr<TF1> fitInput, HistoSetup & setupHisto ){
    //Search the peak parameter meaning vector for "PEAK"
    //If found return this parameter
    
    //If not found, return -1;
    //warn the user
    
    //Variable Declaration
    int iParamPos = -1;
    
    float ret_Val = -1;
    
    vector<string>::iterator iterParamMeaning = std::find(setupHisto.vec_strFit_ParamMeaning.begin(), setupHisto.vec_strFit_ParamMeaning.end(), "PEAK");
    
    if ( iterParamMeaning != setupHisto.vec_strFit_ParamMeaning.end() ) { //Case: Parameter Found!!!
        
        iParamPos = std::distance(setupHisto.vec_strFit_ParamMeaning.begin(), iterParamMeaning);
        
        ret_Val = fitInput->GetParError(iParamPos);
    } //End Case: Parameter Found!!!
    else{ //Case: Parameter NOT Found
        printClassMethodMsg("AnalyzeResponseUniformity","getPeakPos","Error! - I Do not know which parameter in your fit function represents the peak!\n");
        printClassMethodMsg("AnalyzeResponseUniformity","getPeakPos","\tPlease Cross-check input analysis config file.\n");
        printClassMethodMsg("AnalyzeResponseUniformity","getPeakPos","\tEnsure the field 'Fit_Param_Map' has a value 'PEAK' and the posi 'PEAK' matches\n");
        printClassMethodMsg("AnalyzeResponseUniformity","getPeakPos","\tThe position of 'PEAK' in the list must match the numeric index of the parameter\n");
        printClassMethodMsg("AnalyzeResponseUniformity","getPeakPos","\te.g. if Parameter [2] represents the spectrum peak than 'PEAK' should be the third member in the list given to 'Fit_Param_Map'\n");
    } //End Case: Parameter NOT Foun
    
    return ret_Val;
} //End AnalyzeResponseUniformity::getPeakPosError

//Given an input histogram and TSpectrum returns a numeric value based on the input keyword; supported keywords are "AMPLITUDE,MEAN,PEAK,SIGMA"
float AnalyzeResponseUniformity::getValByKeyword(string strInputKeyword, shared_ptr<TH1F> hInput, TSpectrum &specInput){
    
    //Try to automatically assign a value
    if (0 == strInputKeyword.compare("AMPLITUDE") ) { //Case: Histo Amplitude
        return hInput->GetBinContent( hInput->GetMaximumBin() );
    } //End Case: Histo Amplitude
    else if (0 == strInputKeyword.compare("MEAN") ) { //Case: Histo Mean
        return hInput->GetMean();
    } //End Case: Histo Mean
    else if ( 0 == strInputKeyword.compare("PEAK") ){ //Case: Histo Peak
        Double_t *dPeakPos = specInput.GetPositionX();
        
        return dPeakPos[0];
    } //End Case: Histo Peak
    else if (0 == strInputKeyword.compare("SIGMA") ) { //Case: Histo RMS
        return hInput->GetRMS();
    } //End Case: Histo RMS
    else{ //Case: manual assignment
        printClassMethodMsg("AnalyzeResponseUniformity","getValByKeyword","Error! Input Keyword Not Recognized");
        printClassMethodMsg("AnalyzeResponseUniformity","getValByKeyword", ("\tGiven: " + strInputKeyword ).c_str() );
        printClassMethodMsg("AnalyzeResponseUniformity","getValByKeyword","\tRecognized Keywords:\n");
        
        for (int i=0; i < vec_strSupportedKeywords.size(); ++i) {
            printClassMethodMsg("AnalyzeResponseUniformity","getValByKeyword", vec_strSupportedKeywords[i].c_str() );
        }
        
        printClassMethodMsg("AnalyzeResponseUniformity","getValByKeyword","\tUndefined Behavior May Occur");
        
        return -1e12;
    } //End Case: manual assignment
} //End AnalyzeResponseUniformity::getValByKeyword()
