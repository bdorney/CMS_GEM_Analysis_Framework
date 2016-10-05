//
//  PlotterGraph.h
//  
//
//  Created by Brian L Dorney on 05/10/16.
//
//

#ifndef ____PlotterGraph__
#define ____PlotterGraph__

//C++ Includes
#include <map>
#include <memory>
#include <stdio.h>
#include <string>

//Framework Includes
#include "PlotterGeneric.h"
#include "PlotterUtilityTypes.h"

//ROOT Includes
#include "TFile.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TROOT.h"

namespace QualityControl {
    namespace Plotter {
        class PlotterGraph : public PlotterGeneric {
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            //PlotterGraph();
            
            //Destructor
            //------------------------------------------------------------------------------------------------------------------------------------------
            ~PlotterGraph(){
                //m_canv.reset();
                //m_tdrStyle.reset();
                m_map_graphs.clear();
            }
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
        protected:
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Intializes the plots defined in m_canvInfo
            //To be over-ridded by inherited classes
            void initPlot(InfoPlot & plotInfo);
            
            //Makes the plots defined in m_canvInfo
            //To be over-ridded by inherited classes
            //virtual void makePlots(TCanvas & inputCanvas, TLegend & inputLegend, InfoPlot & plotInfo);
            void makePlot(TLegend & inputLegend, InfoPlot & plotInfo);
            
            //Saves all TObjects to an output ROOT file
            //To be over-ridded by inherited classes
            virtual void write2RootFile();
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Attributes
            //------------------------------------------------------------------------------------------------------------------------------------------
            std::map<std::string, std::shared_ptr<TGraphErrors> > m_map_graphs;
        };
    } //End namespace Plotter
} //End namespace QualityControl


#endif /* defined(____PlotterGraph__) */
