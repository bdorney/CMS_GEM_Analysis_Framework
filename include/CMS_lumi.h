//
//  CMS_lumi.h
//
// Taken from https://ghm.web.cern.ch/ghm/plots/
//
//

#ifndef ____CMSLumi__
#define ____CMSLumi__

//C++ Includes
#include <iostream>
#include <memory>
#include <stdio.h>
#include <string>

//Framework Includes

//ROOT Includes
#include "TASImage.h"
#include "TBox.h"
#include "TLatex.h"
#include "TLine.h"
#include "TPad.h"

namespace Luminosity {
    class CMSLumi {
    public:
        //Constructors
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Default
        CMSLumi();
        
        //Destructor
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Just use the default compiler gives you
        
        //Actions - Methods that Do Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //void CMS_lumi( TPad* pad, int iPeriod=3, int iPosX=10 );
        void drawLumi( std::shared_ptr<TPad> pad, int iPeriod=3, int iPosX=10 );
        
        //Getters - Methods that Get (i.e. Return) Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Printers - Methods that Print Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Setters - Methods that Set Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        virtual void setDrawLogo(bool bInput){
            m_bDrawLogo = bInput;
            return;
        }
        
        //True -> "CMS Preliminary"
        //False -> "CMS"
        virtual void setPreliminary(bool bInput){
            m_bWriteExtraText = bInput;
            return;
        }
        
        
    private:
        //Actions - Methods that Do Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Getters - Methods that Get (i.e. Return) Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Printers - Methods that Print Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Setters - Methods that Set Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Attributes
        //------------------------------------------------------------------------------------------------------------------------------------------
        bool m_bDrawLogo;
        bool m_bWriteExtraText;
        
        float m_fCmsTextFont;  // default is helvetic-bold
        
        // ratio of "CMS" and extra text size
        float m_fExtraOverCmsTextSize;
        
        float m_fExtraTextFont;  // default is helvetica-italics

        // text sizes and text offsets with respect to the top frame
        // in unit of the top margin size
        float m_fCmsTextSize;
        float m_fCmsTextOffset;  // only used in outOfFrame version
        float m_fLumiTextSize;
        float m_fLumiTextOffset;
        
        float m_fRelPosX;
        float m_fRelPosY;
        float m_fRelExtraDY;
        
        std::string m_strCmsText;
        std::string m_strExtraText;
        std::string m_strLumi_13TeV;
        std::string m_strLumi_8TeV;
        std::string m_strLumi_7TeV;
        std::string m_strLumi_sqrtS;
    }; //End Class CMSLumi
} //End namespace Luminosity

#endif /* defined(____CMSLumi__) */
