//
//  InterfaceRun.h
//  
//
//  Created by Brian L Dorney on 04/07/16.
//
//

#ifndef ____InterfaceRun__
#define ____InterfaceRun__

//C++ Includes
#include <memory>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

//Framework Includes
//#include "DetectorMPGD.h"
#include "Interface.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"

namespace QualityControl {
    namespace Uniformity {
        class InterfaceRun : public Interface {
            friend class InterfaceAnalysis;
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            InterfaceRun();
            
            //Set the detector and anlysis parameters at construction
            //InterfaceRun(Uniformity::AnalysisSetupUniformity inputSetup, Uniformity::DetectorMPGD & inputDet);
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
        private:
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            virtual void analyzeInputAmoreSRS(std::string strInputRun);
            
            //Analyze Run - Clusters Only
            void analyzeAmoreSRSRunClustersOnly(std::string strInputRun);
            
            ////Analyze Run - Hits Only
            void analyzeAmoreSRSRunHitsOnly(std::string strInputRun);
            
            //Analyze Run - Hits & Clusters, Re-reco Clusters
            void analyzeAmoreSRSRunFull(std::string strInputRun);
            
            //Analyze Run - Hits & Clusters
            void analyzeAmoreSRSRunFullNoReReco(std::string strInputRun);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //virtual Uniformity::Event getEvent(TTree * inputTree, int iEvtNum, bool bFrmwrkTree);
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //virtual void setEvent(Uniformity::Event &inputEvt, TTree * inputTree, int iEvtNum);
            
            //Attributes
            //------------------------------------------------------------------------------------------------------------------------------------------
            
        }; //End class InterfaceRun
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____InterfaceRun__) */