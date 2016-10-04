//
//  InterfaceReco.h
//  
//
//  Created by Brian L Dorney on 04/10/16.
//
//

#ifndef ____InterfaceReco__
#define ____InterfaceReco__

//C++ Includes
#include <map>
#include <stdio.h>
#include <utility>
#include <vector>

//Framework Includes
#include "FrameworkBase.h"

//ROOT Includes

namespace QualityControl {
    namespace Uniformity{
        class InterfaceReco : public FrameworkBase {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            InterfaceReco();
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            virtual void reconstruct();
            virtual void reconstruct(int iNum_Run, std::string strInputRun){
                m_vec_pairedRunList.clear();
                m_vec_pairedRunList.push_back(std::make_pair(iNum_Run, strInputRun ) );
                reconstruct();
                return;
            }
            virtual void reconstruct(std::vector<std::pair<int, std::string> > vec_inputPairedRunList){
                m_vec_pairedRunList = vec_inputPairedRunList;
                reconstruct();
                return;
            }
            
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
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            std::vector<std::pair<int, std::string> > m_vec_pairedRunList;
            
        }; //End class InterfaceReco
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____InterfaceReco__) */
