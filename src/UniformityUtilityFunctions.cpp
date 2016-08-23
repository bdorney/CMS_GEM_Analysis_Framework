//
//  UniformityUtilityFunctions.cpp
//  
//
//  Created by Brian L Dorney on 25/01/16.
//
//

//C++ Includes

//My Includes
#include "UniformityUtilityFunctions.h"

//ROOT Includes

using std::cout;
using std::endl;
using std::string;

using QualityControl::Timing::getString;

using namespace QualityControl::Uniformity;

//Math
//==========================================

std::tuple<double, double, double> QualityControl::Uniformity::addTuple(const std::tuple<double, double, double> & a, const std::tuple<double, double, double> & b) { return a+b; };

std::pair<int,int> QualityControl::Uniformity::getEventRange(int iFirstEvt, int iNEvt, int iNTreeEntries){
    //Variable Declaration
    std::pair<int,int> pair_iRange = std::make_pair(1,0);
    
    if ( -1 == iNEvt ) { //Case: All Events
        iFirstEvt = 0;
        iNEvt = iNTreeEntries;
    } //End Case: All Events
    else{ //Case: Event Range
        if ( iFirstEvt > iNTreeEntries ) { //Case: Incorrect Event Range, 1st Event Requested Beyond All Events
            printClassMethodMsg("UniformityUtilityFunctions.cpp","getEventRange", ("Error, First Event Requested as " + getString( iFirstEvt ) + " Greater Thant Total Number of Events " + getString( iNTreeEntries ) ).c_str() );
            printClassMethodMsg("UniformityUtilityFunctions.cpp","getEventRange", "Exiting!!!");
            
            return pair_iRange;
        } //End Case: Incorrect Event Range, 1st Event Requested Beyond All Events
        else if( (iFirstEvt + iNEvt) > iNTreeEntries ){
            iNEvt = iNTreeEntries - iFirstEvt;
        }
        else if( iFirstEvt < 0){
            iFirstEvt = 0;
        }
    } //End Case: Event Range
    
    pair_iRange.first = iFirstEvt;
    pair_iRange.second = iNEvt;
    
    return pair_iRange;
} //End HandlerEvent::getEventRange();

//Printers
//==========================================

//Prints a message to the user indicating class/method template: "charClass::charMethod() - charMsg"
void QualityControl::Uniformity::printClassMethodMsg(const char charClass[], const char charMethod[], const char charMsg[]){
    //cout<< (strClass + "::" + strMethod + "() - " + strMsg ).c_str() << endl;
    cout<< charClass << "::" << charMethod << "() - " << charMsg << endl;
    
    return;
} //End Uniformity::printClassMethodMsg

//String manipulation
//==========================================

//strips alphabetical characters from the input string leaving only numbers
//There has to be a better way to do the below O_o
string QualityControl::Uniformity::getStringOnlyNumeric(string strInput){
    //Variable Declaration
    string strTemp = strInput;  //store the original
    
    //remove all alphabetical characters from the input string
    strTemp.erase(std::remove_if(strTemp.begin(),strTemp.end(), [](char c){ return !std::isalpha(c); } ) );
    
    //determine when in the original string the first non-
    int iPosLastAlpha = strInput.find_first_not_of(strTemp);
    
    strInput.erase(strInput.begin(),strInput.begin()+iPosLastAlpha);
    
    return strInput;
} //End QualityControl::Uniformity::getStringOnlyNumeric

//Needs more work to be exception safe, right now if strSub1 is at the END of the string a problem might occur
/*void QualityControl::replaceSubStr1WithSubStr2(std::string &strInput, std::string strSub1, std::string strSub2){
    
    int iPosStrSub1 = strInput.find(strSub1);
    
    strInput.erase(strInput.find(strSub1), strInput.length() - strInput.find(strSub1) );
    strInput.insert(iPosStrSub1+1, strSub2);
    //strInput = strInput + strSub2;
    
    return;
} //End QualityControl::replaceSubStr1WithSubStr2()*/