//
//  SelectorTiming.h
//  
//
//  Created by Brian L Dorney on 18/11/16.
//
//

#ifndef ____SelectorTiming__
#define ____SelectorTiming__

//C++ Includes
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

#ifdef __MAKECINT__
#pragma link C++ class vector<double>+;
#pragma link C++ class map<string, vector<double> >+;
#pragma link C++ class map<string, double>+;
#endif

//Framework Includes
#include "Selector.h"
#include "TestStandVME.h"
#include "TimingEvent.h"
#include "TimingRunSetup.h"
#include "TimingUtilityFunctions.h"
#include "UniformityUtilityFunctions.h"

//ROOT Includes
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"

namespace QualityControl {
    namespace Timing {
        class SelectorTiming : public Uniformity::Selector {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            SelectorTiming(Timing::RunSetup & inputSetup);
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Filters an input vector<Timing::EventReco> object
            virtual std::vector<Timing::EventReco> filterEvents(std::vector<Timing::EventReco> vec_inputEvts);
            
            //Filters an input vector<Timing::EventReco> object based on the provided Uniformity::AnalysisSetupUniformity object
            virtual std::vector<Timing::EventReco> filterEvents(std::vector<Timing::EventReco> vec_inputEvts, Uniformity::AnalysisSetupUniformity inputSetup){
                setAnalysisParameters(inputSetup);
                return filterEvents(vec_inputEvts);
            };
            
            //Check if Event Passes selection stored in aSetupUniformity? True -> Passes; False -> Fails
            bool eventPassesSelection(Timing::EventReco &inputEvt);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Given an output ROOT file from VME System with events
            //Returns a vector<Timing::EventReco> object of those EventReco objects passing event selection
            //Input is a std::string storing the physical filename
            virtual std::vector<Timing::EventReco> getEventsReco(std::string &strInputRootFileName, Timing::TestStandVME &testStand);
            
            //Given an output ROOT file from VME System with events
            //Returns a vector<Timing::EventReco> object of those EventReco objects passing event selection
            //Input is a TFile *
            virtual std::vector<Timing::EventReco> getEventsReco(TFile * file_InputRootFile, Timing::TestStandVME & testStand);
            
            //As above but overwrites the stored AnalysisSetupUniformity object
            //Input is a std::string storing the physical filename
            virtual std::vector<Timing::EventReco> getEventsReco(std::string &strInputRootFileName, Timing::TestStandVME &testStand, Uniformity::AnalysisSetupUniformity inputSetup){
                setAnalysisParameters(inputSetup);
                return getEventsReco(strInputRootFileName, testStand);
            };
            
            //As above but overwrites the stored AnalysisSetupUniformity object
            //Input is a TFile *
            virtual std::vector<Timing::EventReco> getEventsReco(TFile * file_InputRootFile, Timing::TestStandVME & testStand, Uniformity::AnalysisSetupUniformity inputSetup){
                setAnalysisParameters(inputSetup);
                return getEventsReco(file_InputRootFile, testStand);
            };
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            virtual void setDAQSetup(Timing::RunSetup & inputSetup){
                m_daqSetup = inputSetup;
                return;
            };
            
        protected:
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            virtual int getTransformedChanNum(int iChInputNum, int iMaxChanNum);
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------

            //Attributes - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Timing::RunSetup m_daqSetup;
        };
    } //End namespace Timing
} //End namespace QualityControl

#endif /* defined(____SelectorTiming__) */