//
//  AnalyzeResponseUniformityClusters.cpp
//  
//
//  Created by Brian L Dorney on 28/01/16.
//
//

#include "DetectorMPGD.h"
#include "AnalyzeResponseUniformityClusters.h"

using std::cout;
using std::endl;
using std::make_shared;
using std::map;
using std::shared_ptr;
using std::string;
using std::vector;

using Timing::getString;
using Timing::printROOTFileStatus;
using Timing::HistoSetup;
using Timing::stofSafe;

using namespace Uniformity;

//Default Constructor
AnalyzeResponseUniformityClusters::AnalyzeResponseUniformityClusters(){
    strAnalysisName = "analysis";
} //End Default Constructor

//Set inputs at construction
AnalyzeResponseUniformityClusters::AnalyzeResponseUniformityClusters(AnalysisSetupUniformity inputSetup, DetectorMPGD & inputDet){
    strAnalysisName = "analysis";
    
    //Store Analysis Parameters
    aSetup = inputSetup;
    
    //Store Detector
    detMPGD = inputDet;
} //End Constructor

//Loops over all stored clusters in detMPGD and Book histograms for the full detector
void AnalyzeResponseUniformityClusters::fillHistos(){
    //Variable Declaration
    
    //Loop Over Stored iEta Sectors
    for (auto iterEta = detMPGD.map_sectorsEta.begin(); iterEta != detMPGD.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        
        //Loop Over Stored iPhi Sectors
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over iPhi Sectors
            
            //Loop Over Stored Clusters
            for (auto iterClust = (*iterPhi).second.vec_clusters.begin(); iterClust != (*iterPhi).second.vec_clusters.end(); ++iterClust) { //Loop Over Stored Clusters
                //Fill iEta Histograms
                (*iterEta).second.clustHistos.hADC->Fill( (*iterClust).fADC );
                //(*iterEta).second.clustHistos.hMulti->Fill( (*iterClust). );
                (*iterEta).second.clustHistos.hPos->Fill( (*iterClust).fPos_X );
                (*iterEta).second.clustHistos.hSize->Fill( (*iterClust).iSize );
                (*iterEta).second.clustHistos.hTime->Fill( (*iterClust).iTimeBin );
                
                (*iterEta).second.clustHistos.hADC_v_Pos->Fill( (*iterClust).fPos_X, (*iterClust).fADC );
                (*iterEta).second.clustHistos.hADC_v_Size->Fill( (*iterClust).iSize, (*iterClust).fADC );
                (*iterEta).second.clustHistos.hADC_v_Time->Fill( (*iterClust).iTimeBin, (*iterClust).fADC );
                
                //Fill iPhi Histograms
                (*iterPhi).second.clustHistos.hADC->Fill( (*iterClust).fADC );
                //(*iterPhi).second.clustHistos.hMulti
                (*iterPhi).second.clustHistos.hSize->Fill( (*iterClust).iSize);
                (*iterPhi).second.clustHistos.hTime->Fill( (*iterClust).iTimeBin);
                
                (*iterPhi).second.clustHistos.hADC_v_Pos->Fill( (*iterClust).fPos_X, (*iterClust).fADC );
                (*iterPhi).second.clustHistos.hADC_v_Size->Fill( (*iterClust).iSize, (*iterClust).fADC );
                (*iterPhi).second.clustHistos.hADC_v_Time->Fill( (*iterClust).iTimeBin, (*iterClust).fADC );
            } //End Loop Over Stored Clusters
            
            //Slices
            //Now that all clusters have been analyzed we extract the slices
            for (int i=1; i<= aSetup.iUniformityGranularity; ++i) { //Loop Over Slices
                
                //Two cases, the slice histogram is empty (first analyzed run), or the slice histogram has nonzero entries (subsequent runs)
                //Projecting a slice from the 2D histogram, setting this to a temporary histogram, and then adding it to the slice histogram covers both cases!!!
                //TH1F *hTempSliceHisto = (TH1D*) (*iterPhi).second.clustHistos.hADC_v_Pos->ProjectionY( ("h_iEta" + getString( (*iterEta).first ) + "iPhi" + getString( (*iterPhi).first ) + "Slice" + getString(i) + "_clustADC").c_str(),i,i,"");
                
                std::shared_ptr<TH1F> hTempSliceHisto = make_shared<TH1F>( *( (TH1F*) (*iterPhi).second.clustHistos.hADC_v_Pos->ProjectionY( "hTempSliceHisto",i,i,"") ) );

                (*iterPhi).second.map_slices[i].hSlice_ClustADC->Add( hTempSliceHisto.get() );
                
                //delete hTempSliceHisto;
            } //End Loop Over Slices
        } //End Loop Over iPhi Sectors
        
	//Debugging
        //std::cout<<"(*iterEta).second.clustHistos.hADC->Integral() = " << (*iterEta).second.clustHistos.hADC->Integral() << std::endl;
    } //End Loop Over iEta Sectors
    
    return;
} //End AnalyzeResponseUniformityClusters::fillHistos() - Full Detector

//Loops over all slices in detMPGD and fits Booked histograms for the full detector
//Assumes Histos have been filled already (obviously)
void AnalyzeResponseUniformityClusters::fitHistos(){
    //Variable Declaration
    Double_t *dPeakPos;
    
    TSpectrum specADC(1,2);    //One peak; 2 sigma away from any other peak
    
    //float fChi2 = -1;
    float fMin = -1e12, fMax = 1e12;
    //float fNDF = -1;
    float fNormChi2;
    float fPkPos = 0, fPkPosErr = 0;        //Peak Position
    //float fPkRes = 0, fPkResErr = 0;        //Peak Error
    float fPkWidth = 0, fPkWidthErr = 0;    //Peak Width
    
    vector<float> vec_fFitRange;
    
    //Clear Information from any previous analysis
    detMPGD.vec_allADCPeaks.clear();
    
    //Loop Over Stored iEta Sectors
    for (auto iterEta = detMPGD.map_sectorsEta.begin(); iterEta != detMPGD.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        
        //Loop Over Stored iPhi Sectors
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over iPhi Sectors
            
            //Loop Over Stored Slices
            for (auto iterSlice = (*iterPhi).second.map_slices.begin(); iterSlice != (*iterPhi).second.map_slices.end(); ++iterSlice ) { //Loop Over Slices
                cout<<"Attempting to Fit (iEta, iPhi, iSlice) = (" << (*iterEta).first << ", " << (*iterPhi).first << ", " << (*iterSlice).first << ")\n";
                
                //Clear the calculated fit range from the previous slice
                vec_fFitRange.clear();
                
                //Check if Histogram does not exist
                if ( (*iterSlice).second.hSlice_ClustADC == nullptr) continue;
                
                //Find peak & store it's position
                specADC.Search( (*iterSlice).second.hSlice_ClustADC.get(), 2, "nobackground", 0.5 );
                dPeakPos = specADC.GetPositionX();
                
                //Initialize Fit
                (*iterSlice).second.fitSlice_ClustADC = make_shared<TF1>( getFit( (*iterEta).first, (*iterPhi).first, (*iterSlice).first, aSetup.histoSetup_clustADC, (*iterSlice).second.hSlice_ClustADC, specADC) );
                
                for (auto iterRange = aSetup.histoSetup_clustADC.vec_strFit_Range.begin(); iterRange != aSetup.histoSetup_clustADC.vec_strFit_Range.end(); ++iterRange) { //Loop Over Fit Range
                    vec_fFitRange.push_back( getFitBoundary( (*iterRange), (*iterSlice).second.hSlice_ClustADC, specADC ) );
                } //End Loop Over Fit Range
                
                //Perform Fit & Store the Result
                TFitResult fitRes_ADC;
                
                if (vec_fFitRange.size() > 1) { //Case: Fit within the user specific range
                    fMin = (*std::min_element(vec_fFitRange.begin(), vec_fFitRange.end() ) );
                    fMax = (*std::max_element(vec_fFitRange.begin(), vec_fFitRange.end() ) );
                    
                    fitRes_ADC = *((*iterSlice).second.hSlice_ClustADC->Fit( (*iterSlice).second.fitSlice_ClustADC.get(),aSetup.histoSetup_clustADC.strFit_Option.c_str(),"", fMin, fMax) );
                } //End Case: Fit within the user specific range
                else{ //Case: No range to use
                    fitRes_ADC = *((*iterSlice).second.hSlice_ClustADC->Fit( (*iterSlice).second.fitSlice_ClustADC.get(),aSetup.histoSetup_clustADC.strFit_Option.c_str(),"") );
                } //End Case: No range to use
                
                //Determine which point in the TGraphs this is
                int iPoint = std::distance( (*iterPhi).second.map_slices.begin(), iterSlice) + aSetup.iUniformityGranularity * std::distance((*iterEta).second.map_sectorsPhi.begin(), iterPhi);
                
                //cout<<"iPoint = " << iPoint << endl;
                
                //Store info from spectrum
                //Store - Number of Peaks (from spectrum)
                (*iterEta).second.gEta_ClustADC_Spec_NumPks->SetPoint(iPoint, (*iterSlice).second.fPos_Center, specADC.GetNPeaks() );
                (*iterEta).second.gEta_ClustADC_Spec_NumPks->SetPointError(iPoint, 0.5 * (*iterSlice).second.fWidth, 0 );
                
                //Store - Peak Position (from spectrum)
                (*iterEta).second.gEta_ClustADC_Spec_PkPos->SetPoint(iPoint, (*iterSlice).second.fPos_Center, dPeakPos[0] );
                (*iterEta).second.gEta_ClustADC_Spec_PkPos->SetPointError(iPoint, 0.5 * (*iterSlice).second.fWidth, 0. );
                (*iterEta).second.mset_fClustADC_Spec_PkPos.insert( dPeakPos[0] );
                
                //Was the Fit Valid?
                //i.e. did the minimizer succeed in finding the minimm
                if ( fitRes_ADC.IsValid() ) { //Case: Valid Fit!!!
                    //Get the Peak Position
                    fPkPos      = getParam( (*iterSlice).second.fitSlice_ClustADC, aSetup.histoSetup_clustADC, "PEAK" );
                    fPkPosErr   = getParamError( (*iterSlice).second.fitSlice_ClustADC, aSetup.histoSetup_clustADC, "PEAK" );
                    
                    //Get the Peak Width
                    auto iterParamHWHM = std::find(aSetup.histoSetup_clustADC.vec_strFit_ParamMeaning.begin(), aSetup.histoSetup_clustADC.vec_strFit_ParamMeaning.end(), "HWHM");
                    auto iterParamFWHM = std::find(aSetup.histoSetup_clustADC.vec_strFit_ParamMeaning.begin(), aSetup.histoSetup_clustADC.vec_strFit_ParamMeaning.end(), "FWHM");
                    
                    if ( iterParamHWHM != aSetup.histoSetup_clustADC.vec_strFit_ParamMeaning.end() ){ //Case: Fit Parameter List Has Meaning HWHM
                        fPkWidth = 2. * getParam( (*iterSlice).second.fitSlice_ClustADC, aSetup.histoSetup_clustADC, "HWHM" );
                    } //End Case: Fit Parameter List Has Meaning HWHM
                    else if( iterParamFWHM != aSetup.histoSetup_clustADC.vec_strFit_ParamMeaning.end() ){ //Case: Fit Parameter List Has Meaning FWHM
                        fPkWidth = getParam( (*iterSlice).second.fitSlice_ClustADC, aSetup.histoSetup_clustADC, "FWHM" );
                    } //End Case: Fit Parameter List Has Meaning FWHM
                    
                    //Record the PkPos for the summary stat (Used for checking uniformity)
                    //(*iterEta).second.mset_fClustADC_Fit_PkPos.insert( getPeakPos( (*iterSlice).second.fitSlice_ClustADC, aSetup.histoSetup_clustADC ) );
                    (*iterEta).second.mset_fClustADC_Fit_PkPos.insert( fPkPos );
                    
                    //Store Fit parameters - NormChi2
                    fNormChi2 = (*iterSlice).second.fitSlice_ClustADC->GetChisquare() / (*iterSlice).second.fitSlice_ClustADC->GetNDF();
                    if ( std::isinf(fNormChi2) ){ fNormChi2 = -1; }
                    if ( std::isnan(fNormChi2) ){ fNormChi2 = -1; }
                    (*iterEta).second.gEta_ClustADC_Fit_NormChi2->SetPoint(iPoint, (*iterSlice).second.fPos_Center,  fNormChi2  );
                    (*iterEta).second.gEta_ClustADC_Fit_NormChi2->SetPointError(iPoint, 0.5 * (*iterSlice).second.fWidth, 0. );
                    
                    //Store Fit parameters - Peak Position (from fit)
                    //(*iterEta).second.gEta_ClustADC_Fit_PkPos->SetPoint(iPoint, (*iterSlice).second.fPos_Center, getPeakPos( (*iterSlice).second.fitSlice_ClustADC, aSetup.histoSetup_clustADC ) );
                    (*iterEta).second.gEta_ClustADC_Fit_PkPos->SetPoint(iPoint, (*iterSlice).second.fPos_Center, fPkPos );
                    //(*iterEta).second.gEta_ClustADC_Fit_PkPos->SetPointError(iPoint, 0.5 * (*iterSlice).second.fWidth, getPeakPosError( (*iterSlice).second.fitSlice_ClustADC, aSetup.histoSetup_clustADC ) );
                    (*iterEta).second.gEta_ClustADC_Fit_PkPos->SetPointError(iPoint, 0.5 * (*iterSlice).second.fWidth, fPkPosErr );
                    
                    //Store Fit parameters - Peak Resolution (from fit)
                    (*iterEta).second.gEta_ClustADC_Fit_PkRes->SetPoint(iPoint, (*iterSlice).second.fPos_Center, fPkWidth / fPkPos );
                    (*iterEta).second.gEta_ClustADC_Fit_PkRes->SetPointError(iPoint, 0.5 * (*iterSlice).second.fWidth, sqrt( pow( fPkWidthErr / fPkPos, 2) + pow( ( fPkPosErr * fPkWidth ) / ( fPkPos * fPkPos), 2 ) ) );
                } //End Case: Valid Fit!!!
                else{ //Case: Invalid Fit (minimizer did not find minumum)
                    //Store Fit parameters - Peak Position (from fit); when failing
                    //(*iterEta).second.gEta_ClustADC_Fit_Failures->SetPoint(iPoint, (*iterSlice).second.fPos_Center, getPeakPos( (*iterSlice).second.fitSlice_ClustADC, aSetup.histoSetup_clustADC ) );
                    (*iterEta).second.gEta_ClustADC_Fit_Failures->SetPoint(iPoint, (*iterSlice).second.fPos_Center, fPkPos );
                    //(*iterEta).second.gEta_ClustADC_Fit_Failures->SetPointError(iPoint, 0.5 * (*iterSlice).second.fWidth, getPeakPosError( (*iterSlice).second.fitSlice_ClustADC, aSetup.histoSetup_clustADC ) );
                    (*iterEta).second.gEta_ClustADC_Fit_Failures->SetPointError(iPoint, 0.5 * (*iterSlice).second.fWidth, fPkPosErr );
                } //End Case: Invalid Fit (minimizer did not find minum)
            } //End Loop Over Slices
            
            //Calculate statistics for this iPhi sector
            //Placeholder
        } //End Loop Over iPhi Sectors
        
        //Calculate statistics for this iEta sector - Fit
        if ((*iterEta).second.mset_fClustADC_Fit_PkPos.size() > 0) { //Check if stored fit positions exist
            calcStatistics( (*iterEta).second.statClustADC_Fit_PkPos, (*iterEta).second.mset_fClustADC_Fit_PkPos );
        } //End Check if stored fit positions exist
        
        //Calculate statistics for this iEta sector - Spec
        if ((*iterEta).second.mset_fClustADC_Spec_PkPos.size() > 0) { //Check if stored Spec positions exist
            calcStatistics( (*iterEta).second.statClustADC_Spec_PkPos, (*iterEta).second.mset_fClustADC_Spec_PkPos );
        } //End Check if stored Spec positions exist
    } //End Loop Over iEta Sectors
    
    return;
} //End AnalyzeResponseUniformityClusters::fitHistos()

//Loops through the detector and initializes all cluster graphs
void AnalyzeResponseUniformityClusters::initGraphsClusters(){
    //Variable Declaration
    
    //Loop Over Stored iEta Sectors
    for (auto iterEta = detMPGD.map_sectorsEta.begin(); iterEta != detMPGD.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        //Initialize Response uniformity graphs - Fit norm Chi2
        (*iterEta).second.gEta_ClustADC_Fit_NormChi2 = make_shared<TGraphErrors>( TGraphErrors( aSetup.iUniformityGranularity * (*iterEta).second.map_sectorsPhi.size() ) );
        (*iterEta).second.gEta_ClustADC_Fit_NormChi2->SetName( ( getNameByIndex( (*iterEta).first, -1, -1, "g", "clustADC_Fit_NormChi2" ) ).c_str() );
        
        //Initialize Response uniformity graphs - Fit peak pos
        (*iterEta).second.gEta_ClustADC_Fit_PkPos = make_shared<TGraphErrors>( TGraphErrors( aSetup.iUniformityGranularity * (*iterEta).second.map_sectorsPhi.size() ) );
        (*iterEta).second.gEta_ClustADC_Fit_PkPos->SetName( ( getNameByIndex( (*iterEta).first, -1, -1, "g", "clustADC_Fit_PkPos" ) ).c_str() );
        
        //Initialize Response uniformity graphs - Fit peak resolution
        (*iterEta).second.gEta_ClustADC_Fit_PkRes = make_shared<TGraphErrors>( TGraphErrors( aSetup.iUniformityGranularity * (*iterEta).second.map_sectorsPhi.size() ) );
        (*iterEta).second.gEta_ClustADC_Fit_PkRes->SetName( ( getNameByIndex( (*iterEta).first, -1, -1, "g", "clustADC_Fit_PkRes" ) ).c_str() );
        
        //Initialize Response uniformity graphs - Positions Were Fit Fails
        (*iterEta).second.gEta_ClustADC_Fit_Failures = make_shared<TGraphErrors>( TGraphErrors( aSetup.iUniformityGranularity * (*iterEta).second.map_sectorsPhi.size() ) );
        (*iterEta).second.gEta_ClustADC_Fit_Failures->SetName( ( getNameByIndex( (*iterEta).first, -1, -1, "g", "clustADC_Fit_Failures" ) ).c_str() );
        
        //Initialize Response uniformity graphs - Spec Number of Peaks
        (*iterEta).second.gEta_ClustADC_Spec_NumPks = make_shared<TGraphErrors>( TGraphErrors( aSetup.iUniformityGranularity * (*iterEta).second.map_sectorsPhi.size() ) );
        (*iterEta).second.gEta_ClustADC_Spec_NumPks->SetName( ( getNameByIndex( (*iterEta).first, -1, -1, "g", "clustADC_Spec_NumPks" ) ).c_str() );
        
        //Initialize Response uniformity graphs - Spec Peak Pos
        (*iterEta).second.gEta_ClustADC_Spec_PkPos = make_shared<TGraphErrors>( TGraphErrors( aSetup.iUniformityGranularity * (*iterEta).second.map_sectorsPhi.size() ) );
        (*iterEta).second.gEta_ClustADC_Spec_PkPos->SetName( ( getNameByIndex( (*iterEta).first, -1, -1, "g", "clustADC_Spec_PkPos" ) ).c_str() );
    } //End Loop Over iEta Sectors
    
    return;
} //End AnalyzeResponseUniformityClusters::initGraphsClusters()

//Loops through the detector and initalizes all cluster histograms
void AnalyzeResponseUniformityClusters::initHistosClusters(){
    //Loop Over Stored iEta Sectors
    for (auto iterEta = detMPGD.map_sectorsEta.begin(); iterEta != detMPGD.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        
        //Grab Eta Sector width (for clustPos Histo)
        aSetup.histoSetup_clustPos.iHisto_nBins  = 3. * aSetup.iUniformityGranularity;
        aSetup.histoSetup_clustPos.fHisto_xLower = -0.5*(*iterEta).second.fWidth;
        aSetup.histoSetup_clustPos.fHisto_xUpper = 0.5*(*iterEta).second.fWidth;
        
        //Initialize iEta Histograms - 1D
        (*iterEta).second.clustHistos.hADC = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_clustADC ) );
        //(*iterEta).second.clustHistos.hMulti = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_clustMulti ) );
        (*iterEta).second.clustHistos.hPos = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_clustPos ) );
        (*iterEta).second.clustHistos.hSize = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_clustSize ) );
        (*iterEta).second.clustHistos.hTime = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_clustTime ) );
        
        //Initialize iEta Histograms - 2D
        (*iterEta).second.clustHistos.hADC_v_Pos = make_shared<TH2F>( getHistogram2D((*iterEta).first, -1, aSetup.histoSetup_clustPos, aSetup.histoSetup_clustADC ) );
        (*iterEta).second.clustHistos.hADC_v_Size = make_shared<TH2F>( getHistogram2D((*iterEta).first, -1, aSetup.histoSetup_clustSize, aSetup.histoSetup_clustADC ) );
        (*iterEta).second.clustHistos.hADC_v_Time = make_shared<TH2F>( getHistogram2D((*iterEta).first, -1, aSetup.histoSetup_clustTime, aSetup.histoSetup_clustADC ) );
        
        //Loop Over Stored iPhi Sectors
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over iPhi Sectors
		//Special case for Cluster Position, the number of bins here should be factor 3 less than requested (eta case)
		Timing::HistoSetup setupClustPosPhi	= aSetup.histoSetup_clustPos;
		setupClustPosPhi.iHisto_nBins		= aSetup.iUniformityGranularity;
		setupClustPosPhi.fHisto_xLower		= (*iterPhi).second.fPos_Xlow;
		setupClustPosPhi.fHisto_xUpper 		= (*iterPhi).second.fPos_Xhigh;

            //Initialize iPhi Histograms - 1D
            (*iterPhi).second.clustHistos.hADC = make_shared<TH1F>(getHistogram( (*iterEta).first, (*iterPhi).first, aSetup.histoSetup_clustADC ) );
            (*iterPhi).second.clustHistos.hMulti = make_shared<TH1F>(getHistogram( (*iterEta).first, (*iterPhi).first, aSetup.histoSetup_clustMulti ) );
            (*iterPhi).second.clustHistos.hSize = make_shared<TH1F>(getHistogram( (*iterEta).first, (*iterPhi).first, aSetup.histoSetup_clustSize ) );
            (*iterPhi).second.clustHistos.hTime = make_shared<TH1F>(getHistogram( (*iterEta).first, (*iterPhi).first, aSetup.histoSetup_clustTime ) );
            
            //Initialize iPhi Histograms - 2D
            (*iterPhi).second.clustHistos.hADC_v_Pos = make_shared<TH2F>( getHistogram2D( (*iterEta).first, (*iterPhi).first, setupClustPosPhi, aSetup.histoSetup_clustADC ) );
            (*iterPhi).second.clustHistos.hADC_v_Size = make_shared<TH2F>( getHistogram2D( (*iterEta).first, (*iterPhi).first, aSetup.histoSetup_clustSize, aSetup.histoSetup_clustADC ) );
            (*iterPhi).second.clustHistos.hADC_v_Time = make_shared<TH2F>( getHistogram2D( (*iterEta).first, (*iterPhi).first, aSetup.histoSetup_clustTime, aSetup.histoSetup_clustADC ) );
            
            //Setup the Slices
            for (int i=1; i<= aSetup.iUniformityGranularity; ++i) { //Loop Over Slices
                //Create the slice
                SectorSlice slice;
                
                //Grab ADC spectrum for this slice
                slice.hSlice_ClustADC = make_shared<TH1F>( *( (TH1F*) (*iterPhi).second.clustHistos.hADC_v_Pos->ProjectionY( ("h_iEta" + getString( (*iterEta).first ) + "iPhi" + getString( (*iterPhi).first ) + "Slice" + getString(i) + "_clustADC").c_str(),i,i,"") ) );
                
                //Store position information for this slice
                slice.fPos_Center = (*iterPhi).second.clustHistos.hADC_v_Pos->GetXaxis()->GetBinCenter(i);
                slice.fWidth = (*iterPhi).second.clustHistos.hADC_v_Pos->GetXaxis()->GetBinWidth(i);
                
                //Store the slice
                (*iterPhi).second.map_slices[i] = slice;
            } //End Loop Over Slices
        } //End Loop Over iPhi Sectors
    } //End Loop Over iEta Sectors
    
    return;
} //End AnalyzeResponseUniformityClusters::initHistosClusters()

//Loads a ROOT file previously created by an instance of AnalyzeResponseUniformityClusters
//Loads all TObjects found in the input ROOT file into detMPGD;
//Any previously stored information in detMPGD is lost.
//Takes a std::string which stores the physical filename as input
void AnalyzeResponseUniformityClusters::loadHistosFromFile( std::string & strInputMappingFileName, std::string & strInputROOTFileName ){
    //Variable Declaration
    TFile *ptr_fileInput = nullptr;
    
    //This method will be called when the user wants to re-run the fitting on a previously created batch of histograms
    //The user will directly supply an AMORE mapping file, this will make the DetectorMPGD structure so that it matches the one created when the histograms where first booked
    //The user will indirectly supply an analysis config file because they want to re-run the fits and need to give new information
    //Use previously existing framework code to setup the detector MPGD, then this method behaves as the reverse of storeHistos()
    
    //Setup the MPGD object
    //------------------------------------------------------
    //ParameterLoaderAmoreSRS amoreLoader;
    //amoreLoader.loadAmoreMapping(strInputMappingFileName);
    //detMPGD = amoreLoader.getDetector();
    
    //Open the requested ROOT file
    //------------------------------------------------------
    ptr_fileInput = new TFile(strInputROOTFileName.c_str(),"READ","",1);
    
    //Check to see if data file opened successfully, if so load the tree
    //------------------------------------------------------
    if ( !ptr_fileInput->IsOpen() || ptr_fileInput->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("Uniformity::AnalyzeResponseUniformityClusters::loadHistosFromFile() - error while opening file: " + strInputROOTFileName ).c_str() );
        Timing::printROOTFileStatus(ptr_fileInput);
        std::cout << "Exiting!!!\n";
        
        return;
    } //End Case: failed to load ROOT file
    
    //Call loadHistosFromFile below
    loadHistosFromFile(strInputMappingFileName, ptr_fileInput);
    
    //Close the file
    //------------------------------------------------------
    ptr_fileInput->Close();
    
    return;
} //End AnalyzeResponseUniformityClusters::loadHistosFromFile()

//Loads a ROOT file previously created by an instance of AnalyzeResponseUniformityClusters
//Loads all TObjects found in the input ROOT file into detMPGD;
//Any previously stored information in detMPGD is lost.
//Takes a TFile * which the histograms are written to as input
void AnalyzeResponseUniformityClusters::loadHistosFromFile(std::string & strInputMappingFileName, TFile * file_InputRootFile ){
    //Variable Declaration
    
    //This method will be called when the user wants to re-run the fitting on a previously created batch of histograms
    //The user will directly supply an AMORE mapping file, this will make the DetectorMPGD structure so that it matches the one created when the histograms where first booked
    //The user will indirectly supply an analysis config file because they want to re-run the fits and need to give new information
    //Use previously existing framework code to setup the detector MPGD, then this method behaves as the reverse of storeHistos()
    
    //Setup the MPGD object
    //------------------------------------------------------
    ParameterLoaderAmoreSRS amoreLoader;
    amoreLoader.loadAmoreMapping(strInputMappingFileName);
    detMPGD = amoreLoader.getDetector();
    
    //Check to see if data file opened successfully, if so load the tree
    //------------------------------------------------------
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("Uniformity::AnalyzeResponseUniformityClusters::loadHistosFromFile() - error while opening file: " + (string) file_InputRootFile->GetName() ).c_str() );
        Timing::printROOTFileStatus(file_InputRootFile);
        std::cout << "Exiting!!!\n";
        
        return;
    } //End Case: failed to load ROOT file
    
    //Loop Through the file and load all stored TObjects
    //------------------------------------------------------
    //Loop Over Stored iEta Sectors
    for (auto iterEta = detMPGD.map_sectorsEta.begin(); iterEta != detMPGD.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        
        //Get Directory
        //-------------------------------------
        //Check to see if the directory exists already
        TDirectory *dir_SectorEta = file_InputRootFile->GetDirectory( ( "SectorEta" + getString( (*iterEta).first ) ).c_str(), false, "GetDirectory" );
        
        //If the above pointer is null the directory does NOT exist, skip this Eta Sector
        if (dir_SectorEta == nullptr) continue;
        
        //Debugging
        //cout<<"dir_SectorEta->GetName() = " << dir_SectorEta->GetName()<<endl;
        
        //Load Histograms - SectorEta Level
        //-------------------------------------
        dir_SectorEta->cd();
        (*iterEta).second.clustHistos.hADC = make_shared<TH1F>( *((TH1F*) dir_SectorEta->Get( ("h_iEta" + getString( (*iterEta).first ) +  "_clustADC").c_str() ) ) );
        (*iterEta).second.clustHistos.hPos = make_shared<TH1F>( *((TH1F*) dir_SectorEta->Get( ("h_iEta" + getString( (*iterEta).first ) +  "_clustPos").c_str() ) ) );
        (*iterEta).second.clustHistos.hSize = make_shared<TH1F>( *((TH1F*) dir_SectorEta->Get( ("h_iEta" + getString( (*iterEta).first ) +  "_clustSize").c_str() ) ) );
        (*iterEta).second.clustHistos.hTime = make_shared<TH1F>( *((TH1F*) dir_SectorEta->Get( ("h_iEta" + getString( (*iterEta).first ) +  "_clustTime").c_str() ) ) );
        (*iterEta).second.clustHistos.hADC_v_Pos    = make_shared<TH2F>( *((TH2F*) dir_SectorEta->Get( ("h_iEta" + getString( (*iterEta).first ) +  "_clustADC_v_clustPos").c_str() ) ) );
        (*iterEta).second.clustHistos.hADC_v_Size   = make_shared<TH2F>( *((TH2F*) dir_SectorEta->Get( ("h_iEta" + getString( (*iterEta).first ) +  "_clustADC_v_clustSize").c_str() ) ) );

        //Loop Over Stored iPhi Sectors within this iEta Sector
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over Stored iPhi Sectors
            //Get Directory
            //-------------------------------------
            //Check to see if the directory exists already
            TDirectory *dir_SectorPhi = dir_SectorEta->GetDirectory( ( "SectorPhi" + getString( (*iterPhi).first ) ).c_str(), false, "GetDirectory"  );
            
            //If the above pointer is null the directory does NOT exist, skip this Phi Sector
            if (dir_SectorPhi == nullptr) continue;
            
            //Debugging
            //cout<<"dir_SectorPhi->GetName() = " << dir_SectorPhi->GetName()<<endl;
            
            //Load Histograms - SectorPhi Level
            //-------------------------------------
            dir_SectorPhi->cd();
            (*iterPhi).second.clustHistos.hADC = make_shared<TH1F>( *((TH1F*) dir_SectorPhi->Get( ("h_iEta" + getString( (*iterEta).first ) + "iPhi" + getString( (*iterPhi).first ) + "_clustADC").c_str() ) ) );
            (*iterPhi).second.clustHistos.hSize = make_shared<TH1F>( *((TH1F*) dir_SectorPhi->Get( ("h_iEta" + getString( (*iterEta).first ) + "iPhi" + getString( (*iterPhi).first ) + "_clustSize").c_str() ) ) );
            (*iterPhi).second.clustHistos.hTime = make_shared<TH1F>( *((TH1F*) dir_SectorPhi->Get( ("h_iEta" + getString( (*iterEta).first ) + "iPhi" + getString( (*iterPhi).first ) + "_clustTime").c_str() ) ) );
            (*iterPhi).second.clustHistos.hADC_v_Pos    = make_shared<TH2F>( *((TH2F*) dir_SectorPhi->Get( ("h_iEta" + getString( (*iterEta).first ) + "iPhi" + getString( (*iterPhi).first ) + "_clustADC_v_clustPos").c_str() ) ) );
            (*iterPhi).second.clustHistos.hADC_v_Size   = make_shared<TH2F>( *((TH2F*) dir_SectorPhi->Get( ("h_iEta" + getString( (*iterEta).first ) + "iPhi" + getString( (*iterPhi).first ) + "_clustADC_v_clustSize").c_str() ) ) );
            
            //Check to see if 2D histo retrieved successfully
            if ( (*iterPhi).second.clustHistos.hADC_v_Pos == nullptr) continue;
            
            //Set to Global Directory - SectorPhi Level
            //-------------------------------------
            //Prevents seg faulting when closing ptr_fileInput
            (*iterPhi).second.clustHistos.hADC_v_Pos->SetDirectory(gROOT);
            
            //Slices
            //Trickery, detMPGD now has eta and phi sectors setup.
            //Slices are NOT setup
            //Here loop from 1 to aSetup.iUniformityGranularity and either:
            //      Option 1: Load slices from the file (slower?)
            //      Option 2: project out from SectorPhi::clustHistos::hADC_v_Pos
            //Implemented Option 2; faster many large number of I/O operations??
            for (int i=1; i <= aSetup.iUniformityGranularity; ++i ) { //Loop Over Slices
                //Set Histograms - Slice Level
                //-------------------------------------
                //Creat the slice
                SectorSlice slice;
                
                slice.hSlice_ClustADC = make_shared<TH1F>( *( (TH1F*) (*iterPhi).second.clustHistos.hADC_v_Pos->ProjectionY( ("h_iEta" + getString( (*iterEta).first ) + "iPhi" + getString( (*iterPhi).first ) + "Slice" + getString(i) + "_clustADC").c_str(),i,i,"") ) );
                
                //Make sure to set this histo to the global directory
                slice.hSlice_ClustADC->SetDirectory(gROOT);
                
                //Store position information for this slice
                slice.fPos_Center = (*iterPhi).second.clustHistos.hADC_v_Pos->GetXaxis()->GetBinCenter(i);
                slice.fWidth = (*iterPhi).second.clustHistos.hADC_v_Pos->GetXaxis()->GetBinWidth(i);
                
                //Store the slice
                (*iterPhi).second.map_slices[i] = slice;
            } //End Loop Over Slices
        } //End Loop Over Stored iPhi Sectors
    } //End Loop Over Stored iEta Sectors

    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End AnalyzeResponseUniformityClusters::loadHistosFromFile()

//Stores booked histograms (for those histograms that are non-null)
//Takes a std::string which stores the physical filename as input
void AnalyzeResponseUniformityClusters::storeHistos( string & strOutputROOTFileName, std::string strOption ){
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("AnalyzeResponseUniformityClusters","storeHistos","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("AnalyzeResponseUniformityClusters","storeHistos", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("AnalyzeResponseUniformityClusters","storeHistos", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Call the store histos sequence
    storeHistos(ptr_fileOutput);
    
    //Close the ROOT file
    ptr_fileOutput->Close();
    
    return;
} //End storeHistos()

//Stores booked histograms (for those histograms that are non-null)
//Takes a TFile * which the histograms are written to as input
void AnalyzeResponseUniformityClusters::storeHistos( TFile * file_InputRootFile){
    //Variable Declaration
    
	//cout<<"AnalyzeResponseUniformityClusters::storeHistos() - file_InputRootFile = " << file_InputRootFile << endl;

    //Check if File Failed to Open Correctly
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("AnalyzeResponseUniformityClusters","storeHistos","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("AnalyzeResponseUniformityClusters","storeHistos", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("AnalyzeResponseUniformityClusters","storeHistos", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Loop over ieta's
    //Create/Load file structure
    //Store ieta level histograms
    //Loop over iphi's within ieta's
    //Create/Load file structure
    //Store iphi level histograms
    //Loop over slices
    //Create/Load file structure
    //store slice level histograms
    //Close File
    
    //Setup the summary histograms
    TH1F hclustADC_All( getHistogram(-1, -1, aSetup.histoSetup_clustADC) );
    TH1F hclustPos_All( getHistogram(-1, -1, aSetup.histoSetup_clustPos) );
    TH1F hclustSize_All( getHistogram(-1, -1, aSetup.histoSetup_clustSize) );
    TH1F hclustTime_All( getHistogram(-1, -1, aSetup.histoSetup_clustTime) );
    
    //Get/Make the Summary Directory
    //Check to see if the directory exists already
    TDirectory *dir_Summary = file_InputRootFile->GetDirectory("Summary", false, "GetDirectory" );
    
    //If the above pointer is null the directory does NOT exist, create it
    if (dir_Summary == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_Summary = file_InputRootFile->mkdir("Summary");
    } //End Case: Directory did not exist in file, CREATE
    
    //Loop Over Stored iEta Sectors
    for (auto iterEta = detMPGD.map_sectorsEta.begin(); iterEta != detMPGD.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        
        //Get Directory
        //-------------------------------------
        //Check to see if the directory exists already
        TDirectory *dir_SectorEta = file_InputRootFile->GetDirectory( ( "SectorEta" + getString( (*iterEta).first ) ).c_str(), false, "GetDirectory" );
        
        //If the above pointer is null the directory does NOT exist, create it
        if (dir_SectorEta == nullptr) { //Case: Directory did not exist in file, CREATE
            dir_SectorEta = file_InputRootFile->mkdir( ( "SectorEta" + getString( (*iterEta).first ) ).c_str() );
        } //End Case: Directory did not exist in file, CREATE
        
        //Debugging
        //cout<<"dir_SectorEta->GetName() = " << dir_SectorEta->GetName()<<endl;
        
        //Add this sector to the summary histogram
        hclustADC_All.Add((*iterEta).second.clustHistos.hADC.get() );	//cout<<"AnalyzeResponseUniformityClusters::storeHistos() - (*iterEta).second.clustHistos.hADC = " << (*iterEta).second.clustHistos.hADC << endl;
        hclustPos_All.Add((*iterEta).second.clustHistos.hPos.get() );	//cout<<"AnalyzeResponseUniformityClusters::storeHistos() - (*iterEta).second.clustHistos.hPos = " << (*iterEta).second.clustHistos.hPos << endl;
        hclustSize_All.Add((*iterEta).second.clustHistos.hSize.get() );	//cout<<"AnalyzeResponseUniformityClusters::storeHistos() - (*iterEta).second.clustHistos.hSize = " << (*iterEta).second.clustHistos.hSize << endl;
        hclustTime_All.Add((*iterEta).second.clustHistos.hTime.get() );	//cout<<"AnalyzeResponseUniformityClusters::storeHistos() - (*iterEta).second.clustHistos.hTime = " << (*iterEta).second.clustHistos.hTime << endl;
        
        //Store Histograms - SectorEta Level
        //-------------------------------------
        dir_SectorEta->cd();
        (*iterEta).second.clustHistos.hADC->Write();
        (*iterEta).second.clustHistos.hPos->Write();
        (*iterEta).second.clustHistos.hSize->Write();
        (*iterEta).second.clustHistos.hTime->Write();
        (*iterEta).second.clustHistos.hADC_v_Pos->Write();
        (*iterEta).second.clustHistos.hADC_v_Size->Write();
        
        //Loop Over Stored iPhi Sectors within this iEta Sector
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over Stored iPhi Sectors
            //Get Directory
            //-------------------------------------
            //Check to see if the directory exists already
            TDirectory *dir_SectorPhi = dir_SectorEta->GetDirectory( ( "SectorPhi" + getString( (*iterPhi).first ) ).c_str(), false, "GetDirectory"  );
            
            //If the above pointer is null the directory does NOT exist, create it
            if (dir_SectorPhi == nullptr) { //Case: Directory did not exist in file, CREATE
                dir_SectorPhi = dir_SectorEta->mkdir( ( "SectorPhi" + getString( (*iterPhi).first ) ).c_str() );
            } //End Case: Directory did not exist in file, CREATE
            
            //Debugging
            //cout<<"dir_SectorPhi->GetName() = " << dir_SectorPhi->GetName()<<endl;
            
            //Store Histograms - SectorPhi Level
            //-------------------------------------
            dir_SectorPhi->cd();
            (*iterPhi).second.clustHistos.hADC->Write();
            (*iterPhi).second.clustHistos.hSize->Write();
            (*iterPhi).second.clustHistos.hTime->Write();
            (*iterPhi).second.clustHistos.hADC_v_Pos->Write();
            (*iterPhi).second.clustHistos.hADC_v_Size->Write();
            
            //Slices
            //Now that all clusters have been analyzed we extract the slices
            for (auto iterSlice = (*iterPhi).second.map_slices.begin(); iterSlice != (*iterPhi).second.map_slices.end(); ++iterSlice ) { //Loop Over Slices
                //Get Directory
                //-------------------------------------
                //Check to see if the directory exists already
                TDirectory *dir_Slice = dir_SectorPhi->GetDirectory( ( "Slice" + getString( (*iterSlice).first ) ).c_str(), false, "GetDirectory"  );
                
                //If the above pointer is null the directory does NOT exist, create it
                if (dir_Slice == nullptr) { //Case: Directory did not exist in file, CREATE
                    dir_Slice = dir_SectorPhi->mkdir( ( "Slice" + getString( (*iterSlice).first ) ).c_str() );
                } //End Case: Directory did not exist in file, CREATE
                
                //Store Histograms - Slice Level
                //-------------------------------------
                dir_Slice->cd();
                (*iterSlice).second.hSlice_ClustADC->Write();
            } //End Loop Over Slices
        } //End Loop Over Stored iPhi Sectors
    } //End Loop Over Stored iEta Sectors
    
    //Store the Summary Histograms
    dir_Summary->cd();
    hclustADC_All.Write();
    hclustPos_All.Write();
    hclustSize_All.Write();
    hclustTime_All.Write();
    
    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End storeHistos()

//Stores booked cluster fits (for those fits that are non-null)
//Takes a std::string which stores the physical filename as input
void AnalyzeResponseUniformityClusters::storeFits( string & strOutputROOTFileName, std::string strOption ){
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("AnalyzeResponseUniformityClusters","storeFits","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("AnalyzeResponseUniformityClusters","storeFits", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("AnalyzeResponseUniformityClusters","storeFits", "\tExiting; No Fits have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Call the store fits sequence
    storeFits(ptr_fileOutput);
    
    //Close the ROOT file
    ptr_fileOutput->Close();
    
    return;
} //End storeHistos()

//Stores booked cluster fits (for those fits that are non-null)
//Takes a TFile * which the histograms are written to as input
void AnalyzeResponseUniformityClusters::storeFits( TFile * file_InputRootFile){
    //Variable Declaration
    
    //Check if File Failed to Open Correctly
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("AnalyzeResponseUniformityClusters","storeFits","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("AnalyzeResponseUniformityClusters","storeFits", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("AnalyzeResponseUniformityClusters","storeFits", "\tExiting; No Fits have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Loop Over Stored iEta Sectors
    for (auto iterEta = detMPGD.map_sectorsEta.begin(); iterEta != detMPGD.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        
        //Get Directory
        //-------------------------------------
        //Check to see if the directory exists already
        TDirectory *dir_SectorEta = file_InputRootFile->GetDirectory( ( "SectorEta" + getString( (*iterEta).first ) ).c_str(), false, "GetDirectory" );
        
        //If the above pointer is null the directory does NOT exist, create it
        if (dir_SectorEta == nullptr) { //Case: Directory did not exist in file, CREATE
            dir_SectorEta = file_InputRootFile->mkdir( ( "SectorEta" + getString( (*iterEta).first ) ).c_str() );
        } //End Case: Directory did not exist in file, CREATE
        
        //Debugging
        //cout<<"dir_SectorEta->GetName() = " << dir_SectorEta->GetName()<<endl;
        
        //Store Fits - SectorEta Level
        //-------------------------------------
        dir_SectorEta->cd();
        (*iterEta).second.gEta_ClustADC_Fit_NormChi2->Write();
        (*iterEta).second.gEta_ClustADC_Fit_PkPos->Write();
        (*iterEta).second.gEta_ClustADC_Fit_PkRes->Write();
        (*iterEta).second.gEta_ClustADC_Fit_Failures->Write();
        
        (*iterEta).second.gEta_ClustADC_Spec_NumPks->Write();
        (*iterEta).second.gEta_ClustADC_Spec_PkPos->Write();
        
        //Loop Over Stored iPhi Sectors within this iEta Sector
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over Stored iPhi Sectors
            //Get Directory
            //-------------------------------------
            //Check to see if the directory exists already
            TDirectory *dir_SectorPhi = dir_SectorEta->GetDirectory( ( "SectorPhi" + getString( (*iterPhi).first ) ).c_str(), false, "GetDirectory"  );
            
            //If the above pointer is null the directory does NOT exist, create it
            if (dir_SectorPhi == nullptr) { //Case: Directory did not exist in file, CREATE
                dir_SectorPhi = dir_SectorEta->mkdir( ( "SectorPhi" + getString( (*iterPhi).first ) ).c_str() );
            } //End Case: Directory did not exist in file, CREATE
            
            //Debugging
            //cout<<"dir_SectorPhi->GetName() = " << dir_SectorPhi->GetName()<<endl;
            
            //Store Fits - SectorPhi Level
            //-------------------------------------
            dir_SectorPhi->cd();
            
            //No Fits defined at this level - yet

            //Slices
            //Now that all clusters have been analyzed we extract the slices
            for (auto iterSlice = (*iterPhi).second.map_slices.begin(); iterSlice != (*iterPhi).second.map_slices.end(); ++iterSlice ) { //Loop Over Slices
                
                //Get Directory
                //-------------------------------------
                //Check to see if the directory exists already
                TDirectory *dir_Slice = dir_SectorPhi->GetDirectory( ( "Slice" + getString( (*iterSlice).first ) ).c_str(), false, "GetDirectory"  );
                
                //If the above pointer is null the directory does NOT exist, create it
                if (dir_Slice == nullptr) { //Case: Directory did not exist in file, CREATE
                    dir_Slice = dir_SectorPhi->mkdir( ( "Slice" + getString( (*iterSlice).first ) ).c_str() );
                } //End Case: Directory did not exist in file, CREATE
                
                //Store Fits - Slice Level
                //-------------------------------------
                dir_Slice->cd();
                (*iterSlice).second.fitSlice_ClustADC->Write();
            } //End Loop Over Slices
        } //End Loop Over Stored iPhi Sectors
    } //End Loop Over Stored iEta Sectors
    
    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End storeHistos()

