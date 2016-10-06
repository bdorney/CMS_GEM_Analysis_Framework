//
//  frameworkMain.cpp
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
#include <memory>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

//Framework Includes
#include "DetectorMPGD.h"   //Needs to be included before AnalyzeResponseUniformity.h and ParameterloadDetectorSRS.h
#include "InterfaceAnalysis.h"
#include "ParameterLoaderDetector.h"
#include "ParameterLoaderAnaysis.h"
#include "ParameterLoaderRun.h"
#include "QualityControlSectionNames.h"
#include "SRSMain.h"
#include "UniformityUtilityTypes.h"
#include "VisualizeComparison.h"

//ROOT Includes
#include "TROOT.h"
#include "TFile.h"

//Namespaces
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
//using std::make_unique; //requires C++14
using std::map;
using std::pair;
using std::string;
using std::unique_ptr;
using std::vector;

using QualityControl::Timing::convert2bool;
using QualityControl::Timing::printStreamStatus;

using namespace QualityControl::Uniformity;

//Print the Help Menu
void printHelpMenu(){
    //Calling Syntax
    cout<<endl;
    cout<<endl;
    cout<<"------------------------------------------------------------------------------------------\n";
    cout<<"frameworkMain\n";
    cout<<"Author: Brian L. Dorney\n";
    cout<<"Usage options:\n";
    cout<<"\tThis Menu:\t"<<"./frameworkMain -h\n";
    cout<<"\tAnalysis:\t"<<"./frameworkMain <Run_Config_File> <Verbose Mode true/false>\n";
    
    //Summary Info
    cout<<endl;
    cout<<endl;
    cout<<"------------------------------------------------------------------------------------------\n";
    cout<<"\tThe Run_Config_File must have a 'run list' header, either a 'run info' header or 'compare info' header\n";
    cout<<"\tExamples of each header are shown below.";
    cout<<endl;
    cout<<"\tIncluding the 'run info' header will use the input files to produce a framework output file\n";
    cout<<"\tHere you must have a Config_Analysis & Config_Mapping file, examples of Config_Analysis shown below\n";
    cout<<"\tThe Config_Mapping file is your amoreSRS mapping file, default files are included in $GEM_BASE/config/\n";
    cout<<endl;
    cout<<"\tIf Including the 'compare info' header the input must be framework output files\n";
    cout<<"\t These input files must be have been produced when running with a 'run info' header\n";
    cout<<endl;
    cout<<"\tIf you have both a 'run info' and a 'compare info' header only the last one will be used\n";
    
    //Header - Run List
    cout<<endl;
    cout<<endl;
    cout<<"------------------------------------------------------------------------------------------\n";
    cout<<"\tRun List Header (include the lines below in your Run_Config_File):";
    cout<<"\t(omit angle brackets, i.e. <. and >)\n";
    cout<<endl;
    cout<<"\t[BEGIN_RUN_LIST]\n";
    cout<<"\t\t<PHYSICAL FILENAME OF INPUT FILE 1>\n";
    cout<<"\t\t...\n";
    cout<<"\t\t...\n";
    cout<<"\t\t...\n";
    cout<<"\t\t<PHYSICAL FILENAME OF INPUT FILE N>\n";
    cout<<"\t[END_RUN_LIST]\n";
    
    //Header - Run Info
    cout<<endl;
    cout<<endl;
    cout<<"------------------------------------------------------------------------------------------\n";
    cout<<"\tRun Info Header (include the lines below in your Run_Config_File):";
    cout<<"\t(omit angle brackets, i.e. <. and >)\n";
    cout<<endl;
    cout<<"\t[BEGIN_RUN_INFO]\n";
    cout<<"\t\tConfig_Analysis = '<PHYSICAL FILENAME OF ANALYSIS CONFIG FILE>'\n";
    cout<<"\t\tConfig_Mapping = '<PHYSICAL FILENAME OF MAPPING CONFIG FILE>';\n";
    cout<<"\t\tInput_Is_Frmwrk_Output = '<ANALYZE PREVIOUS OUTPUT true/false>';\n";
    cout<<"\t\tInput_Identifier = '<STRING IDENTIFIER FOUND IN FILENAME>';\n";
    cout<<"\t\tOutput_File_Name = '<OUTPUT ROOT FILE NAME>';\n";
    cout<<"\t\tOutput_File_Option = '<OPTION FOR OUTPUT ROOT FILE (e.g. CREATE, RECRETAE, UPDATE, etc...)>';\n";
    cout<<"\t\tOutput_Individual = '<OUTPUT ONE ROOT FILE PER INPUT RUN true/false>';\n";
    cout<<"\t\tAna_Reco_Clusters = '<RECONSTRUCT CLUSTERS FROM THIT TREE true/false>';\n";
    cout<<"\t\tAna_Hits = '<PERFORM THE HIT ANALYSIS true/false>';\n";
    cout<<"\t\tAna_Clusters = '<PERFORM THE CLUSTER ANALYSIS true/false>';\n";
    cout<<"\t\tAna_Fitting = '<FIT OUTPUT HISTOGRAMS true/false>';\n";
    cout<<"\t\tVisualize_AutoSaveImages = '<SAVE CANVASES AS *.PDF & *.PNG true/false>';\n";
    cout<<"\t\tVisualize_DrawPhiLines = '<DENOTE IPHI REGIONS IN SUMMARY PLOTS true/false>';\n";
    cout<<"\t\tVisualize_Plots = '<MAKE SUMMARY PLOTS true/false>';\n";
    cout<<"\t[END_RUN_INFO]\n";
    
    //Header - Comp Info
    cout<<endl;
    cout<<endl;
    cout<<"------------------------------------------------------------------------------------------\n";
    cout<<"\tComp Info Header (include the lines below in your Run_Config_File):";
    cout<<"\t(omit angle brackets, i.e. <. and >)\n";
    cout<<endl;
    cout<<"\t[BEGIN_COMP_INFO]\n";
    cout<<"\t\tObs_Name = '<OBSERVABLE NAME>';\n";
    cout<<"\t\tObs_Eta = '<ETA INDEX or -1>';\n";
    cout<<"\t\tObs_Phi = '<PHI INDEX or -1>';\n";
    cout<<"\t\tObs_Eta = '<SLICE INDEX or -1>';\n";
    cout<<"\t\tInput_Identifier = '<STRING IDENTIFIER FOUND IN FILENAME>';\n";
    cout<<"\t\tOutput_File_Name = '<OUTPUT ROOT FILE NAME>';\n";
    cout<<"\t\tOutput_File_Option = '<OPTION FOR OUTPUT ROOT FILE (e.g. CREATE, RECRETAE, UPDATE, etc...)>';\n";
    cout<<"\t\tVisualize_AutoSaveImages = '<SAVE CANVASES AS *.PDF & *.PNG true/false>';\n";
    cout<<"\t\tVisualize_DrawNormalized = '<NORMALIZE COMPARED PLOTS TO UNITY true/false>';\n";
    cout<<"\t\tVisualize_DrawPhiLines = '<DENOTE IPHI REGIONS IN SUMMARY PLOTS true/false>';\n";
    cout<<"\t[END_COMP_INFO]\n";
    
    //Format of analysis config
    cout<<endl;
    cout<<endl;
    cout<<"------------------------------------------------------------------------------------------\n";
    cout<<"#-----Start of Analysis Config File-----\n";
    cout<<"\t(omit angle brackets, i.e. <. and >)\n";
    cout<<endl;
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
    
    return;
} //End printHelpMenu

//Perform the Analysis
void runModeAnalysis(RunSetup & rSetup, vector<string> & vec_strInputFiles, vector<pair<int, string> > & vec_pairedRunList, bool bVerboseMode){
    //Load the requested amore parameters & setup the detector
    //------------------------------------------------------
    ParameterLoaderDetector loadDetector;
    loadDetector.loadAmoreMapping( rSetup.strFile_Config_Map  );
    
    DetectorMPGD detMPGD;
    detMPGD = loadDetector.getDetector();
    detMPGD.setName( rSetup.strDetName );
    
    //Load the requested analysis parameters
    //------------------------------------------------------
    ParameterLoaderAnaysis loaderAnalysis;
    AnalysisSetupUniformity aSetup = loaderAnalysis.getAnalysisParameters( rSetup.strFile_Config_Ana );
    
    //Setup the analysis interface
    //------------------------------------------------------
    InterfaceAnalysis anaInterface;
    anaInterface.setAnalysisParameters(aSetup);
    anaInterface.setDetector(detMPGD);
    anaInterface.setRunParameters(rSetup);
    anaInterface.setVerboseMode(bVerboseMode);
    
    //Perform the user defined analysis interface
    //------------------------------------------------------
    if( rSetup.bInputFromFrmwrk ){	anaInterface.analyzeInput(vec_strInputFiles); }
    else {                          anaInterface.analyzeInput(vec_pairedRunList); }
    
    return;
} //End runModeAnalysis()

//Performs the Reconstruction
void runModeReconstruction(RunSetup & rSetup, vector<pair<int, string> > & vec_pairedRunList){
    for (auto iterRun = vec_pairedRunList.begin(); iterRun != vec_pairedRunList.end(); ++iterRun) { //Loop Over input Runs
        //C++14 only
        //unique_ptr<SRSMain> recoInterface = make_unique<SRSMain>(& SRSMain::Reprocessor( (*iterRun).second, rSetup.strFile_Config_Reco ) );
        
        unique_ptr<SRSMain> recoInterface(new SRSMain( (*iterRun).second, rSetup.strFile_Config_Reco ) );
        recoInterface->Reprocess();
        
        //remove the *.raw from the filename, append with _dataTree.root
        if ( (*iterRun).second.find(".raw") != std::string::npos ) {
            (*iterRun).second.erase((*iterRun).second.find(".raw"), (*iterRun).second.length() - (*iterRun).second.find(".raw") );
            (*iterRun).second = (*iterRun).second + "_dataTree.root";
        }
        
        //Delete the interface
        recoInterface.reset();
    } //End Loop Over input Runs
    
    return;
} //End runModeReconstruction()

//Input Parameters
//  0 -> Executable
//  1 -> Run config file
//  2 -> verbose mode
//Usage examples:
//  Help menu: ./frameworkMain -h
//  Analysis: ./frameworkMain config/configRun.cfg true
int main( int argc_, char * argv_[] ){
    //Transfer Input Arguments into vec_strInputArgs
    //------------------------------------------------------
    vector<string> vec_strInputArgs;
    vec_strInputArgs.resize(argc_);
    std::copy(argv_, argv_ + argc_, vec_strInputArgs.begin() );

    //Check input Arguments, load parameters on good input
    //------------------------------------------------------
    bool bVerboseMode = false;
    ifstream file_Config;
    ParameterLoaderRun loaderRun;
    if(vec_strInputArgs.size() == 1 ){ //Case: Usage
        printHelpMenu();
        
        return 1;
    } //End Case: Usage
    else if (vec_strInputArgs.size() == 2 && vec_strInputArgs[1].compare("-h") == 0) { //Case: Help Menu
        printHelpMenu();

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
            cout<<"main(): for help menu call: ./frameworkMain -h\n";
            cout<<"main(): exitting\n";
            
            return -2;
        } //End Case: Input Not Understood

        //Set the run config file
        //strFile_Config_Run = vec_strInputArgs[1];
        
        //Load the Input Config File
        //------------------------------------------------------
        //file_Config.open( vec_strInputArgs[1].c_str() );
        //file_Config = loaderRun.getFileStream( vec_strInputArgs[1], bVerboseMode );
        loaderRun.setFileStream( vec_strInputArgs[1], file_Config, bVerboseMode );

        //Check to see if the config file opened successfully
        if (!file_Config.is_open()) {
            perror( ("main() - error while opening file: " + vec_strInputArgs[1]).c_str() );
            printStreamStatus(file_Config);
            cout<<"main(): for help menu call: ./frameworkMain -h\n";
            cout<<"main(): exitting\n";
            
            return -1;
        } //End Case: Input Not Understood
    } //End Case: Analysis!
    else{ //Case: Input Not Understood
        cout<<"main(): I was expecting either 2 or 3 input parameters.  But you provided:\n";
        
        for (int i=0; i < vec_strInputArgs.size(); ++i) { //Loop over input parameters
            cout<<"\t"<<vec_strInputArgs[i]<<endl;
        } //End Loop over input parameters
        
        cout<<"main(): for help menu call: ./frameworkMain -h\n";
        cout<<"main(): exiting\n";
        
        return -3;
    } //End Case: Input Not Understood
    
    //Set the Run Info and get input file(s)
    //------------------------------------------------------
    RunSetup rSetup = loaderRun.getRunParameters(file_Config, bVerboseMode);
    
    if (!rSetup.bLoadSuccess) {
        perror( ("main() - error while setting Run Setup config from file: " + vec_strInputArgs[1]).c_str() );
        printStreamStatus(file_Config);
        cout<<"main(): for help menu call: ./frameworkMain -h\n";
        cout<<"main(): exitting\n";
        
        return -1;
    }
    
    vector<string> vec_strInputFiles;
    vector<pair<int, string> > vec_pairedRunList;
    if( rSetup.bInputFromFrmwrk ){ //Case: Framework Input
        vec_strInputFiles = loaderRun.getRunList(file_Config, bVerboseMode);
        
        if (vec_strInputFiles.size() == 0) {
            cout<<"main() - no valid runs found in " << vec_strInputArgs[1].c_str() << endl;
            cout<<"main(): for help menu call: ./frameworkMain -h\n";
            cout<<"main(): exiting\n";
            
            return -4;
        }
        
    } //End Case: Framework Input
    else { //Case: amoreSRS Input
        vec_pairedRunList = loaderRun.getPairedRunList(file_Config, rSetup.strIdent, bVerboseMode);
        
        //Check if
        if (vec_pairedRunList.size() == 0) {
            cout<<"main() - no valid runs found in " << vec_strInputArgs[1].c_str() << endl;
            cout<<"\tMaybe you forgot to have a field 'RunX' in the input filenames?\n";
            cout<<"\tThis should be separated by underscores e.g. '_', and for each input file be some unique interger X\n";
            cout<<"main(): for help menu call: ./frameworkMain -h\n";
            cout<<"main(): exiting\n";
            
            return -4;
        }
    } //End Case: amoreSRS Input
    
    file_Config.close();
    
    //Check the Analysis Run Mode
    //------------------------------------------------------
    RunModes m_modes_run;

	//Debugging
	cout<<"rSetup.strRunMode = " << rSetup.strRunMode.c_str() << endl;

    if ( 0 == rSetup.strRunMode.compare( m_modes_run.m_strOnlyAna ) ) { //Run Mode: Analysis
        /*//Load the requested amore parameters & setup the detector
        //------------------------------------------------------
        ParameterLoaderDetector loadDetector;
        loadDetector.loadAmoreMapping( rSetup.strFile_Config_Map  );
        
        DetectorMPGD detMPGD;
        detMPGD = loadDetector.getDetector();
        detMPGD.setName( rSetup.strDetName );

        //Load the requested analysis parameters
        //------------------------------------------------------
        ParameterLoaderAnaysis loaderAnalysis;
        AnalysisSetupUniformity aSetup = loaderAnalysis.getAnalysisParameters( rSetup.strFile_Config_Ana );

        //Setup the analysis interface
        //------------------------------------------------------
        InterfaceAnalysis anaInterface;
        anaInterface.setAnalysisParameters(aSetup);
        anaInterface.setDetector(detMPGD);
        anaInterface.setRunParameters(rSetup);
        anaInterface.setVerboseMode(bVerboseMode);
        
        //Perform the user defined analysis interface
        //------------------------------------------------------
        if( rSetup.bInputFromFrmwrk ){	anaInterface.analyzeInput(vec_strInputFiles); }
        else {                          anaInterface.analyzeInput(vec_pairedRunList); }*/
        
        runModeAnalysis(rSetup, vec_strInputFiles, vec_pairedRunList, bVerboseMode);
    } //End Run Mode: Analysis
    else if ( 0 == rSetup.strRunMode.compare( m_modes_run.m_strOnlyCompare ) ){ //Run Mode: Comparison
        VisualizeComparison visualizeComp;
        
        visualizeComp.setDrawOption(rSetup.strDrawOption);
        visualizeComp.setIdentifier(rSetup.strIdent);
        visualizeComp.setInputFiles(vec_strInputFiles);
        visualizeComp.setNormalize(rSetup.bDrawNormalized);
        visualizeComp.setPosFull(rSetup.iEta, rSetup.iPhi, rSetup.iSlice );
        
        visualizeComp.storeCanvasComparisonHisto(
            rSetup.strFile_Output_Name,
            rSetup.strFile_Output_Option,
            rSetup.strObsName
        );
    } //End Run Mode: Comparison
    else if ( 0 == rSetup.strRunMode.compare( m_modes_run.m_strOnlyReco ) ) { //Run Mode: Reconstruction
	//Right now this is only supported for one input file
        /*for (auto iterRun = vec_pairedRunList.begin(); iterRun != vec_pairedRunList.end(); ++iterRun) { //Loop Over input Runs
            //C++14 only
            //unique_ptr<SRSMain> recoInterface = make_unique<SRSMain>(& SRSMain::Reprocessor( (*iterRun).second, rSetup.strFile_Config_Reco ) );
            
            unique_ptr<SRSMain> recoInterface(new SRSMain( (*iterRun).second, rSetup.strFile_Config_Reco ) );
            recoInterface->Reprocess();
            
            recoInterface.reset();
        } //End Loop Over input Runs*/
        
        runModeReconstruction(rSetup, vec_pairedRunList);
    } //End Run Mode: Reconstruction
    else if ( 0 == rSetup.strRunMode.compare( m_modes_run.m_strRecoNAna) ) { //Run Mode: Reconstruction & Analysis
        //Reconstruct Events
        runModeReconstruction(rSetup, vec_pairedRunList);

        //Analyze Events
        runModeAnalysis(rSetup, vec_strInputFiles, vec_pairedRunList, bVerboseMode);
    } //End Run Mode: Reconstruction & Analysis
    else{ //Run Mode: Unrecognized
        cout<<"main() - Run Mode: " << rSetup.strRunMode << " not recognized!\n";
        cout<<"\tPlease double check input run config file.\n";
        cout<<"\tExiting\n";
        
        return -5;
    } //End Run Mode: Unrecognized
    
    cout<<"Finished!"<<endl;
    
    return 0;
} //End main()
