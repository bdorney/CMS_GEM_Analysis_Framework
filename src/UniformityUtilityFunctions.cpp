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

using namespace Uniformity;

//Math
//==========================================

std::tuple<double, double, double> Uniformity::addTuple(const std::tuple<double, double, double> & a, const std::tuple<double, double, double> & b) { return a+b; };

//Printers
//==========================================

//Prints a message to the user indicating class/method template: "charClass::charMethod() - charMsg"
void Uniformity::printClassMethodMsg(const char charClass[], const char charMethod[], const char charMsg[]){
    //cout<< (strClass + "::" + strMethod + "() - " + strMsg ).c_str() << endl;
    cout<< charClass << "::" << charMethod << "() - " << charMsg << endl;
    
    return;
} //End Uniformity::printClassMethodMsg

//String manipulation
//==========================================

//strips alphabetical characters from the input string leaving only numbers
//There has to be a better way to do the below O_o
string Uniformity::getStringOnlyNumeric(string strInput){
    //Variable Declaration
    string strTemp = strInput;  //store the original
    
    //remove all alphabetical characters from the input string
    strTemp.erase(std::remove_if(strTemp.begin(),strTemp.end(), [](char c){ return !std::isalpha(c); } ) );
    
    //determine when in the original string the first non-
    int iPosLastAlpha = strInput.find_first_not_of(strTemp);
    
    strInput.erase(strInput.begin(),strInput.begin()+iPosLastAlpha);
    
    return strInput;
}
