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
//using std::map;
using std::pair;
using std::string;
using std::vector;

using QualityControl::Timing::getCharSeparatedList;
using QualityControl::Timing::convert2bool;
using QualityControl::Timing::getlineNoSpaces;
using QualityControl::Timing::getParsedLine;
using QualityControl::Timing::stoiSafe;

using namespace QualityControl::Uniformity;

//Default Constructor
ParameterLoaderRun::ParameterLoaderRun(){
    strSecBegin_RunInfo = "[BEGIN_RUN_INFO]";
    strSecBegin_RunList = "[BEGIN_RUN_LIST]";
    
    strSecEnd_RunInfo = "[END_RUN_INFO]";
    strSecEnd_RunList = "[END_RUN_LIST]";
} //End Default Constructor

void ParameterLoaderRun::loadRunParameters(ifstream &file_Input, bool bVerboseMode, RunSetup & inputRunSetup){
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
                    
                    //Below is alphabetized by order in Uniformity::RunSetup
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
    
    inputRunSetup.bLoadSuccess = true;
    
    return;
} //End ParameterLoaderRun::loadRunParameters()

//Maps a run number, found in an input filename, to an input run found in the input config file
//Only those input files having a run number will be returned
//  ->First is run number
//  ->Second is run name
std::vector<std::pair<int, string> > ParameterLoaderRun::getPairedRunList(ifstream &file_Input, bool bVerboseMode){
    //Variable Declaration
    bool bHeaderEnd = false;
    
    int iNum_Run = -1;
    
    string strLine = "";
    
    vector<std::pair<int, string> > vec_retPairedRuns;
    
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
                        cout<<"ParameterLoaderRun::getRunMap(): End of run list header reached!\n";
                        cout<<"ParameterLoaderRun::getRunMap(): The following runs will be analyzed:\n";
                        
                        for (auto iterPairedRuns = vec_retPairedRuns.begin(); iterPairedRuns != vec_retPairedRuns.end(); ++iterPairedRuns) { //Loop over stored runs
                            cout<<"\t"<<"Run"<<(*iterPairedRuns).first<<"\t"<<(*iterPairedRuns).second<<endl;
                        } //End Loop over stored runs
                    } //End Case: User Requested Verbose Input/Output
                    
                    bHeaderEnd = true;
                    break;
                } //End Case: End of run list header
                
                //Get the run number from the file name
                iNum_Run = getRunNumber(strLine);
                
                //Set the run or inform the user of a problem
                if (iNum_Run > -1 ) { //Case: Success, Run Set!
                    vec_retPairedRuns.push_back(std::make_pair(iNum_Run, strLine ) );
                } //End Case: Success, Run Set!
                else{ //End Case: Input not understood/not parsed correctly
                    cout<<"ParameterLoaderRun::getRunMap() - ERROR!!! input file:\n";
                    cout<<"\t"<<strLine.c_str()<<endl;
                    
                    cout<<"ParameterLoaderRun::getRunMap(): Run number not found/parsed from filename!\n";
                    cout<<"ParameterLoaderRun::getRunMap(): you must have the field 'RunX' in the filename, separated by underscores '_', for some unique integer X\n";
                    cout<<"ParameterLoaderRun::getRunMap(): This file will be skipped!!! Please cross-check input file!!!\n";
                    
                    continue;
                } //Case: Input not understood/not parsed correctly
            } //End loop through run list header
        } //End Case: Run list header
        
        if (bHeaderEnd) break;
    } //End Loop over input file
    if ( file_Input.bad() && bVerboseMode) {
        perror( "getRunList(): error while reading config file" );
        Timing::printStreamStatus(file_Input);
    }
    
    //Do not close the input file, it will be used elsewhere
    
    return vec_retPairedRuns;
} //End ParameterLoaderRun::getPairedRunList()

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

int ParameterLoaderRun::getRunNumber(std::string strRunName){
    //Variable Declaration
    int iRetVal;

	//cout<<"getRunNumber() strRunName = " << strRunName.c_str() << endl;   
 
    std::transform(strRunName.begin(), strRunName.end(), strRunName.begin(), toupper);
   
	//cout<<"getRunNumber() post transfer strRunName = " << strRunName.c_str() << endl;
 
    vector<string> vec_strParsedName = getCharSeparatedList(strRunName, '_');

	/*cout<<"i\tParsedInput\tContains\n";
	for(int i=0; i < vec_strParsedName.size(); ++i){
		contains cont("RUN");
		cout<<i<<"\t"<<vec_strParsedName[i]<<"\t"<<cont(vec_strParsedName[i])<<endl;
	}*/

    auto iterStr = std::find_if(vec_strParsedName.begin(), vec_strParsedName.end(), QualityControl::Uniformity::contains("RUN") );
    
	//cout<<"getRunNumber() - iterStr = 

    //Input file name did not contain the phrase "RUN"
    if ( iterStr == vec_strParsedName.end() ) {
        return -1;
    }
    
	//cout<<"getRunNumber() - iterStr = " << (*iterStr).c_str() << endl;

    //(*iterStr).erase(remove((*iterStr).begin(), (*iterStr).end(), "RUN"), (*iterStr).end() );
    
    //cout<<"getRunNumber() - (*iterStr).find('RUN') = " << (*iterStr).find("RUN") << endl;
   
    (*iterStr).erase( (*iterStr).find("RUN"), 3);
    iRetVal = stoiSafe( (*iterStr) );
    
    return iRetVal;
} //End ParameterLoaderRun::getRunNumber()
