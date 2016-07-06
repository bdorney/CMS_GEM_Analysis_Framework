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
#include <map>
#include <stdio.h>
#include <string>
#include <vector>

//Framework Includes
//#include "SelectorCluster.h"
//#include "SelectorHit.h"
//#include "DetectorMPGD.h"   //Needs to be included before AnalyzeResponseUniformity.h and ParameterLoaderAmoreSRS.h
//#include "AnalyzeResponseUniformityClusters.h"
//#include "AnalyzeResponseUniformityHits.h"
//#include "ParameterLoaderAmoreSRS.h"
//#include "ParameterLoaderAnaysis.h"
//#include "ParameterLoaderRun.h"
#include "Interface.h"
#include "InterfaceRun.h"
#include "UniformityUtilityTypes.h"
//#include "VisualizeUniformity.h"

//ROOT Includes
#include "TROOT.h"
#include "TFile.h"

//using namespace ROOT;

namespace QualityControl {
    namespace Uniformity {
        class InterfaceAnalysis : public Interface {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            InterfaceAnalysis();
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Calls either analyzeInputAmoreSRS() or analyzeInputFrmwrk() based on input rSetup
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
            
            //Sets the analysis setup in all classes
            //Initializes all histograms
            //virtual void initialize(Uniformity::AnalysisSetupUniformity inputAnaSetup, Uniformity::RunSetup inputRunSetup, Uniformity::DetectorMPGD & inputDet);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //virtual Uniformity::DetectorMPGD getDetector(){ return detMPGD; };
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Sets the Analysis Setup
            //virtual void setAnalysisParameters(Uniformity::AnalysisSetupUniformity inputSetup){ aSetup = inputSetup; return; };
            
            //Sets the Detector
            /*virtual void setDetector(Uniformity::DetectorMPGD & inputDet){ detMPGD = inputDet;
                //std::cout<<"InterfaceAnalysis::setDetector() - detMPGD.getName() = " << detMPGD.getName() << std::endl;
                return; };*/
            
            //Sets the Run Setup
            //virtual void setRunParameters(Uniformity::RunSetup inputSetup){ rSetup = inputSetup; return; };
            
            //Sets the Verbose Output Mode
            //virtual void setVerboseMode(bool bInput){ bVerboseMode = bInput; return; };
            
            //Data Memebers
            
        protected:
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Performs the analysis on the detMPGD object defined by rSetup and aSetup on the input files stored in vec_strRunList
            //Runs the analysis framework on input created by amoreSRS
            virtual void analyzeInputAmoreSRS();
            
            //Runs the analysis framework on input created by the CMS_GEM_AnalysisFramework
            virtual void analyzeInputFrmwrk();
            
            //Stores the results of the analysis based on rSetup
            virtual void storeResults(TFile * file_Results);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Data Memebers
            //bool bInitialized;
            //bool bVerboseMode;
            
            std::vector<std::string> vec_strRunList;
            
            //Analyzers
            //AnalyzeResponseUniformityClusters clustAnalyzer;
            //AnalyzeResponseUniformityHits hitAnalyzer;
            
            //Containers
            //AnalysisSetupUniformity aSetup;
            //RunSetup rSetup;
            
            //Detector
            //DetectorMPGD detMPGD;
            
            //Selectors
            //SelectorCluster clustSelector;
            //SelectorHit hitSelector;
            
            //Visualizer
            //VisualizeUniformity visualizeUni;
            
            //Interfaces
            //InterfaceRun runInterface;
            
        }; //End InterfaceAnalysis
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____InterfaceAnalysis__) */
