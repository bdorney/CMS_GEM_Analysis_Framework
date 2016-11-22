//
//  Analyzer.cpp
//  
//
//  Created by Brian L Dorney on 28/01/16.
//
//

#include "Analyzer.h"

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

using QualityControl::Uniformity::inRange;
using QualityControl::Uniformity::printClassMethodMsg;
using QualityControl::Uniformity::SummaryStatistics;


using namespace QualityControl;

//Default Constructor
Analyzer::Analyzer(){
    strAnalysisName = "analysis";
} //End Default Constructor

bool Analyzer::isQualityFit(shared_ptr<TF1> fitInput){
    //Variable Declaration
    bool bIsQuality = true;

    for( int i=0; i < fitInput->GetNpar(); i++){
	bIsQuality = isQualityFit(fitInput, i);

	if(!bIsQuality) break;
    }    

    return bIsQuality;
} //End Analyzer::isQualityFit()

bool Analyzer::isQualityFit(shared_ptr<TF1> fitInput, int iPar){
    //Variable Declaration
    double dPar, dPar_Err;
    double dParLimit_Lower, dParLimit_Upper;

	dPar = fitInput->GetParameter(iPar);
	dPar_Err = fitInput->GetParError(iPar);
	fitInput->GetParLimits(iPar,dParLimit_Lower,dParLimit_Upper);

	if (dPar == 0) {
		//cout<<"Failed, Parameter " << iPar << " is zero\n";
		//cout<<"dPar = " << dPar << endl;
		return false;
	}
	else if ( ( fabs(dPar - dParLimit_Lower) / dPar ) < 0.001 ){
		//cout<<"Failed, Parameter " << iPar << " is too close to lower boundary\n";
		//cout<<"dPar = " << dPar << endl;
		//cout<<"dParLimit_Lower = " << dParLimit_Lower << endl;
		return false;
	}
	else if ( ( fabs(dPar - dParLimit_Upper) / dPar) < 0.001 ){
		//cout<<"Failed, Parameter " << iPar << " is too close to upper boundary\n";
		//cout<<"dPar = " << dPar << endl;
		//cout<<"dParLimit_Upper = " << dParLimit_Upper << endl;		
		return false;
	}
	else if ( ( dPar_Err / dPar ) > 0.1 ) {
		//cout<<"Failed, Parameter " << iPar << " has too large of an uncertainty\n";
		//cout<<"dPar = " << dPar << endl;
		//cout<<"dPar_Err = " << dPar_Err << endl;
		//cout<<"Percent Error = " << dPar_Err / dPar << endl;
		return false;
	}

    return true;
} //End Analyzer::isQualityFit()

TF1 Analyzer::getFit(InfoFit & setupFit, shared_ptr<TH1F> hInput, TSpectrum &specInput ){
    //Variable Declaration
    float fLimit_Max = hInput->GetBinLowEdge(hInput->GetNbinsX() + 1 );
    float fLimit_Min = hInput->GetBinLowEdge(1);
    
    vector<string>::const_iterator iterVec_IGuess; //Iterator to use for setting initial guess of fit
    vector<float> vec_fFitRange;
    
    for (auto iterRange = setupFit.m_vec_strFit_Range.begin(); iterRange != setupFit.m_vec_strFit_Range.end(); ++iterRange) { //Loop Over Fit Range
        vec_fFitRange.push_back( getParsedInput( (*iterRange), hInput, specInput ) );
    } //End Loop Over Fit Range
    
    if (vec_fFitRange.size() > 1) {
        fLimit_Min = (*std::min_element(vec_fFitRange.begin(), vec_fFitRange.end() ) );
        fLimit_Max = (*std::max_element(vec_fFitRange.begin(), vec_fFitRange.end() ) );
    }
    
    TF1 ret_Func( ("fit" + setupFit.m_strFit_Name).c_str(), setupFit.m_strFit_Formula.c_str(), fLimit_Min, fLimit_Max);
    
    //Check to see if the number of parameters in the TF1 meets the expectation
    if ( ret_Func.GetNpar() < setupFit.m_vec_strFit_ParamIGuess.size() || ret_Func.GetNpar() < setupFit.m_vec_strFit_ParamLimit_Min.size() || ret_Func.GetNpar() < setupFit.m_vec_strFit_ParamLimit_Max.size() ) { //Case: Set points for initial parameters do not meet expectations

        printClassMethodMsg("Analyzer","getFit","Error! Number of Parameters in Function Less Than Requested Initial Guess Parameters!");
        printClassMethodMsg("Analyzer","getFit", ("\tNum Parameter: " + getString( ret_Func.GetNpar() ) ).c_str() );
        printClassMethodMsg("Analyzer","getFit", ("\tNum Initial Guesses: " + getString( setupFit.m_vec_strFit_ParamIGuess.size() ) ).c_str() );
        printClassMethodMsg("Analyzer","getFit", ("\tNum Initial Guess Limits (Min): " + getString( setupFit.m_vec_strFit_ParamLimit_Min.size() ) ).c_str() );
        printClassMethodMsg("Analyzer","getFit", ("\tNum Initial Guess Limits (Max): " + getString( setupFit.m_vec_strFit_ParamLimit_Max.size() ) ).c_str() );
        printClassMethodMsg("Analyzer","getFit", "No Initial Parameters Have Been Set! Please Cross-Check Input Analysis Config File" );
        
        return ret_Func;
    } //End Case: Set points for initial parameters do not meet expectations
    
    //Set Fit Parameters - Initial Value
    //------------------------------------------------------
    //Keywords are defined in vec_strSupportedKeywords
    for (int i=0; i<setupFit.m_vec_strFit_ParamIGuess.size(); ++i) { //Loop over parameters - Initial Guess
        ret_Func.SetParameter(i, getParsedInput( setupFit.m_vec_strFit_ParamIGuess[i], hInput, specInput) );
    } //End Loop over parameters - Initial Guess
    
    //Set Fit Parameters - Boundaries
    //------------------------------------------------------
    if (setupFit.m_vec_strFit_ParamLimit_Min.size() == setupFit.m_vec_strFit_ParamLimit_Max.size() ) { //Check: Stored Parameter Limits Match

        //Here we use vec_strFit_ParamLimit_Min but we know it has the same number of parameters as vec_strFit_ParamLimit_Max
        //For each fit parameter, set the boundary
        for (int i=0; i<setupFit.m_vec_strFit_ParamLimit_Min.size(); ++i) { //Loop over boundary parameters
            fLimit_Min = getParsedInput(setupFit.m_vec_strFit_ParamLimit_Min[i], hInput, specInput);
            fLimit_Max = getParsedInput(setupFit.m_vec_strFit_ParamLimit_Max[i], hInput, specInput);

            (fLimit_Max > fLimit_Min) ? ret_Func.SetParLimits(i, fLimit_Min, fLimit_Max ) : ret_Func.SetParLimits(i, fLimit_Max, fLimit_Min );
        } //End Loop over boundary parameters
    } //End Check: Stored Parameter Limits Match
    
    //Set Fit Parameters - Fixed?
    //------------------------------------------------------
    
    //Placeholder; maybe we add functionality in the future
    
    //Set Other Fit Data Members
    //------------------------------------------------------
    ret_Func.SetLineColor(setupFit.m_iColor);
    ret_Func.SetLineWidth(setupFit.m_fSizeLine);
    
    //Return fit
    //------------------------------------------------------
    return ret_Func;
} //End Analyzer::getFit()

float Analyzer::getParsedInput(std::string &strInputExp, std::shared_ptr<TH1F> hInput, TSpectrum &specInput){
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
} //End Analyzer::getParsedInput()

TGraphErrors Analyzer::getGraph(HistoSetup & setupHisto){
    //Variable Declaration
    string strName = "g"+setupHisto.strHisto_Name;
    
    //Graph Declaration
    TGraphErrors ret_Graph;
    
    //Set Histo Data Members
    ret_Graph.GetXaxis()->SetTitle( setupHisto.strHisto_Title_X.c_str() );
    
    return ret_Graph;
} //End Analyzer::getGraph()

//Returns a histogram whose parmeters match those defined in hte input HistoSetup object
TH1F Analyzer::getHistogram(HistoSetup &setupHisto){
    //Variable Declaration
    string strName = "h" + setupHisto.strHisto_Name;
    
    //Histo Declaration
    TH1F ret_Histo(strName.c_str(), "", setupHisto.iHisto_nBins, setupHisto.fHisto_xLower, setupHisto.fHisto_xUpper );

    //Set Histo Data Members
    ret_Histo.SetXTitle( setupHisto.strHisto_Title_X.c_str() );
    ret_Histo.SetYTitle( setupHisto.strHisto_Title_Y.c_str() );
    
    ret_Histo.Sumw2();
    
    //Set Directory to the global directory
    ret_Histo.SetDirectory(gROOT);

    //Return Histogram
    return ret_Histo;
} //End Analyzer::getHistogram()

TH2F Analyzer::getHistogram2D(HistoSetup &setupHisto_X, HistoSetup &setupHisto_Y){
    //Variable Declaration
    string strName = "h" + setupHisto_Y.strHisto_Name + "_v_" + setupHisto_X.strHisto_Name;
    
    //Histo Declaration
    TH2F ret_Histo(strName.c_str(), "", setupHisto_X.iHisto_nBins, setupHisto_X.fHisto_xLower, setupHisto_X.fHisto_xUpper, setupHisto_Y.iHisto_nBins, setupHisto_Y.fHisto_xLower, setupHisto_Y.fHisto_xUpper);
    
    //Set Histo Data Members
    ret_Histo.SetXTitle( setupHisto_X.strHisto_Title_X.c_str() );
    ret_Histo.SetYTitle( setupHisto_Y.strHisto_Title_X.c_str() );
    
    ret_Histo.Sumw2();
    
    //Set Directory to the global directory
    ret_Histo.SetDirectory(gROOT);
    
    //Return Histogram
    return ret_Histo;
} //End Analyzer::getHistogram2D

//Searches the input TF1 for a parameter with meaning given by strParam and stored in HistoSetup
//This parameter is then returned to the user
float Analyzer::getParam( shared_ptr<TF1> fitInput, InfoFit & setupFit, std::string strParam ){
    //Variable Declaration
    int iParamPos = -1;
    
    float ret_Val = -1;
    
    vector<string>::iterator iterParamMeaning = std::find(setupFit.m_vec_strFit_ParamMeaning.begin(), setupFit.m_vec_strFit_ParamMeaning.end(), strParam);
    
    if ( iterParamMeaning != setupFit.m_vec_strFit_ParamMeaning.end() ) { //Case: Parameter Found!!!
        
        iParamPos = std::distance(setupFit.m_vec_strFit_ParamMeaning.begin(), iterParamMeaning);
        
        ret_Val = fitInput->GetParameter(iParamPos);
    } //End Case: Parameter Found!!!
    else{ //Case: Parameter NOT Found
        printClassMethodMsg("Analyzer","getParam",("Error! - I did not find your requested parameter: " + strParam + "!\n").c_str() );
        printClassMethodMsg("Analyzer","getPeakPos","\tPlease Cross-check input analysis config file.\n");
    } //End Case: Parameter NOT Foun
    
    return ret_Val;
} //End Analyzer::getParam

//Searches the input TF1 for a parameter with meaning given by strParam and stored in HistoSetup
//The error on this parameter is then returned to the user
float Analyzer::getParamError( shared_ptr<TF1> fitInput, Plotter::InfoFit & setupFit, std::string strParam ){
    //Variable Declaration
    int iParamPos = -1;
    
    float ret_Val = -1;
    
    vector<string>::iterator iterParamMeaning = std::find(setupFit.m_vec_strFit_ParamMeaning.begin(), setupFit.m_vec_strFit_ParamMeaning.end(), strParam);
    
    if ( iterParamMeaning != setupFit.m_vec_strFit_ParamMeaning.end() ) { //Case: Parameter Found!!!
        
        iParamPos = std::distance(setupFit.m_vec_strFit_ParamMeaning.begin(), iterParamMeaning);
        
        ret_Val = fitInput->GetParError(iParamPos);
    } //End Case: Parameter Found!!!
    else{ //Case: Parameter NOT Found
        printClassMethodMsg("Analyzer","getParamError",("Error! - I did not find your requested parameter: " + strParam + "!\n").c_str() );
        printClassMethodMsg("Analyzer","getParamError","\tPlease Cross-check input analysis config file.\n");
    } //End Case: Parameter NOT Foun
    
    return ret_Val;
} //End Analyzer::getParamError

//Given an input histogram and TSpectrum returns a numeric value based on the input keyword; supported keywords are "AMPLITUDE,MEAN,PEAK,SIGMA"
float Analyzer::getValByKeyword(string strInputKeyword, shared_ptr<TH1F> hInput, TSpectrum &specInput){
    
    //Try to automatically assign a value
    if ( 0 == strInputKeyword.compare("AMPLITUDE") ) { //Case: Histo Amplitude
        return hInput->GetBinContent( hInput->GetMaximumBin() );
    } //End Case: Histo Amplitude
    else if( 0 == strInputKeyword.compare("FWHM") ){ //Case: Full Width Half Max
        //Right now as estimate we just use the histo RMS
        return hInput->GetRMS();
    } //End Case: Full Width Half Max
    else if( 0 == strInputKeyword.compare("HWHM") ){ //Case: Half Width Half Max
        //Right now as estimate we just use half the histo RMS
        return 0.5 * hInput->GetRMS();
    } //End Case: Half Width Half Max
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
        printClassMethodMsg("Analyzer","getValByKeyword","Error! Input Keyword Not Recognized");
        printClassMethodMsg("Analyzer","getValByKeyword", ("\tGiven: " + strInputKeyword ).c_str() );
        printClassMethodMsg("Analyzer","getValByKeyword","\tRecognized Keywords:\n");
        
        for (int i=0; i < vec_strSupportedKeywords.size(); ++i) {
            printClassMethodMsg("Analyzer","getValByKeyword", vec_strSupportedKeywords[i].c_str() );
        }
        
        printClassMethodMsg("Analyzer","getValByKeyword","\tUndefined Behavior May Occur");
        
        return -1e12;
    } //End Case: manual assignment
} //End Analyzer::getValByKeyword()
