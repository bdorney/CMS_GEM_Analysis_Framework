//
//  ParameterLoaderComparison.cpp
//  
//
//  Created by Brian L Dorney on 11/09/16.
//
//

#include "ParameterLoaderComparison.h"

using namespace QualityControl::Uniformity;

//Default Constructor
ParameterLoaderComparison::ParameterLoaderComparison(){
    m_strSecBegin_CompInfo  = "[BEGIN_COMP_INFO]";
    m_strSecBegin_RunList   = "[BEGIN_RUN_LIST]";
    
    m_strSecEnd_CompInfo    = "[END_COMP_INFO]";
    m_strSecEnd_RunList     = "[END_RUN_LIST]";
} //End default constructor