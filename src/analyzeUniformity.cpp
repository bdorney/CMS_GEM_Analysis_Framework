//
//  analyzeUniformity.cpp
//  
//
//  Created by Brian L Dorney on 25/01/16.
//
//

//C++ Includes
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string>
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
using std::string;
using std::vector;

using namespace Uniformity;

//Input Parameters
//  0 -> Executable
//  1 -> Amore Mapping File
//  2 -> Analysis Config File
//  3 -> ROOT File to be analyzed
//  4 -> ROOT File to be Generated
//  5 -> Option
int main( int argc_, char * argv_[] ){
    //Variable Declaration
    ParameterLoaderAmoreSRS amoreLoader;
    ParameterLoaderAnaysis analysisLoader;
    
    SectorEta etaSector;
    
    vector<string> vec_strInputArgs;
    
    //Transfer Input Arguments into vec_strInputArgs
    //------------------------------------------------------
    vec_strInputArgs.resize(argc_);
    std::copy(argv_, argv_ + argc_, vec_strInputArgs.begin() );

    //Load amore mapping file
    amoreLoader.loadAmoreMapping( vec_strInputArgs[1] );
    
    DetectorMPGD myDet;
    
    myDet = amoreLoader.getDetector();
    
    cout<<"iEta\tY_Pos\tWidth\tX_Low\tX_High\tX_Low\tX_High\tX_Low\tX_High\n";
    for (int i=1; i <= myDet.getNumEtaSectors(); ++i) {
        cout<<i<<"\t";
        
        etaSector = myDet.getEtaSector(i);
        
        cout<<etaSector.fPos_Y<<"\t"<<etaSector.fWidth<<"\t";
        
        for (auto iterPhi = etaSector.map_sectorsPhi.begin(); iterPhi != etaSector.map_sectorsPhi.end(); ++iterPhi) {
            cout<<(*iterPhi).second.fPos_Xlow<<"\t"<<(*iterPhi).second.fPos_Xhigh<<"\t";
        }
        
        cout<<endl;
    }
    
    Uniformity::AnalysisSetupUniformity aSetup = analysisLoader.getAnalysisParameters( vec_strInputArgs[2] );

	//Hit Analysis

	cout<<"Hit ADC Min = " << aSetup.selHit.iCut_ADCNoise << endl;
	cout<<"Hit ADC Max = " << aSetup.selHit.iCut_ADCSat << endl;
	cout<<"Hit Multi Min = " << aSetup.selHit.iCut_MultiMin << endl;
	cout<<"Hit Multi Max = " << aSetup.selHit.iCut_MultiMax << endl;
    cout<<"Hit Time, Min = " << aSetup.selHit.iCut_TimeMin << endl;
    cout<<"Hit Time, Max = " << aSetup.selHit.iCut_TimeMax << endl;

	SelectorHit mySelectionHits;
	mySelectionHits.setHits(vec_strInputArgs[3], myDet, aSetup);

    cout<<"Number of Selected Hits = " << myDet.getHits().size() << endl;

	AnalyzeResponseUniformityHits myAnalyzerHit(aSetup, myDet);
    myAnalyzerHit.fillHistos();
    myAnalyzerHit.storeHistos(vec_strInputArgs[4], vec_strInputArgs[5]);

	myDet = myAnalyzerHit.getDetector();

	cout<<"Pointer References\n";
	for(int i=1; i <= myDet.getNumEtaSectors(); ++i){
		cout<<i<<"\t"<<( myDet.getEtaSector(i) ).hitHistos.hADC.get()<<endl;
	}
    
    //Visualize Hits
    VisualizeUniformity myVisualizer(aSetup, myDet);
    myVisualizer.storeHistos(vec_strInputArgs[4], "UPDATE", "HITPOS", "E1");
    
	//Cluster Analysis
	
    /*cout<<"Clust ADC, Min = " << aSetup.selClust.iCut_ADCNoise << endl;
    cout<<"Clust Size, Min = " << aSetup.selClust.iCut_SizeMin << endl;
    cout<<"Clust Size, Max = " << aSetup.selClust.iCut_SizeMax << endl;
    cout<<"Clust Time, Min = " << aSetup.selClust.iCut_TimeMin << endl;
    cout<<"Clust Time, Max = " << aSetup.selClust.iCut_TimeMax << endl;
    
    SelectorCluster mySelectionClusters;
    
    mySelectionClusters.setClusters(vec_strInputArgs[3], myDet, aSetup);
    
    cout<<"Number of Selected Clusters = " << myDet.getClusters().size() << endl;
    
    AnalyzeResponseUniformityClusters myAnalyzerCluster(aSetup, myDet);
    
    myAnalyzerCluster.fillHistos();
    myAnalyzerCluster.storeHistos(vec_strInputArgs[4], "UPDATE");
    myAnalyzerCluster.fitHistos();
    myAnalyzerCluster.storeFits(vec_strInputArgs[4], "UPDATE");*/
    //myAnalyzerCluster.checkUniformity();
    

    //Debugging
    //myAnalyzerCluster.loadHistosFromFile(vec_strInputArgs[1], vec_strInputArgs[4]);
    //myAnalyzerCluster.fitHistos();
    
    cout<<"Success!"<<endl;
    
    return 0;
}
