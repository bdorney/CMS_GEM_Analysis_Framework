//
//  Selector.cpp
//  
//
//  Created by Brian L Dorney on 14/03/16.
//
//

#include "Selector.h"

using namespace QualityControl::Uniformity;

//Defaul Constructor
Selector::Selector(){
    bVerboseMode = false;
} //End Default Constructor

//With Analysis Setup File Given at time of construction
Selector::Selector(QualityControl::Uniformity::AnalysisSetupUniformity &inputSetup){
    aSetup = inputSetup;
} //End Constructor
