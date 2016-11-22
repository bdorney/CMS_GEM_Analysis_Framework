//
//  DetectorSuperchamber.h
//  
//
//  Created by Brian L Dorney on 17/11/16.
//
//

#ifndef ____DetectorSuperchamber__
#define ____DetectorSuperchamber__

//C++ Includes
#include <iostream>
#include <map>
#include <memory>
#include <stdio.h>
#include <string>

//Framework Includes
#include "DetectorTiming.h"
#include "TimingUtilityTypes.h"
#include "TimingUtilityFunctions.h"

//ROOT Includes

namespace QualityControl{
    namespace Timing{
        
        //Declare class
        //class DetectorSuperchamber;
        
        //Define detector iterator
        typedef
        //std::map<int, std::shared_ptr<Timing::DetectorTiming> >::const_iterator imap_detIter_t;
        //std::map<int, Timing::DetectorTiming>::const_iterator imap_detIter_t;
        std::map<int, Timing::DetectorTiming>::iterator imap_detIter_t;
        
        //Define class
        class DetectorSuperchamber {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            DetectorSuperchamber();
            
            //Destructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            /*~DetectorSuperchamber(){
                reset();
            };*/
            
            //Operators
            //------------------------------------------------------------------------------------------------------------------------------------------
            /*DetectorSuperchamber & operator=(const DetectorSuperchamber & other){
                if (this != &other) { //Protects against invalid self-assignment
                    m_strName = other.m_strName;  //Name of the detector
                    m_map_strBaseAddr2iDetPos = other.m_map_strBaseAddr2iDetPos;
                    detType = other.detType;
                    
                    for (auto iterDet = other.m_map_detectors.begin(); iterDet != other.m_map_detectors.end(); ++iterDet) {
                        m_map_detectors[ (*iterDet).first ] = (*iterDet).second;
                    }
                } //Protects against invalid self-assignment
                
                return *this;
            }*/ //End Assignment operator
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            virtual void calcAndDeltaOr();
            
            virtual void reset();
            virtual void resetChannels();
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Get Detector - by index within superchamber
            /*virtual std::shared_ptr<Timing::DetectorTiming> getDetector(int iDetPos){
                return m_map_detectors[iDetPos];
            };*/
            virtual Timing::DetectorTiming getDetector(int iDetPos){
                return m_map_detectors[iDetPos];
            };
            
            //Get Detector - by VME base address
            //virtual std::shared_ptr<Timing::DetectorTiming> getDetector(std::string & strBaseAddress);
            virtual Timing::DetectorTiming getDetector(std::string & strBaseAddress);
            
            //Get the AND of all detectors
            virtual double getDetectorAND(){ return m_dDetAND; };
            
            //Get the difference between fastest and slowest time from set of detectors
            virtual double getDetectorDelta(){ return m_dDetDelta; };
            
            //Get the OR of all detectors (first time, any number of detectors)
            virtual double getDetectorOR(){ return m_dDetOR; };
            
            //Get Pointer to the beginning of m_map_detectors
            virtual Timing::imap_detIter_t getDetectorPtrBegin(){
                return m_map_detectors.begin();
            };
            
            //Get Pointer to the ending of m_map_detectors
            virtual Timing::imap_detIter_t getDetectorPtrEnd(){
                return m_map_detectors.end();
            };
            
            
            //Get Detector's name
            virtual std::string getName(){
                return m_strName;
            };
            
            //Get Number of detectors
            virtual int getNDetectors(){
                return m_map_detectors.size();
            };
            
            virtual Timing::DetType getType(){
                return m_detType;
            };
            
            virtual bool hasData();
            virtual bool hasData(int & iNumNonZeroDet);
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Set a detector
            //virtual void setDetector(int iDetPos, std::shared_ptr<Timing::DetectorTiming> inputDetector);
            //virtual void setDetector(int iDetPos, Timing::DetectorTiming &inputDetector);
            virtual void setDetector(int iDetPos, Timing::DetectorTiming inputDetector);
            
            //Set Detector's name
            virtual void setName(std::string strInput){
                m_strName = strInput;
                return;
            };
            
            //Set the detector's type
            virtual void setType(Timing::DetType inputDetType){
                m_detType = inputDetType;
                return;
            };
            
        protected:
            //Attributes - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            double m_dDetAND;
            double m_dDetDelta;
            double m_dDetOR;
            
            std::string m_strName;  //Name of the detector
            
            //Key value is detector position within superchamber
            //Mapped value is base address the of the detector
            std::map<std::string, int> m_map_strBaseAddr2iDetPos;
            
            //std::map<int, std::shared_ptr<Timing::DetectorTiming> > m_map_detectors;
            std::map<int, Timing::DetectorTiming> m_map_detectors;
            
            Timing::DetType m_detType;
            
        }; //End class DetectorSuperchamber
        
        //Define detector matrix
        //Don't necessarily need to wrap this map in a struct,
        //But in case we need to add other information in the future I have.
        struct DetectorMatrix{
            //Key value is the (i,j) or (i,k) index on the stand
            //The superchamber holds either detectors or pmts
            //std::map<std::pair<int,int>, std::shared_ptr<Timing::DetectorSuperchamber> > m_map_detectors;
            std::map<std::pair<int,int>, Timing::DetectorSuperchamber> m_map_detectors;
            
            //Default Destructor
            ~DetectorMatrix(){
                clear();
            }
            
            /*std::map<std::pair<int,int>, std::shared_ptr<Timing::DetectorSuperchamber> >::iterator begin(){
             return m_map_detectors.begin();
             }*/
            
            std::map<std::pair<int,int>, Timing::DetectorSuperchamber>::iterator begin(){
                return m_map_detectors.begin();
            }
            
            void clear(){
                m_map_detectors.clear();
            }
            
            /*std::map<std::pair<int,int>, std::shared_ptr<Timing::DetectorSuperchamber> >::iterator end(){
             return m_map_detectors.end();
             }*/
            
            std::map<std::pair<int,int>, Timing::DetectorSuperchamber>::iterator end(){
                return m_map_detectors.end();
            }
            
            /*std::shared_ptr<Timing::DetectorSuperchamber> & operator[](std::pair<int,int> pair_Pos){
             return m_map_detectors[pair_Pos];
             }*/ //End subscript operator
            
            Timing::DetectorSuperchamber & operator[](std::pair<int,int> pair_Pos){
                return m_map_detectors[pair_Pos];
            } //End subscript operator
            
            /*DetectorMatrix & operator=(const DetectorMatrix & other){ //assignment operator
             if (this != &other) { //Protects against invalid self-assignment
             for (auto iterSC = other.m_map_detectors.begin(); iterSC != other.m_map_detectors.end(); ++iterSC) {
             m_map_detectors[ (*iterSC).first ] = (*iterSC).second;
             }
             } //Protects against invalid self-assignment
             
             return *this;
             }*/ //End assignment operator
        }; //End DetectorMatrix
    } //End namespace Timing
} //End namespace QualityControl


#endif /* defined(____DetectorSuperchamber__) */
