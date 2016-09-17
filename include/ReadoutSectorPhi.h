//
//  ReadoutSectorPhi.h
//  
//
//  Created by Brian L Dorney on 02/09/16.
//
//

#ifndef _ReadoutSectorPhi_h
#define _ReadoutSectorPhi_h

//C++ Includes
#include <map>
#include <stdio.h>

//Framework Includes
#include "ReadoutSector.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes

namespace QualityControl{
    namespace Uniformity {
        class ReadoutSectorPhi : public ReadoutSector {
            
        public:
            //Constructions
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            ReadoutSectorPhi();
            
            //Copy
            ReadoutSectorPhi(const ReadoutSectorPhi & other);
            
            //Data Members
            //------------------------------------------------------------------------------------------------------------------------------------------
            float fNFitSuccess;	//Number of successful fits;

            float fPos_Xlow;    //Lower Bound X Position
            float fPos_Xhigh;   //Upper Bound X Position
            
            int iStripNum_Min;  //Minimum Strip number of sector (inclusive)
            int iStripNum_Max;  //Maximum Strip number of sector (exclusive)
            
            std::map<int, Uniformity::SectorSlice> map_slices;  //Slices of this sector
            
            std::multimap<int, Uniformity::Cluster> map_clusters;   //key value understood as event number
            std::multimap<int, Uniformity::Hit> map_hits;           //key value understood as event number
            
            //Operators
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Assignment operator
            ReadoutSectorPhi & operator=(const ReadoutSectorPhi & other){
                if (this != &other ){ //Protects against invalid self-assignment
                    fNFitSuccess = other.fNFitSuccess;

                    fPos_Xlow  = other.fPos_Xlow;
                    fPos_Xhigh = other.fPos_Xhigh;
                    
                    fWidth = other.fWidth;
                    
                    iStripNum_Min = other.iStripNum_Min;
                    iStripNum_Max = other.iStripNum_Max;
                    
                    map_slices  = other.map_slices;
                    map_hits    = other.map_hits;
                    map_clusters= other.map_clusters;
                    
                    clustHistos = other.clustHistos;
                    hitHistos   = other.hitHistos;
                } //Protects against invalid self-assignment
                
                return *this;
            } //End Assignment Operator
        }; //End ReadoutSectorPhi
    } //End namespace Uniformity
} //End namespace QualityControl

#endif
