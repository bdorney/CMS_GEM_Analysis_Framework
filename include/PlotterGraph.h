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
#include <iterator>
#include <map>
#include <memory>
#include <stdio.h>
#include <string>
#include <utility>

//Framework Includes
#include "PlotterGeneric.h"
#include "PlotterUtilityTypes.h"

//ROOT Includes
#include "TFile.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "TROOT.h"

namespace QualityControl {
    namespace Plotter {
        class PlotterGraph : public PlotterGeneric {
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            PlotterGraph();
            
            //Destructor
            //------------------------------------------------------------------------------------------------------------------------------------------
            ~PlotterGraph(){
                //m_canv.reset();
                //m_tdrStyle.reset();
                m_map_graphs.clear();
                m_mgraph_Obs.reset();
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
            void addPlot(TLegend & inputLegend, InfoPlot & plotInfo);
            
	    //Draw Fits, right now fits are assumed to always be TF1 objects
            //  Draws all fits defined for each plot
            //Developer can override though
            //virtual void drawFits(TLegend & inputLegend);
            
            //To be over-ridded by inherited classes
            virtual void drawPlots();
            
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
            std::multimap<std::string, std::shared_ptr<TGraph> > m_map_graphs;
            
            std::shared_ptr<TMultiGraph> m_mgraph_Obs;
        };
    } //End namespace Plotter
} //End namespace QualityControl


#endif /* defined(____PlotterGraph__) */
