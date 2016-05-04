//
//  ParameterLoaderRun.cpp
//  
//
//  Created by Brian L Dorney on 03/05/16.
//
//

#include "ParameterLoaderRun.h"

using std::cout;
using std::ifstream;
using std::string;
using std::vector;

using Timing::convert2bool;
using Timing::getlineNoSpaces;
using Timing::getParsedLine;

using namespace Uniformity;

//Default Constructor
ParameterLoaderRun::ParameterLoaderRun(){
    strSecBegin_RunInfo = "[BEGIN_RUN_INFO]";
    strSecBegin_RunList = "[BEGIN_RUN_LIST]";
    
    strSecEnd_RunInfo = "[END_RUN_INFO]";
    strSecEnd_RunList = "[END_RUN_LIST]";
} //End Default Constructor

void ParameterLoaderRun::loadRunParameters(string & strInputSetupFile, bool bVerboseMode, RunSetup & inputRunSetup){
    
    //Variable Declaration
    bool bHeaderEnd = false;
    
    std::pair<string,string> pair_strParam;
    
    string strLine = "";
    
    //Loop through input file
    //Check for faults immediately afterward
    //------------------------------------------------------
    while ( getlineNoSpaces(file_Input, strLine) ) { //Loop through input file
        //Skip commented lines
        if (strLine.compare(0,1,"#") == 0) continue;
        
        //Check for start of run info header
        if ( strLine.compare( strSecBegin_RunInfo ) == 0 ) { //Case: Run info header found!
            cout<<"ParameterLoaderRun::loadRunParameters(): Run info header found!\n";
            
            while ( getlineNoSpaces(file_Input, strLine) ) { //Loop through run info header
                bool bExitSuccess;
                
                //Skip commented lines
                if (strLine.compare(0,1,"#") == 0) continue;
                
                //Has this header ended?
                //Has the header ended?
                if ( strLine.compare( strSecEnd_RunInfo ) == 0 ) { //Case: End of run list header
                    if (bVerboseMode) { //Case: User Requested Verbose Input/Output
                        cout<<"ParameterLoaderRun::loadRunParameters(): End of run info header reached!\n";
                        
                        /*
                         
                         Placeholder
                         
                         */
                        
                    } //End Case: User Requested Verbose Input/Output
                    
                    bHeaderEnd = true;
                    break;
                } //End Case: End of run list header
                
                //Get Parameter Pairing
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
                    else{ //Case: Parameter Not Recognized
                        cout<<"ParameterLoaderRun::loadRunParameters(): input field name:\n";
                        cout<<"\t"<<pair_strParam.first<<endl;
                        cout<<"ParameterLoaderRun::loadRunParameters(): not recognized! Please cross-check input file!!!\n";
                    } //End Case: Parameter Not Recognized
                } //End Case: Parameter Fetched Successfully
                else{ //Case: Input line did NOT parse correctly
                    cout<<"ParameterLoaderRun::loadRunParameters(): input line:\n";
                    cout<<"\t"<<strLine<<endl;
                    cout<<"ParameterLoaderRun::loadRunParameters(): did not parse correctly, please cross-check input file\n";
                } //End Case: Input line did NOT parse correctly
            } //End Loop through run info header
        } //End Case: Run info header found!
        
        if (bHeaderEnd) break;
    } //End Loop through input file
    if ( file_Input.bad() && bVerboseMode) {
        perror( "ParameterLoaderRun::loadRunParameters(): error while reading config file" );
        Timing::printStreamStatus(file_Input);
    }
    
    //Do not close input file, it will be used elsewhere
    
    return;
} //End ParameterLoaderRun::loadRunParameters()

//Gets the list of input runs from the input config file
vector<string> ParameterLoaderRun::getRunList(ifstream &file_Input, bool bVerboseMode){
    //Variable Declaration
    bool bHeaderEnd = false;
    
    string strLine = "";
    
    vector<string> vec_strRetRuns;
    
    //Loop Through Data File
    //Check for faults immediately afterward
    //------------------------------------------------------
    while ( getlineNoSpaces(file_Input, strLine )  ) { //Loop over input file
        //Skip commented lines
        if (strLine.compare(0,1,"#") == 0) continue;
        
        //Check for start of run list header
        if ( strLine.compare( strSecBegin_RunList ) == 0 ) { //Case: Run list header
            
            while ( getlineNoSpaces(file_Input, strLine) ) { //Loop through run list header
                //Skip commented lines
                if (strLine.compare(0,1,"#") == 0 ) continue;
                
                //Has the header ended?
                if ( strLine.compare( strSecEnd_RunList ) == 0 ) { //Case: End of run list header
                    if (bVerboseMode) { //Case: User Requested Verbose Input/Output
                        cout<<"ParameterLoaderRun::getRunList(): End of run list header reached!\n";
                        cout<<"ParameterLoaderRun::getRunList(): The following runs will be analyzed:\n";
                        
                        for (int i=0; i < vec_strRetRuns.size(); ++i) { //Loop over stored runs
                            cout<<"\t"<<vec_strRetRuns[i]<<endl;
                        } //End Loop over stored runs
                    } //End Case: User Requested Verbose Input/Output
                    
                    bHeaderEnd = true;
                    break;
                } //End Case: End of run list header
                
                vec_strRetRuns.push_back(strLine);
            } //End loop through run list header
        } //End Case: Run list header
        
        if (bHeaderEnd) break;
    } //End Loop over input file
    if ( file_Input.bad() && bVerboseMode) {
        perror( "getRunList(): error while reading config file" );
        Timing::printStreamStatus(file_Input);
    }
    
    //Do not close the input file, it will be used elsewhere
    
    return vec_strRetRuns;
} //End ParameterLoaderRun::getRunList()