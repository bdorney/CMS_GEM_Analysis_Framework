//
//  Selector.cpp
//  
//
//  Created by Brian L Dorney on 14/03/16.
//
//

#include "Selector.h"

using namespace Uniformity;

//Defaul Constructor
Selector::Selector(){
    
} //End Default Constructor

//With Analysis Setup File Given at time of construction
Selector::Selector(Uniformity::AnalysisSetupUniformity &inputSetup){
    aSetupUniformity = inputSetup;
} //End Constructor