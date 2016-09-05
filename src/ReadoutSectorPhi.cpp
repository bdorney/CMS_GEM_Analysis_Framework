//
//  ReadoutSectorPhi.cpp
//  
//
//  Created by Brian L Dorney on 02/09/16.
//
//

#include "ReadoutSectorPhi.h"

using namespace QualityControl::Uniformity;

//Default constructor
ReadoutSectorPhi::ReadoutSectorPhi(){
    fPos_Xlow = fPos_Xhigh = fWidth = -1;
} //End default constructor

//Copy constructor
ReadoutSectorPhi::ReadoutSectorPhi(const ReadoutSectorPhi & other){
    fPos_Xlow   = other.fPos_Xlow;
    fPos_Xhigh  = other.fPos_Xhigh;
    
    fWidth = other.fWidth;
    
    iStripNum_Min = other.iStripNum_Min;
    iStripNum_Max = other.iStripNum_Max;
    
    map_slices = other.map_slices;
    
    //vec_hits    = other.vec_hits;
    map_hits    = other.map_hits;
    //vec_clusters= other.vec_clusters;
    map_clusters= other.map_clusters;
    
    clustHistos = other.clustHistos;
    hitHistos   = other.hitHistos;
} //End copy constructor
