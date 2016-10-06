//
//  ParameterLoader.cpp
//  
//
//  Created by Brian L Dorney on 10/09/16.
//
//

#include "ParameterLoader.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::pair;
using std::string;
using std::vector;

using QualityControl::Timing::getCharSeparatedList;
using QualityControl::Timing::getlineNoSpaces;

using QualityControl::Timing::printStreamStatus;
using QualityControl::Timing::stoiSafe;

using namespace QualityControl::Uniformity;

//Default Constructor
ParameterLoader::ParameterLoader(){
    m_bVerboseMode_IO       = false;
    
    //m_strSecBegin_RunList   = "[BEGIN_RUN_LIST]";
    //m_strSecEnd_RunList     = "[END_RUN_LIST]";
}

//Opens a text file set by the user and loads the requested parameters
//Over-written by inherited classes
void ParameterLoader::loadParameters(std::ifstream &file_Input, bool bVerboseMode, RunSetup & inputRunSetup){
    
    cout<<"Brian it doesn't work\n";
    cout<<"Press Ctrl+C Now\n";
    
    int dummy;
    std::cin>>dummy;
    
    return;
} //End

//Doesn't work in gcc < 5.0 since ifstream object is by default deleted implicitly
//No ROOT version yet compiled on gcc >= 5.0
/*ifstream ParameterLoader::getFileStream(std::string strInputSetupFile, bool bVerboseMode){
    //Open the Data File
    //------------------------------------------------------
    if (bVerboseMode) { //Case: User Requested Verbose Error Messages - I/O
        printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParameters", ("trying to open and read: " + strInputSetupFile).c_str() );
    } //End Case: User Requested Verbose Error Messages - I/O
    
    ifstream ret_fStream( strInputSetupFile.c_str() );
    
    //Check to See if Data File Opened Successfully
    //------------------------------------------------------
    if (!ret_fStream.is_open() && bVerboseMode) {
        perror( ("Uniformity::ParameterLoaderAnaysis::loadAnalysisParameters(): error while opening file: " + strInputSetupFile).c_str() );
        printStreamStatus(ret_fStream);
    }
    
    return ret_fStream;
}*/ //End ParameterLoader::getFileStream()

//Maps a run number, found in an input filename, to an input run found in the input config file
//Only those input files having a run number will be returned
//  ->First is run number
//  ->Second is run name
std::vector<std::pair<int, string> > ParameterLoader::getPairedRunList(ifstream &file_Input, string strIdent, bool bVerboseMode){
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
        
	//cout<<"strLine = " << strLine << endl;

        //Check for start of run list header
        if ( strLine.compare( m_headers_Run.m_strSecBegin_RunList ) == 0 ) { //Case: Run list header
            
            while ( getlineNoSpaces(file_Input, strLine) ) { //Loop through run list header
                //Skip commented lines
                if (strLine.compare(0,1,"#") == 0 ) continue;
                
                //Has the header ended?
                if ( strLine.compare( m_headers_Run.m_strSecEnd_RunList ) == 0 ) { //Case: End of run list header
                    if (bVerboseMode) { //Case: User Requested Verbose Input/Output
                        cout<<"ParameterLoader::getPairedRunList(): End of run list header reached!\n";
                        cout<<"ParameterLoader::getPairedRunList(): The following runs will be analyzed:\n";
                        
                        for (auto iterPairedRuns = vec_retPairedRuns.begin(); iterPairedRuns != vec_retPairedRuns.end(); ++iterPairedRuns) { //Loop over stored runs
                            cout<<"\t"<<"Run"<<(*iterPairedRuns).first<<"\t"<<(*iterPairedRuns).second<<endl;
                        } //End Loop over stored runs
                    } //End Case: User Requested Verbose Input/Output
                    
                    bHeaderEnd = true;
                    break;
                } //End Case: End of run list header
                
                //Get the run number from the file name
                iNum_Run = getRunNumber(strLine, strIdent);
                
                //Set the run or inform the user of a problem
                if (iNum_Run > -1 ) { //Case: Success, Run Set!
                    vec_retPairedRuns.push_back(std::make_pair(iNum_Run, strLine ) );
                } //End Case: Success, Run Set!
                else{ //End Case: Input not understood/not parsed correctly
                    cout<<"ParameterLoader::getPairedRunList() - ERROR!!! input file:\n";
                    cout<<"\t"<<strLine.c_str()<<endl;
                    
                    cout<<"ParameterLoader::getPairedRunList(): Run number not found/parsed from filename!\n";
                    cout<<"ParameterLoader::getPairedRunList(): you must have the field 'RunX' in the filename, separated by underscores '_', for some unique integer X\n";
                    cout<<"ParameterLoader::getPairedRunList(): This file will be skipped!!! Please cross-check input file!!!\n";
                    
                    continue;
                } //Case: Input not understood/not parsed correctly
            } //End loop through run list header
        } //End Case: Run list header
        
        if (bHeaderEnd) break;
    } //End Loop over input file
    if ( file_Input.bad() && bVerboseMode) {
        perror( "ParameterLoader::getPairedRunList(): error while reading config file" );
        Timing::printStreamStatus(file_Input);
    }
    
    //Do not close the input file, it will be used elsewhere
    
    return vec_retPairedRuns;
} //End ParameterLoader::getPairedRunList()

//Gets the list of input runs from the input config file
vector<string> ParameterLoader::getRunList(ifstream &file_Input, bool bVerboseMode){
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
        if ( strLine.compare( m_headers_Run.m_strSecBegin_RunList ) == 0 ) { //Case: Run list header
            
            while ( getlineNoSpaces(file_Input, strLine) ) { //Loop through run list header
                //Skip commented lines
                if (strLine.compare(0,1,"#") == 0 ) continue;
                
                //Has the header ended?
                if ( strLine.compare( m_headers_Run.m_strSecEnd_RunList ) == 0 ) { //Case: End of run list header
                    if (bVerboseMode) { //Case: User Requested Verbose Input/Output
                        cout<<"ParameterLoader::getRunList(): End of run list header reached!\n";
                        cout<<"ParameterLoader::getRunList(): The following runs will be analyzed:\n";
                        
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
} //End ParameterLoader::getRunList()

void ParameterLoader::setFileStream(std::string strInputSetupFile, ifstream &file_Input, bool bVerboseMode){
    //Open the Data File
    //------------------------------------------------------
    if (bVerboseMode) { //Case: User Requested Verbose Error Messages - I/O
        printClassMethodMsg("ParameterLoader","setFileStream", ("trying to open and read: " + strInputSetupFile).c_str() );
    } //End Case: User Requested Verbose Error Messages - I/O
    
    if ( !file_Input.is_open() ) {
	file_Input.open( strInputSetupFile.c_str() );
    }
    else {
	if ( bVerboseMode ) {
       		printClassMethodMsg("ParameterLoader","setFileStream", ("file: " + strInputSetupFile + " already open, doing nothing").c_str() );
	}

	return;
    }
    
    //Check to See if Data File Opened Successfully
    //------------------------------------------------------
    if (!file_Input.is_open() && bVerboseMode) {
        perror( ("Uniformity::ParameterLoader::setFileStream(): error while opening file: " + strInputSetupFile).c_str() );
        printStreamStatus(file_Input);
    }
    
    return;
} //End ParameterLoader::getFileStream()

int ParameterLoader::getRunNumber(std::string strRunName, string strIdent){
    //Variable Declaration
    int iRetVal;
    
    std::transform(strRunName.begin(), strRunName.end(), strRunName.begin(), toupper);
    std::transform(strIdent.begin(), strIdent.end(), strIdent.begin(), toupper);
    
    vector<string> vec_strParsedName = getCharSeparatedList(strRunName, '_');
    
    //auto iterStr = std::find_if(vec_strParsedName.begin(), vec_strParsedName.end(), QualityControl::Uniformity::contains("RUN") );
    auto iterStr = std::find_if(vec_strParsedName.begin(), vec_strParsedName.end(), QualityControl::Uniformity::contains(strIdent) );
    
    //Input file name did not contain the phrase "RUN"
    if ( iterStr == vec_strParsedName.end() ) {
        return -1;
    }
    
    (*iterStr).erase( (*iterStr).find(strIdent), 3);
    iRetVal = stoiSafe( (*iterStr) );
    
    return iRetVal;
} //End ParameterLoader::getRunNumber()
