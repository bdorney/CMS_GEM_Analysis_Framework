//
//  TestStandVME.h
//  
//
//  Created by Brian L Dorney on 26/10/16.
//
//

#ifndef ____TestStandVME__
#define ____TestStandVME__

//C++ Includes
#include <map>
#include <stdio.h>
#include <utility>
#include <vector>

//Framework Includes
#include "DetectorTiming.h"
#include "DetectorSuperchamber.h"
#include "TimingEvent.h"
#include "TimingUtilityTypes.h"

//ROOT Includes

namespace QualityControl {
    namespace Timing {
        
        class TestStandVME {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Destructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            ~TestStandVME(){
                reset();
            };
            
            //Operators
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            virtual void reset();
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Get a single event
            virtual Timing::EventReco getEventReco(Timing::EventDigi & inputEvtDIGI);
            
            virtual Timing::DetectorMatrix getMatrix(){
                return m_detMatrix;
            };
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Add a detector to the stand
            virtual void setDetector(int iRow, int iCol, std::shared_ptr<Timing::DetectorTiming> inputDet, Timing::DetType detType);

            //Add a superchamber to the stand
            //Erases any superchamber previously at that position
            virtual void setSuperchamber(int iRow, int iCol, std::shared_ptr<Timing::DetectorSuperchamber> inputSuperchamber);
            
        protected:
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Attributes - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Map of layers; key value is the row number
            Timing::DetectorMatrix m_detMatrix;
            
        }; //End class TestStandVME
    } //End namespace Timing
} //End namespace QualityControl

#endif /* defined(____TestStandVME__) */