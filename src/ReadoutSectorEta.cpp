//
//  ReadoutSectorEta.cpp
//  
//
//  Created by Brian L Dorney on 02/09/16.
//
//

#include "ReadoutSectorEta.h"

using namespace QualityControl::Uniformity;

//Default constructor
ReadoutSectorEta::ReadoutSectorEta(){
    fPos_Y = fWidth = -1;
} //End Default Constructor

//Copy constructor
ReadoutSectorEta::ReadoutSectorEta(const ReadoutSectorEta& other){
    fPos_Y = other.fPos_Y;
    fWidth = other.fWidth;
    
    map_sectorsPhi = other.map_sectorsPhi;
    
    //histograms
    clustHistos = other.clustHistos;
    hitHistos = other.hitHistos;
    
    //Deep Copy
    if( other.gEta_ClustADC_Fit_NormChi2 != NULL )  gEta_ClustADC_Fit_NormChi2  = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Fit_NormChi2.get() );
    if( other.gEta_ClustADC_Fit_PkPos != NULL )     gEta_ClustADC_Fit_PkPos     = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Fit_PkPos.get() );
    if( other.gEta_ClustADC_Fit_PkRes != NULL )     gEta_ClustADC_Fit_PkRes     = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Fit_PkRes.get() );
    if( other.gEta_ClustADC_Fit_Failures != NULL )  gEta_ClustADC_Fit_Failures  = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Fit_Failures.get() );
    
    if( other.gEta_ClustADC_Spec_NumPks != NULL )   gEta_ClustADC_Spec_NumPks   = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Spec_NumPks.get() );
    if( other.gEta_ClustADC_Spec_PkPos != NULL )    gEta_ClustADC_Spec_PkPos    = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Spec_PkPos.get() );
} //End copy constructor
