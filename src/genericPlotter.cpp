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
#include "PlotterUtilityTypes.h"

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
    
    //Range - X
    cout<<"\tplotSetup.m_fXAxis_Min = " << inputPlotInfo.m_fXAxis_Min << endl;
    cout<<"\tplotSetup.m_fXAxis_Max = " << inputPlotInfo.m_fXAxis_Max << endl;
    
    //Range - Y
    cout<<"\tplotSetup.m_fYAxis_Min = " << inputPlotInfo.m_fYAxis_Min << endl;
    cout<<"\tplotSetup.m_fYAxis_Max = " << inputPlotInfo.m_fYAxis_Max << endl;
    
    //Legend Entry
    cout<<"\tplotSetup.m_strLegEntry = " << inputPlotInfo.m_strLegEntry << endl;
    
    //Name
    cout<<"\tplotSetup.m_strName = " << inputPlotInfo.m_strName << endl;
    
    //Draw Option
    cout<<"\tplotSetup.m_strOptionDraw = " << inputPlotInfo.m_strOptionDraw << endl;
    
    //Titles
    cout<<"\tplotSetup.m_strTitle_X = " << inputPlotInfo.m_strTitle_X << endl;
    cout<<"\tplotSetup.m_strTitle_Y = " << inputPlotInfo.m_strTitle_Y << endl;
    
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
    
    //Size
    cout<<"canvSetup.m_iSize_X = " << inputCanvInfo.m_iSize_X << endl;
    cout<<"canvSetup.m_iSize_Y = " << inputCanvInfo.m_iSize_Y << endl;
    
    //Legend Position
    cout<<"canvSetup.m_fLegNDCPos_X1 = " << inputCanvInfo.m_fLegNDCPos_X1 << endl;
    cout<<"canvSetup.m_fLegNDCPos_X2 = " << inputCanvInfo.m_fLegNDCPos_X2 << endl;
    cout<<"canvSetup.m_fLegNDCPos_Y1 = " << inputCanvInfo.m_fLegNDCPos_Y1 << endl;
    cout<<"canvSetup.m_fLegNDCPos_Y2 = " << inputCanvInfo.m_fLegNDCPos_Y2 << endl;
    
    //Latex Lines
    cout<<"Tex_X\tTex_Y\tString\n";
    for (auto iterTexLine = inputCanvInfo.m_vec_LatexNPos.begin(); iterTexLine != inputCanvInfo.m_vec_LatexNPos.end(); ++iterTexLine) {
        cout<<get<0>(*iterTexLine)<<"\t"<<get<1>(*iterTexLine)<<"\t"<<get<2>(*iterTexLine)<<endl;
    } //End Loop Over Defined Latex Lines
    
    //Name
    cout<<"canvSetup.m_strName = " << inputCanvInfo.m_strName << endl;
    
    //Title
    cout<<"canvSetup.m_strTitle = " << inputCanvInfo.m_strTitle << endl;
    
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
    
    InfoCanvas canvSetup;
    loaderPlots.loadParameters(file_Config, bVerboseMode, canvSetup);
    
    printInfoCanv(canvSetup);
    
    return 0;
} //End main()
