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

//Framework Includes

//ROOT Includes

//Description:
/*
 
 
 */

namespace Uniformity {
    //UnaryPredicate - returns true if greater than
    struct greaterThan{
        float fVal;
        
        //Initialization
        greaterThan(float fInput){
            fVal = fInput;
        }
        
        bool operator()(float &fInput ) const{
            return (fInput > fVal);
        }
        //usage example:
        //std::find_if(myvec.begin(), myvec.end(), lessThan(x));
    }; //End lessThan
    
    //UnaryPredicate - returns true if less than or equal to
    struct greaterThanOrEqual{
        float fVal;
        
        //Initialization
        greaterThanOrEqual(float fInput){
            fVal = fInput;
        }
        
        bool operator()(float &fInput ) const{
            return (fInput >= fVal);
        }
        //usage example:
        //std::find_if(myvec.begin(), myvec.end(), lessThan(x));
    }; //End lessThanOrEqual
    
    //UnaryPredicate - returns true if less than
    struct lessThan{
        float fVal;
        
        //Initialization
        lessThan(float fInput){
            fVal = fInput;
        }
        
        bool operator()(float &fInput ) const{
            return (fInput < fVal);
        }
        //usage example:
        //std::find_if(myvec.begin(), myvec.end(), lessThan(x));
    }; //End lessThan
    
    //UnaryPredicate - returns true if less than or equal to
    struct lessThanOrEqual{
        float fVal;
        
        //Initialization
        lessThanOrEqual(float fInput){
            fVal = fInput;
        }
        
        bool operator()(float &fInput ) const{
            return (fInput <= fVal);
        }
        //usage example:
        //std::find_if(myvec.begin(), myvec.end(), lessThan(x));
    }; //End lessThanOrEqual
    
    //UnaryPredicate - returns true if outside of range defined by f1 and f2; e.g. x not in (f1, f2)
    struct outOfRange{
        float fLower, fUpper;
        
        //Initialization
        outOfRange(float f1, float f2){
            fLower = std::min(f1,f2);
            fUpper = std::max(f1,f2);
        }
        
        bool operator()(float &fInput) const{
            return (fInput < fLower && fUpper < fInput);
        }
        //usage example
        //std::copy_if(myvec1.begin(), myvec1.end(), myvec2.begin(), outOfRange(x,y));
    };
    
    //UnaryPredicate - returns true if inside range defined by f1 and f2; e.g. x in [f1,f2]
    struct inRange{
        float fLower, fUpper;
        
        //Initialization
        inRange(float f1, float f2){
            fLower = std::min(f1,f2);
            fUpper = std::max(f1,f2);
        }
        
        bool operator()(float &fInput) const{
            return (fLower <= fInput && fInput <= fUpper);
        }
        //usage example
        //std::copy_if(myvec1.begin(), myvec1.end(), myvec2.begin(), outOfRange(x,y));
    };
} //End namespace Uniformity

#endif
