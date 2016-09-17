//
//  ReadoutSector.h
//  
//
//  Created by Brian L Dorney on 02/09/16.
//
//

#ifndef ____ReadoutSector__
#define ____ReadoutSector__

//C++ Includes
#include <stdio.h>

//Framework Includes
#include "UniformityUtilityTypes.h"

//ROOT Includes

namespace QualityControl{
    namespace Uniformity {
        class ReadoutSector {
            
        public:
            //Constructions
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            ReadoutSector();
            
            //Copy
            ReadoutSector(const ReadoutSector & other);
            
            //Data Members
            //------------------------------------------------------------------------------------------------------------------------------------------
            float fWidth;
            
            Uniformity::HistosPhysObj clustHistos;
            Uniformity::HistosPhysObj hitHistos;
            
            //Operators
            //------------------------------------------------------------------------------------------------------------------------------------------
        }; //End class ReadoutSector
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____ReadoutSector__) */
