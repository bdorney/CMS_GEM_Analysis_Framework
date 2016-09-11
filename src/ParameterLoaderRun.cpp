//
//  ParameterLoaderRun.cpp
//  
//
//  Created by Brian L Dorney on 03/05/16.
//
//

#include "ParameterLoaderRun.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::pair;
using std::string;

using QualityControl::Timing::convert2bool;
using QualityControl::Timing::getlineNoSpaces;
using QualityControl::Timing::getParsedLine;
using QualityControl::Timing::printStreamStatus;
using QualityControl::Timing::stoiSafe;

using namespace QualityControl::Uniformity;

//Default Constructor
ParameterLoaderRun::ParameterLoaderRun(){
    m_bVerboseMode_IO       = false;
    
    m_strSecBegin_CompInfo  = "[BEGIN_COMP_INFO]";
    m_strSecBegin_RunInfo   = "[BEGIN_RUN_INFO]";
    m_strSecBegin_RunList   = "[BEGIN_RUN_LIST]";
    
    m_strSecEnd_CompInfo    = "[END_COMP_INFO]";
    m_strSecEnd_RunInfo     = "[END_RUN_INFO]";
    m_strSecEnd_RunList     = "[END_RUN_LIST]";
} //End Default Constructor

void ParameterLoaderRun::loadParameters(ifstream &file_Input, bool bVerboseMode, RunSetup & inputRunSetup){
    //Variable Declaration
    string strLine = "";
    
    ////Loop Over data Input File
    //------------------------------------------------------
    //Read the file via std::getline().  Obey good coding practice rules:
    //  -first the I/O operation, then error check, then data processing
    //  -failbit and badbit prevent data processing, eofbit does not
    //See: http://gehrcke.de/2011/06/reading-files-in-c-using-ifstream-dealing-correctly-with-badbit-failbit-eofbit-and-perror/
    while ( getlineNoSpaces(file_Input, strLine) ) {
        //Does the user want to comment out this line?
        if ( 0 == strLine.compare(0,1,"#") ) continue;
        
        //Identify Section Headers
        if ( 0 == strLine.compare(m_strSecEnd_CompInfo) || 0 == strLine.compare(m_strSecEnd_RunInfo) ) { //Case: Reached End of Interest
            break;
        } //End Case: Reached End of Interest
        else if ( 0 == strLine.compare(m_strSecBegin_RunInfo ) ){ //Case: Run Info Header
            loadParametersRun(file_Input, bVerboseMode, inputRunSetup);
        } //End Case: Run Info Header
        else if ( 0 == strLine.compare(m_strSecBegin_CompInfo ) ){ //Case: Comp Info Header
            loadParametersCompare(file_Input, bVerboseMode, inputRunSetup);
        } //End Case: Comp Info Header
        else { //Case: Unsorted Parameters
            
            //Place holder
            
        } //End Case: Unsorted Parameters
    } //End Loop over input file
    
    //Check to see if we had problems while reading the file
    if (file_Input.bad() && bVerboseMode) {
        //perror( ("Uniformity::ParameterLoaderRun::loadParameters(): error while reading file: " + strInputSetupFile).c_str() );
        perror( "Uniformity::ParameterLoaderRun::loadParameters(): error while reading file" );
        printStreamStatus(file_Input);
    }
    
    //Do not close input file, it will be used elsewhere
    
    inputRunSetup.bLoadSuccess = true;
    
    return;
} //End ParameterLoaderRun::loadRunParameters()

//Sets up inputRunSetup for running in analysis mode
void ParameterLoaderRun::loadParametersRun(std::ifstream &file_Input, bool bVerboseMode, RunSetup & inputRunSetup){
    //Variable Declaration
    bool bExitSuccess = false;
    
    std::pair<string,string> pair_strParam;
    
    string strLine = "";
    
    if (bVerboseMode) { //Case: User Requested Verbose Error Messages - I/O
        printClassMethodMsg("ParameterLoaderRun","loadParametersRun", "Found Run Heading");
    } //End Case: User Requested Verbose Error Messages - I/O
    
    //Set Defaults
    //------------------------------------------------------
    inputRunSetup.strRunMode            = "ANALYSIS";
    
    //Loop through input file
    //Check for faults immediately afterward
    //------------------------------------------------------
    while ( getlineNoSpaces(file_Input, strLine) ) { //Loop through input file
        //Skip commented lines
        if (strLine.compare(0,1,"#") == 0) continue;
        
        //Do we reach the end of the section?
        if ( 0 == strLine.compare( m_strSecEnd_RunInfo ) ) {
            if (bVerboseMode) { //Case: User Requested Verbose Input/Output
                cout<<"ParameterLoaderRun::loadParametersRun(): End of run info header reached!\n";
            } //End Case: User Requested Verbose Input/Output
            
            break;
        }
        
        //Parse the line
        pair_strParam = getParsedLine(strLine, bExitSuccess);
        
        if (bExitSuccess) { //Case: Parameter Fetched Successfully
            //Transform input field name to all capitals for case-insensitive string comparison
            string strTmp = pair_strParam.first;
            std::transform(strTmp.begin(), strTmp.end(), strTmp.begin(), toupper);
            
            pair_strParam.first = strTmp;
            
            if ( pair_strParam.first.compare("ANA_CLUSTERS") == 0 ) {
                inputRunSetup.bAnaStep_Clusters = convert2bool(pair_strParam.second, bExitSuccess);
            }
            else if ( pair_strParam.first.compare("ANA_FITTING") == 0 ) {
                inputRunSetup.bAnaStep_Fitting = convert2bool(pair_strParam.second, bExitSuccess);
            }
            else if ( pair_strParam.first.compare("ANA_HITS") == 0 ) {
                inputRunSetup.bAnaStep_Hits = convert2bool(pair_strParam.second, bExitSuccess);
            }
            else if ( pair_strParam.first.compare("ANA_RECO_CLUSTERS") == 0 ) {
                inputRunSetup.bAnaStep_Reco = convert2bool(pair_strParam.second, bExitSuccess);
            }
            else if ( pair_strParam.first.compare("VISUALIZE_PLOTS") == 0 ) {
                inputRunSetup.bAnaStep_Visualize = convert2bool(pair_strParam.second, bExitSuccess);
            }
            else if ( pair_strParam.first.compare("INPUT_IS_FRMWRK_OUTPUT") == 0 ) {
                inputRunSetup.bInputFromFrmwrk = convert2bool(pair_strParam.second, bExitSuccess);
            }
            else if ( pair_strParam.first.compare("OUTPUT_INDIVIDUAL") == 0 ) {
                inputRunSetup.bMultiOutput = convert2bool(pair_strParam.second, bExitSuccess);
            }
            else if ( pair_strParam.first.compare("VISUALIZE_DRAWPHILINES") == 0 ) {
                inputRunSetup.bVisPlots_PhiLines = convert2bool(pair_strParam.second, bExitSuccess);
            }
            else if ( pair_strParam.first.compare("VISUALIZE_AUTOSAVEIMAGES") == 0 ) {
                inputRunSetup.bVisPlots_AutoSaving = convert2bool(pair_strParam.second, bExitSuccess);
            }
            else if ( pair_strParam.first.compare("CONFIG_ANALYSIS") == 0 ) {
                inputRunSetup.strFile_Config_Ana = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("CONFIG_MAPPING") == 0 ) {
                inputRunSetup.strFile_Config_Map = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("OUTPUT_FILE_NAME") == 0 ) {
                inputRunSetup.strFile_Output_Name = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("OUTPUT_FILE_OPTION") == 0 ) {
                inputRunSetup.strFile_Output_Option = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("DETECTOR_NAME") == 0 ){
                inputRunSetup.strDetName = pair_strParam.second;
            }
            else{ //Case: Parameter Not Recognized
                cout<<"ParameterLoaderRun::loadParametersRun(): input field name:\n";
                cout<<"\t"<<pair_strParam.first<<endl;
                cout<<"ParameterLoaderRun::loadParametersRun(): not recognized! Please cross-check input file!!!\n";
            } //End Case: Parameter Not Recognized
        } //End Case: Parameter Fetched Successfully
        else{ //Case: Input line did NOT parse correctly
            cout<<"ParameterLoaderRun::loadParametersRun(): input line:\n";
            cout<<"\t"<<strLine<<endl;
            cout<<"ParameterLoaderRun::loadParametersRun(): did not parse correctly, please cross-check input file\n";
        } //End Case: Input line did NOT parse correctly
    } //End Loop through input file
    if ( file_Input.bad() && bVerboseMode) {
        perror( "ParameterLoaderRun::loadParametersRun(): error while reading config file" );
        printStreamStatus(file_Input);
    }
    
    return;
} //End ParameterLoaderRun::loadParametersRun()

//Sets up inputRunSetup for running in comparison mode
void ParameterLoaderRun::loadParametersCompare(std::ifstream &file_Input, bool bVerboseMode, RunSetup & inputRunSetup){
    //Variable Declaration
    bool bExitSuccess = false;
    
    std::pair<string,string> pair_strParam;
    
    string strLine = "";
    
    if (bVerboseMode) { //Case: User Requested Verbose Error Messages - I/O
        printClassMethodMsg("ParameterLoaderRun","loadParametersCompare", "Found Compare Heading");
    } //End Case: User Requested Verbose Error Messages - I/O
    
    //Set Defaults
    //------------------------------------------------------
    inputRunSetup.strRunMode            = "COMPARISON";
    inputRunSetup.bAnaStep_Visualize    = true;
    inputRunSetup.bInputFromFrmwrk      = true;
    inputRunSetup.bMultiOutput          = false;
    
    //Loop through input file
    //Check for faults immediately afterward
    //------------------------------------------------------
    while ( getlineNoSpaces(file_Input, strLine) ) { //Loop through input file
        //Skip commented lines
        if (strLine.compare(0,1,"#") == 0) continue;
        
        //Do we reach the end of the section?
        if ( 0 == strLine.compare( m_strSecEnd_CompInfo ) ) {
            if (bVerboseMode) { //Case: User Requested Verbose Input/Output
                cout<<"ParameterLoaderRun::loadParametersRun(): End of compare info header reached!\n";
            } //End Case: User Requested Verbose Input/Output
            
            break;
        }
        
        //Parse the line
        pair_strParam = getParsedLine(strLine, bExitSuccess);
        
        if (bExitSuccess) { //Case: Parameter Fetched Successfully
            //Transform input field name to all capitals for case-insensitive string comparison
            string strTmp = pair_strParam.first;
            std::transform(strTmp.begin(), strTmp.end(), strTmp.begin(), toupper);
            
            pair_strParam.first = strTmp;
            
            //Below is alphabetized by order in Uniformity::RunSetup
            if ( pair_strParam.first.compare("OBS_NAME") == 0 ) {
                inputRunSetup.strObsName = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("OBS_ETA") == 0 ) {
                inputRunSetup.iEta = stoiSafe(pair_strParam.first, pair_strParam.second);
            }
            else if ( pair_strParam.first.compare("OBS_PHI") == 0 ) {
                inputRunSetup.iPhi = stoiSafe(pair_strParam.first, pair_strParam.second);
            }
            else if ( pair_strParam.first.compare("OBS_SLICE") == 0 ) {
                inputRunSetup.iSlice = stoiSafe(pair_strParam.first, pair_strParam.second);
            }
            else if ( pair_strParam.first.compare("INPUT_IDENTIFIER") == 0 ) {
                inputRunSetup.strIdent = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("OUTPUT_FILE_NAME") == 0 ) {
                inputRunSetup.strFile_Output_Name = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("OUTPUT_FILE_OPTION") == 0 ) {
                inputRunSetup.strFile_Output_Option = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("VISUALIZE_AUTOSAVEIMAGES") == 0 ) {
                inputRunSetup.bVisPlots_AutoSaving = convert2bool(pair_strParam.second, bExitSuccess);
            }
            else if ( pair_strParam.first.compare("VISUALIZE_DRAWNORMALIZED") == 0 ) {
                inputRunSetup.bVisPlots_PhiLines = convert2bool(pair_strParam.second, bExitSuccess);
            }
            else if ( pair_strParam.first.compare("VISUALIZE_DRAWPHILINES") == 0 ) {
                inputRunSetup.bVisPlots_PhiLines = convert2bool(pair_strParam.second, bExitSuccess);
            }
            else{ //Case: Parameter Not Recognized
                cout<<"ParameterLoaderRun::loadParametersRun(): input field name:\n";
                cout<<"\t"<<pair_strParam.first<<endl;
                cout<<"ParameterLoaderRun::loadParametersRun(): not recognized! Please cross-check input file!!!\n";
            } //End Case: Parameter Not Recognized
        } //End Case: Parameter Fetched Successfully
        else{ //Case: Input line did NOT parse correctly
            cout<<"ParameterLoaderRun::loadParametersRun(): input line:\n";
            cout<<"\t"<<strLine<<endl;
            cout<<"ParameterLoaderRun::loadParametersRun(): did not parse correctly, please cross-check input file\n";
        } //End Case: Input line did NOT parse correctly
    } //End Loop through input file
    if ( file_Input.bad() && bVerboseMode) {
        perror( "ParameterLoaderRun::loadParametersRun(): error while reading config file" );
        printStreamStatus(file_Input);
    }
    
    return;
} //End ParameterLoaderRun::loadParametersCompare()
