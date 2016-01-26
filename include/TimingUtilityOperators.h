#ifndef ____TimingUtilityOperators__
#define ____TimingUtilityOperators__

//C++ Includes
#include <algorithm>
#include <iostream>
#include <map>
#include <stdio.h>
#include <sstream>
#include <string>
#include <utility>

//My Includes
#include "TimingUtilityTypes.h"

/* Operator List:
 * 
 *  map_cmp_str, determines how two string key values should be ordered in a map
 *
 *
 */

namespace Timing {
    //Determins how two strings should be ordered
    struct map_cmp_str{
        bool operator() (const std::string& str1, const std::string& str2) const
        {
            return str1.size() == str2.size() && (str1.compare(str2) < 0);
        }
    };
    
    //Comparison Operators
    struct cmp_chan{
        bool operator()(const std::string &str1, const std::string &str2) const{
            std::string alphabet = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz_";
            
            return std::stoi(str1.substr(str1.find_first_not_of(alphabet), str1.length() - str1.find_first_not_of(alphabet) ) ) > std::stoi(str2.substr(str2.find_first_not_of(alphabet), str2.length() - str2.find_first_not_of(alphabet) ) );
        }
    };
    
    /*struct cmp_mappedVal{
     bool operator()(std::map<std::string,int> map1, std::map<std::string,int> map2) const{
     return map1.second < map2.second;
     }
     };*/
    
    typedef std::pair<std::string, int> PairType_Str_Int;
    struct CompareSecond_Min
    {
        bool operator()(const PairType_Str_Int& left, const PairType_Str_Int& right) const
        {
            return left.second < right.second;
        }
    };
    struct CompareSecond_Max
    {
        bool operator()(const PairType_Str_Int& left, const PairType_Str_Int& right) const
        {
            return left.second > right.second;
        }
    };
    
} //End namespace

#endif
