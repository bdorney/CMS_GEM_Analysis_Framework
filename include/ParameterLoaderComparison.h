//
//  ParameterLoaderComparison.h
//  
//
//  Created by Brian L Dorney on 11/09/16.
//
//

#ifndef ____ParameterLoaderComparison__
#define ____ParameterLoaderComparison__

//C++ Includes
#include <stdio.h>

//Framework Includes
#include "ParameterLoader.h"

//ROOT Includes

namespace  QualityControl {
    namespace Uniformity {
        
        class ParameterLoaderComparison : public ParameterLoader {
            ParameterLoaderComparison
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            ParameterLoaderComparison();
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Data Memebers
            
        private:
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Attributes
            //------------------------------------------------------------------------------------------------------------------------------------------
            std::string m_strSecBegin_CompInfo, m_strSecEnd_CompInfo;
        };
    } //End namespace Uniformity
} //End namespace QualityControl


#endif /* defined(____ParameterLoaderComparison__) */
