//
//  UniformityUtilityFunctions.h
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
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <utility>

//Framework Includes
#include "UniformityUtilityOperators.h"

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
    //BinaryOperation - Uses the tuple addition operator defined in UniformityUtilityOperators.h for stl algorithms (e.g. std::accumulate)
    //accumulate is unable to resolve this templated type
    //template<typename T1, typename T2, typename T3>
    //std::tuple<T1, T2, T3> addTuple(const std::tuple<T1, T2, T3> & a, const std::tuple<T1, T2, T3> & b) { return a+b; };
    std::tuple<double, double, double> addTuple(const std::tuple<double, double, double> & a, const std::tuple<double, double, double> & b);
    
    //Recursive function, like std::ceil but rounds tInput upward, returning the smallest integral power of 10 that is not less than tInput.
    //T -> type;
    template<typename T>
    T ceilPowerTen(T tInput, int iPower);
    template<typename T>
    T ceilPowerTen(T tInput, int iScalar, int iPower);
    
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
        if ( tInput > std::pow(10, iPower) ) {
            return ceilPowerTen(tInput, ++iPower);
        }
        else{
            return std::pow(10, iPower);
        }
    } //End ceilPowerTen()
    
    template<typename T>
    T ceilPowerTen(T tInput, int iScalar, int iPower){
        if ( tInput > (iScalar * std::pow(10, iPower) ) ) { //Case: recursion
            if ( iScalar >= 9 ) { //Case: Increment Exponent
                return ceilPowerTen(tInput, 1, ++iPower);
            } //End Case: Increment Exponent
            else{ //Case: Increment Scalar
                return ceilPowerTen(tInput, ++iScalar, iPower);
            } //End Case: Increment Scalar
        } //End Case: recursion
        else{ //Case: value
            return iScalar * std::pow(10, iPower);
        } //End Case: value
    } //End ceilPowerTen()
    
    //string manipulation
    //----------------------------------------------------------------------------------------
    //template method must be defined in the header file
    
    
} //End namespace Uniformity

#endif
