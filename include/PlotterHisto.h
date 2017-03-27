//
//  PlotterGraph.h
//  
//
//  Created by Brian L Dorney on 05/10/16.
//
//

#ifndef ____PlotterHisto__
#define ____PlotterHisto__

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
#include "TH1F.h"
//#include "TGraphErrors.h"
#include "TLegend.h"
//#include "TMultiGraph.h"
#include "TROOT.h"

namespace QualityControl {
    namespace Plotter {
        class PlotterHisto : public PlotterGeneric {
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            PlotterHisto();
            
            //Destructor
            //------------------------------------------------------------------------------------------------------------------------------------------
            ~PlotterHisto(){
                m_map_histos.clear();
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
            
            //To be over-ridded by inherited classes
            virtual void drawPlots();
            
            //In inherited class developer should:
            //  Perform a fit and draws it for the requested plot
            //To be over-ridded by inherited classes
            void performAndDrawFit(TLegend & inputLegend, InfoFit & fitInfo, InfoPlot & plotInfo);
            
            //Saves all TObjects to an output ROOT file
            //To be over-ridded by inherited classes
            virtual void write2RootFile();
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Intializes a plot defined in an input InfoPlot (aka plotInfo)
            std::shared_ptr<TH1F> getPlot(InfoPlot & plotInfo);

            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Attributes
            //------------------------------------------------------------------------------------------------------------------------------------------
            //std::map<std::string, std::shared_ptr<TGraphErrors> > m_map_graphs;
            //std::map<std::string, std::shared_ptr<TH1F> > m_map_histos;
            std::multimap<std::string, std::shared_ptr<TH1F> > m_map_histos;
            
            //std::shared_ptr<TMultiGraph> mgraph_Obs;
        };
    } //End namespace Plotter
} //End namespace QualityControl


#endif /* defined(____PlotterGraph__) */
