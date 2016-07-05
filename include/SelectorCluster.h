//
//  SelectorCluster.h
//  
//
//  Created by Brian L Dorney on 28/01/16.
//
//

#ifndef ____SelectorCluster__
#define ____SelectorCluster__

//C++ Includes
#include <stdio.h>
#include <string>
#include <vector>

//Framework Includes
#include "DetectorMPGD.h"
#include "Selector.h"
#include "TimingUtilityFunctions.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"

namespace QualityControl {
    namespace Uniformity {
        class SelectorCluster : public Selector {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            SelectorCluster();
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Filters an input vector<Uniformity::Cluster object based on the stored Uniformity::AnalysisSetupUniformity attribute
            virtual std::vector<Uniformity::Cluster> filterClusters(std::vector<Uniformity::Cluster> vec_inputClusts);
            
            //Filters an input vector<Uniformity::Cluster> object based on the provided Uniformity::AnalysisSetupUniformity object
            virtual std::vector<Uniformity::Cluster> filterClusters(std::vector<Uniformity::Cluster> vec_inputClusts, Uniformity::AnalysisSetupUniformity inputSetup){
                setAnalysisParameters(inputSetup);
                return filterClusters(vec_inputClusts);
            };
            
            //Check if Cluster Passes selection stored in aSetupUniformity? True -> Passes; False -> Fails
            bool clusterPassesSelection(Uniformity::Cluster &inputClust);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Given an output ROOT file from amoreSRS with clusters
            //Applies the cluster selection and stores those selected clusters in inputDet
            //Input is a std::string storing the physical filename
            virtual void setClusters(std::string &strInputRootFileName, Uniformity::DetectorMPGD &inputDet);
            
            //Given an output ROOT file from amoreSRS with clusters
            //Applies the cluster selection and stores those selected clusters in inputDet
            //Input is a TFile *
            virtual void setClusters(TFile * file_InputRootFile, Uniformity::DetectorMPGD &inputDet);
            
            //Sets the clusters found in event number iEvtNum in TTree inputTree to event inputEvt
            //virtual void setClusters(Uniformity::Event &inputEvt, TTree * inputTree, int iEvtNum);
            
            //As above but overwrites the stored AnalysisSetupUniformity object
            //Input is a std::string storing the physical filename
            virtual void setClusters(std::string &strInputRootFileName, Uniformity::DetectorMPGD &inputDet, Uniformity::AnalysisSetupUniformity inputSetup){
                setAnalysisParameters(inputSetup);
                setClusters(strInputRootFileName, inputDet);
                return;
            };
            
            //As above but overwrites the stored AnalysisSetupUniformity object
            //Input is a TFile *
            virtual void setClusters(TFile * file_InputRootFile, Uniformity::DetectorMPGD &inputDet, Uniformity::AnalysisSetupUniformity inputSetup){
                setAnalysisParameters(inputSetup);
                setClusters(file_InputRootFile, inputDet);
                return;
            };
            
        private:
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
        }; //End class SelectorCluster
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____SelectorCluster__) */
