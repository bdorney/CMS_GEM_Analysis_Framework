//
//  DetectorMPGD.h
//  
//
//  Created by Brian L Dorney on 20/01/16.
//
//

#ifndef ____DetectorMPGD__
#define ____DetectorMPGD__

//C++ Includes
#include <map>
#include <stdio.h>
#include <vector>

//My Includes
#include "TimingUtilityFunctions.h"
#include "UniformityUtilityTypes.h"
#include "UniformityUtilityFunctions.h"

//ROOT Includes
#include "TROOT.h"

using namespace ROOT;

namespace Uniformity {
    class ParameterLoaderAmoreSRS;  //Defined in "ParameterLoaderAmoreSRS.h"
    class AnalyzeResponseUniformity; //Defined in "AnalyzeResponseUniformity.h"
    
    class DetectorMPGD {
        friend class ParameterLoaderAmoreSRS;
        friend class AnalyzeResponseUniformity;
        
    public:
        //Constructors
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Default Constructor (empty for now)
        DetectorMPGD();
        
        //Constructor to use when supplying a vector of clusters
        DetectorMPGD(std::vector<Cluster> vec_inputClusters);
        
        //Constructor to use when supplying a list of sectors;
        DetectorMPGD(std::map<int, SectorEta> map_inputSectors);
        
        //Getters - Methods that Get (i.e. Return) Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Returns all clusters
        virtual std::vector<Cluster> getClusters();
        
        //Returns clusters for a given iEta value (all iPhi)
        virtual std::vector<Cluster> getClusters(int iEta);
        
        //Returns clusters for a given (iEta,iPhi value)
        virtual std::vector<Cluster> getClusters(int iEta, int iPhi);
        
        //returns the position of an iEta sector
        virtual float getEtaPos(int iEta);
        
        //returns the eta sector
        virtual SectorEta getEtaSector(int iEta);
        
        //returns the width of an iEta sector
        virtual float getEtaWidth(int iEta);
        
        //returns the number of eta sectors
        virtual int getNumEtaSectors(){ return map_sectorsEta.size(); };

        //returns the phi sector
        virtual SectorPhi getPhiSector(int iEta, int iPhi);
        
        //Printers - Methods that Print Something
        //------------------------------------------------------------------------------------------------------------------------------------------
       
        //Setters - Methods that Set Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Sets a cluster
        virtual void setCluster(Cluster &inputCluster);
        
        //Sets all clusters
        virtual void setCluster(std::vector<Cluster> &vec_inputClusters){
            for (int i=0; i<vec_inputClusters.size(); ++i) {
                setCluster(vec_inputClusters[i]);
            }
            
            return;
        };
        
        //Defines an eta sector; will NOT over-write what is currently stored
        virtual void setEtaSector(int iEta, float fInputPos_Y, float fInputWidth);
        
        //Sets a single eta sector; over-writes what is currently stored (if any)
        virtual void setEtaSector(int iEta, SectorEta inputEtaSector){
            map_sectorsEta[iEta] = inputEtaSector; return;
        };
        
        //Sets all eta sectors; over-writes what is currently stored (if any)
        virtual void setEtaSector(std::map<int, SectorEta> map_inputSectors){
            map_sectorsEta = map_inputSectors; return;
        };
        
    private:
        std::map<int, SectorEta> map_sectorsEta;
    };
} //End namespace Uniformity


#endif /* defined(____DetectorMPGD__) */
