#include "CMS_lumi.h"

using std::cout;
using std::endl;
using std::string;

using namespace Luminosity;

//using namespace ROOT;

//Constructor
CMSLumi::CMSLumi(){
    m_strCmsText     = "CMS";
    m_fCmsTextFont   = 61;  // default is helvetic-bold
    
    m_bWriteExtraText = false;
    m_strExtraText   = "Preliminary";
    m_fExtraTextFont = 52;  // default is helvetica-italics
    
    // text sizes and text offsets with respect to the top frame
    // in unit of the top margin size
    m_fLumiTextSize     = 0.6;
    m_fLumiTextOffset   = 0.2;
    m_fCmsTextSize      = 0.75;
    m_fCmsTextOffset    = 0.1;  // only used in outOfFrame version
    
    m_fRelPosX    = 0.045;
    m_fRelPosY    = 0.035;
    m_fRelExtraDY = 1.2;
    
    // ratio of "CMS" and extra text size
    m_fExtraOverCmsTextSize  = 0.76;
    
    m_strLumi_13TeV = "20.1 fb^{-1}";
    m_strLumi_8TeV  = "19.7 fb^{-1}";
    m_strLumi_7TeV  = "5.1 fb^{-1}";
    m_strLumi_sqrtS = "";
    
    m_bDrawLogo      = false;
}

void CMSLumi::drawLumi( std::shared_ptr<TPad> pad, int iPeriod, int iPosX ) {
    bool outOfFrame    = false;
    if( iPosX/10==0 )
    {
        outOfFrame = true;
    }
    int iAlignY_=3;
    int iAlignX_=2;
    if( iPosX/10==0 ) iAlignX_=1;
    if( iPosX==0    ) iAlignX_=1;
    if( iPosX==0    ) iAlignY_=1;
    if( iPosX/10==1 ) iAlignX_=1;
    if( iPosX/10==2 ) iAlignX_=2;
    if( iPosX/10==3 ) iAlignX_=3;
    //if( iPosX == 0  ) relPosX = 0.12;
    int iAlign_ = 10*iAlignX_ + iAlignY_;
    
    float H = pad->GetWh();
    float W = pad->GetWw();
    float l = pad->GetLeftMargin();
    float t = pad->GetTopMargin();
    float r = pad->GetRightMargin();
    float b = pad->GetBottomMargin();
    //  float e = 0.025;
    
    pad->cd();
    
    string strLumiText;
    if( iPeriod==1 )
    {
        strLumiText += m_strLumi_7TeV;
        strLumiText += " (7 TeV)";
    }
    else if ( iPeriod==2 )
    {
        strLumiText += m_strLumi_8TeV;
        strLumiText += " (8 TeV)";
    }
    else if( iPeriod==3 )
    {
        strLumiText = m_strLumi_8TeV;
        strLumiText += " (8 TeV)";
        strLumiText += " + ";
        strLumiText += m_strLumi_7TeV;
        strLumiText += " (7 TeV)";
    }
    else if ( iPeriod==4 )
    {
        strLumiText += m_strLumi_13TeV;
        strLumiText += " (13 TeV)";
    }
    else if ( iPeriod==7 )
    {
        if( outOfFrame ) strLumiText += "#scale[0.85]{";
        strLumiText += m_strLumi_13TeV;
        strLumiText += " (13 TeV)";
        strLumiText += " + ";
        strLumiText += m_strLumi_8TeV;
        strLumiText += " (8 TeV)";
        strLumiText += " + ";
        strLumiText += m_strLumi_7TeV;
        strLumiText += " (7 TeV)";
        if( outOfFrame) strLumiText += "}";
    }
    else if ( iPeriod==12 )
    {
        strLumiText += "8 TeV";
    }
    else if ( iPeriod==0 )
    {
        strLumiText += m_strLumi_sqrtS;
    }
    
    std::cout << strLumiText << endl;
    
    TLatex latex;
    latex.SetNDC();
    latex.SetTextAngle(0);
    latex.SetTextColor(kBlack);
    
    float fExtraTextSize = m_fExtraOverCmsTextSize * m_fCmsTextSize;
    
    latex.SetTextFont(42);
    latex.SetTextAlign(31);
    latex.SetTextSize(m_fLumiTextSize*t);
    latex.DrawLatex(1-r,1-t+m_fLumiTextOffset*t, strLumiText.c_str() );
    
    if( outOfFrame )
    {
        latex.SetTextFont(m_fCmsTextFont);
        latex.SetTextAlign(11);
        latex.SetTextSize(m_fCmsTextSize*t);
        latex.DrawLatex(l,1-t+m_fLumiTextOffset*t,m_strCmsText.c_str() );
    }
    
    pad->cd();
    
    float posX_=0;
    if( iPosX%10<=1 )
    {
        posX_ =   l + m_fRelPosX*(1-l-r);
    }
    else if( iPosX%10==2 )
    {
        posX_ =  l + 0.5*(1-l-r);
    }
    else if( iPosX%10==3 )
    {
        posX_ =  1-r - m_fRelPosX*(1-l-r);
    }
    float posY_ = 1-t - m_fRelPosY*(1-t-b);
    if( !outOfFrame )
    {
        if( m_bDrawLogo )
        {
            /*posX_ =   l + 0.045*(1-l-r)*W/H;
            posY_ = 1-t - 0.045*(1-t-b);
            float xl_0 = posX_;
            float yl_0 = posY_ - 0.15;
            float xl_1 = posX_ + 0.15*H/W;
            float yl_1 = posY_;
            TASImage* CMS_logo = new TASImage("CMS-BW-label.png");
            TPad* pad_logo = new TPad("logo","logo", xl_0, yl_0, xl_1, yl_1 );
            pad_logo->Draw();
            pad_logo->cd();
            CMS_logo->Draw("X");
            pad_logo->Modified();
            pad->cd();*/
        }
        else
        {
            latex.SetTextFont(m_fCmsTextFont);
            latex.SetTextSize(m_fCmsTextSize*t);
            latex.SetTextAlign(iAlign_);
            latex.DrawLatex(posX_, posY_, m_strCmsText.c_str() );
            if( m_bWriteExtraText )
            {
                latex.SetTextFont(m_fExtraTextFont);
                latex.SetTextAlign(iAlign_);
                latex.SetTextSize(fExtraTextSize*t);
                latex.DrawLatex(posX_, posY_- m_fRelExtraDY*m_fCmsTextSize*t, m_strExtraText.c_str() );
            }
        }
    }
    else if( m_bWriteExtraText )
    {
        if( iPosX==0)
        {
            posX_ =   l +  m_fRelPosX*(1-l-r);
            posY_ =   1-t+m_fLumiTextOffset*t;
        }
        latex.SetTextFont(m_fExtraTextFont);
        latex.SetTextSize(fExtraTextSize*t);
        latex.SetTextAlign(iAlign_);
        latex.DrawLatex(posX_, posY_, m_strExtraText.c_str() );
    }
    return;
}