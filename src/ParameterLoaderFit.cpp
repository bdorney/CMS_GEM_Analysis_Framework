//
//  ParameterLoaderFit.cpp
//  
//
//  Created by Brian L Dorney on 10/10/16.
//
//

#include "ParameterLoaderFit.h"

using std::ifstream;
using std::pair;
using std::stoi;
using std::string;
using std::transform;
using std::vector;

using QualityControl::Plotter::convert2Color;
using QualityControl::Plotter::InfoFit;

using QualityControl::Timing::convert2bool;
using QualityControl::Timing::getCharSeparatedList;
using QualityControl::Timing::getlineNoSpaces;
using QualityControl::Timing::getParsedLine;
using QualityControl::Timing::printStreamStatus;

using namespace QualityControl::Uniformity;

/*ParameterLoaderFit::ParameterLoaderFit(){
    m_bVerboseMode_IO = false;
}*/

void ParameterLoaderFit::loadAnalysisParametersFits(ifstream & inputFileStream, InfoFit &inputFitInfo){
    //Variable Declaration
    bool bExitSuccess = false;
    
    int iMin, iMax;
    
    pair<string,string> pair_strParam; //Input file is setup in <Field, Value> pairs; not used here yet but placeholder
    
    string strLine = "";    //Line taken from the input file
    string strTmp = "";
    
    vector<string> vec_strList; //For storing char separated input; not used here yet but placeholder
    
    if (m_bVerboseMode_IO) { //Case: User Requested Verbose Error Messages - I/O
        printClassMethodMsg("ParameterLoaderFit","loadAnalysisParametersFits", "Found Fit Heading");
    } //End Case: User Requested Verbose Error Messages - I/O
    
    while ( getlineNoSpaces(inputFileStream, strLine) ) {
        //Does the user want to comment out this line?
        if ( 0 == strLine.compare(0,1,"#") ) continue;
        
        //Do we reach the end of the section?
        if ( 0 == strLine.compare(m_headers_Ana.m_strSecEnd_Uniformity_Fit) ) break;
        else if ( 0 == strLine.compare(m_headers_plots.m_strSecEnd_Fit) ) break;
        
        //Debugging
        //cout<<"strLine: = " << strLine.c_str() << endl;
        
        //Parse the line
        pair_strParam = getParsedLine(strLine,bExitSuccess);
        
        if (bExitSuccess) { //Case: Parameter Fetched Correctly
            //Change to all capitals;
            strTmp = pair_strParam.first;
            transform(strTmp.begin(), strTmp.end(), strTmp.begin(), toupper);
            
            pair_strParam.first = strTmp;
            
            //cout<<pair_strParam.first<<"\t"<<pair_strParam.second;
            
            if ( pair_strParam.first.compare("FIT_COLOR") == 0 ) {
                inputFitInfo.m_iColor = convert2Color(pair_strParam.second);
            }
            else if( 0 == pair_strParam.first.compare("FIT_FORMULA") ){ //Case: ADC Spectrum Fit Equation
                inputFitInfo.m_strFit_Formula = pair_strParam.second;
            } //End Case: ADC Spectrum Fit Equation
            else if( 0 == pair_strParam.first.compare("FIT_FORMULA_SIG") ){ //Case: ADC Spectrum Fit Equation Signal
                inputFitInfo.m_strFit_Formula_Sig = pair_strParam.second;
            } //End Case: ADC Spectrum Fit Equation Signal
            else if( 0 == pair_strParam.first.compare("FIT_FORMULA_SIG_PARAM_IDX_RANGE") ){ //Case: ADC Spectrum Fit Equation Bkg Range
                vec_strList = getCharSeparatedList(pair_strParam.second,',');
                
                //iMin = stoiSafe(pair_strParam.first, (*std::min_element(vec_strList.begin(), vec_strList.end() ) ) );
                //iMax = stoiSafe(pair_strParam.first, (*std::max_element(vec_strList.begin(), vec_strList.end() ) ) );
                
                iMin = stoi( (*std::min_element(vec_strList.begin(), vec_strList.end() ) ) );
                iMax = stoi( (*std::max_element(vec_strList.begin(), vec_strList.end() ) ) );
                
                inputFitInfo.m_pair_iParamRange_Sig = std::make_pair(iMin, iMax);
            } //End Case: ADC Spectrum Fit Equation Bkg Range
            else if( 0 == pair_strParam.first.compare("FIT_FORMULA_BKG") ){ //Case: ADC Spectrum Fit Equation Background
                inputFitInfo.m_strFit_Formula_Bkg = pair_strParam.second;
            } //End Case: ADC Spectrum Fit Equation Background
            else if( 0 == pair_strParam.first.compare("FIT_FORMULA_BKG_PARAM_IDX_RANGE") ){ //Case: ADC Spectrum Fit Equation Bkg Range
                vec_strList = getCharSeparatedList(pair_strParam.second,',');
                
                //iMin = stoiSafe(pair_strParam.first, (*std::min_element(vec_strList.begin(), vec_strList.end() ) ) );
                //iMax = stoiSafe(pair_strParam.first, (*std::max_element(vec_strList.begin(), vec_strList.end() ) ) );
                
                iMin = stoi( (*std::min_element(vec_strList.begin(), vec_strList.end() ) ) );
                iMax = stoi( (*std::max_element(vec_strList.begin(), vec_strList.end() ) ) );
                
                inputFitInfo.m_pair_iParamRange_Bkg = std::make_pair(iMin, iMax);
            } //End Case: ADC Spectrum Fit Equation Bkg Range
            else if ( pair_strParam.first.compare("FIT_LEGENTRY") == 0 ) {
                inputFitInfo.m_strLegEntry = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("FIT_LINE_SIZE") == 0 ) {
                inputFitInfo.m_fSizeLine = stof(pair_strParam.second);
            }
            else if ( pair_strParam.first.compare("FIT_LINE_STYLE") == 0 ) {
                inputFitInfo.m_iStyleLine = stoi(pair_strParam.second);
            }
            else if( 0 == pair_strParam.first.compare("FIT_OPTION") ){ //Case: ADC Spectrum Fit Equation
                inputFitInfo.m_strFit_Option = pair_strParam.second;
                
                //Ensure that the result of the fit is returned in the TFitResultPtr by included the option "S" by default
                if (inputFitInfo.m_strFit_Option.find("S") == std::string::npos ) {
                    inputFitInfo.m_strFit_Option = inputFitInfo.m_strFit_Option + "S";
                }
            } //End Case: ADC Spectrum Fit Equation
            else if( 0 == pair_strParam.first.compare("FIT_PARAM_IGUESS") ){
                inputFitInfo.m_vec_strFit_ParamIGuess = getCharSeparatedList(pair_strParam.second,',');
            }
            else if( 0 == pair_strParam.first.compare("FIT_PARAM_LIMIT_MAX") ){
                inputFitInfo.m_vec_strFit_ParamLimit_Max = getCharSeparatedList(pair_strParam.second, ',');
            }
            else if( 0 == pair_strParam.first.compare("FIT_PARAM_LIMIT_MIN") ){
                inputFitInfo.m_vec_strFit_ParamLimit_Min = getCharSeparatedList(pair_strParam.second, ',');
            }
            else if( 0 == pair_strParam.first.compare("FIT_PARAM_MAP") ){
                inputFitInfo.m_vec_strFit_ParamMeaning = getCharSeparatedList(pair_strParam.second,',');
            }
            else if ( pair_strParam.first.compare("FIT_NAME") == 0 ) {
                inputFitInfo.m_strFit_Name = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("FIT_ROOT_FILE") == 0 ) {
                inputFitInfo.m_strFile_Name = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("FIT_ROOT_PATH") == 0 ) {
                inputFitInfo.m_strFile_Path = pair_strParam.second;
            }
            else if( 0 == pair_strParam.first.compare("FIT_PERFORM") ){
                 inputFitInfo.m_bFit = convert2bool(pair_strParam.second, bExitSuccess);
            }
            else if( 0 == pair_strParam.first.compare("FIT_RANGE") ){
                inputFitInfo.m_vec_strFit_Range = getCharSeparatedList(pair_strParam.second, ',');
            }
            else{ //Case: Parameter Not Recognized
                printClassMethodMsg("ParameterLoaderFit","loadAnalysisParametersFits","Error!!! Parameter Not Recognizd:\n");
                //printClassMethodMsg("ParameterLoaderFit","loadAnalysisParametersUniformity",( "\t(Field,Value) = (" + pair_strParam.first "," + pair_strParam.second + ")\n" ).c_str() );
                printClassMethodMsg("ParameterLoaderFit","loadAnalysisParametersFits",( "\tField = " + pair_strParam.first + "\n" ).c_str() );
                printClassMethodMsg("ParameterLoaderFit","loadAnalysisParametersFits",( "\tValue = " + pair_strParam.second + "\n" ).c_str() );
            } //End Case: Parameter Not Recognized
        } //End Case: Parameter Fetched Correctly
        else{ //Case: Parameter Failed to fetch correctly
            printClassMethodMsg("ParameterLoaderFit","loadAnalysisParametersFits","Error!!!  I didn't parse parameter correctly\n");
            printClassMethodMsg("ParameterLoaderFit","loadAnalysisParametersFits",("\tCurrent line: " + strLine).c_str() );
        } //End Case: Parameter Failed to fetch correctly
    } //End Loop through Fit Heading
    if ( inputFileStream.bad() && m_bVerboseMode_IO) {
        perror( "ParameterLoaderRun::loadParametersRun(): error while reading config file" );
        printStreamStatus(inputFileStream);
    }
    
    return;
} //End ParameterLoaderFit::loadAnalysisParametersFits
