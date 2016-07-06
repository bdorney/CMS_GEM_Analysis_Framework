//
//  InterfaceAnalysis.cpp
//  
//
//  Created by Brian L Dorney on 03/05/16.
//
//

#include "InterfaceAnalysis.h"

using std::cout;
using std::endl;
using std::map;
using std::string;

using namespace QualityControl::Uniformity;

//Default Constructor
InterfaceAnalysis::InterfaceAnalysis(){
    //bInitialized = false;
    bVerboseMode = false;
} //End Default Constructor

//Performs the analysis on the detMPGD object defined by rSetup and aSetup on the input files stored in vec_strRunList
void InterfaceAnalysis::analyzeInput(){
    //Variable Declaration
    
    //Evaluate the two possible usage cases
    //  Case 1: input created by amoreSRS (rSetup.bInputFromFrmwrk == false)
    //  Case 2: input created by framework (rSetup.bInputFromFrmwrk == true)
    if (rSetup.bInputFromFrmwrk) { //Case: Input created by CMS_GEM_Analysis_Framework
        analyzeInputFrmwrk();
    } //End Case: Input created by CMS_GEM_Analysis_Framework
    else{ //Case: Input created by amoreSRS
        analyzeInputAmoreSRS();
    } //Case: Input created by amoreSRS
    
    return;
} //End InterfaceAnalysis::analyzeInput()

/*void InterfaceAnalysis::initialize(AnalysisSetupUniformity inputAnaSetup, RunSetup inputRunSetup, DetectorMPGD & inputDet){
    
    aSetup = inputAnaSetup;
    rSetup = inputRunSetup;
    detMPGD= inputDet;
    
    //initialize Hit Related Items
    if (rSetup.bAnaStep_Hits) {
        hitSelector.setAnalysisParameters(aSetup);
        
        hitAnalyzer.setAnalysisParameters(aSetup);
        hitAnalyzer.setDetector(detMPGD);
        hitAnalyzer.initHistosHits();
        
        detMPGD = hitAnalyzer.getDetector();
    } //End Case: Hits Desired
    
    //initialize Cluster Related Items
    if (rSetup.bAnaStep_Clusters) {
        clustSelector.setAnalysisParameters(aSetup);
        
        clustAnalyzer.setAnalysisParameters(aSetup);
        clustAnalyzer.setDetector(detMPGD);
        clustAnalyzer.initGraphsClusters();
        clustAnalyzer.initHistosClusters();
        
        detMPGD = clustAnalyzer.getDetector();
    }
    
    runInterface.initialize(inputAnaSetup, inputRunSetup);
    
    return;
}*/ //End InterfaceAnalysis::initialize()

//Runs the analysis framework on input created by amoreSRS
void InterfaceAnalysis::analyzeInputAmoreSRS(){
   //TFile does not automatically own histograms 
   TH1::AddDirectory(kFALSE);

    //Variable Declaration
    //ParameterLoaderAmoreSRS amoreLoader;
    
    SectorEta etaSector;
    
    TFile *file_ROOTInput, *file_ROOTOutput_All, *file_ROOTOutput_Single;
    //TFile *file_ROOTOutput_All, *file_ROOTOutput_Single;
    
	//Debugging
	//cout<<"InterfaceAnalysis::analyzeInputAmoreSRS(): detMPGD.getName() = " << detMPGD.getName() << endl;

    //Print the detector info & analysis parameters to the user
    //------------------------------------------------------
    if (bVerboseMode) { //Case: Verbose Printing
        
        //Detector info
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
        
        //Analysis parameters
        cout<<"-----------------Hit Parameters-----------------\n";
        cout<<"Hit ADC, Min = " << aSetup.selHit.iCut_ADCNoise << endl;
        cout<<"Hit ADC, Max = " << aSetup.selHit.iCut_ADCSat << endl;
        cout<<"Hit Multi, Min = " << aSetup.selHit.iCut_MultiMin << endl;
        cout<<"Hit Multi, Max = " << aSetup.selHit.iCut_MultiMax << endl;
        cout<<"Hit Time, Min = " << aSetup.selHit.iCut_TimeMin << endl;
        cout<<"Hit Time, Max = " << aSetup.selHit.iCut_TimeMax << endl;
        cout<<"-----------------Cluster Parameters-----------------\n";
        cout<<"Clust ADC, Min = " << aSetup.selClust.iCut_ADCNoise << endl;
        cout<<"Clust Size, Min = " << aSetup.selClust.iCut_SizeMin << endl;
        cout<<"Clust Size, Max = " << aSetup.selClust.iCut_SizeMax << endl;
        cout<<"Clust Time, Min = " << aSetup.selClust.iCut_TimeMin << endl;
        cout<<"Clust Time, Max = " << aSetup.selClust.iCut_TimeMax << endl;
    } //End Case: Verbose Printing

	//Debugging
	//cout<<"InterfaceAnalysis::analyzeInputAmoreSRS(): detMPGD.getName() = " << detMPGD.getName() << endl;

    //Loop over input files
    //------------------------------------------------------
    for (int i=0; i < vec_strRunList.size(); ++i) { //Loop over vec_strRunList
        //Wipe physics objects from previous file (prevent double counting)
        detMPGD.resetPhysObj();
        
        //Open this run's root file & check to see if data file opened successfully
        //------------------------------------------------------
        file_ROOTInput = new TFile(vec_strRunList[i].c_str(),"READ","",1);
        
        if ( !file_ROOTInput->IsOpen() || file_ROOTInput->IsZombie() ) { //Case: failed to load ROOT file
            perror( ("InterfaceAnalysis::analyzeInputAmoreSRS() - error while opening file: " + vec_strRunList[i] ).c_str() );
            Timing::printROOTFileStatus(file_ROOTInput);
            std::cout << "Skipping!!!\n";
            
            continue;
        } //End Case: failed to load ROOT file
        
        //Debugging
        //cout<<"InterfaceAnalysis::analyzeInputAmoreSRS(): Pre Hit Ana detMPGD.getName() = " << detMPGD.getName() << endl;

        //Hit Analysis
        //------------------------------------------------------
        //Force the hit analysis if the user requested cluster reconstruction
        if ( rSetup.bAnaStep_Hits || rSetup.bAnaStep_Reco) { //Case: Hit Analysis
            //Hit Selection
            /*hitSelector.setHits(file_ROOTInput, detMPGD, aSetup);
            
            if (bVerboseMode) { //Print Number of Selected Hits to User
                cout<<vec_strRunList[i] << " has " << detMPGD.getHits().size() << " hits passing selection" << endl;
            } //End Print Number of Selected Hits to User
            
            //Load the required input parameters
            if (i == 0) { hitAnalyzer.setAnalysisParameters(aSetup); } //Fixed for all runs
            hitAnalyzer.setDetector(detMPGD);
            
            if (i == 0 || rSetup.bMultiOutput) { hitAnalyzer.initHistosHits(); }
            
            //Hit Analysis
            hitAnalyzer.fillHistos();
            
            //Update the detector
            detMPGD = hitAnalyzer.getDetector();*/
            
            //Hit Selection
            hitSelector.setHits(file_ROOTInput, detMPGD, aSetup);
             
             if (bVerboseMode) { //Print Number of Selected Hits to User
             cout<<vec_strRunList[i] << " has " << detMPGD.getHits().size() << " hits passing selection" << endl;
             } //End Print Number of Selected Hits to User
             
             //Load the required input parameters
             if (i == 0) { hitAnalyzer.setAnalysisParameters(aSetup); } //Fixed for all runs
             //hitAnalyzer.setDetector(detMPGD);
             
             if (i == 0 || rSetup.bMultiOutput) { hitAnalyzer.initHistosHits(detMPGD); }
             
             //Hit Analysis
             hitAnalyzer.fillHistos(detMPGD);
        } //End Case: Hit Analysis
        
        //Cluster Reconstruction
        //------------------------------------------------------
        if (rSetup.bAnaStep_Reco) { //Case: Cluster Reconstruction
            
            //Place holder for now
            
        } //End Case: Cluster Reconstruction
      
        //Cluster Analysis
        //------------------------------------------------------
        if ( rSetup.bAnaStep_Clusters ) { //Case: Cluster Analysis
            //Cluster Selection
            /*clustSelector.setClusters(file_ROOTInput, detMPGD, aSetup);
            
            if (bVerboseMode) { //Print Number of Selected Clusters to User
                cout<<vec_strRunList[i] << " has " << detMPGD.getClusters().size() << " clusters passing selection" << endl;
            } //End Print Number of Selected Clusters to User
            
            //Load the required input parameters
            if (i == 0) { clustAnalyzer.setAnalysisParameters(aSetup); } //Fixed for all runs
            clustAnalyzer.setDetector(detMPGD);
            
            //Initialize the cluster histograms if this is the first run
            if (i == 0 || rSetup.bMultiOutput) {
                clustAnalyzer.initGraphsClusters();
                clustAnalyzer.initHistosClusters();
            }
            
            //Cluster Analysis
            clustAnalyzer.fillHistos();
            
            //Update the Detector!
            detMPGD = clustAnalyzer.getDetector();*/
            
            //Cluster Selection
            clustSelector.setClusters(file_ROOTInput, detMPGD, aSetup);
            
            if (bVerboseMode) { //Print Number of Selected Clusters to User
                cout<<vec_strRunList[i] << " has " << detMPGD.getClusters().size() << " clusters passing selection" << endl;
            } //End Print Number of Selected Clusters to User
            
            //Load the required input parameters
            if (i == 0) { clustAnalyzer.setAnalysisParameters(aSetup); } //Fixed for all runs
            
            //Initialize the cluster histograms if this is the first run
            if (i == 0 || rSetup.bMultiOutput) {
                clustAnalyzer.initGraphsClusters(detMPGD);
                clustAnalyzer.initHistosClusters(detMPGD);
            }
            
            //Cluster Analysis
            clustAnalyzer.fillHistos(detMPGD);
        } //End Case: Cluster Analysis
        
        //User requests multiple output files?
        //------------------------------------------------------
        if (rSetup.bMultiOutput) { //Case: User wants one output file per input file
            //Setup the name of the output file
            string strTempRunName = vec_strRunList[i];
            
            if ( strTempRunName.find("dataTree.root") != string::npos ){
                strTempRunName.erase(strTempRunName.find("dataTree.root"), strTempRunName.length() - strTempRunName.find("dataTree.root") );
                strTempRunName = strTempRunName + "Ana.root";
            } //End Case: Input Tree File
            else if ( strTempRunName.find(".root") != string::npos ) {
                strTempRunName.erase(strTempRunName.find(".root"), strTempRunName.length() - strTempRunName.find(".root") );
                strTempRunName = strTempRunName + "Ana.root";
            } //End Case: Other ROOT file
            
            //Create the file
            file_ROOTOutput_Single = new TFile(strTempRunName.c_str(), rSetup.strFile_Output_Option.c_str(),"",1);
            
            if ( !file_ROOTOutput_Single->IsOpen() || file_ROOTOutput_Single->IsZombie() ) { //Case: failed to load ROOT file
                perror( ("InterfaceAnalysis::analyzeInputAmoreSRS() - error while opening file: " + strTempRunName ).c_str() );
                Timing::printROOTFileStatus(file_ROOTOutput_Single);
                std::cout << "Skipping!!!\n";
                
                //Close the file & delete pointer before the next iter
                file_ROOTInput->Close();
                delete file_ROOTInput;
                
                //Move to next iteration
                continue;
            } //End Case: failed to load ROOT file
            
            //Store the results
            storeResults(file_ROOTOutput_Single);
            
            //Close the file before the next iteration
            file_ROOTOutput_Single->Close();
            delete file_ROOTOutput_Single;
        } //End Case: User wants one output file per input file
        
        //Close the file & delete pointer before the next iter
        //------------------------------------------------------
        file_ROOTInput->Close();
        delete file_ROOTInput;
    } //End Loop over vec_strRunList
    
	//Debugging
	//cout<<"InterfaceAnalysis::analyzeInputAmoreSRS(): Pre Storing detMPGD.getName() = " << detMPGD.getName() << endl;

    //Create the summary TFile (only if multi file output is NOT requested)
    //------------------------------------------------------
    if(!rSetup.bMultiOutput){ //Case: Create Summary TFile
        file_ROOTOutput_All = new TFile(rSetup.strFile_Output_Name.c_str(), rSetup.strFile_Output_Option.c_str(),"",1);
    
    	if ( !file_ROOTOutput_All->IsOpen() || file_ROOTOutput_All->IsZombie() ) { //Case: failed to load ROOT file
        	perror( ("InterfaceAnalysis::analyzeInputAmoreSRS() - error while opening file: " + rSetup.strFile_Output_Name ).c_str() );
        	Timing::printROOTFileStatus(file_ROOTOutput_All);
        	std::cout << "Exiting!!!\n";
        
        	return;
    	} //End Case: failed to load ROOT file
    
    	//Store the results
    	storeResults(file_ROOTOutput_All);
    
        //Close the file
    	file_ROOTOutput_All->Close();
    } //End Case: Create Summary TFile

    return;
} //End InterfaceAnalysis::analyzeInputAmoreSRS()

//Runs the analysis framework on input created by the CMS_GEM_AnalysisFramework
void InterfaceAnalysis::analyzeInputFrmwrk(){
    //TFile does not automatically own histograms 
    TH1::AddDirectory(kFALSE);

    //Variable Declaration
    TFile *file_ROOTInput, *file_ROOTOutput_All, *file_ROOTOutput_Single;
    
    //Loop over input files
    for (int i=0; i < vec_strRunList.size(); ++i) { //Loop over input files
        //Open this run's root file & check to see if data file opened successfully
        //------------------------------------------------------
        file_ROOTInput = new TFile(vec_strRunList[i].c_str(),"READ","",1);
        
        if ( !file_ROOTInput->IsOpen() || file_ROOTInput->IsZombie() ) { //Case: failed to load ROOT file
            perror( ("InterfaceAnalysis::analyzeInputFrmwrk() - error while opening file: " + vec_strRunList[i] ).c_str() );
            Timing::printROOTFileStatus(file_ROOTInput);
            std::cout << "Skipping!!!\n";
            
            continue;
        } //End Case: failed to load ROOT file
        
        //Hit Analysis
        //------------------------------------------------------
        //Force the hit analysis if the user requested cluster reconstruction
        if ( rSetup.bAnaStep_Hits || rSetup.bAnaStep_Reco) { //Case: Hit Analysis
        
            //Placeholder
            
        } //End Case: Hit Analysis
        
        //Cluster Reconstruction
        //------------------------------------------------------
        if (rSetup.bAnaStep_Reco) { //Case: Cluster Reconstruction
            
            //Place holder for now
            
        } //End Case: Cluster Reconstruction
        
        //Cluster Analysis
        //------------------------------------------------------
        if ( rSetup.bAnaStep_Clusters ) { //Case: Cluster Analysis
	    //Load the required input parameters
            /*if (i == 0) { clustAnalyzer.setAnalysisParameters(aSetup); } //Fixed for all runs
            
            //Load previous cluster histograms & setup the detector
            clustAnalyzer.loadHistosFromFile(rSetup.strFile_Config_Map, file_ROOTInput);

            //Initialize Graphs
            clustAnalyzer.initGraphsClusters();
            
            detMPGD = clustAnalyzer.getDetector();*/
            
            if (i == 0) { clustAnalyzer.setAnalysisParameters(aSetup); } //Fixed for all runs
            
            //Load previous cluster histograms & setup the detector
            clustAnalyzer.loadHistosFromFile(rSetup.strFile_Config_Map, file_ROOTInput);
            detMPGD = clustAnalyzer.getDetector();
            
            //Initialize Graphs
            clustAnalyzer.initGraphsClusters(detMPGD);
        } //End Case: Cluster Analysis
        
        //Store the Output
        //When running over framework input only the one output file per input file mode is supported
        //  e.g. there is no "aggregate file"
        //------------------------------------------------------
        //Setup the name of the output file
        string strTempRunName = vec_strRunList[i];
        
        if ( strTempRunName.find("Ana.root") != string::npos ) {
            strTempRunName.erase(strTempRunName.find("Ana.root"), strTempRunName.length() - strTempRunName.find("Ana.root") );
            strTempRunName = strTempRunName + "NewAna.root";
        } //End Case: Other ROOT file
        
        //Create the file
        file_ROOTOutput_Single = new TFile(strTempRunName.c_str(), rSetup.strFile_Output_Option.c_str(),"",1);
        
        if ( !file_ROOTOutput_Single->IsOpen() || file_ROOTOutput_Single->IsZombie() ) { //Case: failed to load ROOT file
            perror( ("InterfaceAnalysis::analyzeInputFrmwrk() - error while opening file: " + strTempRunName ).c_str() );
            Timing::printROOTFileStatus(file_ROOTOutput_Single);
            std::cout << "Skipping!!!\n";
            
            //Close the file & delete pointer before the next iter
            file_ROOTInput->Close();
            delete file_ROOTInput;
            
            //Move to next iteration
            continue;
        } //End Case: failed to load ROOT file
        
        //Store the results
        //cout<<"InterfaceAnalysis::analyzeInputFrmwrk(): file_ROOTOutput_Single = " << file_ROOTOutput_Single << endl;
        storeResults(file_ROOTOutput_Single);
        
        //Close the files & delete pointers before the next iter
        //------------------------------------------------------
        file_ROOTInput->Close();
        file_ROOTOutput_Single->Close();
        
        delete file_ROOTInput;
        delete file_ROOTOutput_Single;
    } //End Loop over input files
    
    return;
} //End InterfaceAnalysis::analyzeInputFrmwrk()

void InterfaceAnalysis::storeResults(TFile * file_Results){
    //Variable Declaration
    map<string,string> map_clust_ObsAndDrawOpt; //Cluster observables & draw option
    map<string,string> map_hit_ObsAndDrawOpt;   //as above but for hits
    map<string,string> map_res_ObsAndDrawOpt;   //as above but for results (e.g. fits)
    
	//cout<<"InterfaceAnalysis::storeResults(): file_Results = " << file_Results << endl;
	//cout<<"InterfaceAnalysis::storeResults(): detMPGD.getName() = " << detMPGD.getName() << endl;

    //Store Histograms After Analyzing all input files
    //------------------------------------------------------
    //if ( rSetup.bAnaStep_Hits) hitAnalyzer.storeHistos(file_Results);
    //if ( rSetup.bAnaStep_Clusters) clustAnalyzer.storeHistos(file_Results);
    if ( rSetup.bAnaStep_Hits) hitAnalyzer.storeHistos(file_Results, detMPGD);
    if ( rSetup.bAnaStep_Clusters) clustAnalyzer.storeHistos(file_Results, detMPGD);
    
    //Fit Histograms After Analyzing all input files
    //------------------------------------------------------
    if ( rSetup.bAnaStep_Fitting){ //Case: Fitting Stored Distributions
        if ( rSetup.bAnaStep_Clusters){ //Case: Cluster Analysis
            /*clustAnalyzer.setDetector(detMPGD); //Update the detector just in case
             clustAnalyzer.fitHistos();
             clustAnalyzer.storeFits(file_Results);
             
             //Update the Detector!
             detMPGD = clustAnalyzer.getDetector();*/
            
            clustAnalyzer.fitHistos(detMPGD);
            clustAnalyzer.storeFits(file_Results, detMPGD);
        } //End Case: Cluster Analysis
    } //End Case: Fitting Stored Distributions
    
    //Visualize Results
    //------------------------------------------------------
    if ( rSetup.bAnaStep_Visualize ) { //Case: Visualize Output
        visualizeUni.setAnalysisParameters(aSetup);
        visualizeUni.setAutoSaveCanvas( rSetup.bVisPlots_AutoSaving );
        visualizeUni.setDetector(detMPGD);
        
        if (rSetup.bAnaStep_Hits) { //Case: Hit Analysis
            map_hit_ObsAndDrawOpt["HitADC"]="E1";
            map_hit_ObsAndDrawOpt["HitTime"]="E1";
            
            visualizeUni.storeCanvasHistoSegmented(file_Results, "HitPos", "E1", rSetup.bVisPlots_PhiLines);
            visualizeUni.storeListOfCanvasesHistoSegmented(file_Results, map_hit_ObsAndDrawOpt, false);
            //visualizeUni.storeCanvasHisto2D(file_Results,"HitPos","TRI2");
        } //End Case: Hit Analysis
        
        if (rSetup.bAnaStep_Clusters) { //Case: Cluster Analysis
            map_clust_ObsAndDrawOpt["ClustADC"]="E1";
            map_clust_ObsAndDrawOpt["ClustSize"]="E1";
            map_clust_ObsAndDrawOpt["ClustTime"]="E1";
            
            visualizeUni.storeCanvasHistoSegmented(file_Results, "ClustPos", "E1", rSetup.bVisPlots_PhiLines);
            visualizeUni.storeListOfCanvasesHistoSegmented(file_Results, map_clust_ObsAndDrawOpt, false);
            //visualizeUni.storeCanvasHisto2D(file_Results,"ClustPos","TRI2");

            if (rSetup.bAnaStep_Fitting) { //Case: Fitting
                map_res_ObsAndDrawOpt["ResponseFitChi2"]="APE1";
                map_res_ObsAndDrawOpt["ResponseFitPkPos"]="APE1";
                map_res_ObsAndDrawOpt["ResponseFitPkRes"]="APE1";
                
                visualizeUni.storeCanvasData(file_Results, "ResponseFitPkPos", "E1");
                visualizeUni.storeCanvasData(file_Results, "ResponseFitPkRes", "E1");
                visualizeUni.storeListOfCanvasesGraph(file_Results,map_res_ObsAndDrawOpt, rSetup.bVisPlots_PhiLines);
                visualizeUni.storeCanvasGraph2D(file_Results,"ResponseFitPkPos","TRI2Z",false);
                visualizeUni.storeCanvasGraph2D(file_Results,"ResponseFitPkPos","TRI2Z",true);	//Normalized version
                visualizeUni.storeCanvasGraph2D(file_Results,"ResponseFitPkRes","TRI2Z",false);
                visualizeUni.storeCanvasGraph2D(file_Results,"ResponseFitPkRes","TRI2Z",true);	//Normalized version
            } //End Case: Fitting
        } //End Case: Cluster Analysis
    } //End Case: Visualize Output
    
    return;
} //End InterfaceAnalysis::storeResults()
