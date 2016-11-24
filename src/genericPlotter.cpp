//
//  genericPlotter.cpp
//  
//
//  Created by Brian L Dorney on 05/10/16.
//
//

//C++ Includes
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <utility>

//Framework Includes
#include "ParameterLoaderPlotter.h"
#include "PlotterGraph.h"
#include "PlotterGraph2D.h"
#include "PlotterGraphErrors.h"
#include "PlotterHisto.h"
#include "PlotterHisto2D.h"
#include "PlotterUtilityTypes.h"
#include "QualityControlSectionNames.h"

//ROOT Includes

//Namespaces
using std::cout;
using std::endl;
using std::get;
using std::string;
using std::vector;

using QualityControl::Timing::convert2bool;
using QualityControl::Timing::printStreamStatus;

using namespace QualityControl::Plotter;

void printHelpMenu(){
    
} //End printHelpMenu()

void printInfoPlot(InfoPlot & inputPlotInfo){
    cout<<"\t--------------------Begin Plot--------------------\n";
    
    //Color
    cout<<"\tplotSetup.m_iColor = " << inputPlotInfo.m_iColor << endl;
    
    //Style
    cout<<"\tplotSetup.m_iStyleLine = " << inputPlotInfo.m_iStyleLine << endl;
    cout<<"\tplotSetup.m_iStyleMarker = " << inputPlotInfo.m_iStyleMarker << endl;
    
    //Size
    cout<<"\tplotSetup.m_fSizeLine = " << inputPlotInfo.m_fSizeLine << endl;
    cout<<"\tplotSetup.m_fSizeMarker = " << inputPlotInfo.m_fSizeMarker << endl;
        
    //Legend Entry
    cout<<"\tplotSetup.m_strLegEntry = " << inputPlotInfo.m_strLegEntry << endl;
    
    //Name
    cout<<"\tplotSetup.m_strName = " << inputPlotInfo.m_strName << endl;
        
    //Data
    cout<<"\t\tX\tX_Err\tY\tY_Err\n";
    for (int i=0; i<inputPlotInfo.m_vec_DataPts.size(); ++i) {
        cout<<"\t";
        cout<<"\t"<<inputPlotInfo.m_vec_DataPts[i].m_fX;
        cout<<"\t"<<inputPlotInfo.m_vec_DataPts[i].m_fX_Err;
        cout<<"\t"<<inputPlotInfo.m_vec_DataPts[i].m_fY;
        cout<<"\t"<<inputPlotInfo.m_vec_DataPts[i].m_fY_Err<<endl;
    } //End Loop Over Data Points
    
    cout<<"\t--------------------End Plot--------------------\n";
    
    return;
} //End printInfoPlot()

void printInfoCanv(InfoCanvas & inputCanvInfo){
    cout<<"====================Begin Canvas====================\n";
    
    //Log scale
    cout<<"canvSetup.m_bLog_X = " << inputCanvInfo.m_bLog_X << endl;
    cout<<"canvSetup.m_bLog_Y = " << inputCanvInfo.m_bLog_Y << endl;
    
    //Grid
    cout<<"canvSetup.m_bGrid_X = " << inputCanvInfo.m_bGrid_X << endl;
    cout<<"canvSetup.m_bGrid_Y = " << inputCanvInfo.m_bGrid_Y << endl;
    
    //Range - X
    cout<<"canvSetup.m_fXAxis_Min = " << inputCanvInfo.m_fXAxis_Min << endl;
    cout<<"canvSetup.m_fXAxis_Max = " << inputCanvInfo.m_fXAxis_Max << endl;
    
    //Range - Y
    cout<<"canvSetup.m_fYAxis_Min = " << inputCanvInfo.m_fYAxis_Min << endl;
    cout<<"canvSetup.m_fYAxis_Max = " << inputCanvInfo.m_fYAxis_Max << endl;

    //Size
    cout<<"canvSetup.m_iSize_X = " << inputCanvInfo.m_iSize_X << endl;
    cout<<"canvSetup.m_iSize_Y = " << inputCanvInfo.m_iSize_Y << endl;
    
    //Legend Position
    cout<<"canvSetup.m_fLegNDCPos_X1 = " << inputCanvInfo.m_fLegNDCPos_X1 << endl;
    cout<<"canvSetup.m_fLegNDCPos_X2 = " << inputCanvInfo.m_fLegNDCPos_X2 << endl;
    cout<<"canvSetup.m_fLegNDCPos_Y1 = " << inputCanvInfo.m_fLegNDCPos_Y1 << endl;
    cout<<"canvSetup.m_fLegNDCPos_Y2 = " << inputCanvInfo.m_fLegNDCPos_Y2 << endl;
    
    //Margins
    cout<<"canvSetup.m_fMargin_Bot = " << inputCanvInfo.m_fMargin_Bot << endl;
    cout<<"canvSetup.m_fMargin_Lf = " << inputCanvInfo.m_fMargin_Lf << endl;
    cout<<"canvSetup.m_fMargin_Rt = " << inputCanvInfo.m_fMargin_Rt << endl;
    cout<<"canvSetup.m_fMargin_Top = " << inputCanvInfo.m_fMargin_Top << endl;

    //Latex Lines
    cout<<"Tex_X\tTex_Y\tString\n";
    for (auto iterTexLine = inputCanvInfo.m_vec_LatexNPos.begin(); iterTexLine != inputCanvInfo.m_vec_LatexNPos.end(); ++iterTexLine) {
        cout<<get<0>(*iterTexLine)<<"\t"<<get<1>(*iterTexLine)<<"\t"<<get<2>(*iterTexLine)<<endl;
    } //End Loop Over Defined Latex Lines
    
    //Draw Option
    cout<<"canvSetup.m_strOptionDraw = " << inputCanvInfo.m_strOptionDraw << endl;

    //Name
    cout<<"canvSetup.m_strName = " << inputCanvInfo.m_strName << endl;
    
    //Title
    cout<<"canvSetup.m_strTitle = " << inputCanvInfo.m_strTitle << endl;
    cout<<"canvSetup.m_strTitle_X = " << inputCanvInfo.m_strTitle_X << endl;
    cout<<"canvSetup.m_strTitle_Y = " << inputCanvInfo.m_strTitle_Y << endl;

    //Defined Plots
    for (auto iterPlot = inputCanvInfo.m_map_infoPlot.begin(); iterPlot != inputCanvInfo.m_map_infoPlot.end(); ++iterPlot) {
        printInfoPlot( (*iterPlot).second );
    }
    
    cout<<"====================End Canvas====================\n";
    
    return;
} //End printInfoCanv()

int main( int argc_, char * argv_[] ){
    //Transfer Input Arguments into vec_strInputArgs
    //------------------------------------------------------
    vector<string> vec_strInputArgs;
    vec_strInputArgs.resize(argc_);
    std::copy(argv_, argv_ + argc_, vec_strInputArgs.begin() );

    //Check input Arguments
    //------------------------------------------------------
    bool bVerboseMode = false;
    
    std::ifstream file_Config;
    
    ParameterLoaderPlotter loaderPlots;
    
    if(vec_strInputArgs.size() == 1 ){ //Case: Usage
        printHelpMenu();
        
        return 1;
    } //End Case: Usage
    else if (vec_strInputArgs.size() == 2 && vec_strInputArgs[1].compare("-h") == 0) { //Case: Help Menu
        printHelpMenu();
        
        return 0;
    } //End Case: Help Menu
    else if(vec_strInputArgs.size() == 3){ //Case: Analysis!
        bool bExitSuccess = false;
        
        //Set the verbose mode
        bVerboseMode = convert2bool(vec_strInputArgs[2], bExitSuccess);
        if (!bExitSuccess) { //Case: Input Not Understood
            cout<<"main() - vec_strInputArgs[3] expected to be boolean!!!\n";
            cout<<"main() - Parameter given:\n";
            cout<<"\t"<<vec_strInputArgs[2]<<endl;
            cout<<"main(): for help menu call: ./analyzeUniformity -h\n";
            cout<<"main(): exitting\n";
            
            return -2;
        } //End Case: Input Not Understood
        
        //Load the Input Config File
        //------------------------------------------------------
        loaderPlots.setFileStream( vec_strInputArgs[1], file_Config, bVerboseMode );
        
        //Check to see if the config file opened successfully
        if (!file_Config.is_open()) {
            perror( ("main() - error while opening file: " + vec_strInputArgs[1]).c_str() );
            printStreamStatus(file_Config);
            cout<<"main(): for help menu call: ./analyzeUniformity -h\n";
            cout<<"main(): exitting\n";
            
            return -1;
        } //End Case: Input Not Understood
    } //End Case: Analysis!
    else{ //Case: Input Not Understood
        cout<<"main(): I was expecting either 2 or 3 input parameters.  But you provided:\n";
        
        for (int i=0; i < vec_strInputArgs.size(); ++i) { //Loop over input parameters
            cout<<"\t"<<vec_strInputArgs[i]<<endl;
        } //End Loop over input parameters
        
        cout<<"main(): for help menu call: ./analyzeUniformity -h\n";
        cout<<"main(): exiting\n";
        
        return -3;
    } //End Case: Input Not Understood
    
    //Load the canvas info & print stored info if requested
    //------------------------------------------------------
    InfoCanvas canvSetup;
    loaderPlots.loadParameters(file_Config, bVerboseMode, canvSetup);
    if (bVerboseMode) { printInfoCanv(canvSetup); }
    
    
    //Determine the Plot Type, Setup the Plotter, and Plot
    //------------------------------------------------------
    PlotTypesPlotter typePlot;
    if ( 0 == canvSetup.m_strPlotType.compare( typePlot.m_strGraph ) ) {
        PlotterGraph graphPlotter;
        graphPlotter.setCanvasParameters(canvSetup);
        graphPlotter.setLogoPos(canvSetup.m_iLogoPos);
        graphPlotter.setPreliminary(canvSetup.m_bIsPrelim);
        graphPlotter.setOutputNameNOption("plotterOutput.root","RECREATE");
        graphPlotter.plotAndStore();
    }
    else if ( 0 == canvSetup.m_strPlotType.compare( typePlot.m_strGraph2D ) ) {
        PlotterGraph2D graphPlotter2D;
        graphPlotter2D.setCanvasParameters(canvSetup);
        graphPlotter2D.setLogoPos(canvSetup.m_iLogoPos);
        graphPlotter2D.setPreliminary(canvSetup.m_bIsPrelim);
        graphPlotter2D.setOutputNameNOption("plotterOutput.root","RECREATE");
        graphPlotter2D.plotAndStore();
    }
    if ( 0 == canvSetup.m_strPlotType.compare( typePlot.m_strGraphErrors ) ) {
        PlotterGraphErrors graphPlotterErr;
        graphPlotterErr.setCanvasParameters(canvSetup);
        graphPlotterErr.setLogoPos(canvSetup.m_iLogoPos);
        graphPlotterErr.setPreliminary(canvSetup.m_bIsPrelim);
        graphPlotterErr.setOutputNameNOption("plotterOutput.root","RECREATE");
        graphPlotterErr.plotAndStore();
    }
    else if ( 0 == canvSetup.m_strPlotType.compare( typePlot.m_strHisto ) ) {
        PlotterHisto histoPlotter;
        histoPlotter.setCanvasParameters(canvSetup);
        histoPlotter.setLogoPos(canvSetup.m_iLogoPos);
        histoPlotter.setPreliminary(canvSetup.m_bIsPrelim);
        histoPlotter.setOutputNameNOption("plotterOutput.root","RECREATE");
        histoPlotter.plotAndStore();
    }
    else if ( 0 == canvSetup.m_strPlotType.compare( typePlot.m_strHisto2D ) ) {
        PlotterHisto2D histoPlotter2D;
        histoPlotter2D.setCanvasParameters(canvSetup);
        histoPlotter2D.setLogoPos(canvSetup.m_iLogoPos);
        histoPlotter2D.setPreliminary(canvSetup.m_bIsPrelim);
        histoPlotter2D.setOutputNameNOption("plotterOutput.root","RECREATE");
        histoPlotter2D.plotAndStore();
    }
    else{
        cout<<"main() - Plot type: " << canvSetup.m_strPlotType << endl;
        cout<<"\tNot Recognized\n";
        cout<<"\tPlease cross-check " << vec_strInputArgs[1].c_str() << " and try again\n";
        cout<<"\tExitting!\n";
        
        return -4;
    }
    
    cout<<"Finished\n";
    
    return 0;
} //End main()
