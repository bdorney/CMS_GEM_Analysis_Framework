//
//  UniformityUtilityOperators.h
//  
//
//  Created by Brian L Dorney on 23/02/16.
//
//

#ifndef _UniformityUtilityOperators_h
#define _UniformityUtilityOperators_h

//C++ Includes
#include <algorithm>
#include <utility>

//Framework Includes

//ROOT Includes

//Description:
/*
 
 
 */

namespace Uniformity {
    //std::tuple addition operator
    template<typename T1, typename T2, typename T3>
    std::tuple<T1, T2, T3> operator+(const std::tuple<T1, T2, T3> & a,
                                  const std::tuple<T1, T2, T3> & b)
    {
        return std::make_tuple(std::get<0>(a) + std::get<0>(b), std::get<1>(a) + std::get<1>(b), std::get<2>(a) + std::get<2>(b) );
    }; //End std::tuple addition operator
    
    //UnaryPredicate - divides input by a scalar
    struct divides{
        double dVal;
        
        //Constructor
        divides(double dInput){ dVal = dInput; }
        
        std::tuple<double, double, double> operator()(std::tuple<double, double, double> a){
            
            return std::make_tuple( std::get<0>(a) / dVal, std::get<1>(a) / dVal, std::get<2>(a) / dVal );
        }
    };
    
    //UnaryPredicate - returns true if greater than
    struct greaterThan{
        float fVal;
        
        //Constructor
        greaterThan(float fInput){
            fVal = fInput;
        }
        
        bool operator()(float fInput ) const{
            return (fInput > fVal);
        }
        //usage example:
        //std::find_if(myvec.begin(), myvec.end(), lessThan(x));
    }; //End lessThan
    
    //UnaryPredicate - returns true if less than or equal to
    struct greaterThanOrEqual{
        float fVal;
        
        //Constructor
        greaterThanOrEqual(float fInput){
            fVal = fInput;
        }
        
        bool operator()(float fInput ) const{
            return (fInput >= fVal);
        }
        //usage example:
        //std::find_if(myvec.begin(), myvec.end(), lessThan(x));
    }; //End lessThanOrEqual
    
    //UnaryPredicate - returns true if less than
    struct lessThan{
        float fVal;
        
        //Constructor
        lessThan(float fInput){
            fVal = fInput;
        }
        
        bool operator()(float fInput ) const{
            return (fInput < fVal);
        }
        //usage example:
        //std::find_if(myvec.begin(), myvec.end(), lessThan(x));
    }; //End lessThan
    
    //UnaryPredicate - returns true if less than or equal to
    struct lessThanOrEqual{
        float fVal;
        
        //Constructor
        lessThanOrEqual(float fInput){
            fVal = fInput;
        }
        
        bool operator()(float fInput ) const{
            return (fInput <= fVal);
        }
        //usage example:
        //std::find_if(myvec.begin(), myvec.end(), lessThan(x));
    }; //End lessThanOrEqual
    
    //UnaryPredicate - returns true if outside of range defined by f1 and f2; e.g. x not in (f1, f2)
    struct outOfRange{
        float fLower, fUpper;
        
        //Constructor
        outOfRange(float f1, float f2){
            fLower = std::min(f1,f2);
            fUpper = std::max(f1,f2);
        }
        
        bool operator()(float fInput) const{
            return (fInput < fLower && fUpper < fInput);
        }
        //usage example
        //std::copy_if(myvec1.begin(), myvec1.end(), myvec2.begin(), outOfRange(x,y));
    };
    
    //UnaryPredicate - returns true if inside range defined by f1 and f2; e.g. x in [f1,f2]
    struct inRange{
        float fLower, fUpper;
        
        //Constructor
        inRange(float f1, float f2){
            fLower = std::min(f1,f2);
            fUpper = std::max(f1,f2);
        }
        
        bool operator()(float fInput) {
            return (fLower <= fInput && fInput <= fUpper);
        }
        //usage example
        //std::copy_if(myvec1.begin(), myvec1.end(), myvec2.begin(), outOfRange(x,y));
    };
} //End namespace Uniformity

#endif
