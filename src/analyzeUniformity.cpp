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
//#include "DetectorMPGD.h"   //Needs to be included before AnalyzeResponseUniformity.h and ParameterLoaderAmoreSRS.h
//#include "AnalyzeResponseUniformityClusters.h"
//#include "AnalyzeResponseUniformityHits.h"
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
using std::string;
using std::vector;

//using Timing::convert2bool;
//using Timing::getlineNoSpaces;
//using Timing::getParsedLine;

using namespace Uniformity;

void runAnalysis(bool bVerboseMode, RunSetup & rSetup, AnalysisSetupUniformity & aSetup, DetectorMPGD & detMPGD){
    
    
    return;
} //End runAnalysis

//Runs the analysis framework on a TFile created by amoreSRS
void runOnOutputAmoreSRS(bool bVerboseMode, RunSetup & rSetup, AnalysisSetupUniformity & aSetup){
    //Variable Declaration
    AnalyzeResponseUniformityClusters clustAnalyzer;
    AnalyzeResponseUniformityHits hitAnalyzer;
    
    map<string,string> map_clust_ObsAndDrawOpt; //Cluster observables & draw option
    map<string,string> map_hit_ObsAndDrawOpt;   //as above but for hits
    map<string,string> map_res_ObsAndDrawOpt;   //as above but for results (e.g. fits)
    
    ParameterLoaderAmoreSRS amoreLoader;
    
    SectorEta etaSector;
    SelectorCluster clustSelector;
    SelectorHit hitSelector;
    
    TFile *file_ROOTInput, file_ROOTOutput;
    
    VisualizeUniformity visualizeUni;
    
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
        
        //Open this run's root file & check to see if data file opened successfully
        //------------------------------------------------------
        file_ROOTInput = new TFile(vec_strInputFiles[i].c_str(),"READ","",1);
        
        if ( !file_ROOTInput->IsOpen() || file_ROOTInput->IsZombie() ) { //Case: failed to load ROOT file
            perror( ("analyzeUniformity.cpp::runOnOutputAmoreSRS() - error while opening file: " + vec_strInputFiles[i] ).c_str() );
            Timing::printROOTFileStatus(file_ROOTInput);
            std::cout << "Skipping!!!\n";
            
            continue;
        } //End Case: failed to load ROOT file
        
        //Hit Analysis
        //------------------------------------------------------
        //Force the hit analysis if the user requested cluster reconstruction
        if ( rSetup.bAnaStep_Hits || rSetup.bAnaStep_Reco) { //Case: Hit Analysis
            //Hit Selection
            hitSelector.setHits(file_ROOTInput, detMPGD, aSetup);
            
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
        //------------------------------------------------------
        if (rSetup.bAnaStep_Reco) { //Case: Cluster Reconstruction
            
            //Place holder for now
            
        } //End Case: Cluster Reconstruction
        
        //Cluster Analysis
        //------------------------------------------------------
        if ( rSetup.bAnaStep_Clusters ) { //Case: Cluster Analysis
            clustSelector.setClusters(file_ROOTInput, detMPGD, aSetup);
            
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
        
        //Close the file & delete pointer before the next iter
        //------------------------------------------------------
        file_ROOTInput->Close();
        delete file_ROOTInput;
    } //End Loop over vec_strInputFiles
    
    //Create the output ROOT file & check to see if it opened successfully
    //------------------------------------------------------
    file_ROOTOutput = new TFile(rSetup.strFile_Output_Name.c_str(), rSetup.strFile_Output_Option.c_str(),"",1);
    
    if ( !file_ROOTOutput->IsOpen() || file_ROOTOutput->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("analyzeUniformity.cpp::runOnOutputAmoreSRS() - error while opening file: " + vec_strInputFiles[i] ).c_str() );
        Timing::printROOTFileStatus(file_ROOTOutput);
        std::cout << "Skipping!!!\n";
        
        continue;
    } //End Case: failed to load ROOT file
    
    //Store Histograms After Analyzing all input files
    //------------------------------------------------------
    if ( rSetup.bAnaStep_Hits) hitAnalyzer.storeHistos(file_ROOTOutput);
    if ( rSetup.bAnaStep_Clusters) clustAnalyzer.storeHistos(file_ROOTOutput);
    /*if ( rSetup.bAnaStep_Clusters){
        if ( !rSetup.bAnaStep_Hits){
            clustAnalyzer.storeHistos(rSetup.strFile_Output_Name, rSetup.strFile_Output_Option );
        }
        else{
            clustAnalyzer.storeHistos(rSetup.strFile_Output_Name, "UPDATE" );
        }
    }*/
    
    //Fit Histograms After Analyzing all input files
    //------------------------------------------------------
    if ( rSetup.bAnaStep_Fitting){ //Case: Fitting Stored Distributions
        if ( rSetup.bAnaStep_Clusters){ //Case: Cluster Analysis
            clustAnalyzer.setDetector(detMPGD); //Update the detector just in case
            clustAnalyzer.fitHistos();
            clustAnalyzer.storeFits(file_ROOTOutput);
            
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
            map_hit_ObsAndDrawOpt["HitADC"]="E1";
            //map_hit_ObsAndDrawOpt["HitPos"]="E1";
            map_hit_ObsAndDrawOpt["HitTime"]="E1";
            
            visualizeUni.storeCanvasHistoSegmented(file_ROOTOutput, "HitPos", "E1", rSetup.bVisPlots_PhiLines);
            visualizeUni.storeListOfCanvasesHistoSegmented(file_ROOTOutput, map_hit_ObsAndDrawOpt, false);
        } //End Case: Hit Analysis
        
        if (rSetup.bAnaStep_Clusters) { //Case: Cluster Analysis
            map_clust_ObsAndDrawOpt["ClustADC"]="E1";
            //map_clust_ObsAndDrawOpt["ClustPos"]="E1";
            map_clust_ObsAndDrawOpt["ClustSize"]="E1";
            map_clust_ObsAndDrawOpt["ClustTime"]="E1";
            
            visualizeUni.storeCanvasHistoSegmented(file_ROOTOutput, "ClustPos", "E1", rSetup.bVisPlots_PhiLines);
            visualizeUni.storeListOfCanvasesHistoSegmented(file_ROOTOutput, map_clust_ObsAndDrawOpt, false);
            
            if (rSetup.bAnaStep_Fitting) { //Case: Fitting
                map_res_ObsAndDrawOpt["ResponseFitChi2"]="APE1";
                map_res_ObsAndDrawOpt["ResponseFitPkPos"]="APE1";
                
                visualizeUni.storeListOfCanvasesGraph(file_ROOTOutput,map_res_ObsAndDrawOpt, rSetup.bVisPlots_PhiLines);
            } //End Case: Fitting
        } //End Case: Cluster Analysis
    } //End Case: Visualize Output
    
    return;
} //End runOnOutputAmoreSRS()

//Runs the analysis framework on a TFile created by the CMS_GEM_Analysis_Framework
void runOnOutputFrmwrk(){
    
}

//Input Parameters
//  0 -> Executable
//  1 -> Run config file
//  2 -> verbose mode
//Usage examples:
//  Help menu: ./analyzeUniformity -h
//  Analysis: ./analyzeUniformity config/configRun.cfg true
int main( int argc_, char * argv_[] ){
    //Variable Declaration
    bool bVerboseMode = false;
    
    ifstream file_Config;
    
    ParameterLoaderAnaysis analysisLoader;
    
    RunSetup rSetup;
    
    vector<string> vec_strInputArgs;
    vector<string> vec_strInputFiles;
    
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
    
    //Debugging
    //myAnalyzerCluster.loadHistosFromFile(vec_strInputArgs[1], vec_strInputArgs[4]);
    //myAnalyzerCluster.fitHistos();
    
    //Set the Run Info and get input file(s)
    //------------------------------------------------------
    setRunSetup(file_Config, bVerboseMode, rSetup);
    vec_strInputFiles = getRunList(file_Config, bVerboseMode);
    
    //Load the requested analysis parameters
    //------------------------------------------------------
    AnalysisSetupUniformity aSetup = analysisLoader.getAnalysisParameters( rSetup.strFile_Config_Ana );

    
    cout<<"Success!"<<endl;
    
    return 0;
} //End main()
