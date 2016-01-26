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

//My Includes
#include "DetectorMPGD.h"
#include "ParameterLoaderAmoreSRS.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes

//Namespaces
using std::cout;
using std::endl;
using std::string;
using std::vector;

using namespace Uniformity;

int main( int argc_, char * argv_[]){
    //Variable Declaration
    ParameterLoaderAmoreSRS amoreLoader;
    
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
    
    cout<<"Success!"<<endl;
    
    return 0;
}