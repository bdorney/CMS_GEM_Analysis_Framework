//
//  analyzeUniformity.cpp
//  
//
//  Created by Brian L Dorney on 25/01/16.
//
//

//C++ Includes
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

//Framework Includes
//#include "SelectorCluster.h"
//#include "SelectorHit.h"
#include "DetectorMPGD.h"   //Needs to be included before AnalyzeResponseUniformity.h and ParameterLoaderAmoreSRS.h
//#include "AnalyzeResponseUniformityClusters.h"
//#include "AnalyzeResponseUniformityHits.h"
#include "InterfaceAnalysis.h"
#include "ParameterLoaderAmoreSRS.h"
#include "ParameterLoaderAnaysis.h"
#include "ParameterLoaderRun.h"
#include "UniformityUtilityTypes.h"
//#include "VisualizeUniformity.h"

//ROOT Includes
#include "TROOT.h"
#include "TFile.h"

//Namespaces
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::map;
using std::pair;
using std::string;
using std::vector;

using QualityControl::Timing::convert2bool;
using QualityControl::Timing::printStreamStatus;
//using Timing::getlineNoSpaces;
//using Timing::getParsedLine;

using namespace QualityControl::Uniformity;

//Input Parameters
//  0 -> Executable
//  1 -> Run config file
//  2 -> verbose mode
//Usage examples:
//  Help menu: ./analyzeUniformity -h
//  Analysis: ./analyzeUniformity config/configRun.cfg true
int main( int argc_, char * argv_[] ){
    //Variable Declaration
    AnalysisSetupUniformity aSetup;
    
    bool bVerboseMode = false;
    
    DetectorMPGD detMPGD;
    
    ifstream file_Config;
    
    InterfaceAnalysis anaInterface;
    
    ParameterLoaderAmoreSRS loaderAmore;
    ParameterLoaderAnaysis loaderAnalysis;
    ParameterLoaderRun loaderRun;
    
    RunSetup rSetup;
    
    string strFile_Config_Run = "";
    
    vector<string> vec_strInputArgs;
    vector<string> vec_strInputFiles;
    vector<pair<int, string> > vec_pairedRunList;

    //Transfer Input Arguments into vec_strInputArgs
    //------------------------------------------------------
    vec_strInputArgs.resize(argc_);
    std::copy(argv_, argv_ + argc_, vec_strInputArgs.begin() );

    //Check input Arguments
    //------------------------------------------------------
    if(vec_strInputArgs.size() == 1 ){ //Case: Usage
        cout<<"analyzeUniformity\n";
        cout<<"Author: Brian L. Dorney\n";
        cout<<"Usage options:\n";
        cout<<"\tHelp Menu:\t"<<"./analyzeUniformity -h\n";
        cout<<"\tAnalysis:\t"<<"./analyzeUniformity <Run_Config_File> <Verbose Mode true/false>\n";
    
        return 1;
    } //End Case: Usage
    else if (vec_strInputArgs.size() == 2 && vec_strInputArgs[1].compare("-h") == 0) { //Case: Help Menu
        cout<<"analyzeUniformity\n";
        cout<<"Author: Brian L. Dorney\n";
        cout<<"Usage options:\n";
        cout<<"\tHelp Menu:\t"<<"./analyzeUniformity -h\n";
        cout<<"\tAnalysis:\t"<<"./analyzeUniformity <Run_Config_File> <Verbose Mode true/false>\n";

        //Format of run config file
        cout<<"Format of run config file (omit angle brackets, i.e. <. and >):\n";
        cout<<"#-----Start of Run Config File-----\n";
        cout<<"\t[BEGIN_RUN_INFO]\n";
        cout<<"\t\tConfig_Analysis = '<PHYSICAL FILENAME OF ANALYSIS CONFIG FILE>'\n";
        cout<<"\t\tConfig_Mapping = '<PHYSICAL FILENAME OF MAPPING CONFIG FILE>';\n";
        cout<<"\t\tInput_Is_Frmwrk_Output = '<ANALYZE PREVIOUS OUTPUT true/false>';\n";
        cout<<"\t\tOutput_File_Name = '<OUTPUT ROOT FILE NAME>';\n";
        cout<<"\t\tOutput_File_Option = '<OPTION FOR OUTPUT ROOT FILE (e.g. CREATE, RECRETAE, UPDATE, etc...)>';\n";
        cout<<"\t\tOutput_Individual = '<OUTPUT ONE ROOT FILE PER INPUT RUN true/false>';\n";
        cout<<"\t\tAna_Reco_Clusters = '<RECONSTRUCT CLUSTERS FROM THIT TREE true/false>';\n";
        cout<<"\t\tAna_Hits = '<PERFORM THE HIT ANALYSIS true/false>';\n";
        cout<<"\t\tAna_Clusters = '<PERFORM THE CLUSTER ANALYSIS true/false>';\n";
        cout<<"\t\tAna_Fitting = '<FIT OUTPUT HISTOGRAMS true/false>';\n";
        cout<<"\t\tVisualize_Plots = '<MAKE SUMMARY PLOTS true/false>';\n";
        cout<<"\t\tVisualize_DrawPhiLines = '<DENOTE IPHI REGIONS IN SUMMARY PLOTS true/false>';\n";
        cout<<"\t[END_RUN_INFO]\n";
        cout<<"\t[BEGIN_RUN_LIST]\n";
        cout<<"\t\t<PHYSICAL FILENAME OF INPUT FILE 1>\n";
        cout<<"\t\t...\n";
        cout<<"\t\t...\n";
        cout<<"\t\t...\n";
        cout<<"\t\t<PHYSICAL FILENAME OF INPUT FILE N>\n";
        cout<<"\t[END_RUN_LIST]\n";
        cout<<"#-----End of Run Config File-----\n";
        cout<<"The '#' symbol is recognized as a comment indication\n";
        
        //Format of analysis config
        cout<<"#-----Start of Analysis Config File-----\n";
        cout<<"\t[BEGIN_ANALYSIS_INFO]\n";
        cout<<"\t\t[BEGIN_UNIFORMITY_INFO]\n";
        cout<<"\t\t\tCut_ADC_Min = '<INTEGER>';\n";
        cout<<"\t\t\tCut_ClusterMulti_Min = '<INTEGER>';\n";
        cout<<"\t\t\tCut_ClusterMulti_Max = '<INTEGER>';\n";
        cout<<"\t\t\tCut_ClusterSize_Min = '<INTEGER>';\n";
        cout<<"\t\t\tCut_ClusterSize_Max = '<INTEGER>';\n";
        cout<<"\t\t\tCut_ClusterTime_Min = '<INTEGER>';\n";
        cout<<"\t\t\tCut_ClusterTime_Max = '<INTEGER>';\n";
        cout<<"\t\t\tCut_HitMulti_Min = '<INTEGER>';\n";
        cout<<"\t\t\tCut_HitMulti_Max = '<INTEGER>';\n";
        cout<<"\t\t\tCut_HitTime_Min = '<INTEGER>';\n";
        cout<<"\t\t\tCut_HitTime_Max = '<INTEGER>';\n";
        cout<<"\t\t\tEvent_First = '<INTEGER>';\n";
        cout<<"\t\t\tEvent_Total = '<INTEGER>';\n";
        cout<<"\t\t\tUniformity_Granularity = '<INTEGER>';\n";
        cout<<"\t\t\t[BEGIN_ADC_FIT_INFO]\n";
        cout<<"\t\t\t\tFit_Formula = '<STRING>';\n";
        cout<<"\t\t\t\tFit_Option = '<STRING>';\n";
        cout<<"\t\t\t\tFit_Param_Map = '<COMMA SEPARATED LIST OF STRINGS (SEE README.txt)>';\n";
        cout<<"\t\t\t\tFit_Param_IGuess = '<COMMA SEPARATED LIST OF STRINGS (SEE README.txt)>';\n";
        cout<<"\t\t\t\tFit_Param_Limit_Min = '<COMMA SEPARATED LIST OF STRINGS (SEE README.txt)>';\n";
        cout<<"\t\t\t\tFit_Param_Limit_Max = '<COMMA SEPARATED LIST OF STRINGS (SEE README.txt)>';\n";
        cout<<"\t\t\t\tFit_Range = '<COMMA SEPARATED LIST OF FLOATS>';\n";
        cout<<"\t\t\t[END_ADC_FIT_INFO]\n";
        cout<<"\t\t\t[BEGIN_HISTO_INFO]\n";
        cout<<"\t\t\t\tHisto_Name = '<STRING>';\n";
        cout<<"\t\t\t\tHisto_XTitle = '<STRING>';\n";
        cout<<"\t\t\t\tHisto_YTitle = '<STRING>';\n";
        cout<<"\t\t\t\tHisto_BinRange = '<COMMA SEPARATED LIST OF FLOATS>';\n";
        cout<<"\t\t\t\tHisto_NumBins = '<INTEGER>';\n";
        cout<<"\t\t\t[END_HISTO_INFO]\n";
        cout<<"\t\t\t...Defining Histograms...\n";
        cout<<"\t\t\t...Defining Histograms...\n";
        cout<<"\t\t\t...See README.txt for possible Histograms...\n";
        cout<<"\t\t\t...See README.txt for possible Histograms...\n";
        cout<<"\t\t\t...Defining Histograms...\n";
        cout<<"\t\t\t...Defining Histograms...\n";
        cout<<"\t\t\t[BEGIN_HISTO_INFO]\n";
        cout<<"\t\t\t\tHisto_Name = '<STRING>';\n";
        cout<<"\t\t\t\tHisto_XTitle = '<STRING>';\n";
        cout<<"\t\t\t\tHisto_YTitle = '<STRING>';\n";
        cout<<"\t\t\t\tHisto_BinRange = '<COMMA SEPARATED LIST OF FLOATS>';\n";
        cout<<"\t\t\t\tHisto_NumBins = '<INTEGER>';\n";
        cout<<"\t\t\t[END_HISTO_INFO]\n";
        cout<<"\t\t[END_UNIFORMITY_INFO]\n";
        cout<<"\t[END_ANALYSIS_INFO]\n";
        cout<<"#-----End of Analysis Config File-----\n";
        cout<<"The '#' symbol is recognized as a comment indication\n";

        return 0;
    } //End Case: Help Menu
    else if(vec_strInputArgs.size() == 3){ //Case: Analysis!
        bool bExitSuccess = false;
        
        //Set the verbose mode
        bVerboseMode = convert2bool(vec_strInputArgs[2], bExitSuccess);
        if (!bExitSuccess) { //Case: Input Not Understood
            cout<<"main() - vec_strInputArgs[3] expected to be boolean!!!\n";
            cout<<"main() - Parameter given:\n";
            cout<<"\t"<<vec_strInputArgs[2]<<endl;
            cout<<"Exitting!!!\n";
            
            return -2;
        } //End Case: Input Not Understood

        //Set the run config file
        strFile_Config_Run = vec_strInputArgs[1];
        
        //Load the Input Config File
        //------------------------------------------------------
        file_Config.open( vec_strInputArgs[1].c_str() );
        
        //Check to see if the config file opened successfully
        if (!file_Config.is_open()) {
            perror( ("main() - error while opening file: " + vec_strInputArgs[1]).c_str() );
            printStreamStatus(file_Config);
            
            cout<<"Exitting!!!\n";
            
            return -1;
        } //End Case: Input Not Understood
    } //End Case: Analysis!
    else{ //Case: Input Not Understood
        cout<<"main(): I was expecting either 2 or 3 input parameters.  But you provided:\n";
        
        for (int i=0; i < vec_strInputArgs.size(); ++i) { //Loop over input parameters
            cout<<"\t"<<vec_strInputArgs[i]<<endl;
        } //End Loop over input parameters
        
        cout<<"main(): please call './analyzeUniformity -h' for help\n";
        cout<<"main(): exiting\n";
        
        return -3;
    } //End Case: Input Not Understood
    
    //Set the Run Info and get input file(s)
    //------------------------------------------------------
    rSetup = loaderRun.getRunParameters(file_Config, bVerboseMode);
    
    if (!rSetup.bLoadSuccess) {
        perror( ("main() - error while setting Run Setup config from file: " + vec_strInputArgs[1]).c_str() );
        printStreamStatus(file_Config);
        
        cout<<"Exitting!!!\n";
        
        return -1;
    }
    
    if( rSetup.bInputFromFrmwrk ){ //Case: Framework Input
        vec_strInputFiles = loaderRun.getRunList(file_Config, bVerboseMode);
        
        if (vec_strInputFiles.size() == 0) {
            cout<<"main() - no valid runs found in " << vec_strInputArgs[1].c_str() << endl;
            cout<<"\tExiting! Please cross-check input file!!!\n";
            
            return -4;
        }
        
    } //End Case: Framework Input
    else { //Case: amoreSRS Input
        vec_pairedRunList = loaderRun.getPairedRunList(file_Config, bVerboseMode);
        
        //Check if
        if (vec_pairedRunList.size() == 0) {
            cout<<"main() - no valid runs found in " << vec_strInputArgs[1].c_str() << endl;
            cout<<"\tMaybe you forgot to have a field 'RunX' in the input filenames?\n";
            cout<<"\tThis should be separated by underscores e.g. '_', and for each input file be some unique interger X\n";
            cout<<"\tExiting! Please cross-check input file!!!\n";
            
            return -4;
        }
    } //End Case: amoreSRS Input
    
    file_Config.close();
    
    //Load the requested amore parameters & setup the detector
    //------------------------------------------------------
    loaderAmore.loadAmoreMapping( rSetup.strFile_Config_Map  );
    detMPGD = loaderAmore.getDetector();
    detMPGD.setName( rSetup.strDetName );
    
    //Load the requested analysis parameters
    //------------------------------------------------------
    aSetup = loaderAnalysis.getAnalysisParameters( rSetup.strFile_Config_Ana );

    //Setup the analysis interface
    //------------------------------------------------------
    anaInterface.setAnalysisParameters(aSetup);
    anaInterface.setDetector(detMPGD);
    anaInterface.setRunParameters(rSetup);
    //anaInterface.initialize(aSetup, rSetup, detMPGD);
    anaInterface.setVerboseMode(bVerboseMode);
    
    //Perform the user defined analysis interface
    //------------------------------------------------------
    if( rSetup.bInputFromFrmwrk ){	anaInterface.analyzeInput(vec_strInputFiles); }
    else {                          anaInterface.analyzeInput(vec_pairedRunList); }

    cout<<"Success!"<<endl;
    
    return 0;
} //End main()
