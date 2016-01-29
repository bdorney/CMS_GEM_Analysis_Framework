//
//  ParameterLoaderAnaysis.cpp
//  
//
//  Created by Brian L Dorney on 28/01/16.
//
//

#include "ParameterLoaderAnaysis.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::pair;
using std::string;
using std::transform;
using std::vector;

//using namespace Timing;
using Timing::getlineNoSpaces;
using Timing::getParsedLine;
using Timing::printStreamStatus;
using Timing::stoiSafe;

using namespace Uniformity;

//Default Constructor
ParameterLoaderAnaysis::ParameterLoaderAnaysis(){
    strSecBegin_Analysis    = "[BEING_ANALYSIS_INFO]";
    strSecBegin_Timing      = "[BEGIN_TIMING_INFO]";
    strSecBegin_Uniformity  = "[BEGIN_UNIFORMITY_INFO]";
    
    strSecEnd_Analysis      = "[END_ANALYSIS_INFO]";
    strSecEnd_Timing        = "[END_TIMING_INFO]";
    strSecEnd_Uniformity    = "[END_UNIFORMITY_INFO]";
} //End default constructor

//Opens a text file set by the user and loads the requested parameters
void ParameterLoaderAnaysis::loadAnalysisParameters(string & strInputSetupFile, AnalysisSetupUniformity &aSetupUniformity){
    //Variable Declaration
    bool bExitSuccess = false;
    
    pair<string,string> pair_strParam; //Input file is setup in <Field, Value> pairs; not used here yet but placeholder
    
    string strLine = "";    //Line taken from the input file
    //string strHeading = ""; //For storing detector heading
    
    vector<string> vec_strList; //For storing char separated input; not used here yet but placeholder
    
    //Open the Data File
    //------------------------------------------------------
    if (bVerboseMode_IO) { //Case: User Requested Verbose Error Messages - I/O
        printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParameters", ("trying to open and read: " + strInputSetupFile).c_str() );
    } //End Case: User Requested Verbose Error Messages - I/O
    
    ifstream fStream( strInputSetupFile.c_str() );
    
    //Check to See if Data File Opened Successfully
    //------------------------------------------------------
    if (!fStream.is_open() && bVerboseMode_IO) {
        perror( ("Uniformity::ParameterLoaderAnaysis::loadAnalysisParameters(): error while opening file: " + strInputSetupFile).c_str() );
        printStreamStatus(fStream);
    }
    
    ////Loop Over data Input File
    //------------------------------------------------------
    //Read the file via std::getline().  Obey good coding practice rules:
    //  -first the I/O operation, then error check, then data processing
    //  -failbit and badbit prevent data processing, eofbit does not
    //See: http://gehrcke.de/2011/06/reading-files-in-c-using-ifstream-dealing-correctly-with-badbit-failbit-eofbit-and-perror/
    while ( getlineNoSpaces(fStream, strLine) ) {
        //Reset exit flag used in string manipulation
        bExitSuccess = false;
        
        //Does the user want to comment out this line?
        if ( 0 == strLine.compare(0,1,"#") ) continue;
        
        //Debugging
        cout<<"strLine = " << strLine.c_str() << endl;
        
        //Identify Section Headers
        if ( 0 == strLine.compare(strSecEnd_Analysis) ) { //Case: Reached End of File
            
            //Debugging
            cout<<"Found End of Analysis Section"<<endl;
            
            break;
        } //End Case: Reached End of File
        else if ( 0 == strLine.compare(strSecBegin_Analysis) ) { //Case: Analysis Header
            
            //Filler for now; intentionally empty
            
        } //End Case: Analysis Header
        else if ( 0 == strLine.compare(strSecBegin_Timing) ) { //Case: Timing Parameters
            //Debugging
            cout<<"Found Start of Timing Section"<<endl;
            
            loadAnalysisParametersTiming(fStream, aSetupUniformity);
        } //End Case: Timing Parameters
        else if ( 0 == strLine.compare(strSecBegin_Uniformity) ) { //Case: Uniformity Parameters
            //Debugging
            cout<<"Found Start of Uniformity Section"<<endl;
            
            loadAnalysisParametersUniformity(fStream, aSetupUniformity);
        } //End Case: Uniformity Parameters
        else { //Case: Unsorted Parameters
            
            //Filler for now; intentionally empty but may return to it later
            
        } //End Case: Unsorted Parameters
    } //End Loop Over Input File
    
    //Check to see if we had problems while reading the file
    if (fStream.bad() && bVerboseMode_IO) {
        perror( ("Uniformity::ParameterLoaderAnaysis::loadAnalysisParameters(): error while reading file: " + strInputSetupFile).c_str() );
        printStreamStatus(fStream);
    }
    
    return;
} //End ParameterLoaderAnaysis::loadAnalysisParameters()

//Time Resolution
//Loads parameters defined in file read by inputFileStream and sets them to the aSetupUniformity
void ParameterLoaderAnaysis::loadAnalysisParametersTiming(ifstream &inputFileStream, AnalysisSetupUniformity &aSetupUniformity){
    
    //To be implemented
    
    return;
} //End ParameterLoaderAnaysis::loadAnalysisParametersTiming()

//Uniformity
//Loads parameters defined in file read by inputFileStream and sets tehm to the aSetupUniformity
//Note this should only be called within the Uniformity heading if the user has configured the file correctly
void ParameterLoaderAnaysis::loadAnalysisParametersUniformity(ifstream &inputFileStream, AnalysisSetupUniformity &aSetupUniformity){
    //Variable Declaration
    bool bExitSuccess = false;
    
    pair<string,string> pair_strParam; //Input file is setup in <Field, Value> pairs; not used here yet but placeholder
    
    //string strField = "";   //From input file we have <Field,Value> pairs
    string strLine = "";    //Line taken from the input file
    //string strHeading = ""; //For storing detector heading
    
    vector<string> vec_strList; //For storing char separated input; not used here yet but placeholder
    
    if (bVerboseMode_IO) { //Case: User Requested Verbose Error Messages - I/O
        printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersUniformity", "Found Uniformity Heading");
    } //End Case: User Requested Verbose Error Messages - I/O
    
    while ( getlineNoSpaces(inputFileStream, strLine) ) {
        //Does the user want to comment out this line?
        if ( 0 == strLine.compare(0,1,"#") ) continue;
        
        //Do we reach teh end of the section?
        if ( 0 == strLine.compare(strSecEnd_Uniformity ) ) break;
        
        //Debugging
        cout<<"strLine: = " << strLine.c_str() << endl;
        
        //Parse the line
        pair_strParam = getParsedLine(strLine,bExitSuccess);
        
        if (bExitSuccess) { //Case: Parameter Fetched Correctly
            //transform(pair_strParam.first.begin(), pair_strParam.second.end(),pair_strParam.first.begin(),toupper);
            
            string strTmp = pair_strParam.first;
            transform(strTmp.begin(), strTmp.end(), strTmp.begin(), toupper);
            
            pair_strParam.first = strTmp;
            
            if ( 0 == pair_strParam.first.compare("CUT_ADC_MIN") ) {
                aSetupUniformity.selClust.iCut_ADCNoise = stoiSafe(pair_strParam.first,pair_strParam.second);
            } //End Case: Minimum ADC Value
            else if( 0 == pair_strParam.first.compare("CUT_CLUSTERSIZE_MIN") ) {
                aSetupUniformity.selClust.iCut_SizeMin = stoiSafe(pair_strParam.first,pair_strParam.second);
            } //End Case: Min Cluster Size
            else if( 0 == pair_strParam.first.compare("CUT_CLUSTERSIZE_MAX") ) {
                aSetupUniformity.selClust.iCut_SizeMax = stoiSafe(pair_strParam.first,pair_strParam.second);
            } //End Case: Max Cluster Size
            else if( 0 == pair_strParam.first.compare("CUT_CLUSTERTIME_MIN") ) {
                aSetupUniformity.selClust.iCut_TimeMin = stoiSafe(pair_strParam.first,pair_strParam.second);
            } //End Case: Min Cluster Time
            else if( 0 == pair_strParam.first.compare("CUT_CLUSTERTIME_MAX") ) {
                aSetupUniformity.selClust.iCut_TimeMax = stoiSafe(pair_strParam.first,pair_strParam.second);
            } //End Case: Max Cluster Time
        } //End Case: Parameter Fetched Correctly
        else{ //Case: Parameter Failed to fetch correctly
            printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersUniformity","Error!!!  I didn't parse parameter correctly\n");
            printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersUniformity",("\tCurrent line: " + strLine).c_str() );
        } //End Case: Parameter Failed to fetch correctly
    } //End Loop through Uniformity Heading
    
    return;
} //End ParameterLoaderAnaysis::loadAnalysisParametersUniformity()

Uniformity::AnalysisSetupUniformity ParameterLoaderAnaysis::getAnalysisParameters(string & strInputSetupFile){
    //Variable Declaration
    Uniformity::AnalysisSetupUniformity aSetupUniformity;
    
    loadAnalysisParameters(strInputSetupFile, aSetupUniformity);
    
    return aSetupUniformity;
} //End ParameterLoaderAnaysis::getAnalysisParameters()