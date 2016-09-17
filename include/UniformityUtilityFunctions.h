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
//#include <list>
#include <stdio.h>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

//Framework Includes
#include "TimingUtilityFunctions.h"
#include "UniformityUtilityOperators.h"

//ROOT Includes

//Description of the below:
/*
 
 printers:
    printClassMethodMsg() -> Prints a message to the user indicating class/method template: "charClass::charMethod() - charMsg"
 
 string manipulation:
    getStringOnlyNumeric() -> strips alphabetical characters from the input string leaving only numbers
 */

namespace QualityControl {
    namespace Uniformity {
        //Math
        //BinaryOperation - Uses the tuple addition operator defined in UniformityUtilityOperators.h for stl algorithms (e.g. std::accumulate)
        std::tuple<double, double, double> addTuple(const std::tuple<double, double, double> & a, const std::tuple<double, double, double> & b);
        
        //iFirstEvt -> a starting event number
        //iNEvt -> a desired number of events
        //iNTreeEntries -> a total number of events ()
        //Returns a valid event range to loop over
        std::pair<int,int> getEventRange(int iFirstEvt, int iNEvt, int iNTreeEntries);
        
        //Recursive function, like std::ceil but rounds tInput upward, returning the smallest integral power of 10 that is not less than tInput.
        //T -> type;
        template<typename T>
        T ceilPowerTen(T tInput, int iPower);
        template<typename T>
        T ceilPowerTen(T tInput, int iScalar, int iPower);
        
        //Recursive function
        //Given the maximum number of strips in a readout sector (iStripNum_Max)
        //Converts strip number in an iEta row [1,N] to strip number in a readout sector [1,M]
        template<typename T>
        T getPhiStripNum(T tInput, int iStripNum_Max=128);
        
        //Recursive function
        //Given the maximum number of strips in a readout sector (iStripNum_Max)
        //Given a strip number in an iEta row
        //Returns the iPhi sector the strip is from
        template<typename T>
        int getPhiSectorVal(T tInput, int iStripNum_Max=128, int iPhi=1);
        
        //map manipulation
        //returns a vector of keys
        //T1 & T2 -> type; C -> Compare; A -> allocator
        template<typename T1, typename T2, typename C, typename A>
        std::vector<T1> getVectorOfKeys(std::map<T1,T2,C,A> inputMap);
        
        //returns a vector of keys in a multimap
        //note duplicate stored keys are removed from the vector  before being returned
        //T1 & T2 -> type; C -> Compare; A -> allocator
        template<typename T1, typename T2, typename C, typename A>
        std::vector<T1> getVectorOfKeys(std::multimap<T1,T2,C,A> inputMap);
        
        //printers
        //void printClassMethodMsg(std::string &strClass, std::string &strMethod, std::string &strMessage);
        void printClassMethodMsg(const char charClass[], const char charMethod[], const char charMsg[]);
        
        //string manipulation
        std::string getStringOnlyNumeric(std::string strInput);
        
        //Finds strSub1 in strInput and replaces it with strSub2
        //Needs more work to be exception safe, right now if strSub1 is at the END of the string a problem might occur
        //void replaceSubStr1WithSubStr2(std::string &strInput, std::string strSub1, std::string strSub2);
        
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
        
        template<typename T>
        T getPhiStripNum(T tInput, int iStripNum_Max){
            return ( (tInput > iStripNum_Max) ?  getPhiStripNum(tInput-iStripNum_Max, iStripNum_Max) : tInput );
        } //End getPhiStripNum()
        
        //Recursive function
        //Given the maximum number of strips in a readout sector (iStripNum_Max)
        //Given a strip number in an iEta row
        //Returns the iPhi sector the strip is from
        template<typename T>
        int getPhiSectorVal(T tInput, int iStripNum_Max, int iPhi){
            return ( (tInput > iStripNum_Max) ?  getPhiSectorVal(tInput - iStripNum_Max, iStripNum_Max, ++iPhi) : iPhi );
        } //End getPhiSectorVal()
        
        //map manipulation
        //----------------------------------------------------------------------------------------
        //T1 & T2 -> type; C -> Compare; A -> allocator
        template<typename T1, typename T2, typename C, typename A>
        std::vector<T1> getVectorOfKeys(std::map<T1,T2,C,A> inputMap){
            
            std::vector<T1> vec_retKeys;
            
            for (auto iterMap = inputMap.begin(); iterMap != inputMap.end(); ++iterMap) {
                vec_retKeys.push_back( (*iterMap).first );
            }
            
            return vec_retKeys;
        } //End getVectorOfKeys()
        
        //T1 & T2 -> type; C -> Compare; A -> allocator
        template<typename T1, typename T2, typename C, typename A>
        std::vector<T1> getVectorOfKeys(std::multimap<T1,T2,C,A> inputMap){
            
            std::vector<T1> vec_retKeys;
            
            for (auto iterMap = inputMap.begin(); iterMap != inputMap.end(); ++iterMap) {
                vec_retKeys.push_back( (*iterMap).first );
            }
            
            auto iterVec = vec_retKeys.begin();
            iterVec = std::unique( vec_retKeys.begin(), vec_retKeys.end() );
            vec_retKeys.resize( std::distance(vec_retKeys.begin(),iterVec) );

            return vec_retKeys;
        } //End getVectorOfKeys()
        
        //template method must be defined in the header file
        
        
    } //End namespace Uniformity    
} //End namespace QualityControl


#endif
