//
//  ReadoutSector.cpp
//  
//
//  Created by Brian L Dorney on 02/09/16.
//
//

#include "ReadoutSector.h"

using namespace QualityControl::Uniformity;

//Default constructor
ReadoutSector::ReadoutSector(){
    fWidth = -1;
} //End default constructor

//Copy constructor
ReadoutSector::ReadoutSector(const ReadoutSector & other){
    fWidth    = other.fWidth;
    
    clustHistos = other.clustHistos;
    hitHistos   = other.hitHistos;
} //End copy constructor
