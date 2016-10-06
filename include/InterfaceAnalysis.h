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
#include <utility>

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
            virtual void analyzeInput(int iNum_Run, std::string strInputRun){
                vec_pairedRunList.clear();
                vec_pairedRunList.push_back(std::make_pair(iNum_Run, strInputRun ) );
                analyzeInput();
                return;
            };
            
            virtual void analyzeInput(std::vector<std::pair<int, std::string> > vec_inputPairedRunList){
                vec_pairedRunList = vec_inputPairedRunList;
                analyzeInput();
                return;
            };
            
            //As above, but resets vec_strRunList to be a new list of files
            //Note no run numbers are assigned here
            virtual void analyzeInput(std::vector<std::string> vec_strInputRuns){
                vec_pairedRunList.clear();
                for (int i=0; i<vec_strInputRuns.size(); ++i) {
                    vec_pairedRunList.push_back(std::make_pair(-1, vec_strInputRuns[i] ) );
                }
                analyzeInput();
                return;
            };
            
            //Sets the analysis setup in all classes
            //Initializes all histograms
            //virtual void initialize(Uniformity::AnalysisSetupUniformity inputAnaSetup, Uniformity::RunSetup inputRunSetup, Uniformity::DetectorMPGD & inputDet);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
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
            virtual void storeResults(TFile * file_Results, std::string strFileName);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Attributes
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            std::vector<std::pair<int, std::string> > vec_pairedRunList; //vec_pairedRunList[i].first -> Run number; vec_pairedRunList[i].second -> filename
            //std::vector<std::string> vec_strRunList;
        }; //End InterfaceAnalysis
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____InterfaceAnalysis__) */
