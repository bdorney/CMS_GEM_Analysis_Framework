//
//  GainUniformityTypes.h
//  
//
//  Created by Brian L Dorney on 20/01/16.
//
//

#ifndef _UniformityUtilityTypes_h
#define _UniformityUtilityTypes_h

//C++ Includes
#include <map>
#include <stdio.h>
#include <vector>

//My Includes

//ROOT Includes
#include "TROOT.h"

using namespace ROOT;

namespace Uniformity {
    
    //Strip Cluster
    struct Cluster{
        float fPos_Y;  //distance from base of trapezoid (in mm); e.g. planeID from AMORE
        float fPos_X;  //position within eta sector (in mm); e.g. clustPos from AMORE
        
        int iADC;       //ADC value of cluster; e.g. clustADCs from AMORE
        int iSize;      //Number of strips in cluster; e.g. clustSize from AMORE
        int iTimeBin;   //Time bin of cluster (not sure what that physically means...); e.g. clustTimeBin from AMORE
        
        Cluster(){
            fPos_Y = fPos_X = -1;
            
            iADC = iSize = iTimeBin = -1;
        } //End initialization
    }; //End Cluster
    
    //Defines the phi sector within the detector
    struct SectorPhi{
        float fPos_Xlow;    //Lower Bound X Position
        float fPos_Xhigh;   //Upper Bound X Position
        
        std::vector<Cluster> vec_clusters;
        
        SectorPhi(){
            fPos_Xlow = fPos_Xhigh = -1;
        }
    };
    
    //Defines the pseudorapidity sector within the detector (note this is considered 3 phi sectors)
    struct SectorEta{
        float fPos_Y;       //Roughly Central Y Position of sector
        float fWidth;       //Width of detector at this iEta value
        
        std::map<int, SectorPhi> map_sectorsPhi;
        
        SectorEta(){
            fPos_Y = fWidth = -1;
        }
    }; //End SectorEta
    
} //End namespace uniformity

#endif
