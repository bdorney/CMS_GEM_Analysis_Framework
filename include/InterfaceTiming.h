//
//  InterfaceTiming.h
//  
//
//  Created by Brian L Dorney on 08/11/16.
//
//

#ifndef ____InterfaceTiming__
#define ____InterfaceTiming__

//C++ Includes
#include <memory>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

//Framework Includes
#include "TestStandVME.h"
#include "TimingEvent.h"

//ROOT Includes
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"

namespace QualityControl {
    namespace Timing {
        class InterfaceTiming {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            InterfaceTiming();
            
            //Destructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            ~InterfaceTiming(){
                reset();
            };
            
            //Operators
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Load events into the test stand
            //Two inpust strings: 1) PFN of the input file, 2) load option
            virtual void loadEventsDigi(std::string & strInputROOTFileName);
            
            //Load events into the test stand
            //Takes an input ROOT file containing the data
            virtual void loadEventsDigi(TFile * file_InputRootFile);
            
            //Reset the class
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
            
            //Attributes - Methods that Set Something
            //-----------------------------------------------------------------------------------------------------------------------------------------
            Timing::TestStandVME m_testStand;   //VME test stand
            
            std::vector<std::pair<std::string,int> > vec_pairVMEBoardInfo;  //
            
        }; //End class InterfaceTiming
    } //End namespace Timing
} //End  namespace QualityControl




#endif /* defined(____InterfaceTiming__) */
