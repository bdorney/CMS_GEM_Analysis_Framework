//
//  PlotterUtilityFunctions.cpp
//  
//
//  Created by Brian L Dorney on 05/10/16.
//
//

#include "PlotterUtilityFunctions.h"

using std::cout;
using std::endl;
using std::stoi;
using std::string;
using std::transform;

//using namespace QualityControl::Plotter;

//Converts an input expression into a EColor value defined in Rtypes.h of ROOT
int QualityControl::Plotter::convert2Color(string str){
    //Variable Declaration
    int ret_Color = kBlack;
    
    int iPos_Operator = std::min(str.find("+",0),str.find("-",0) );
    
    //Three Cases
    if (str.find_first_not_of( "0123456789" ) == string::npos) { //Case: Only Numeric Input
        ret_Color = stoi(str);
    } //End Case: Only Numeric Input
    else if( iPos_Operator == string::npos ) { //Case: Only kColor Input
        //Transform to upper case to be case-insensitive
        transform(str.begin(), str.end(), str.begin(), toupper);
        
        //Evaluate Color
        if (str.compare("KWHITE") == 0) {       ret_Color = kWhite;}
        else if(str.compare("KBLACK") == 0) {   ret_Color = kBlack;}
        else if(str.compare("KGRAY") == 0) {    ret_Color = kGray;}
        else if(str.compare("KRED") == 0) {     ret_Color = kRed;}
        else if(str.compare("KGREEN") == 0) {   ret_Color = kGreen;}
        else if(str.compare("KBLUE") == 0) {    ret_Color = kBlue;}
        else if(str.compare("KYELLOW") == 0) {  ret_Color = kYellow;}
        else if(str.compare("KMAGENTA") == 0) { ret_Color = kMagenta;}
        else if(str.compare("KCYAN") == 0) {    ret_Color = kCyan;}
        else if(str.compare("KORANGE") == 0) {  ret_Color = kOrange;}
        else if(str.compare("KSPRING") == 0) {  ret_Color = kSpring;}
        else if(str.compare("KTEAL") == 0) {    ret_Color = kTeal;}
        else if(str.compare("KAZURE") == 0) {   ret_Color = kAzure;}
        else if(str.compare("KVIOLET") == 0) {  ret_Color = kViolet;}
        else if(str.compare("KPINK") == 0) {    ret_Color = kPink;}
        else{ //Case: Color Not Recognized
            cout<<"QualityControl::Plotter::convert2Color() - Input color "<< str << " not recognized; setting to kBlack!!!"<<endl;
            return ret_Color;
        } //End Case: Color Not Recognized
        
    } //End Case: Only kColor Input
    else{ //Case: Full Expression
        //Find Color
        //string strColor = str.substr(0,iPos_Operator-1);
        string strColor = str.substr(0,iPos_Operator);
        
        //Transform to upper case to be case-insensitive
        transform(strColor.begin(), strColor.end(), strColor.begin(), toupper);
        
        //Evaluate Color
        if (strColor.compare("KWHITE") == 0) {       ret_Color = kWhite;}
        else if(strColor.compare("KBLACK") == 0) {   ret_Color = kBlack;}
        else if(strColor.compare("KGRAY") == 0) {    ret_Color = kGray;}
        else if(strColor.compare("KRED") == 0) {     ret_Color = kRed;}
        else if(strColor.compare("KGREEN") == 0) {   ret_Color = kGreen;}
        else if(strColor.compare("KBLUE") == 0) {    ret_Color = kBlue;}
        else if(strColor.compare("KYELLOW") == 0) {  ret_Color = kYellow;}
        else if(strColor.compare("KMAGENTA") == 0) { ret_Color = kMagenta;}
        else if(strColor.compare("KCYAN") == 0) {    ret_Color = kCyan;}
        else if(strColor.compare("KORANGE") == 0) {  ret_Color = kOrange;}
        else if(strColor.compare("KSPRING") == 0) {  ret_Color = kSpring;}
        else if(strColor.compare("KTEAL") == 0) {    ret_Color = kTeal;}
        else if(strColor.compare("KAZURE") == 0) {   ret_Color = kAzure;}
        else if(strColor.compare("KVIOLET") == 0) {  ret_Color = kViolet;}
        else if(strColor.compare("KPINK") == 0) {    ret_Color = kPink;}
        else{ //Case: Color Not Recognized
            cout<<"QualityControl::Plotter::convert2Color() - Input color "<< strColor << " not recognized; setting to kBlack!!!"<<endl;
            return ret_Color;
        } //End Case: Color Not Recognized
        
        //Debugging
        //cout<<"ret_Color = " << ret_Color << endl;
        
        //Increment the kColor by the desired amount
        if( (str.substr(iPos_Operator,1)).compare("+") == 0 ){ //Case: Increase kColor
            ret_Color = ret_Color + stoi( str.substr( iPos_Operator+1, str.length()-iPos_Operator-1 ) );
        } //End Case: Increase kColor
        else{ //Case: Decrease kColor
            ret_Color = ret_Color - stoi( str.substr( iPos_Operator+1, str.length()-iPos_Operator-1 ) );
        } //End Case: Decrease kColor
        
        //Debugging
        //cout<<"ret_Color = " << ret_Color << endl;
    } //End Case: Full Expression
    
    return ret_Color;
} //End convert2Color()