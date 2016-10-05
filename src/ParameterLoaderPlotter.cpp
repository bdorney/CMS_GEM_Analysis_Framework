//
//  ParameterLoaderPlotter.cpp
//  
//
//  Created by Brian L Dorney on 03/10/16.
//
//

#include "ParameterLoaderPlotter.h"

using std::cout;
using std::endl;
using std::find;
using std::make_tuple;
using std::stoi;    //Maybe Timing::stoiSafe?
using std::streampos;
using std::string;
using std::vector;

using QualityControl::Timing::convert2bool;
using QualityControl::Timing::getCharSeparatedList;
using QualityControl::Timing::getlineNoSpaces;
using QualityControl::Timing::getParsedLine;
using QualityControl::Timing::printStreamStatus;

using QualityControl::Uniformity::printClassMethodMsg;

using namespace QualityControl::Plotter;

ParameterLoaderPlotter::ParameterLoaderPlotter(){
    m_bVerboseMode_IO = false;
}

void ParameterLoaderPlotter::loadParameters(std::ifstream &file_Input, bool bVerboseMode, InfoCanvas &inputCanvInfo){
    //Set Defaults
    //------------------------------------------------------
    m_bVerboseMode_IO = bVerboseMode;
    loadParametersCanvas(file_Input, inputCanvInfo);
    
    ////Loop Over data Input File
    //------------------------------------------------------
    //Read the file via std::getline().  Obey good coding practice rules:
    //  -first the I/O operation, then error check, then data processing
    //  -failbit and badbit prevent data processing, eofbit does not
    //See: http://gehrcke.de/2011/06/reading-files-in-c-using-ifstream-dealing-correctly-with-badbit-failbit-eofbit-and-perror/
    /*string strLine = "";
    while ( getlineNoSpaces(file_Input, strLine) ) {
        //Does the user want to comment out this line?
        if ( 0 == strLine.compare(0,1,"#") ) continue;
        
        //Identify Section Headers
        if ( 0 == strLine.compare(m_headers_plots.m_strSecEnd_Canv ) ) { //Case: Reached End of Interest
            break;
        } //End Case: Reached End of Interest
        else if ( 0 == strLine.compare(m_headers_plots.m_strSecBegin_Canv ) ){ //Case: Run Info Header
            loadParametersCanvas(file_Input, inputCanvInfo);
            continue;
        } //End Case: Run Info Header
        else { //Case: Unsorted Parameters
            
            //Place holder
            
        } //End Case: Unsorted Parameters
    } //End Loop over input file
    
    //Check to see if we had problems while reading the file
    if (file_Input.bad() && bVerboseMode) {
        //perror( ("Uniformity::ParameterLoaderPlotter::loadParameters(): error while reading file: " + strInputSetupFile).c_str() );
        perror( "Plotter::ParameterLoaderPlotter::loadParameters(): error while reading file" );
        printStreamStatus(file_Input);
    }*/
    
    //Do not close input file, it will be used elsewhere
    
    //inputRunSetup.bLoadSuccess = true;
    
    //cout<<"ParameterLoaderPlotter::loadParameters() - strLine = " << strLine << endl;
    
    return;
} //End ParameterLoaderPlotter::loadParameters()

void ParameterLoaderPlotter::loadParametersCanvas(std::ifstream & file_Input, InfoCanvas &inputCanvInfo){
    /*if (m_bVerboseMode_IO) { //Case: User Requested Verbose Error Messages - I/O
        printClassMethodMsg("ParameterLoaderPlotter","loadParametersCanvas", "Found Canvas Heading");
    }*/ //End Case: User Requested Verbose Error Messages - I/O
    
    //Set Defaults
    //------------------------------------------------------
    //Placeholdder
    
    //Loop through input file
    //Check for faults immediately afterward
    //------------------------------------------------------
    bool bExitSuccess = false;
    
    std::pair<string,string> pair_strParam;
    
    //streampos spos_Previous; //previous stream position
    
    string strLine = "";
    
    vector<string> vec_strCommaSepList;
    while ( getlineNoSpaces(file_Input, strLine) ) { //Loop through input file
        //Skip commented lines
        if (strLine.compare(0,1,"#") == 0) continue;
        
        //Do we reach the end of the section or anther section?
	if ( 0 == strLine.compare(m_headers_plots.m_strSecBegin_Canv ) ){ //Case: Run Info Header
            if (m_bVerboseMode_IO) { //Case: User Requested Verbose Input/Output
                cout<<"ParameterLoaderPlotter::loadParametersCanvas(): Start of canvas header reached!\n";
            } //End Case: User Requested Verbose Input/Output

            continue;
        } //End Case: Run Info Header
        else if ( 0 == strLine.compare( m_headers_plots.m_strSecEnd_Canv ) ) { //Case: End of Canvas Section
            if (m_bVerboseMode_IO) { //Case: User Requested Verbose Input/Output
                cout<<"ParameterLoaderPlotter::loadParametersCanvas(): End of canvas header reached!\n";
            } //End Case: User Requested Verbose Input/Output
            
            //file_Input.seekg(spos_Previous);
            break;
        } //End Case: End of Canvas Section
        else if ( 0 == strLine.compare( m_headers_plots.m_strSecBegin_Plot ) ){ //Case: Start of Plot Section
            InfoPlot plotInfo;
            loadParametersPlot(file_Input, plotInfo);
            inputCanvInfo.m_map_infoPlot[plotInfo.m_strName]=plotInfo;

            continue;
        } //End Case: Start of Plot Section
        
	//Debugging
	//cout<<"strLine = " << strLine.c_str() << endl;

        //Parse the line
        pair_strParam = getParsedLine(strLine, bExitSuccess);
        
	//Debugging
	//cout<<"pair_strParam.first = " << pair_strParam.first.c_str() << endl;

        if (bExitSuccess) { //Case: Parameter Fetched Successfully
            //Transform input field name to all capitals for case-insensitive string comparison
            string strTmp = pair_strParam.first;
            std::transform(strTmp.begin(), strTmp.end(), strTmp.begin(), toupper);
            
            pair_strParam.first = strTmp;
            
            if ( pair_strParam.first.compare("CANV_DIM") == 0 ) {
                //Get the comma separated list
                vec_strCommaSepList = getCharSeparatedList( pair_strParam.second, ',' );
                
                //Set the dimensions
                setParameters(vec_strCommaSepList, inputCanvInfo.m_iSize_X, inputCanvInfo.m_iSize_Y);
            }
            else if ( pair_strParam.first.compare("CANV_GRID_XY") == 0 ) {
                //Get the comma separated list
                vec_strCommaSepList = getCharSeparatedList( pair_strParam.second, ',' );
                
                //Set the grid case
                setParameters(vec_strCommaSepList, inputCanvInfo.m_bGrid_X, inputCanvInfo.m_bGrid_Y);
            }
            else if ( pair_strParam.first.compare("CANV_LATEX_LINE") == 0 ) {
                //Get the comma separated list
                vec_strCommaSepList = getCharSeparatedList( pair_strParam.second, ',' );
                
                //Setup the tuple
                if (vec_strCommaSepList.size() == 3) {
                    inputCanvInfo.m_vec_LatexNPos.push_back( std::make_tuple( stof(vec_strCommaSepList[0]), stof(vec_strCommaSepList[1]), vec_strCommaSepList[2]) );
                }
            }
            else if ( pair_strParam.first.compare("CANV_LEGEND_DIM_X") == 0 ) {
                //Get the comma separated list
                vec_strCommaSepList = getCharSeparatedList( pair_strParam.second, ',' );
                
                //Set the dimensions
                setParameters(vec_strCommaSepList, inputCanvInfo.m_fLegNDCPos_X1, inputCanvInfo.m_fLegNDCPos_X2);
            }
            else if ( pair_strParam.first.compare("CANV_LEGEND_DIM_Y") == 0 ) {
                //Get the comma separated list
                vec_strCommaSepList = getCharSeparatedList( pair_strParam.second, ',' );
                
                //Set the dimensions
                setParameters(vec_strCommaSepList, inputCanvInfo.m_fLegNDCPos_Y1, inputCanvInfo.m_fLegNDCPos_Y2);
            }
            else if ( pair_strParam.first.compare("CANV_LOG_XY") == 0 ) {
                //Get the comma separated list
                vec_strCommaSepList = getCharSeparatedList( pair_strParam.second, ',' );
                
                //Set the grid case
                setParameters(vec_strCommaSepList, inputCanvInfo.m_bLog_X, inputCanvInfo.m_bLog_Y);
            }
            else if ( pair_strParam.first.compare("CANV_NAME") == 0 ) {
                inputCanvInfo.m_strName = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("CANV_N_AXIS_X") == 0 ) {
                
                //Placeholder
                
            }
            else if ( pair_strParam.first.compare("CANV_N_AXIS_Y") == 0 ) {
                
                //Placeholder
                
            }
            else{ //Case: Parameter Not Recognized
                cout<<"ParameterLoaderPlotter::loadParametersCanvas(): input field name:\n";
                cout<<"\t"<<pair_strParam.first<<endl;
                cout<<"ParameterLoaderPlotter::loadParametersCanvas(): not recognized! Please cross-check input file!!!\n";
            } //End Case: Parameter Not Recognized
        } //End Case: Parameter Fetched Successfully
        else{ //Case: Input line did NOT parse correctly
            cout<<"ParameterLoaderPlotter::loadParametersCanvas(): input line:\n";
            cout<<"\t"<<strLine<<endl;
            cout<<"ParameterLoaderPlotter::loadParametersCanvas(): did not parse correctly, please cross-check input file\n";
        } //End Case: Input line did NOT parse correctly
        
        //Store previous stream position so main loop over file exits
        //After finding the end header we will return file_Input to the previous stream position so loadParameters loop will exit properly
        //spos_Previous = file_Input.tellg();
    } //End Loop through input file
    if ( file_Input.bad() && m_bVerboseMode_IO) {
        perror( "ParameterLoaderPlotter::loadParametersCanvas(): error while reading config file" );
        printStreamStatus(file_Input);
    }
    
    return;
} //End ParameterLoaderPlotter::loadParametersCanvas()

void ParameterLoaderPlotter::loadParametersPlot(std::ifstream & file_Input, InfoPlot & inputPlotInfo){
    if (m_bVerboseMode_IO) { //Case: User Requested Verbose Error Messages - I/O
        printClassMethodMsg("ParameterLoaderPlotter","loadParametersPlot", "Found Plot Heading");
    } //End Case: User Requested Verbose Error Messages - I/O
    
    //Set Defaults
    //------------------------------------------------------
    //Placeholdder
    
    //Loop through input file
    //Check for faults immediately afterward
    //------------------------------------------------------
    bool bExitSuccess = false;
    
    std::pair<string,string> pair_strParam;
    
    //streampos spos_Previous; //previous stream position
    
    string strLine = "";
    
    vector<string> vec_strCommaSepList;
    while ( getlineNoSpaces(file_Input, strLine) ) { //Loop through input file
        //Skip commented lines
        if (strLine.compare(0,1,"#") == 0) continue;
        
        //Do we reach the end of the section or anther section?
        if ( 0 == strLine.compare( m_headers_plots.m_strSecEnd_Plot ) ) { //Case: End of Canvas Section
            if (m_bVerboseMode_IO) { //Case: User Requested Verbose Input/Output
                cout<<"ParameterLoaderPlotter::loadParametersPlot(): End of plot header reached!\n";
            } //End Case: User Requested Verbose Input/Output
            
            //file_Input.seekg(spos_Previous);
            break;
        } //End Case: End of Canvas Section
        else if ( 0 == strLine.compare( m_headers_plots.m_strSecBegin_Data ) ){ //Case: Start of Plot Section
            inputPlotInfo.m_vec_DataPts = loadData(file_Input);
            continue;
        } //End Case: Start of Plot Section
        
        //Parse the line
        pair_strParam = getParsedLine(strLine, bExitSuccess);
        
        if (bExitSuccess) { //Case: Parameter Fetched Successfully
            //Transform input field name to all capitals for case-insensitive string comparison
            string strTmp = pair_strParam.first;
            std::transform(strTmp.begin(), strTmp.end(), strTmp.begin(), toupper);
            
            pair_strParam.first = strTmp;
            
            if ( pair_strParam.first.compare("PLOT_COLOR") == 0 ) {
                inputPlotInfo.m_iColor = convert2Color(pair_strParam.second);
            }
            else if ( pair_strParam.first.compare("PLOT_DRAWOPT") == 0 ) {
                inputPlotInfo.m_strOptionDraw = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("PLOT_LEGENTRY") == 0 ) {
                inputPlotInfo.m_strLegEntry = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("PLOT_LINE_SIZE") == 0 ) {
                inputPlotInfo.m_fSizeLine = stof(pair_strParam.second);
            }
            else if ( pair_strParam.first.compare("PLOT_LINE_STYLE") == 0 ) {
                inputPlotInfo.m_iStyleLine = stoi(pair_strParam.second);
            }
            else if ( pair_strParam.first.compare("PLOT_MARKER_SIZE") == 0 ) {
                inputPlotInfo.m_fSizeMarker = stof(pair_strParam.second);
            }
            else if ( pair_strParam.first.compare("PLOT_MARKER_STYLE") == 0 ) {
                inputPlotInfo.m_iStyleMarker = stoi(pair_strParam.second);
            }
            else if ( pair_strParam.first.compare("PLOT_NAME") == 0 ) {
                inputPlotInfo.m_strName = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("PLOT_RANGE_X") == 0 ) {
                //Get the comma separated list
                vec_strCommaSepList = getCharSeparatedList( pair_strParam.second, ',' );
                
                //Set the dimensions
                setParameters(vec_strCommaSepList, inputPlotInfo.m_fXAxis_Min, inputPlotInfo.m_fXAxis_Max);
                
                //Check max & min are set correctly
                if(inputPlotInfo.m_fXAxis_Max < inputPlotInfo.m_fXAxis_Min){
                    float fTemp = inputPlotInfo.m_fXAxis_Min;
                    inputPlotInfo.m_fXAxis_Min = inputPlotInfo.m_fXAxis_Max;
                    inputPlotInfo.m_fXAxis_Max = fTemp;
                }
                
                //Update the flag
                inputPlotInfo.m_bXAxis_UserRange = true;
            }
            else if ( pair_strParam.first.compare("PLOT_RANGE_Y") == 0 ) {
                //Get the comma separated list
                vec_strCommaSepList = getCharSeparatedList( pair_strParam.second, ',' );
                
                //Set the dimensions
                setParameters(vec_strCommaSepList, inputPlotInfo.m_fYAxis_Min, inputPlotInfo.m_fYAxis_Max);
                
                //Check max & min are set correctly
                if(inputPlotInfo.m_fYAxis_Max < inputPlotInfo.m_fYAxis_Min){
                    float fTemp = inputPlotInfo.m_fYAxis_Min;
                    inputPlotInfo.m_fYAxis_Min = inputPlotInfo.m_fYAxis_Max;
                    inputPlotInfo.m_fYAxis_Max = fTemp;
                }
                
                //Update the flag
                inputPlotInfo.m_bXAxis_UserRange = true;
            }
            else if ( pair_strParam.first.compare("PLOT_TITLE_X") == 0 ) {
                inputPlotInfo.m_strTitle_X = pair_strParam.second;
            }
            else if ( pair_strParam.first.compare("PLOT_TITLE_Y") == 0 ) {
                inputPlotInfo.m_strTitle_Y = pair_strParam.second;
            }
            else{ //Case: Parameter Not Recognized
                cout<<"ParameterLoaderPlotter::loadParametersPlot(): input field name:\n";
                cout<<"\t"<<pair_strParam.first<<endl;
                cout<<"ParameterLoaderPlotter::loadParametersPlot(): not recognized! Please cross-check input file!!!\n";
            } //End Case: Parameter Not Recognized
        } //End Case: Parameter Fetched Successfully
        else{ //Case: Input line did NOT parse correctly
            cout<<"ParameterLoaderPlotter::loadParametersPlot(): input line:\n";
            cout<<"\t"<<strLine<<endl;
            cout<<"ParameterLoaderPlotter::loadParametersPlot(): did not parse correctly, please cross-check input file\n";
        } //End Case: Input line did NOT parse correctly
        
        //Store previous stream position so main loop over file exits
        //After finding the end header we will return file_Input to the previous stream position so loadParameters loop will exit properly
        //spos_Previous = file_Input.tellg();
    } //End Loop through input file
    if ( file_Input.bad() && m_bVerboseMode_IO) {
        perror( "ParameterLoaderPlotter::loadParametersPlot(): error while reading config file" );
        printStreamStatus(file_Input);
    }
    
    //cout<<"ParameterLoaderPlotter::loadParametersPlot() - strLine = " << strLine << endl;
    
    return;
} //End ParameterLoaderPlotter::loadParametersPlots()

vector<DataPoint> ParameterLoaderPlotter::loadData(std::ifstream & file_Input){
    if (m_bVerboseMode_IO) { //Case: User Requested Verbose Error Messages - I/O
        printClassMethodMsg("ParameterLoaderPlotter","loadData", "Found Data Heading");
    } //End Case: User Requested Verbose Error Messages - I/O
    
    //Set Defaults
    //------------------------------------------------------
    //Placeholdder
    
    //Loop through input file
    //Check for faults immediately afterward
    //------------------------------------------------------
    bool bExitSuccess = false;
    bool bPosSet = false;
    
    DataPoint dataPt;
    
    int iPos_X = -1 , iPos_X_Err = -1;
    int iPos_Y = -1 , iPos_Y_Err = -1;
    
    //streampos spos_Previous; //previous stream position
    
    string strLine = "";
    
    vector<DataPoint> vec_retData;
    vector<string> vec_strDelimSepList;
    while ( getlineNoSpaces(file_Input, strLine) ) { //Loop through input file
	//Skip commented lines
        if (strLine.compare(0,1,"#") == 0) continue;
        
        //Do we reach the end of the section or anther section?
        if ( 0 == strLine.compare( m_headers_plots.m_strSecEnd_Data ) ) { //Case: End of Canvas Section
            if (m_bVerboseMode_IO) { //Case: User Requested Verbose Input/Output
                cout<<"ParameterLoaderPlotter::loadData(): End of data header reached!\n";
            } //End Case: User Requested Verbose Input/Output
            
            //file_Input.seekg(spos_Previous);
            break;
        } //End Case: End of Canvas Section
        
	//cout<<strLine<<endl;

	//The data is delimited, get a line
	vec_strDelimSepList = getCharSeparatedList(strLine, ',');
	//cout<<"vec_strDelimSepList.size() = " << vec_strDelimSepList.size() << endl;

	/*for(int i=0; i < vec_strDelimSepList.size(); ++i){
		cout<<vec_strDelimSepList[i]<<endl;
	}*/

        //Determine which columns have what meaning by reading in the column labels
        if (!bPosSet) {
            iPos_X = getColLabelPosition( vec_strDelimSepList, m_col_labels.m_strColX );
            iPos_Y = getColLabelPosition( vec_strDelimSepList, m_col_labels.m_strColY );
            iPos_X_Err = getColLabelPosition( vec_strDelimSepList, m_col_labels.m_strColX_Err );
            iPos_Y_Err = getColLabelPosition( vec_strDelimSepList, m_col_labels.m_strColY_Err );
            
            bPosSet = true;

            //Now move to next line
            //continue;
        }
	else{
        	if (iPos_X > -1) {      dataPt.m_fX = stof(vec_strDelimSepList[iPos_X]); }
        	if (iPos_Y > -1) {      dataPt.m_fY = stof(vec_strDelimSepList[iPos_Y]); }
        	if (iPos_X_Err > -1) {  dataPt.m_fX_Err = stof(vec_strDelimSepList[iPos_X_Err]); }
        	if (iPos_Y_Err > -1) {  dataPt.m_fY_Err = stof(vec_strDelimSepList[iPos_Y_Err]); }
        
        	vec_retData.push_back(dataPt);
		dataPt.clear();
	}
                
        //Store previous stream position so main loop over file exits
        //After finding the end header we will return file_Input to the previous stream position so loadParameters loop will exit properly
        //spos_Previous = file_Input.tellg();
    } //End Loop through input file
    if ( file_Input.bad() && m_bVerboseMode_IO) {
        perror( "ParameterLoaderPlotter::loadData(): error while reading config file" );
        printStreamStatus(file_Input);
    }
    
    return vec_retData;
} //End ParameterLoaderPlotter::loadData()

int ParameterLoaderPlotter::getColLabelPosition(std::vector<std::string> vec_strInputParam, std::string & strInputLabel){
    auto iterColLabel = find( vec_strInputParam.begin(), vec_strInputParam.end(), strInputLabel );
    if ( iterColLabel != vec_strInputParam.end() ) {
        return distance( vec_strInputParam.begin(), iterColLabel );
    }
    
    return -1;
} //End ParameterLoaderPlotter::getColLabelPosition()

//Sets two bool parameters
void ParameterLoaderPlotter::setParameters(std::vector<std::string> & vec_strInputParam, bool &bInput1, bool &bInput2){
    bool bExitSuccess;
    
    //Extract size from the list
    if (vec_strInputParam.size() >= 2) { //Case: User Input
        bInput1 = convert2bool(vec_strInputParam[0], bExitSuccess);
        bInput2 = convert2bool(vec_strInputParam[1], bExitSuccess);
    } //End Case: User Input
    
    return;
} //End setParameters() - boolean

//Sets two float parameters
void ParameterLoaderPlotter::setParameters(std::vector<std::string> & vec_strInputParam, float &fInput1, float &fInput2){
    
    //Extract size from the list
    if (vec_strInputParam.size() >= 2) { //Case: User Input
        fInput1 = stof(vec_strInputParam[0]);
        fInput2 = stof(vec_strInputParam[1]);
    } //End Case: User Input
    
    return;
} //End setParameters() - float

//Sets two int parameters
void ParameterLoaderPlotter::setParameters(std::vector<std::string> & vec_strInputParam, int &iInput1, int &iInput2){
    
    //Extract size from the list
    if (vec_strInputParam.size() >= 2) { //Case: User Input
        iInput1 = stoi(vec_strInputParam[0]);
        iInput2 = stoi(vec_strInputParam[1]);
    } //End Case: User Input
    
    return;
} //End setParameters() - float
