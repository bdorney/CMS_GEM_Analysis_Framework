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
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

//Framework Includes
#include "SelectorCluster.h"
#include "SelectorHit.h"
#include "DetectorMPGD.h"   //Needs to be included before AnalyzeResponseUniformity.h and ParameterLoaderAmoreSRS.h
#include "AnalyzeResponseUniformityClusters.h"
#include "AnalyzeResponseUniformityHits.h"
#include "ParameterLoaderAmoreSRS.h"
#include "ParameterLoaderAnaysis.h"
#include "UniformityUtilityTypes.h"
#include "VisualizeUniformity.h"

//ROOT Includes

//Namespaces
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

using Timing::convert2bool;
using Timing::getlineNoSpaces;
using Timing::getParsedLine;

using namespace Uniformity;

struct RunSetup{
    bool bAnaStep_Clusters;         //true -> perform the cluster analysis; false -> do not
    bool bAnaStep_Fitting;          //true -> run fitting on output histo's; false -> do not
    bool bAnaStep_Hits;             //true -> perform the hit analysis (NOTE if bAnaStep_Reco is true this must also be true); false -> do not
    bool bAnaStep_Reco;             //true -> reconstruct clusters from input hits; false -> use clusters provided in amoreSRS output file;
    bool bAnaStep_Visualize;        //true -> make summary plots at end of analysis; false -> do not
    
    bool bInputFromFrmwrk;          //true -> input file is a framework output file, not from amoreSRS; false -> input file(s) are from amoreSRS
    
    bool bMultiOutput;              //true -> one output file per input run; false -> one output file representing the "sum" of the input runs
    bool bVisPlots_PhiLines;        //true -> summary plots have phi lines segmenting sectors; false -> they do not
    
    string strFile_Config_Ana;      //Name of analysis config file
    string strFile_Config_Map;      //Name of mapping file
    
    string strFile_Output_Name;     //Name of output TFile to be created
    string strFile_Output_Option;   //Option for TFile: CREATE, RECREATE, UPDATE, etc...
    
    //Default constructor
    RunSetup(){
        bAnaStep_Reco = false;
        bAnaStep_Clusters = bAnaStep_Fitting = bAnaStep_Hits = bAnaStep_Visualize = true;
        
        bInputFromFrmwrk = false;
        
        bMultiOutput = false;
        
        bVisPlots_PhiLines = true;
        
        strFile_Config_Ana = "config/configAnalysis.cfg";
        strFile_Config_Map = "config/GE7MappingCMScernData2016.cfg";
        
        strFile_Output_Name = "FrameworkOutput.root";
        strFile_Output_Option = "RECREATE";
    } //End Default constructor
};

//Gets the list of input runs from the input config file
vector<string> getRunList(ifstream &file_Input, bool bVerboseMode){
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
        if ( strLine.compare( "[BEGIN_RUN_LIST]" ) == 0 ) { //Case: Run list header
            
            while ( getlineNoSpaces(file_Input, strLine) ) { //Loop through run list header
                //Skip commented lines
                if (strLine.compare(0,1,"#") == 0 ) continue;
                
                //Has the header ended?
                if ( strLine.compare( "[END_RUN_LIST]" ) == 0 ) { //Case: End of run list header
                    if (bVerboseMode) { //Case: User Requested Verbose Input/Output
                        cout<<"getRunList(): End of run list header reached!\n";
                        cout<<"getRunList(): The following runs will be analyzed:\n";
                        
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
} ///End getRunList

//Gets the run setup parameters from the input config file
void setRunSetup(ifstream &file_Input, bool bVerboseMode, RunSetup & inputRunSetup){
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
        if ( strLine.compare( "[BEGIN_RUN_INFO]" ) == 0 ) { //Case: Run info header found!
            cout<<"setRunSetup(): Run info header found!\n";
            
            while ( getlineNoSpaces(file_Input, strLine) ) { //Loop through run info header
                bool bExitSuccess;
                
                //Skip commented lines
                if (strLine.compare(0,1,"#") == 0) continue;
        
                //Has this header ended?
                //Has the header ended?
                if ( strLine.compare( "[END_RUN_INFO]" ) == 0 ) { //Case: End of run list header
                    if (bVerboseMode) { //Case: User Requested Verbose Input/Output
                        cout<<"setRunSetup(): End of run info header reached!\n";
                        
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
                        cout<<"setRunSetup(): input field name:\n";
                        cout<<"\t"<<pair_strParam.first<<endl;
                        cout<<"setRunSetup(): not recognized! Please cross-check input file!!!\n";
                    } //End Case: Parameter Not Recognized
                } //End Case: Parameter Fetched Successfully
                else{ //Case: Input line did NOT parse correctly
                    cout<<"setRunSetup(): input line:\n";
                    cout<<"\t"<<strLine<<endl;
                    cout<<"setRunSetup(): did not parse correctly, please cross-check input file\n";
                } //End Case: Input line did NOT parse correctly
            } //End Loop through run info header
        } //End Case: Run info header found!
        
        if (bHeaderEnd) break;
    } //End Loop through input file
    if ( file_Input.bad() && bVerboseMode) {
        //perror( ("getProInfo(): error while reading file: " + strInputFileName).c_str() );
        perror( "getProInfo(): error while reading config file" );
        Timing::printStreamStatus(file_Input);
    }
    
    //Do not close input file, it will be used elsewhere
    
    return;
} //End setRunSetup

//Input Parameters
//  0 -> Executable
//  1 -> Run config file
//  2 -> verbose mode
//Usage examples:
//  Help menu: ./analyzeUniformity -h
//  Analysis: ./analyzeUniformity config/configRun.cfg true
int main( int argc_, char * argv_[] ){
    //Variable Declaration
    AnalyzeResponseUniformityClusters clustAnalyzer;
    AnalyzeResponseUniformityHits hitAnalyzer;
    
    bool bVerboseMode = false;
    
    ifstream file_Config;
    
    ParameterLoaderAmoreSRS amoreLoader;
    ParameterLoaderAnaysis analysisLoader;
    
    RunSetup rSetup;
    
    SectorEta etaSector;
    
    SelectorCluster clustSelector;
    SelectorHit hitSelector;
    
    vector<string> vec_strInputArgs;
    vector<string> vec_strInputFiles;
    
    VisualizeUniformity visualizeUni;

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

        //Set input config file
        //The below is never actually running...
        if (bVerboseMode) { //Case: User Requested Verbose Error Messages - I/O
            cout<< "main(): trying to open and read: " << vec_strInputArgs[1] << endl;
        } //End Case: User Requested Verbose Error Messages - I/O
        
        file_Config.open(vec_strInputArgs[1].c_str() );
        
        //Check to see if run list file opened successfully
        if (!file_Config.is_open()) {
            perror( ("main(): error while opening file: " + vec_strInputArgs[1]).c_str() );
            Timing::printStreamStatus(file_Config);
            
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
    setRunSetup(file_Config, bVerboseMode, rSetup);
    vec_strInputFiles = getRunList(file_Config, bVerboseMode);
    
    //Load amore mapping file & setup the DetectorMPGD object
    //------------------------------------------------------
    amoreLoader.loadAmoreMapping( rSetup.strFile_Config_Map );
    DetectorMPGD detMPGD = amoreLoader.getDetector();
    
    //Print the detector info to the user
    if (bVerboseMode) { //Case: Verbose Printing
        cout<<"iEta\tY_Pos\tWidth\tX_Low\tX_High\tX_Low\tX_High\tX_Low\tX_High\n";
        for (int i=1; i <= detMPGD.getNumEtaSectors(); ++i) { //Loop through Detector's SectorEta objects
            cout<<i<<"\t";
            
            etaSector = detMPGD.getEtaSector(i);
            
            cout<<etaSector.fPos_Y<<"\t"<<etaSector.fWidth<<"\t";
            
            for (auto iterPhi = etaSector.map_sectorsPhi.begin(); iterPhi != etaSector.map_sectorsPhi.end(); ++iterPhi) { //Loop through SectorEta's SectorPhi objects
                cout<<(*iterPhi).second.fPos_Xlow<<"\t"<<(*iterPhi).second.fPos_Xhigh<<"\t";
            } //End Loop through SectorEta's SectorPhi objects
            
            cout<<endl;
        } //End Loop through Detector's SectorEta objects
        
    } //End Case: Verbose Printing
    
    //Load the requested analysis parameters
    //------------------------------------------------------
    Uniformity::AnalysisSetupUniformity aSetup = analysisLoader.getAnalysisParameters( rSetup.strFile_Config_Ana );

	//Print the analysis parameters to the user
    if (bVerboseMode) { //Case: Print user defined selection cuts
        cout<<"-----------------Hit Parameters-----------------\n";
        cout<<"Hit ADC Min = " << aSetup.selHit.iCut_ADCNoise << endl;
        cout<<"Hit ADC Max = " << aSetup.selHit.iCut_ADCSat << endl;
        cout<<"Hit Multi Min = " << aSetup.selHit.iCut_MultiMin << endl;
        cout<<"Hit Multi Max = " << aSetup.selHit.iCut_MultiMax << endl;
        cout<<"Hit Time, Min = " << aSetup.selHit.iCut_TimeMin << endl;
        cout<<"Hit Time, Max = " << aSetup.selHit.iCut_TimeMax << endl;
        cout<<"-----------------Cluster Parameters-----------------\n";
        cout<<"Clust ADC, Min = " << aSetup.selClust.iCut_ADCNoise << endl;
        cout<<"Clust Size, Min = " << aSetup.selClust.iCut_SizeMin << endl;
        cout<<"Clust Size, Max = " << aSetup.selClust.iCut_SizeMax << endl;
        cout<<"Clust Time, Min = " << aSetup.selClust.iCut_TimeMin << endl;
        cout<<"Clust Time, Max = " << aSetup.selClust.iCut_TimeMax << endl;
    } //End Case: Print user defined selection cuts
    
    //Loop over input files
    //------------------------------------------------------
    for (int i=0; i < vec_strInputFiles.size(); ++i) { //Loop over vec_strInputFiles
        //Wipe physics objects from previous file (prevent double counting)
        detMPGD.resetPhysObj();
        
        //Hit Analysis
        //Force the hit analysis if the user requested cluster reconstruction
        if ( rSetup.bAnaStep_Hits || rSetup.bAnaStep_Reco) { //Case: Hit Analysis
            //Hit Selection
            hitSelector.setHits(vec_strInputFiles[i], detMPGD, aSetup);

            if (bVerboseMode) { //Print Number of Selected Hits to User
                cout<<vec_strInputFiles[i] << " has " << detMPGD.getHits().size() << " hits passing selection" << endl;
            } //End Print Number of Selected Hits to User
            
            //Load the required input parameters
            if (i == 0) { hitAnalyzer.setAnalysisParameters(aSetup); } //Fixed for all runs
            hitAnalyzer.setDetector(detMPGD);
            
            //Initialize the hit histograms if this is the first run
            if (i == 0) { hitAnalyzer.initHistosHits(); }
            
            //Hit Analysis
            hitAnalyzer.fillHistos();
            
            //Update the Detector!
            detMPGD = hitAnalyzer.getDetector();
        } //End Case: Hit Analysis
        
        //Cluster Reconstruction
        if (rSetup.bAnaStep_Reco) { //Case: Cluster Reconstruction
            
            //Place holder for now
            
        } //End Case: Cluster Reconstruction
        
        //Cluster Analysis
        if ( rSetup.bAnaStep_Clusters ) { //Case: Cluster Analysis
            clustSelector.setClusters(vec_strInputFiles[i], detMPGD, aSetup);
    
            if (bVerboseMode) { //Print Number of Selected Clusters to User
                cout<<vec_strInputFiles[i] << " has " << detMPGD.getClusters().size() << " clusters passing selection" << endl;
            } //End Print Number of Selected Clusters to User
            
            //Load the required input parameters
            if (i == 0) { clustAnalyzer.setAnalysisParameters(aSetup); } //Fixed for all runs
            clustAnalyzer.setDetector(detMPGD);
            
            //Initialize the cluster histograms if this is the first run
            if (i == 0) { clustAnalyzer.initHistosClusters(); }
            
            //Cluster Analysis
            clustAnalyzer.fillHistos();
            
            //Update the Detector!
            detMPGD = clustAnalyzer.getDetector();
        } //End Case: Cluster Analysis
    } //End Loop over vec_strInputFiles
    
    //Store Histograms After Analyzing all input files
    //------------------------------------------------------
    if ( rSetup.bAnaStep_Hits) hitAnalyzer.storeHistos(rSetup.strFile_Output_Name, rSetup.strFile_Output_Option);
    if ( rSetup.bAnaStep_Clusters){
	if ( !rSetup.bAnaStep_Hits){
		clustAnalyzer.storeHistos(rSetup.strFile_Output_Name, rSetup.strFile_Output_Option );
	}
	else{
		clustAnalyzer.storeHistos(rSetup.strFile_Output_Name, "UPDATE" );
	}
    }
    
    //Fit Histograms After Analyzing all input files
    //------------------------------------------------------
    if ( rSetup.bAnaStep_Fitting){ //Case: Fitting Stored Distributions
        if ( rSetup.bAnaStep_Clusters){ //Case: Cluster Analysis
            clustAnalyzer.setDetector(detMPGD); //Update the detector just in case
            clustAnalyzer.fitHistos();
            clustAnalyzer.storeFits(rSetup.strFile_Output_Name, "UPDATE");

	    //Update the Detector!
            detMPGD = clustAnalyzer.getDetector();
        } //End Case: Cluster Analysis
    } //End Case: Fitting Stored Distributions
    
    //Visualize Results
    //------------------------------------------------------
    if ( rSetup.bAnaStep_Visualize ) { //Case: Visualize Output
        visualizeUni.setAnalysisParameters(aSetup);
        visualizeUni.setDetector(detMPGD);
        
        if (rSetup.bAnaStep_Hits) { //Case: Hit Analysis
            visualizeUni.storeCanvasHistoSegmented(rSetup.strFile_Output_Name, "UPDATE", "HitADC", "E1", false);
            visualizeUni.storeCanvasHistoSegmented(rSetup.strFile_Output_Name, "UPDATE", "HitPos", "E1", rSetup.bVisPlots_PhiLines);
            visualizeUni.storeCanvasHistoSegmented(rSetup.strFile_Output_Name, "UPDATE", "HitTime", "E1", false);
        } //End Case: Hit Analysis
        
        if (rSetup.bAnaStep_Clusters) { //Case: Cluster Analysis
            visualizeUni.storeCanvasHistoSegmented(rSetup.strFile_Output_Name, "UPDATE", "ClustADC", "E1", false);
            visualizeUni.storeCanvasHistoSegmented(rSetup.strFile_Output_Name, "UPDATE", "ClustPos", "E1", rSetup.bVisPlots_PhiLines);
            visualizeUni.storeCanvasHistoSegmented(rSetup.strFile_Output_Name, "UPDATE", "ClustSize", "E1", false);
            visualizeUni.storeCanvasHistoSegmented(rSetup.strFile_Output_Name, "UPDATE", "ClustTime", "E1", false);

            if (rSetup.bAnaStep_Fitting) { //Case: Fitting
                visualizeUni.storeCanvasGraph(rSetup.strFile_Output_Name, "UPDATE", "ResponseFitChi2", "APE1", rSetup.bVisPlots_PhiLines);
                visualizeUni.storeCanvasGraph(rSetup.strFile_Output_Name, "UPDATE", "ResponseFitPkPos", "APE1", rSetup.bVisPlots_PhiLines);
            } //End Case: Fitting
        } //End Case: Cluster Analysis
    } //End Case: Visualize Output
    
    //Debugging
    //myAnalyzerCluster.loadHistosFromFile(vec_strInputArgs[1], vec_strInputArgs[4]);
    //myAnalyzerCluster.fitHistos();
    
    cout<<"Success!"<<endl;
    
    return 0;
} //End main()
