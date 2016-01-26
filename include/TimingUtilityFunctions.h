
#ifndef ____TimingUtilityFunctions__
#define ____TimingUtilityFunctions__

//C++ Includes
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <stdio.h>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

//My Includes
#include "TimingUtilityOperators.h"

//ROOT Includes
#include "TColor.h"
#include "TFile.h"

/* 
 * //boolean
 * convert2bool -> converts a string from the set {t,true,1,f,false,0} to boolean (case-insenstive)
 *
 * //File I/O
 * getlineNoSpaces -> gets a line from a file, leading/trailing spaces and tabs are removed
 *
 * //Math
 * addVal -> Performs addition of a value to a stl container; for use with stl algorithms
 * deltaMean -> Determines the difference in mean between two datasets
 * getDeltaTForChannel() -> Determines the largest diff between a map's elements
 * getMaxForChannelAND() -> Determines the maximum time of a map's elements (returns -1 if there are less than 2 nonzero elements)
 * getMinForChannelOR() -> Determines the minimum time of a map's elements (returns -1 if there are no nonzero elements)
 *
 *
 * //printers
 * printROOTFileStatus -> prints the status bits of a ROOT file
 * printStreamStatus -> prints the status bits of an std::ifstream
 * printStringNotFoundMsg -> prints a message instructing the user a specific pattern requested in file I/O not found; also informs user of which method, and which file the missing pattern is in
 *
 * //string manipulation
 * getCommaSeparatedList -> returns a std::vector<std::string> of a comma separated list, ignoring whitespaces
 * getCharSeparatedList -> returns a std::vector<std::string> of a character separated list, ignoring whitespaces
 * getParsedLine -> returns a std::pair<std::string,std::string> of a "Field,Value" under the format "Field = 'Value';" note the 4 important characters used for parsing {=,',',;}
 * getString -> converts an input type to a string
 * stofSafe -> converts a string to a float; checks for non-numeric characters; offers user chance to correct;
 * stoiSafe -> converts a string to an int; checks for non-numeric characters; offers user chance to correct
 *
 */

namespace Timing {
    //Function Declaration
    //========================================================================================
    //boolean
    bool convert2bool(std::string str, bool &bExitSuccess);
    
    //File I/O
    std::istream & getlineNoSpaces(std::istream & stream, std::string & str);
    
    //Math
    //Addition method
    //template<class T>
    /*template<typename T>
    struct addVal{
        T tVal;
     
        addVal(T tAdd){
            tVal = tAdd;
        }
     
        void operator()(T &tInput) const{
            //input += val_fixed; //internet says "input += fixed"
            tInput += tVal;
        }
        //usage example:
        //std::for_each(myvec.begin(), myvec.end(), addVal(1.0));
     };*/
    
    //Can't seem to get the templated form above to work
    struct addVal{
        int fVal;
        
        addVal(int fAdd){
            fVal = fAdd;
        }
        
        void operator()(int &fInput) const{
            fInput += fVal;
        }
        //usage example:
        //std::for_each(myvec.begin(), myvec.end(), addVal(1.0));
    };
    
    //T -> Type; A -> Allocator
    template<typename T, typename A>
    float deltaMean( std::vector<T,A> const &vec1, std::vector<T,A> const &vec2);
    
    int getDeltaTForChannel(std::map<std::string, int> inputMap);
    int getMaxForChannelAND(std::map<std::string, int> inputMap);
    int getMinForChannelOR(std::map<std::string, int> inputMap);
    
    //printers
    void printROOTFileStatus(TFile *file_ROOT);
    void printStreamStatus(std::ifstream &inputStream);
    void printStringNotFoundMsg(std::string strCallingMethod, std::string strPatternNotFound, std::string strLine, std::string strFilename);
    
    //string manipulation
    std::vector<std::string> getCharSeparatedList(std::string strInput, char cDelimiter);
    //std::vector<std::string> getCommaSeparatedList(std::string strInput);
    std::pair<std::string,std::string> getParsedLine(std::string strInputLine, bool &bExitSuccess);
    template<class TConversion>
    std::string getString(TConversion input);
    float stofSafe(std::string strInputField, std::string strInputValue);
    int stoiSafe(std::string strInputField, std::string strInputValue);
    
    //unsorted
    int getCyclicColor(int iInput);
    
    //Code Begins for TEMPLATED functions
    //========================================================================================
    
    //Math
    //----------------------------------------------------------------------------------------
    //T -> Type; A -> Allocator
    //Determines the difference in mean between two datasets
    template<typename T, typename A>
    float deltaMean( std::vector<T,A> const &vec1, std::vector<T,A> const &vec2){
        float fMean1 = (std::accumulate(vec1.begin(), vec1.end(), 0.0) / (float) vec1.size() );
        float fMean2 = (std::accumulate(vec2.begin(), vec2.end(), 0.0) / (float) vec2.size() );
        
        return fMean1 - fMean2;
    } //End deltaMean

    //string manipulation
    //----------------------------------------------------------------------------------------    
    //template method must be defined in the header file
    template<class TConversion>
    std::string getString(TConversion input){
        std::stringstream sstream;
        sstream<<input;
        return sstream.str();
    } //End getString()

} //End namespace

#endif
