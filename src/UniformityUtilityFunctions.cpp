//
//  UniformityUtilityFunctions.cpp
//  
//
//  Created by Brian L Dorney on 25/01/16.
//
//

//C++ Includes
#include <iostream>

//My Includes
#include "UniformityUtilityFunctions.h"

//ROOT Includes

using std::cout;
using std::endl;
using std::string;

using namespace Uniformity;

//Printers
//==========================================
//void Uniformity::printClassMethodMsg(string &strClass, string &strMethod, string &strMsg ){
void Uniformity::printClassMethodMsg(const char charClass[], const char charMethod[], const char charMsg[]){
    //cout<< (strClass + "::" + strMethod + "() - " + strMsg ).c_str() << endl;
    cout<< charClass << "::" << charMethod << "() - " << charMsg << endl;
    
    return;
} //End Uniformity::printClassMethodMsg

//String manipulation
//==========================================
string Uniformity::getStringOnlyNumeric(string strInput){
    strInput.erase(std::remove_if(strInput.begin(),strInput.end(), [](char c){ return !std::isalpha(c); } ) );
    
    return strInput;
}
