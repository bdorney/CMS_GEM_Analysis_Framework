//
//  VisualizeComparison.h
//  
//
//  Created by Brian L Dorney on 10/09/16.
//
//

#ifndef ____VisualizeComparison__
#define ____VisualizeComparison__

//C++ Includes
#include <iterator>
#include <iostream>
#include <map>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

//Framework Includes
#include "Visualizer.h"

//ROOT Includes
#include "TDirectory.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TROOT.h"

namespace QualityControl {
    namespace Uniformity {
        class VisualizeComparison : public Visualizer {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            VisualizeComparison();
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Uses stored RunSetup object to a draw a given observable onto a single pad
            virtual void storeCanvasComparisonHisto(){
                storeCanvasComparisonHisto(rSetup.strFile_Output_Name, rSetup.strFile_Output_Option, rSetup.strObsName);
                
                return;
            };
            
            //Draws a given observable onto a single pad of canvas
            //Takes a std::string which stores the physical filename as input
            virtual void storeCanvasComparisonHisto(std::string strOutputROOTFileName, std::string strOption, std::string strObsName);
            
            //Draws a given observable onto a single pad of canvas
            //Takes a TFile * which the histograms are written to as input
            virtual void storeCanvasComparisonHisto(TFile * file_InputRootFile, std::string strObsName);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
	    //Sets the draw option
	    virtual void setDrawOption(std::string strInput){ m_strDrawOption = strInput; return; };

            //Sets the input identifier
            virtual void setIdentifier(std::string strInput){ m_strIdent = strInput; return; };
            
            //Sets the input files to be considered
            virtual void setInputFiles(std::vector<std::string> vec_strInput){
                m_vec_strFileList = vec_strInput;
                return;
            };
            
            //Should the created distributions be normalized?
            virtual void setNormalize(bool bInput){ m_bNormalize = bInput; return; };

            //Sets the observable position - One index at a time
            virtual void setPosEta(int iInput){ m_iEta = iInput; return; };
            virtual void setPosPhi(int iInput){ m_iPhi = iInput; return; };
            virtual void setPosSlice(int iInput){ m_iSlice = iInput; return; };
            
            //Sets the observable position - (iEta, iPhi, iSlice)
            virtual void setPosFull(int iInputEta, int iInputPhi, int iInputSlice){
                setPosEta(iInputEta);
                setPosPhi(iInputPhi);
                setPosSlice(iInputSlice);
                return;
            };
            
            //Sets the observable position - (iEta, iPhi)
            virtual void setPosEtaPhi(int iInputEta, int iInputPhi){
                setPosFull(iInputEta, iInputPhi, -1);
                return;
            };
            
            virtual void setRunParameters(Uniformity::RunSetup inputSetup);
            
            
        private:
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            virtual std::shared_ptr<TH1F> getObsHisto(TFile * file_InputRootFile, std::string strObsName);
            
            //Returns a map of identifier (key) and observable histogram
            virtual std::map<std::string, std::shared_ptr<TH1F> > getObsHistoMap(std::string strObsName);
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Data Members
            //------------------------------------------------------------------------------------------------------------------------------------------
            bool m_bNormalize;
            
            int m_iEta, m_iPhi, m_iSlice;
            
            std::string m_strIdent, m_strDrawOption;
            
            std::vector<std::string> m_vec_strFileList;
            
        }; //End class VisualizeComparison
    } //End namespace Uniformity
} //End namespace QualityControl


#endif /* defined(____VisualizeComparison__) */
