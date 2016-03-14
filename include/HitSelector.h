//
//  HitSelector.h
//  
//
//  Created by Brian L Dorney on 11/03/16.
//
//

#ifndef ____HitSelector__
#define ____HitSelector__

//C++ Includes
#include <stdio.h>

//Framework Includes
#include "Selector.h"

//ROOT Includes

namespace Uniformity {
    class HitSelector : public Selector {
        //Constructors
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Default
        HitSelector();
        
        //Actions - Methods that Do Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Getters - Methods that Get (i.e. Return) Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Printers - Methods that Print Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Setters - Methods that Set Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Given an output ROOT file from amoreSRS with hits
        //Applies the hit selection and stores those selected hits in inputDet
        virtual void setHits(std::string &strInputRootFileName, Uniformity::DetectorMPGD &inputDet);
        
        //As above but overwrites the stored AnalysisSetupUniformity object
        virtual void setHits(std::string &strInputRootFileName, Uniformity::DetectorMPGD &inputDet, Uniformity::AnalysisSetupUniformity inputSetup){
            setAnalysisParameters(inputSetup);
            setHits(strInputRootFileName, inputDet);
            return;
        };
        
    private:
        //Actions - Methods that Do Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Check if Hit Passes selection stored in aSetupUniformity? True -> Passes; False -> Fails
        bool hitPassesSelection(Uniformity::Hit &inputHit);
        
        //Getters - Methods that Get (i.e. Return) Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Printers - Methods that Print Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Setters - Methods that Set Something
        //------------------------------------------------------------------------------------------------------------------------------------------
    };
} //End namespace Uniformity


#endif /* defined(____HitSelector__) */
