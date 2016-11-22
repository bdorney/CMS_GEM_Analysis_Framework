//
//  AnalyzeTiming.h
//  
//
//  Created by Brian L Dorney on 21/11/16.
//
//

#ifndef ____AnalyzeTiming__
#define ____AnalyzeTiming__

//C++ Includes
#include <iostream>
#include <map>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>
#include <utility>

//Framework Includes
#include "Analyzer.h"
#include "PlotterUtilityTypes.h"
#include "TestStandVME.h"
#include "TimingEvent.h"
#include "TimingRunSetup.h"
#include "TimingUtilityTypes.h"
#include "UniformityUtilityFunctions.h"

//ROOT Includes
#include "TH1F.h"
#include "TFile.h"
#include "TROOT.h"

namespace QualityControl {
    namespace Timing {
        class AnalyzeTiming : public QualityControl::Analyzer {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Destructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            ~AnalyzeTiming(){
                reset();
            };
            
            //Operators
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Filling*****************
            //Fills histograms stored in m_mtrxHistos;
            virtual void fillHistos(std::vector<Timing::EventReco> vec_inputEvtsReco);
            
            //Fitting*****************
            //Fits histograms stored in m_mtrxHistos
            virtual void fitHistos();
            
            //Initialize**************
            //Initializes all TH1F objects for all detectors in a test stand
            virtual void initHistos(Timing::DetectorMatrix detMatrix);
	    //virtual void initHistos(Timing::TestStandVME testStand);            

            //Loading*****************

            //Storing*****************
            //Stores booked histograms (for those histograms that are non-null)
            //Takes a std::string which stores the physical filename as input
            void storeHistos(std::string & strOutputROOTFileName, std::string strOption);
            
            //Stores booked histograms (for those histograms that are non-null)
            //Takes a TFile * which the histograms are written to as input
            void storeHistos(TFile * file_InputRootFile);

            //Reseting****************
            virtual void reset();
            
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
        protected:
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Gets a histogram for channel iChan, iChan = -1 treated as the "All Channel" case
            virtual TF1 getFit(int iChan, std::string strLogic, Plotter::InfoFit & setupFit, std::shared_ptr<TH1F> hInput, TSpectrum &specInput);
            
            //Gets a histogram for channel iChan, iChan = -1 treated as the "All Channel" case
            virtual TH1F getHistogram(int iChan, std::string strLogic, Timing::HistoSetup setupHisto);
            
            //Gets a histogram for channel iChan, iChan = -1 treated as teh "All Channel" case
            virtual TH2F getHistogram2D(int iChan, Timing::HistoSetup &setupHisto_X, Timing::HistoSetup &setupHisto_Y);
            
            //Gets a name for a channel iChan, iChan = -1 treated as the "All Channel" case
            virtual std::string getNameByChanNum(int iChan, std::string strPrefix, std::string strName);
            
            //Attributes - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            Timing::HistoMatrix m_mtrxHistos;
            
        }; //End class AnalyzeTiming
    } //End namespace Timing
} //End namespace QualityControl

#endif /* defined(____AnalyzeTiming__) */
