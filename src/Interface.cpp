//
//  Interface.cpp
//  
//
//  Created by Brian L Dorney on 05/07/16.
//
//

#include "Interface.h"

using namespace QualityControl::Uniformity;

//Sets the analysis setup in all classes
void Interface::initialize(AnalysisSetupUniformity inputAnaSetup, RunSetup inputRunSetup){
    
    aSetup = inputAnaSetup;
    rSetup = inputRunSetup;
    
    //Initialize Hit Related Items
    if (rSetup.bAnaStep_Hits) {
        hitSelector.setAnalysisParameters(aSetup);
        
        hitAnalyzer.setAnalysisParameters(aSetup);
    } //End Case: Hits Desired
    
    //Initialize Cluster Related Items
    if (rSetup.bAnaStep_Clusters) {
        clustSelector.setAnalysisParameters(aSetup);
        
        clustAnalyzer.setAnalysisParameters(aSetup);
    }
    
    return;
} //End InterfaceAnalysis::initialize()

//Sets the analysis setup in all classes
//Initializes all histograms
void Interface::initialize(AnalysisSetupUniformity inputAnaSetup, RunSetup inputRunSetup, DetectorMPGD & inputDet){
    
    aSetup = inputAnaSetup;
    rSetup = inputRunSetup;
    detMPGD= inputDet;
    
    //Initialize Hit Related Items
    if (rSetup.bAnaStep_Hits) {
        hitSelector.setAnalysisParameters(aSetup);
        
        hitAnalyzer.setAnalysisParameters(aSetup);
        //hitAnalyzer.setDetector(detMPGD);
        hitAnalyzer.initHistosHits(detMPGD);
        
        //detMPGD = hitAnalyzer.getDetector();
    } //End Case: Hits Desired
    
    //Initialize Cluster Related Items
    if (rSetup.bAnaStep_Clusters) {
        clustSelector.setAnalysisParameters(aSetup);
        
        clustAnalyzer.setAnalysisParameters(aSetup);
        //clustAnalyzer.setDetector(detMPGD);
        clustAnalyzer.initGraphsClusters(detMPGD);
        clustAnalyzer.initHistosClusters(detMPGD);
        
        //detMPGD = clustAnalyzer.getDetector();
    }
    
    return;
} //End InterfaceAnalysis::initialize()