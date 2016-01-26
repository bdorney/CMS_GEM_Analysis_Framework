//
//  GainUniformityUtilities.h
//  
//
//  Created by Brian L Dorney on 25/01/16.
//
//

#ifndef _UniformityUtilityFunctions_h
#define _UniformityUtilityFunctions_h

//C++ Includes
#include <algorithm>
#include <stdio.h>
#include <sstream>
#include <string>

//My Includes

//ROOT Includes

//Description of the below:
/*
 
 printers:
    printClassMethodMsg() -> Prints a message to the user indicating class/method template: "charClass::charMethod() - charMsg"
 
 string manipulation:
    getStringOnlyNumeric() -> removes
 */

namespace Uniformity {
    //printers
    //void printClassMethodMsg(std::string &strClass, std::string &strMethod, std::string &strMessage);
    void printClassMethodMsg(const char charClass[], const char charMethod[], const char charMsg[]);
    
    //string manipulation
    std::string getStringOnlyNumeric(std::string strInput);
    
    //Code Begins for TEMPLATED functions
    //========================================================================================
    
    //string manipulation
    //----------------------------------------------------------------------------------------
    //template method must be defined in the header file
    
    
} //End namespace Uniformity

#endif
