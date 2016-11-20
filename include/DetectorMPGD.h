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
#include <algorithm>
#include <map>
#include <set>
#include <stdio.h>
#include <string>
#include <vector>

//Framework Includes
#include "ReadoutSectorEta.h"
#include "ReadoutSectorPhi.h"
#include "TimingUtilityFunctions.h"
#include "UniformityUtilityTypes.h"
#include "UniformityUtilityFunctions.h"

//ROOT Includes
#include "TH1F.h"
#include "TH2F.h"
#include "TROOT.h"

//using namespace ROOT;
namespace QualityControl {
    namespace Uniformity {
        class ParameterLoaderDetector;  //Defined in "ParameterLoaderDetector.h"
        
        class DetectorMPGD {
            friend class ParameterLoaderDetector;
            friend class AnalyzeResponseUniformity;
            friend class AnalyzeResponseUniformityClusters;
            friend class AnalyzeResponseUniformityHits;
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Default Constructor
            DetectorMPGD();
            
            //Copy Constructor
            DetectorMPGD(const DetectorMPGD& other){
                map_sectorsEta              = other.map_sectorsEta;
                strDetName                  = other.strDetName;
                strDetNameNoSpecChar        = other.strDetNameNoSpecChar;
                
                mset_fClustADC_Fit_NormChi2 = other.mset_fClustADC_Fit_NormChi2;                
                mset_fClustADC_Fit_PkPos    = other.mset_fClustADC_Fit_PkPos;
                mset_fClustADC_Fit_PkRes    = other.mset_fClustADC_Fit_PkRes;

                statClustADC_Fit_NormChi2   = other.statClustADC_Fit_NormChi2;                
                statClustADC_Fit_PkPos      = other.statClustADC_Fit_PkPos;
                statClustADC_Fit_PkRes      = other.statClustADC_Fit_PkRes;

                if( other.hMulti_Clust != NULL ){	hMulti_Clust = std::make_shared<TH1F>( *other.hMulti_Clust.get() ); }
                if( other.hMulti_Hit != NULL ){		hMulti_Hit   = std::make_shared<TH1F>( *other.hMulti_Hit.get() ); }
            };
            
            //Constructor to use when supplying a multimap of clusters
            DetectorMPGD(std::multimap<int, Cluster> map_inputClusters);
            
            //Constructor to use when supplying a list of sectors;
            DetectorMPGD(std::map<int, ReadoutSectorEta> map_inputSectors);
            
            //Destructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            ~DetectorMPGD(){
                reset();
            };

            //Operators
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Overloaded Assignment Operator
            DetectorMPGD & operator=(DetectorMPGD other){
                if (this != &other) { //Protects against invalid self-assignment
                    this->map_sectorsEta            = other.map_sectorsEta;
                    this->strDetName                = other.strDetName;
                    this->strDetNameNoSpecChar      = other.strDetNameNoSpecChar;

                    this->mset_fClustADC_Fit_NormChi2=other.mset_fClustADC_Fit_NormChi2;                    
                    this->mset_fClustADC_Fit_PkPos  = other.mset_fClustADC_Fit_PkPos;
                    this->mset_fClustADC_Fit_PkRes  = other.mset_fClustADC_Fit_PkRes;
                    
                    this->statClustADC_Fit_NormChi2 = other.statClustADC_Fit_NormChi2;                    
                    this->statClustADC_Fit_PkPos    = other.statClustADC_Fit_PkPos;
                    this->statClustADC_Fit_PkRes    = other.statClustADC_Fit_PkRes;

                    if( other.hMulti_Clust != NULL ){	this->hMulti_Clust = std::make_shared<TH1F>( *other.hMulti_Clust.get() ); }
                    if( other.hMulti_Hit != NULL ){	this->hMulti_Hit   = std::make_shared<TH1F>( *other.hMulti_Hit.get() ); }
                } //Protects against invalid self-assignment
                
                return *this;
            } //End Overloaded Assignment Operator
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //wipes all stored information
            virtual void reset(){
                map_sectorsEta.clear();

                mset_fClustADC_Fit_NormChi2.clear();
                mset_fClustADC_Fit_PkPos.clear();
                mset_fClustADC_Fit_PkRes.clear();

                statClustADC_Fit_NormChi2.clear();                
                statClustADC_Fit_PkPos.clear();
                statClustADC_Fit_PkRes.clear();

                return;
            } //End reset()
            virtual void resetClusters();
            virtual void resetHits();
            virtual void resetPhysObj();
            virtual void resetResults(){
		mset_fClustADC_Fit_NormChi2.clear();                
		mset_fClustADC_Fit_PkPos.clear();
                mset_fClustADC_Fit_PkRes.clear();

                statClustADC_Fit_NormChi2.clear();                
                statClustADC_Fit_PkPos.clear();
                statClustADC_Fit_PkRes.clear();

                return;
            };
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Returns all clusters
            virtual std::multimap<int, Cluster> getClusters();
            
            //Returns clusters for a given iEta value (all iPhi)
            virtual std::multimap<int, Cluster> getClusters(int iEta);
            
            //Returns clusters for a given (iEta,iPhi value)
            virtual std::multimap<int, Cluster> getClusters(int iEta, int iPhi);
            
            //returns the position of an iEta sector
            virtual float getEtaPos(int iEta);
            
            //returns the eta sector
            virtual ReadoutSectorEta getEtaSector(int iEta);
            //virtual void getEtaSector(int iEta, ReadoutSectorEta & retSector);
            
            //returns the width of an iEta sector
            virtual float getEtaWidth(int iEta);
            
            //Returns all hits
            virtual std::multimap<int, Hit> getHits();
            
            //Returns clusters for a given iEta value (all iPhi)
            virtual std::multimap<int, Hit> getHits(int iEta);
            
            //Returns clusters for a given (iEta,iPhi value)
            virtual std::multimap<int, Hit> getHits(int iEta, int iPhi);
            
            //returns the name of the detector
            virtual std::string getName(){ return strDetName; };
            
            //returns the name of the detector, omitting special characters
            virtual std::string getNameNoSpecial(){ return strDetNameNoSpecChar; };
            
            //returns the number of eta sectors
            virtual int getNumEtaSectors(){ return map_sectorsEta.size(); };
            
            //returns the phi sector
            virtual ReadoutSectorPhi getPhiSector(int iEta, int iPhi);
            
            //returns the summary statistics
            virtual SummaryStatistics getStatNormChi2(){ return statClustADC_Fit_NormChi2; };
            virtual SummaryStatistics getStatPkPos(){ return statClustADC_Fit_PkPos; };
            virtual SummaryStatistics getStatPkRes(){ return statClustADC_Fit_PkRes; };
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Sets a cluster
            virtual void setCluster(int iNum_Evt, Cluster &inputCluster);
            
            //Sets all clusters
            virtual void setClusters(std::multimap<int, Cluster> &map_inputClusters){
                for (auto iterClust = map_inputClusters.begin(); iterClust != map_inputClusters.end(); ++iterClust) {
                    setCluster( (*iterClust).first, (*iterClust).second );
                }
                
                return;
            };
            
            //Defines an eta sector; will NOT over-write what is currently stored
            virtual void setEtaSector(int iEta, float fInputPos_Y, float fInputWidth, int iNumPhiSector);
            
            //Sets a single eta sector; over-writes what is currently stored (if any)
            virtual void setEtaSector(int iEta, ReadoutSectorEta inputEtaSector){
                map_sectorsEta[iEta] = inputEtaSector;
                return;
            };
            
            //Sets all eta sectors; over-writes what is currently stored (if any)
            virtual void setEtaSector(std::map<int, ReadoutSectorEta> map_inputSectors){
                map_sectorsEta = map_inputSectors; return;
            };
            
            //Given an input event sets the hits & clusters in this event
            virtual void setEvent(Event &inputEvt);

            //Given an input event sets the hits & clusters in this event
            virtual void setEventOnlyClusters(Event &inputEvt);

            //Given an input event sets the hits & clusters in this event
            virtual void setEventOnlyHits(Event &inputEvt);

            //Given an input event sets the hits & clusters in this event
            virtual void setEvents(std::vector<Event> & vec_inputEvts){
                for (int i=0; i < vec_inputEvts.size(); ++i) {
                    setEvent(vec_inputEvts[i]);
                }
                
                return;
            };
            
            //Sets a hit
            virtual void setHit(int iNum_Evt, Hit &inputHit);
            
            //Sets all hits
            virtual void setHits(std::multimap<int, Hit> & map_inputHits){
                for (auto iterHit = map_inputHits.begin(); iterHit != map_inputHits.end(); ++iterHit) {
                    setHit( (*iterHit).first, (*iterHit).second );
                }
                
                return;
            };
            
            //Sets the name of the detector
            virtual void setName(std::string strInput){
                strDetName = strDetNameNoSpecChar = strInput;
                strDetNameNoSpecChar.erase( std::remove(strDetNameNoSpecChar.begin(), strDetNameNoSpecChar.end(), '/' ), strDetNameNoSpecChar.end() );
                return;
            };
            
        private:
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Sets a version of the detector name without special characters
            virtual void setNameNoSpecChar(std::string strInput){
                strDetNameNoSpecChar = strInput;
                strDetNameNoSpecChar.erase( std::remove(strDetNameNoSpecChar.begin(), strDetNameNoSpecChar.end(), '/' ), strDetNameNoSpecChar.end() );
                return;
            };
            
            std::map<int, ReadoutSectorEta> map_sectorsEta;
            
            std::multiset<float> mset_fClustADC_Fit_NormChi2;
            std::multiset<float> mset_fClustADC_Fit_PkPos;
            std::multiset<float> mset_fClustADC_Fit_PkRes;
            
            SummaryStatistics statClustADC_Fit_NormChi2;
            SummaryStatistics statClustADC_Fit_PkPos;
            SummaryStatistics statClustADC_Fit_PkRes;
            
            std::string strDetName;             //Name of the detector
            std::string strDetNameNoSpecChar;   //Name of the detector omitting special characters (e.g. '/' )
            
            std::shared_ptr<TH1F> hMulti_Clust; //Cluster Multiplicity over the entire detector
            std::shared_ptr<TH1F> hMulti_Hit; //Hit Multiplicity over the entire detector
        }; //End Class DetectorMPGD
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____DetectorMPGD__) */
