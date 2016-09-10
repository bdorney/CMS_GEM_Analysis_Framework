//
//  Visualizer.h
//  
//
//  Created by Brian L Dorney on 10/09/16.
//
//

#ifndef ____Visualizer__
#define ____Visualizer__

//C++ Includes
#include <stdio.h>
#include <string>

//Framework Includes
#include "AnalyzeResponseUniformity.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes
#include "TCanvas.h"
#include "TPad.h"

namespace QualityControl {
    namespace Uniformity {
        class Visualizer : public AnalyzeResponseUniformity {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            Visualizer();
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Sets the flag for automatically saving canvases
            virtual void setAutoSaveCanvas(bool bInput){ bSaveCanvases = bInput; return; };
            
        protected:
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Saves inputCanv as a *.png file
            //The file is placed in the working directory
            //The name of the file is the TName of the canvas
            virtual void save2png(TCanvas & inputCanvas);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Get a TPad sized for an iEta Grid
            virtual TPad *getPadEta(int iEta, int iNumEta);
            
            //Get a TPad sized for an iPhi Grid
            virtual TPad *getPadPhi(int iEta, int iNumEta, int iPhi, int iNumPhi);
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Data Members
            //------------------------------------------------------------------------------------------------------------------------------------------
            bool bSaveCanvases;
            
        }; //End class Visualizer
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____Visualizer__) */
