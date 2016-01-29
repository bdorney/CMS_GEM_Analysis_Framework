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
#include "ClusterSelector.h"
#include "DetectorMPGD.h"
#include "ParameterLoaderAmoreSRS.h"
#include "ParameterLoaderAnaysis.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes

//Namespaces
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
int main( int argc_, char * argv_[]){
    //Variable Declaration
    ParameterLoaderAmoreSRS amoreLoader;
    ParameterLoaderAnaysis analysisLoader;
    
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
        
        SectorEta etaSector = myDet.getEtaSector(i);
        
        cout<<etaSector.fPos_Y<<"\t"<<etaSector.fWidth<<"\t";
        
        for (auto iterPhi = etaSector.map_sectorsPhi.begin(); iterPhi != etaSector.map_sectorsPhi.end(); ++iterPhi) {
            cout<<(*iterPhi).second.fPos_Xlow<<"\t"<<(*iterPhi).second.fPos_Xhigh<<"\t";
        }
        
        cout<<endl;
    }
    
    Uniformity::AnalysisSetupUniformity aSetup = analysisLoader.getAnalysisParameters( vec_strInputArgs[2] );
    
    cout<<"ADC Noise, Min = " << aSetup.selClust.iCut_ADCNoise << endl;
    cout<<"Clust Size, Min = " << aSetup.selClust.iCut_SizeMin << endl;
    cout<<"Clust Size, Max = " << aSetup.selClust.iCut_SizeMax << endl;
    cout<<"Clust Time, Min = " << aSetup.selClust.iCut_TimeMin << endl;
    cout<<"Clust Time, Max = " << aSetup.selClust.iCut_TimeMax << endl;
    
    ClusterSelector mySelection;
    
    mySelection.setClusters(vec_strInputArgs[3], myDet, aSetup);
    
    cout<<"Success!"<<endl;
    
    return 0;
}