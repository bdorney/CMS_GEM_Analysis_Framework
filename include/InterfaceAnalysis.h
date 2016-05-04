//
//  InterfaceAnalysis.h
//  
//
//  Created by Brian L Dorney on 03/05/16.
//
//

#ifndef ____InterfaceAnalysis__
#define ____InterfaceAnalysis__

//C++ Includes
#include <stdio.h>
#include <vector>

//Framework Includes
#include "SelectorCluster.h"
#include "SelectorHit.h"
#include "DetectorMPGD.h"   //Needs to be included before AnalyzeResponseUniformity.h and ParameterLoaderAmoreSRS.h
#include "AnalyzeResponseUniformityClusters.h"
#include "AnalyzeResponseUniformityHits.h"
//#include "ParameterLoaderAmoreSRS.h"
//#include "ParameterLoaderAnaysis.h"
//#include "ParameterLoaderRun.h"
#include "UniformityUtilityTypes.h"
#include "VisualizeUniformity.h"

//ROOT Includes
#include "TROOT.h"
#include "TFile.h"

using namespace ROOT;

namespace Uniformity {
    class InterfaceAnalysis {
        
    public:
        //Constructors
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Default
        
        //Actions - Methods that Do Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Performs the analysis on the detMPGD object defined by rSetup and aSetup on the input files stored in vec_strRunList
        virtual void analyzeInput();
        
        //As above, but resets vec_strRunList to be a single file
        virtual void analyzeInput(std::string strInputRun){
            vec_strRunList.clear();
            vec_strRunList.push_back(strInputRun);
            analyzeInput();
            return;
        };
        
        //As above, but resets vec_strRunList to be a new list of files
        virtual void analyzeInput(std::vector<std::string> vec_strInputRuns){
            vec_strRunList = vec_strInputRuns;
            analyzeInput();
            return;
        };
        
        //Getters - Methods that Get (i.e. Return) Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Printers - Methods that Print Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Setters - Methods that Set Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Sets the Run Setup
        virtual void setRunParameters(Uniformity::RunSetup inputSetup){ rSetup = inputSetup; return; };
        
        //Sets the Analysis Setup
        virtual void setAnalysisParameters(Uniformity::AnalysisSetupUniformity inputSetup){ aSetup = inputSetup; return; };
        
        //Sets the Detector
        virtual void setDetector(Uniformity::DetectorMPGD & inputDet){ detMPGD = inputDet; return; };
        
        //Data Memebers
        
    private:
        //Actions - Methods that Do Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Runs the analysis framework on a single file
        virtual void analysisRoutine();
        
        //Runs the analysis framework on input created by amoreSRS
        virtual void analyzeInputAmoreSRS();
        
        //Runs the analysis framework on input created by the CMS_GEM_AnalysisFramework
        virtual void analyzeInputFrmwrk();
        
        //Getters - Methods that Get (i.e. Return) Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Printers - Methods that Print Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Setters - Methods that Set Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Data Memebers
        std::vector<std::string> vec_strRunList;
        
        Uniformity::AnalysisSetupUniformity aSetup;
        Uniformity::DetectorMPGD detMPGD;
        Uniformity::RunSetup rSetup;
    }; //End InterfaceAnalysis
} //End namespace Uniformity


#endif /* defined(____InterfaceAnalysis__) */
