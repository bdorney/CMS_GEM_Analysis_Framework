//
//  ReadoutSectorEta.h
//  
//
//  Created by Brian L Dorney on 02/09/16.
//
//

#ifndef ____ReadoutSectorEta__
#define ____ReadoutSectorEta__

//C++ Includes
#include <stdio.h>
#include <utility>

//Framework Includes
#include "ReadoutSector.h"
#include "ReadoutSectorPhi.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes
#include "TGraphErrors.h"
#include "TROOT.h"

namespace QualityControl{
    namespace Uniformity {
        class ReadoutSectorEta : public ReadoutSector {
            
        public:
            //Constructions
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            ReadoutSectorEta();
            
            //Copy
            ReadoutSectorEta(const ReadoutSectorEta & other);
            
            //Data Members
            //------------------------------------------------------------------------------------------------------------------------------------------
            float fPos_Y;       //Roughly Central Y Position of sector
            
            std::map<int, Uniformity::ReadoutSectorPhi> map_sectorsPhi;
            
            //One dimensional graphs
            std::shared_ptr<TGraphErrors> gEta_ClustADC_Fit_NormChi2;
            std::shared_ptr<TGraphErrors> gEta_ClustADC_Fit_PkPos;
            std::shared_ptr<TGraphErrors> gEta_ClustADC_Fit_PkRes;      //Peak Width / Peak Pos
            std::shared_ptr<TGraphErrors> gEta_ClustADC_Fit_Failures;
            
            std::shared_ptr<TGraphErrors> gEta_ClustADC_Spec_NumPks;
            std::shared_ptr<TGraphErrors> gEta_ClustADC_Spec_PkPos;
            
            //Operators
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Assignment operator
            ReadoutSectorEta & operator=(const ReadoutSectorEta & other){
                if (this != &other ){ //Protects against invalid self-assignment
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
                } //Protects against invalid self-assignment
                
                return *this;
            } //End Assignment Operator
        }; //End class ReadoutSector
    } //End namespace Uniformity
} //End namespace QualityControl
#endif /* defined(____ReadoutSectorEta__) */
