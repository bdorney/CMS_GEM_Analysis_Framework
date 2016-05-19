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
#include <cctype>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>

//Framework Includes

//ROOT Includes

//Description of the below:
/*
 
 printers:
    printClassMethodMsg() -> Prints a message to the user indicating class/method template: "charClass::charMethod() - charMsg"
 
 string manipulation:
    getStringOnlyNumeric() -> strips alphabetical characters from the input string leaving only numbers
 */

namespace Uniformity {
    //Math
    //Recursive function, like std::ceil but rounds tInput upward, returning the smallest integral power of 10 that is not less than tInput.
    //T -> type;
    template<typename T>
    T ceilPowerTen(T tInput, int iPower);
    
    //printers
    //void printClassMethodMsg(std::string &strClass, std::string &strMethod, std::string &strMessage);
    void printClassMethodMsg(const char charClass[], const char charMethod[], const char charMsg[]);
    
    //string manipulation
    std::string getStringOnlyNumeric(std::string strInput);
    
    //Code Begins for TEMPLATED functions
    //========================================================================================
    
    //Math
    //----------------------------------------------------------------------------------------
    //Recursive function, like std::ceil but rounds tInput upward, returning the smallest integral power of 10 that is not less than tInput.
    //T -> type;
    template<typename T>
    T ceilPowerTen(T tInput, int iPower){
        if ( tInput > pow(10, iPower) ) {
            return ceilPowerTen(tInput, ++iPower);
        }
        else{
            return pow(10, iPower);
        }
    } //End ceilPowerTen()
    
    //string manipulation
    //----------------------------------------------------------------------------------------
    //template method must be defined in the header file
    
    
} //End namespace Uniformity

#endif
