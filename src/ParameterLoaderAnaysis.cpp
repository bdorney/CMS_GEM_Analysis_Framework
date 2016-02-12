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
using Timing::convert2bool;
using Timing::getCharSeparatedList;
using Timing::getlineNoSpaces;
using Timing::getParsedLine;
using Timing::getString;
using Timing::HistoSetup;
using Timing::printStreamStatus;
using Timing::stofSafe;
using Timing::stoiSafe;

using namespace Uniformity;

//Default Constructor
ParameterLoaderAnaysis::ParameterLoaderAnaysis(){
    strSecBegin_Analysis    = "[BEING_ANALYSIS_INFO]";
    strSecBegin_Timing      = "[BEGIN_TIMING_INFO]";
    strSecBegin_Uniformity  = "[BEGIN_UNIFORMITY_INFO]";
    strSecBegin_Uniformity_Fit = "[BEGIN_ADC_FIT_INFO]";
    strSecBegin_Uniformity_Histo = "[BEGIN_HISTO_INFO]";
       
    
    strSecEnd_Analysis      = "[END_ANALYSIS_INFO]";
    strSecEnd_Timing        = "[END_TIMING_INFO]";
    strSecEnd_Uniformity    = "[END_UNIFORMITY_INFO]";
    strSecEnd_Uniformity_Fit = "[END_ADC_FIT_INFO]";
    strSecEnd_Uniformity_Hiso = "[END_HISTO_INFO]";
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

//
void ParameterLoaderAnaysis::loadAnalysisParametersFits(ifstream & inputFileStream, HistoSetup &hSetup){
    //Variable Declaration
    bool bExitSuccess = false;
    
    pair<string,string> pair_strParam; //Input file is setup in <Field, Value> pairs; not used here yet but placeholder
    
    string strLine = "";    //Line taken from the input file
    
    vector<string> vec_strList; //For storing char separated input; not used here yet but placeholder
    
    if (bVerboseMode_IO) { //Case: User Requested Verbose Error Messages - I/O
        printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersFits", "Found Fit Heading");
    } //End Case: User Requested Verbose Error Messages - I/O
    
    while ( getlineNoSpaces(inputFileStream, strLine) ) {
        //Does the user want to comment out this line?
        if ( 0 == strLine.compare(0,1,"#") ) continue;
        
        //Do we reach the end of the section?
        if ( 0 == strLine.compare(strSecEnd_Uniformity_Fit ) ) break;
        
        //Debugging
        cout<<"strLine: = " << strLine.c_str() << endl;
        
        //Parse the line
        pair_strParam = getParsedLine(strLine,bExitSuccess);
        
        if (bExitSuccess) { //Case: Parameter Fetched Correctly
            //transform(pair_strParam.first.begin(), pair_strParam.second.end(),pair_strParam.first.begin(),toupper);
            
            string strTmp = pair_strParam.first;
            transform(strTmp.begin(), strTmp.end(), strTmp.begin(), toupper);
            
            pair_strParam.first = strTmp;
            
            //cout<<pair_strParam.first<<"\t"<<pair_strParam.second;
            
            if( 0 == pair_strParam.first.compare("FIT_FORMULA") ){ //Case: ADC Spectrum Fit Equation
                hSetup.strFit_Formula = pair_strParam.second;
            } //End Case: ADC Spectrum Fit Equation
            else if( 0 == pair_strParam.first.compare("FIT_OPTION") ){ //Case: ADC Spectrum Fit Equation
                hSetup.strFit_Option = pair_strParam.second;
                
                //Ensure that the result of the fit is returned in the TFitResultPtr by included the option "S" by default
                if (hSetup.strFit_Option.find("S") == std::string::npos ) {
                    hSetup.strFit_Option = hSetup.strFit_Option + "S";
                }
            } //End Case: ADC Spectrum Fit Equation
            else if( 0 == pair_strParam.first.compare("FIT_PARAM_IGUESS") ){
                hSetup.vec_strFit_ParamIGuess = getCharSeparatedList(pair_strParam.second,',');
            }
            else if( 0 == pair_strParam.first.compare("FIT_PARAM_LIMIT_MAX") ){
                hSetup.vec_strFit_ParamLimit_Max = getCharSeparatedList(pair_strParam.second, ',');
            }
            else if( 0 == pair_strParam.first.compare("FIT_PARAM_LIMIT_MIN") ){
                hSetup.vec_strFit_ParamLimit_Min = getCharSeparatedList(pair_strParam.second, ',');
            }
            else if( 0 == pair_strParam.first.compare("FIT_PARAM_MAP") ){
                hSetup.vec_strFit_ParamMeaning = Timing::getCharSeparatedList(pair_strParam.second,',');
            }
            else if( 0 == pair_strParam.first.compare("FIT_RANGE") ){
                hSetup.vec_strFit_Range = getCharSeparatedList(pair_strParam.second, ',');
            }
            else{ //Case: Parameter Not Recognized
                printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersFits","Error!!! Parameter Not Recognizd:\n");
                //printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersUniformity",( "\t(Field,Value) = (" + pair_strParam.first "," + pair_strParam.second + ")\n" ).c_str() );
                printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersFits",( "\tField = " + pair_strParam.first + "\n" ).c_str() );
                printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersFits",( "\tValue = " + pair_strParam.second + "\n" ).c_str() );
            } //End Case: Parameter Not Recognized
        } //End Case: Parameter Fetched Correctly
        else{ //Case: Parameter Failed to fetch correctly
            printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersFits","Error!!!  I didn't parse parameter correctly\n");
            printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersFits",("\tCurrent line: " + strLine).c_str() );
        } //End Case: Parameter Failed to fetch correctly
    } //End Loop through Fit Heading
} //End ParameterLoaderAnaysis::loadAnalysisParametersFits

//Called when loading analysis parameters; relative to histograms
//This is the top level method; this method calls loadAnalysisParametersHistograms(ifstream, Timing::HistoSetup) depending on which histogram is requested by the user
void ParameterLoaderAnaysis::loadAnalysisParametersHistograms(ifstream & inputFileStream, AnalysisSetupUniformity &aSetupUniformity){
    //Variable Declaration
    bool bExitSuccess = false;
    bool bSetup = false;
    
    pair<string,string> pair_strParam; //<Field, Value>

    string strName = "";    //What Histo Case should we create?
    string strLine = "";
    string strTmp = "";     //Used for case insensitive comparison of strName
    
    //Loop through to find "Histo_Name" Should be the first one
    while ( getlineNoSpaces(inputFileStream, strLine) ) { //Loop through file to find "Histo_Name"
	//Debugging
	//cout<<"loadAnalysisParametersHistograms (Top Level); strLine = " << strLine << endl;

        //Does the user want to comment out this line?
        if ( 0 == strLine.compare(0,1,"#") ) continue;
        
        //Do we reach the end of the section? If so the user has configured the file incorrectly
        if ( 0 == strLine.compare(strSecEnd_Uniformity_Hiso) ) { //Section End Reached Prematurely
            printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms","I have reached the END of a Histogram Heading\n");
            printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms","\tBut I Have NOT found the 'Histo_Name' field\n");
            printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms","\tYou have configured this heading incorrectly, the 'Histo_Name' field is expected to be the FIRST line after the Heading\n");
            printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms","\tThis object has been skipped, please cross-check\n");
            
            //Exit the Loop to find "Detector_Name"
            break;
        } //End Case: Section End Reached Prematurely
        
        pair_strParam = getParsedLine(strLine,bExitSuccess);
        
        if (bExitSuccess) { //Case: Parameter Fetched Successfully
            transform(pair_strParam.first.begin(),pair_strParam.first.end(),pair_strParam.first.begin(),toupper);
            
            if ( 0 == pair_strParam.first.compare("HISTO_NAME") ) { //Case: Name found!
                //Store name locally & convert to upper case
                strName = pair_strParam.second;
                
                //Set the correct exit flag
                bSetup = true;
                
                //Exit the Loop to find "Detector_Name"
                break;
            } //End Case: Detector Name found!
            else{ //Case: Detector Name not found!
                printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms","I am parsing a Histogram Heading\n");
                printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms","\tHowever I expected the 'Histo_Name' field to be the first line after the heading\n");
                printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms", ( "\tThe current line I am parsing: " + strLine ).c_str() );
                printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms","\tHas been skipped and may lead to undefined behavior");
            } //End Case: Name not found!
        }//End Case: Parameter Fetched Successfully
        else{ //Case: Parameter was NOT fetched Successfully
            printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms","Sorry I didn't parse parameter correctly\n");
            printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms", ( "\tCurrent line: " + strLine ).c_str() );
        } //End Case: Parameter was NOT fetched Successfully
    } //Loop through file to find "Name"
    
    //Fetch the values for the map for the correct case
    if (bSetup) { //Case: Setup Correct
        strTmp = strName;
        transform(strTmp.begin(), strTmp.end(), strTmp.begin(), toupper);
        
        if (0 == strTmp.compare("CLUSTADC") ) { //Case: Cluster ADC's
            aSetupUniformity.histoSetup_clustADC.strHisto_Name = strName;
            
            loadAnalysisParametersHistograms(inputFileStream, aSetupUniformity.histoSetup_clustADC);
        } //End Case: Cluster ADC's
        else if (0 == strTmp.compare("CLUSTMULTI") ) { //Case: Cluster Multi
            aSetupUniformity.histoSetup_clustMulti.strHisto_Name = strName;
            
            loadAnalysisParametersHistograms(inputFileStream, aSetupUniformity.histoSetup_clustMulti);
        } //End Case: Cluster Multi
        else if (0 == strTmp.compare("CLUSTPOS") ) { //Case: Cluster Position
            aSetupUniformity.histoSetup_clustPos.strHisto_Name = strName;
            
            loadAnalysisParametersHistograms(inputFileStream, aSetupUniformity.histoSetup_clustPos);
        } //End Case: Cluster Position
        else if (0 == strTmp.compare("CLUSTSIZE") ) { //Case: Cluster Size
            aSetupUniformity.histoSetup_clustSize.strHisto_Name = strName;
            
            loadAnalysisParametersHistograms(inputFileStream, aSetupUniformity.histoSetup_clustSize);
        } //End Case: Cluster Size
        else if (0 == strTmp.compare("CLUSTTIME") ) { //Case: Cluster Time
            aSetupUniformity.histoSetup_clustTime.strHisto_Name = strName;
            
            loadAnalysisParametersHistograms(inputFileStream, aSetupUniformity.histoSetup_clustTime);
        } //End Case: Cluster Time
        else{ //Case: Undefined Behavior
            printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms", ( "Histogram Type" + strName + " Not Recognized\n" ).c_str() );
            printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms", "\tI Only Support Case-Insenstive versions from this set {CLUSTADC, CLUSTMULTI, CLUSTPOS, CLUSTSIZE, CLUSTTIME}\n");
        } //End Case: Undefined Behavior
    } //End Case: Setup Correct
    
    return;
} //End ParameterLoaderAnaysis::loadAnalysisParametersHistograms() - Top Level

void ParameterLoaderAnaysis::loadAnalysisParametersHistograms(ifstream &inputFileStream, HistoSetup &hSetup){
    //Variable Declaration
    bool bExitSuccess;
    
    pair<string,string> pair_strParam;
    
    string strLine;
    
    vector<string> vec_strList;
    
    //Loop through the section
    while ( getlineNoSpaces(inputFileStream, strLine) ) {
        bExitSuccess = false;
        
	//Debugging
	//cout<<"loadAnalysisParametersHistograms (Base Level); strLine = " << strLine << endl;

        //Does the user want to comment out this line?
        if ( 0 == strLine.compare(0,1,"#") ) continue;
        
        //Has this section ended?
        if ( 0 == strLine.compare(strSecEnd_Uniformity_Hiso) ) break;
        
        //Get Parameter Pairing
        pair_strParam = Timing::getParsedLine(strLine, bExitSuccess);
        
        //transform field name to upper case
        transform(pair_strParam.first.begin(),pair_strParam.first.end(),pair_strParam.first.begin(),toupper);
        
        //Parse pair
        if (bExitSuccess) { //Case: Parameter Fetched Successfully
            if( 0 == pair_strParam.first.compare("HISTO_BINRANGE") ){
                //Get comma separated list
                vec_strList = Timing::getCharSeparatedList(pair_strParam.second,',');
                
                //Debugging
                //for(int i=0; i<vec_strList.size(); ++i){
                //	cout<<"vec_strList["<<i<<"] = " << vec_strList[i] << endl;
                //}
                
                if (vec_strList.size() >= 2) { //Case: at least 2 numbers
                    //Fetch
                    hSetup.fHisto_xLower = Timing::stofSafe(pair_strParam.first, vec_strList[0]);
                    hSetup.fHisto_xUpper = Timing::stofSafe(pair_strParam.first, vec_strList[1]);
                    
                    //Reset to ensure they are both correctly lower & upper values
                    hSetup.fHisto_xLower = std::min(hSetup.fHisto_xLower, hSetup.fHisto_xUpper);
                    hSetup.fHisto_xUpper = std::max(hSetup.fHisto_xLower, hSetup.fHisto_xUpper);
                    
                    //Tell the user they entered more than what was expected
                    if (vec_strList.size() > 2) { //Case: 3 or more numbers
                        printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms", ( "Sorry you entered 3 or more numbers for " + pair_strParam.first + "\n" ).c_str() );
                        printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms", "\tI have only used the first two and ignored the rest:\n" );
                        printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms", ("\t" + getString( hSetup.fHisto_xLower ) ).c_str() );
                        printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms", ("\t" + getString( hSetup.fHisto_xUpper ) ).c_str() );
                    } //End Case: 3 or more numbers
                } //End Case: at least 2 numbers
                else{ //Case: Not enough numbers
                    if (vec_strList.size() == 1) { //Case: only 1 number entered
                        hSetup.fHisto_xUpper = Timing::stofSafe(pair_strParam.first, vec_strList[0]);
                    } //End Case: only 1 number entered
                    
                    //Reset to ensure they are both correctly lower & upper values
                    hSetup.fHisto_xLower = std::min(hSetup.fHisto_xLower, hSetup.fHisto_xUpper);
                    hSetup.fHisto_xUpper = std::max(hSetup.fHisto_xLower, hSetup.fHisto_xUpper);
                    
                    //Output to User
                    printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms", ( "Sorry I was expecting a comma separated list of 2 numbers for: " + pair_strParam.first + "\n" ).c_str() );
                    printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms", "\tRight now I have set:\n" );
                    printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms", ("\t" + getString( hSetup.fHisto_xLower ) ).c_str() );
                    printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms", ("\t" + getString( hSetup.fHisto_xUpper ) ).c_str() );
                } //End Case: Not enough numbers
            } //End Case: Assign Histo Bin Range
            /*else if( 0 == pair_strParam.first.compare("HISTO_NAME") ) {
                hSetup.strHisto_Name = pair_strParam.second;
            }*/
            else if( 0 == pair_strParam.first.compare("HISTO_NUMBINS") ){
                hSetup.iHisto_nBins = Timing::stoiSafe(pair_strParam.first, pair_strParam.second);
            }
            else if( 0 == pair_strParam.first.compare("HISTO_XTITLE") ){
                hSetup.strHisto_Title_X = pair_strParam.second;
            }
            else if( 0 == pair_strParam.first.compare("HISTO_YTITLE") ){
                hSetup.strHisto_Title_Y = pair_strParam.second;
            }
            else if( 0 == pair_strParam.first.compare("PERFORM_FIT") ){
                hSetup.bFit = convert2bool(pair_strParam.second, bExitSuccess);
            }
            else{ //Case: Parameter not recognized
                printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms","Error!!! Parameter Not Recognized:\n");
                printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms",( "\tField = " + pair_strParam.first + "\n" ).c_str() );
                printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms",( "\tValue = " + pair_strParam.second + "\n" ).c_str() );

            } //End Case: Parameter not recognized
        } //End Case: Parameter Fetched Successfully
        else{ //Case: Parameter was NOT fetched Successfully
            printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms","Error!!!  I didn't parse parameter correctly\n");
            printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersHistograms",("\tCurrent line: " + strLine).c_str() );
        } //End Case: Parameter was NOT fetched Successfully
    } //End Loop through Section
    
    return;
} //End ParameterLoaderAnalysis::loadAnalysisParametersHistograms() - Histogram specific

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
        
        //Do we reach the end of the section?
        if ( 0 == strLine.compare(strSecEnd_Uniformity ) ) break;
        
        //Should we be storing histogram/fit setup parameters?
        if ( 0 == strLine.compare(strSecBegin_Uniformity_Fit) ) { //Case: Fit Setup
            loadAnalysisParametersFits(inputFileStream, aSetupUniformity.histoSetup_clustADC);
	    continue; //Tell it to move to the next loop iteration (e.g. line in file)
        } //End Case: Fit Setup
        else if( 0 == strLine.compare(strSecBegin_Uniformity_Histo) ){ //Case: Histo Setup
            loadAnalysisParametersHistograms(inputFileStream, aSetupUniformity);
	    continue; //Tell it to move to the next loop iteration (e.g. line in file)
        } //End Case: Histo Setup
        
        //Debugging
        cout<<"strLine: = " << strLine.c_str() << endl;
        
        //Parse the line
        pair_strParam = getParsedLine(strLine,bExitSuccess);
        
        if (bExitSuccess) { //Case: Parameter Fetched Correctly
            //transform(pair_strParam.first.begin(), pair_strParam.second.end(),pair_strParam.first.begin(),toupper);
            
            string strTmp = pair_strParam.first;
            transform(strTmp.begin(), strTmp.end(), strTmp.begin(), toupper);
            
            pair_strParam.first = strTmp;
            
            //cout<<pair_strParam.first<<"\t"<<pair_strParam.second;

            if ( 0 == pair_strParam.first.compare("CUT_ADC_MIN") ) {
                aSetupUniformity.selClust.iCut_ADCNoise = stoiSafe(pair_strParam.first,pair_strParam.second);
                //cout<<"\t"<<aSetupUniformity.selClust.iCut_ADCNoise<<endl;
            } //End Case: Minimum ADC Value
            else if( 0 == pair_strParam.first.compare("CUT_CLUSTERMULTI_MIN") ){ //Case: Min Cluster Multiplicity
                aSetupUniformity.selClust.iCut_MultiMin = stoiSafe(pair_strParam.first,pair_strParam.second);
            } //End Case: Max Cluster Multiplicity
            else if( 0 == pair_strParam.first.compare("CUT_CLUSTERMULTI_MAX") ){
                aSetupUniformity.selClust.iCut_MultiMax = stoiSafe(pair_strParam.first,pair_strParam.second);
            } //End Case:
            else if( 0 == pair_strParam.first.compare("CUT_CLUSTERSIZE_MIN") ) {
                aSetupUniformity.selClust.iCut_SizeMin = stoiSafe(pair_strParam.first,pair_strParam.second);
                //cout<<"\t"<<aSetupUniformity.selClust.iCut_SizeMin<<endl;
            } //End Case: Min Cluster Size
            else if( 0 == pair_strParam.first.compare("CUT_CLUSTERSIZE_MAX") ) {
                aSetupUniformity.selClust.iCut_SizeMax = stoiSafe(pair_strParam.first,pair_strParam.second);
                //cout<<"\t"<<aSetupUniformity.selClust.iCut_SizeMax<<endl;
            } //End Case: Max Cluster Size
            else if( 0 == pair_strParam.first.compare("CUT_CLUSTERTIME_MIN") ) {
                aSetupUniformity.selClust.iCut_TimeMin = stoiSafe(pair_strParam.first,pair_strParam.second);
                //cout<<"\t"<<aSetupUniformity.selClust.iCut_TimeMin<<endl;
            } //End Case: Min Cluster Time
            else if( 0 == pair_strParam.first.compare("CUT_CLUSTERTIME_MAX") ) {
                aSetupUniformity.selClust.iCut_TimeMax = stoiSafe(pair_strParam.first,pair_strParam.second);
                //cout<<"\t"<<aSetupUniformity.selClust.iCut_TimeMax<<endl;
            } //End Case: Max Cluster Time
            if( 0 == pair_strParam.first.compare("EVENT_FIRST") ){ //Case: ADC Spectrum Fit Equation
                aSetupUniformity.iEvt_First = stoiSafe(pair_strParam.second);
            } //End Case: ADC Spectrum Fit Equation
            else if( 0 == pair_strParam.first.compare("EVENT_TOTAL") ){ //Case: ADC Spectrum Fit Equation
                aSetupUniformity.iEvt_Total = stoiSafe(pair_strParam.second);
            } //End Case: ADC Spectrum Fit Equation
            else if( 0 == pair_strParam.first.compare("UNIFORMITY_GRANULARITY") ){ //Case: Uniformity Granularity
                aSetupUniformity.iUniformityGranularity = stoiSafe(pair_strParam.first,pair_strParam.second);
            } //End Case: Uniformity Granularity
            else if( 0 == pair_strParam.first.compare("UNIFORMITY_TOLERANCE") ){ //Case: Uniformity Granularity
                aSetupUniformity.fUniformityTolerance = stofSafe(pair_strParam.first,pair_strParam.second);
            } //End Case: Uniformity Granularity
            else{ //Case: Parameter Not Recognized
                printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersUniformity","Error!!! Parameter Not Recognizd:\n");
                printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersUniformity",( "\tField = " + pair_strParam.first + "\n" ).c_str() );
                printClassMethodMsg("ParameterLoaderAnaysis","loadAnalysisParametersUniformity",( "\tValue = " + pair_strParam.second + "\n" ).c_str() );
            } //End Case: Parameter Not Recognized
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
