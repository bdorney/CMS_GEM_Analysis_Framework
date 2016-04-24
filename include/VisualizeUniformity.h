//
//  VisualizeUniformity.h
//  
//
//  Created by Brian L Dorney on 18/02/16.
//
//

#ifndef ____VisualizeUniformity__
#define ____VisualizeUniformity__

//C++ Includes
#include <iterator>
#include <stdio.h>
#include <string>
#include <utility>

//Framework Includes
#include "AnalyzeResponseUniformity.h"
//#include "DetectorMPGD.h" //Done in source file not header file due to inheritance/friendship
#include "TimingUtilityFunctions.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes
#include "TCanvas.h"
//#include "TH1F.h"
#include "TLatex.h"
#include "TLine.h"
#include "TObject.h"
#include "TPad.h"
#include "TROOT.h"

using namespace ROOT;

#endif /* defined(____VisualizeUniformity__) */

namespace Uniformity {
    
    class VisualizeUniformity : public AnalyzeResponseUniformity {
    
    public:
        //Constructors
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Default
        VisualizeUniformity();
        
        VisualizeUniformity(Uniformity::AnalysisSetupUniformity inputSetup, Uniformity::DetectorMPGD inputDet);
        
        //Actions - Methods that Do Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        virtual void storeHistos(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption);
        
        //Getters - Methods that Get (i.e. Return) Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Printers - Methods that Print Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Setters - Methods that Set Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Sets the Analysis Setup
        void setAnalysisParameters(Uniformity::AnalysisSetupUniformity inputSetup){ aSetup = inputSetup; return; };
        
        //Sets the Detector
        void setDetector(Uniformity::DetectorMPGD inputDet){ detMPGD = inputDet; return; };
        
    private:
        //Actions - Methods that Do Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Prepares a Canvas for a requested observable
        virtual std::shared_ptr<TCanvas> drawSectorEtaCanvas(std::string &strObsName, std::string &strDrawOption);
        
        //Draws the distribution pointed to by inputObjPtr on a pad of inputCanvas
        //inputCanvas is split into two columns;
        //The Pad is created when this method is called; iEta and iNumEta define the pad position automatically
        //Odd (even) values of iEta are on the left (right)
        //The SectorEta is used to determine the location of the SectorPhi's
        virtual void drawSectorEtaObs(std::shared_ptr<TObject> inputObjPtr, std::shared_ptr<TCanvas> inputCanvas, std::string &strDrawOption, int iEta, int iNumEta, Uniformity::SectorEta &inputEta);
        
        //Getters - Methods that Get (i.e. Return) Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        std::shared_ptr<TObject> getRootObject(std::string &strObsName, Uniformity::SectorEta &inputEta);
        
        //Printers - Methods that Print Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Setters - Methods that Set Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Data Members
        //------------------------------------------------------------------------------------------------------------------------------------------
        Uniformity::AnalysisSetupUniformity aSetup; //Container to define the analysis setup
        
        Uniformity::DetectorMPGD detMPGD;
    }; //End class VisualizeUniformity
} //End namespace Uniformity
