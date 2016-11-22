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
namespace QualityControl {
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
        
        struct compare{
            bool operator()(const std::pair<int, double> pair1, std::pair<int, double> pair2 ){
                return pair1.second < pair2.second;
            };
            bool operator()(const std::pair<std::string, double> pair1, std::pair<std::string, double> pair2 ){
                return pair1.second < pair2.second;
            };
        };
    } //End namespace Timing
} //End namespace QualityControl

#endif
