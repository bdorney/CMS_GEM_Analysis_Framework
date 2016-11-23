//
//  AnalyzeTiming.cpp
//
//
//  Created by Brian L Dorney on 21/11/16.
//
//

#include "AnalyzeTiming.h"

using std::cin;
using std::cout;
using std::endl;
using std::make_shared;
using std::map;
using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;

using QualityControl::Plotter::InfoFit;

using QualityControl::Timing::imap_iter_t;

using QualityControl::Uniformity::printClassMethodMsg;

using namespace QualityControl::Timing;

//Fills Histograms for all events
void AnalyzeTiming::fillHistos(std::vector<Timing::EventReco> vec_inputEvtsReco){
    //Variable Declaration
    int iEvt = 0;
    int iMultiChan = 0;	//Multiplicity Counters
    int iMultiDet = 0;
    
    //Debugging
    //cout<<"AnalyzeTiming::fillHistos() - vec_inputEvtsReco.size() = " << vec_inputEvtsReco.size() << endl;
    
    for (auto iterEvt = vec_inputEvtsReco.begin(); iterEvt != vec_inputEvtsReco.end(); ++iterEvt) { //Loop Over Events
        if (iEvt % 1000 == 0) cout<<"Filling Histos; " <<iEvt<<" Events Processed\n";
        
        //Fill Event Multiplicity
        m_mtrxHistos.m_hMultiDUT->Fill( (*iterEvt).m_detMatrix_DUTs.m_map_detectors.size() );
        m_mtrxHistos.m_hMultiTrig->Fill( (*iterEvt).m_detMatrix_Trigger.m_map_detectors.size() );
        
        //Loop Through Superchambers - DUT's
        for (auto iterSC = (*iterEvt).m_detMatrix_DUTs.begin(); iterSC != (*iterEvt).m_detMatrix_DUTs.end(); ++iterSC) { //Loop Over Superchambers
            //Fill Superchamber Delta
            if ( (*iterSC).second.getDetectorDelta() != 0 ) {
                m_mtrxHistos[(*iterSC).first].m_hDelta->Fill( (*iterSC).second.getDetectorDelta() );
            }
            
            //Fill Superchamber Detector Multiplicity
            iMultiDet = 0;
            (*iterSC).second.hasData(iMultiDet);
            m_mtrxHistos[(*iterSC).first].m_hMulti->Fill( iMultiDet );
            
            //Fill Superchamber OR
            if ( (*iterSC).second.getDetectorOR() > 0 ) {
                m_mtrxHistos[(*iterSC).first].m_hOR->Fill( (*iterSC).second.getDetectorOR() );
            }
            
            if ((*iterSC).second.getNDetectors() > 1 && (*iterSC).second.getDetectorDelta() < 12.5 ) { //Case: Superchamber has more than one detector
                //Fill Superchamber AND
                if ( (*iterSC).second.getDetectorAND() > 0 ) {
                    m_mtrxHistos[(*iterSC).first].m_hAND->Fill( (*iterSC).second.getDetectorAND() );
                }
                
                //Fill Superchamber Correlation
                double dDet0 = (*iterSC).second.getDetector(0).getChannelOR();
                double dDet1 = (*iterSC).second.getDetector(1).getChannelOR();
                m_mtrxHistos[(*iterSC).first].m_hCorr->Fill(dDet0, dDet1);
            } //End Case: Superchamber has more than one detector
            
            //Loop Over (*iterSC).second's Detectors
            for (auto iterDet = (*iterSC).second.getDetectorPtrBegin(); iterDet != (*iterSC).second.getDetectorPtrEnd(); ++iterDet) {
                //Fill Detector Channel Multiplicity
                iMultiChan = 0;
                (*iterDet).second.hasData(iMultiChan);
                m_mtrxHistos[(*iterSC).first].m_map_HistosDet[(*iterDet).second.getName()].m_hMulti->Fill( iMultiChan );
                
                for (auto iterChan = (*iterDet).second.getChanDataBegin(); iterChan != (*iterDet).second.getChanDataEnd(); ++iterChan) { //Loop Over Detector's Channels
                    
                    //Skip Empty Channels
                    if ( !( (*iterChan).second > 0) ) {
                        continue;
                    }
                    
                    //cout<<(*iterChan).first<<"\t"<<(*iterChan).second<<endl;
                    
                    //Fill Superchamber
                    m_mtrxHistos[(*iterSC).first].m_hAll->Fill( (*iterChan).second );
                    
                    //Fill Detector
                    m_mtrxHistos[(*iterSC).first].m_map_HistosDet[(*iterDet).second.getName()].m_hAll->Fill( (*iterChan).second );
                    
                    //Debugging
                    //cout<<"m_mtrxHistos[("<<(*iterSC).first.first<<","<<(*iterSC).first.second<<")].m_map_HistosDet["<<(*iterDet).second.getName().c_str()<<"].m_map_hChan["<<(*iterChan).first<<"] = " << m_mtrxHistos[(*iterSC).first].m_map_HistosDet[(*iterDet).second.getName()].m_map_hChan[(*iterChan).first] << endl;
                    
                    m_mtrxHistos[(*iterSC).first].m_map_HistosDet[(*iterDet).second.getName()].m_map_hChan[(*iterChan).first]->Fill( (*iterChan).second );
                }  //End Loop Over Detector's Channels
            } //End Loop Over Detectors
        } //End Loop Over Superchambers - DUT's
        
        //Loop Through Superchambers - Trig
        for (auto iterSC = (*iterEvt).m_detMatrix_Trigger.begin(); iterSC != (*iterEvt).m_detMatrix_Trigger.end(); ++iterSC) { //Loop Over Superchambers
            
            //Loop Over (*iterSC).second's Detectors
            for (auto iterDet = (*iterSC).second.getDetectorPtrBegin(); iterDet != (*iterSC).second.getDetectorPtrEnd(); ++iterDet) {
                
                for (auto iterChan = (*iterDet).second.getChanDataBegin(); iterChan != (*iterDet).second.getChanDataEnd(); ++iterChan) { //Loop Over Detector's Channels
                    
                    //Skip Empty Channels
                    if ( !( (*iterChan).second > 0) ) {
                        continue;
                    }
                    
                    //Debugging
                    //cout<<(*iterChan).first<<"\t"<<(*iterChan).second<<endl;
                    
                    //Fill Superchamber
                    m_mtrxHistos[(*iterSC).first].m_hAll->Fill( (*iterChan).second );
                    
                    //Fill Detector
                    m_mtrxHistos[(*iterSC).first].m_map_HistosDet[(*iterDet).second.getName()].m_hAll->Fill( (*iterChan).second );
                    
                    m_mtrxHistos[(*iterSC).first].m_map_HistosDet[(*iterDet).second.getName()].m_map_hChan[(*iterChan).first]->Fill( (*iterChan).second );
                }  //End Loop Over Detector's Channels
            } //End Loop Over Detectors
        } //End Loop Over Superchambers - Trig
        
        ++iEvt;
    } //End Loop Over Events
    
    return;
} //End AnalyzeTiming::fillHistos()

//Fits histograms found in m_mtrxHistos
void AnalyzeTiming::fitHistos(){
    //Variable Declaration
    Double_t *dPeakPos;
    
    float fMin = -1e12, fMax = 1e12;
    float fNormChi2;
    float fPkPos = 0, fPkPosErr = 0;        //Peak Position
    float fPkWidth = 0, fPkWidthErr = 0;    //Peak Width
    
    int iBinMin, iBinMax;	//Bins in histogram encapsulating fMin to fMax
    int iIdxPk, iIdxWidth;	//Position in fit parameter meaning container of the peak and the width parameters
    
    TSpectrum specTime(1,2);    //One peak; 2 sigma away from any other peak
    
    vector<float> vec_fFitRange;
    
    //Loop Over the Histogram Matrix and store All histos
    //-------------------------------------
    //Level - Superchamber
    for (auto iterSC = m_mtrxHistos.m_map_HistosSC_All.begin(); iterSC != m_mtrxHistos.m_map_HistosSC_All.end(); ++iterSC) { //Loop Over Superchamber Histos
        
        //Level - Detector
        for (auto iterDet = (*iterSC).second.m_map_HistosDet.begin(); iterDet != (*iterSC).second.m_map_HistosDet.end(); ++iterDet) { //Loop Over this Superchambers' Detector Histos
            
            //Level - Channel
            for (auto iterChan = (*iterDet).second.m_map_hChan.begin(); iterChan != (*iterDet).second.m_map_hChan.end(); ++iterChan) { //Loop Over this Detector's Channel Histos
                cout<<"=======================================================================\n";
                cout<<"Attempting to Fit (DET, CHAN) = (" << (*iterDet).first << ", " << (*iterChan).first << ")\n";
                
                //Check if the histogram exists, skip it if it doesn't
                if ( (*iterChan).second == nullptr ) continue;
                
                //Check if the histogram has entries, skip it if it doesn't
                if ( (*iterChan).second->GetEntries() > 0) continue;
                
                //Find peak & store it's position
                specTime.Search( (*iterChan).second.get(), 2, "nobackground", 0.5 );
                dPeakPos = specTime.GetPositionX();
                
                //Initialize Fit
                (*iterDet).second.m_map_fitChan[(*iterChan).first] = make_shared<TF1>( getFit( (*iterChan).first,"", aSetupTiming.m_fitSetup_timeRes, (*iterChan).second, specTime) );
                
                //Clear the calculated fit range from the previous channel
                vec_fFitRange.clear();
                for (auto iterRange = aSetupTiming.m_fitSetup_timeRes.m_vec_strFit_Range.begin(); iterRange != aSetupTiming.m_fitSetup_timeRes.m_vec_strFit_Range.end(); ++iterRange) { //Loop Over Fit Range
                    vec_fFitRange.push_back( getParsedInput( (*iterRange), (*iterChan).second, specTime ) );
                } //End Loop Over Fit Range
                
                //Perform Fit & Store the Result
                TFitResult fitRes_Time;
                
                TVirtualFitter::SetMaxIterations(10000);
                
                if (vec_fFitRange.size() > 1) { //Case: Fit within the user specific range
                    fMin = (*std::min_element(vec_fFitRange.begin(), vec_fFitRange.end() ) );
                    fMax = (*std::max_element(vec_fFitRange.begin(), vec_fFitRange.end() ) );
                    
                    iBinMin = std::floor( (fMin - aSetupTiming.m_hSetupTime.fHisto_xLower) / aSetupTiming.m_hSetupTime.fHisto_BinWidth );
                    iBinMax = std::ceil( (fMax - aSetupTiming.m_hSetupTime.fHisto_xLower) / aSetupTiming.m_hSetupTime.fHisto_BinWidth ) + 1;
                    
                    //Skip this histo if integral over fit range is zero
                    if( !( (*iterChan).second->Integral(iBinMin, iBinMax) > 0 ) ){
                        (*iterDet).second.m_map_fitChan[(*iterChan).first].reset();
                        continue;
                    }
                    
                    fitRes_Time = *((*iterChan).second->Fit( (*iterDet).second.m_map_fitChan[(*iterChan).first].get(),aSetupTiming.m_fitSetup_timeRes.m_strFit_Option.c_str(),"", fMin, fMax) );
                } //End Case: Fit within the user specific range
                else{ //Case: No range to use
                    fitRes_Time = *((*iterChan).second->Fit( (*iterDet).second.m_map_fitChan[(*iterChan).first].get(),aSetupTiming.m_fitSetup_timeRes.m_strFit_Option.c_str(),"") );
                } //End Case: No range to use
                
                
            } //End Loop Over this Detector's Channel Histos
        } //End Loop Over this Superchambers' Detector Histos
    } //End Loop Over Superchamber Histos
    
    return;
} //End AnalyzeTiming::fitHistos()

//Initializes all TH1F objects for all detectors in a test stand
//Wipes any existing histograms
void AnalyzeTiming::initHistos(Timing::DetectorMatrix detMatrix){
    //void AnalyzeTiming::initHistos(Timing::TestStandVME testStand){
    
    //cout<<"=======AnalyzeTiming::initHistos()=======\n";
    
    m_mtrxHistos.m_map_HistosSC_All.clear();
    
    //Determine the largest full scale range
    //=======================================
    float fVME_LSB = -1;    //Least Sensitive Bit
    int iBitADC = 0;        //Bits in the ADC
    for (auto iterVMEBoard = m_daqSetup.m_map_vmeBoards.begin(); iterVMEBoard != m_daqSetup.m_map_vmeBoards.end(); ++iterVMEBoard) { //Loop Over Boards
        
        //Store the *worst* least sentive bit from all boards
        if ( (*iterVMEBoard).second.getLSB() >  fVME_LSB) { //Case: LSB is larger
            fVME_LSB = (*iterVMEBoard).second.getLSB();
            iBitADC = (*iterVMEBoard).second.m_iBitADC;
        } //End Case: LSB is larger
    } //End Loop Over Boards
    
    //Setup Timing Histo Containers
    //=======================================
    aSetupTiming.m_hSetupTime.fHisto_BinWidth = fVME_LSB;
    aSetupTiming.m_hSetupTime.fHisto_xLower = 0;
    aSetupTiming.m_hSetupTime.fHisto_xUpper = fVME_LSB * pow(2, iBitADC);
    aSetupTiming.m_hSetupTime.iHisto_nBins = pow(2, iBitADC)-1;
    aSetupTiming.m_hSetupTime.strHisto_Title_X = "Time #left(ns#right)";
    aSetupTiming.m_hSetupTime.strHisto_Title_Y = "N #left(" + getString(std::ceil( fVME_LSB*1.e3) / 1.e3 ) + " ns#right)"; //Should round to thousandths place
    
    //Initialize Histograms - Event Level
    //=======================================
    aSetupTiming.m_hSetupMulti.fHisto_BinWidth = 1;
    aSetupTiming.m_hSetupMulti.fHisto_xLower = 0;
    aSetupTiming.m_hSetupMulti.fHisto_xUpper = 15;
    aSetupTiming.m_hSetupMulti.iHisto_nBins = 15;
    aSetupTiming.m_hSetupMulti.strHisto_Name = "SuperchamberMultiDUT";
    aSetupTiming.m_hSetupMulti.strHisto_Title_X = "Multiplicity";
    aSetupTiming.m_hSetupMulti.strHisto_Title_Y = "N";
    
    m_mtrxHistos.m_hMultiDUT = make_shared<TH1F>( Analyzer::getHistogram(aSetupTiming.m_hSetupMulti) );
    
    aSetupTiming.m_hSetupMulti.strHisto_Name = "SuperchamberMultiTrig";
    m_mtrxHistos.m_hMultiTrig = make_shared<TH1F>( Analyzer::getHistogram(aSetupTiming.m_hSetupMulti) );
    
    //Initialize Histograms - Superchamber/Detector Level
    //=======================================
    for (auto iterSC = detMatrix.begin(); iterSC != detMatrix.end(); ++iterSC) { //Loop Over Superchambers
        //Declare a new HistoSC Container & create TH1F objects for this superchamber
        HistoSC superChamberHistos;
        superChamberHistos.m_strName = (*iterSC).second.getName();
        
        //Channel histograms
        aSetupTiming.m_hSetupTime.strHisto_Name = (*iterSC).second.getName(); //Set this superchambers name
        superChamberHistos.m_hAll = make_shared<TH1F>( getHistogram(-1, "",  aSetupTiming.m_hSetupTime) );
        superChamberHistos.m_hAND = make_shared<TH1F>( getHistogram(-1, "AND",  aSetupTiming.m_hSetupTime) );
        superChamberHistos.m_hDelta = make_shared<TH1F>( getHistogram(-1, "Delta",  aSetupTiming.m_hSetupTime) );
        superChamberHistos.m_hOR = make_shared<TH1F>( getHistogram(-1, "OR",  aSetupTiming.m_hSetupTime) );
        
        superChamberHistos.m_hCorr = make_shared<TH2F>( Analyzer::getHistogram2D(aSetupTiming.m_hSetupTime, aSetupTiming.m_hSetupTime) );
        
        //Multiplicity Histograms
        aSetupTiming.m_hSetupMulti.strHisto_Name = (*iterSC).second.getName() + "DetMulti";
        aSetupTiming.m_hSetupMulti.fHisto_xUpper = (*iterSC).second.getNDetectors()+1;
        aSetupTiming.m_hSetupMulti.iHisto_nBins = (*iterSC).second.getNDetectors()+1;
        superChamberHistos.m_hMulti = make_shared<TH1F>( getHistogram(-1, "",  aSetupTiming.m_hSetupMulti) );
        
        //Special Formatting Cases
        superChamberHistos.m_hDelta->SetXTitle("#Deltat #left(ns#right)");
        superChamberHistos.m_hCorr->SetName( ( superChamberHistos.m_strName + "DetCorr").c_str() );
        
        //Loop Over all Detectors in this Superchamber
        for (auto iterDet = (*iterSC).second.getDetectorPtrBegin(); iterDet != (*iterSC).second.getDetectorPtrEnd(); ++iterDet) {
            //Get the channel map for this detector
            pair<imap_iter_t, imap_iter_t> pair_chanMapIter = (*iterDet).second.getChannelMapVME2Det();
            
            //Declare a new HistoDet container & create TH1F objects for this detector
            HistoDet detectorHistos;
            
            aSetupTiming.m_hSetupTime.strHisto_Name = (*iterDet).second.getName(); //Set this detector's name
            detectorHistos.m_hAll = make_shared<TH1F>( getHistogram(-1, "",  aSetupTiming.m_hSetupTime) );
            
            aSetupTiming.m_hSetupMulti.strHisto_Name = (*iterDet).second.getName() + "ChMulti";
            aSetupTiming.m_hSetupMulti.fHisto_xUpper = (*iterDet).second.getNChan();
            aSetupTiming.m_hSetupMulti.iHisto_nBins = (*iterDet).second.getNChan();
            detectorHistos.m_hMulti = make_shared<TH1F>( getHistogram(-1, "",  aSetupTiming.m_hSetupMulti) );
            
            //cout<<(*iterSC).first.first<<"\t"<<(*iterSC).first.second<<"\t"<<(*iterSC).second.getName()<<"\t"<<(*iterDet).second.getName()<<"\t"<<(*iterDet).second.getNChan()<<endl;
            
            //Make Histograms for each channel
            for (auto iterChan = pair_chanMapIter.first; iterChan != pair_chanMapIter.second; ++iterChan) { //Loop Over Detector's Channels
                detectorHistos.m_map_hChan[(*iterChan).second] = make_shared<TH1F>( getHistogram((*iterChan).second, "",  aSetupTiming.m_hSetupTime) );
                
                //cout<<(*iterSC).first.first<<"\t"<<(*iterSC).first.second<<"\t"<<(*iterSC).second.getName()<<"\t"<<(*iterDet).second.getName()<<"\t"<<(*iterChan).first<<endl;
            } //End Loop Over Detector's Channels
            
            superChamberHistos.m_map_HistosDet[(*iterDet).second.getName()]=detectorHistos;
        } //End Loop Over Detector
        
        m_mtrxHistos.m_map_HistosSC_All[(*iterSC).first]=superChamberHistos;
    } //End Loop Over Superchambers
    
    return;
} //End AnalyzeTiming::initHistos()

//Stores booked fits (for those fits that are non-null)
//Takes a std::string which stores the physical filename as input
void AnalyzeTiming::storeFits( string & strOutputROOTFileName, std::string strOption){
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("AnalyzeTiming","storeFits","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("AnalyzeTiming","storeFits", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("AnalyzeTiming","storeFits", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Call the store histos sequence
    storeFits(ptr_fileOutput);
    
    //Close the ROOT file
    ptr_fileOutput->Close();
    
    return;
} //End storeFits()

//Stores booked fits (for those histograms that are non-null)
//Takes a TFile * which the fits are written to as input
void AnalyzeTiming::storeFits(TFile * file_InputRootFile){
    //Check if File Failed to Open Correctly
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("AnalyzeTiming","storeFits","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("AnalyzeTiming","storeFits", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("AnalyzeTiming","storeFits", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Get Base Directories
    //-------------------------------------
    //Check to see if dir_Summary directory exists already, if not create it
    TDirectory *dir_Summary = file_InputRootFile->GetDirectory("Summary", false, "GetDirectory" );
    if (dir_Summary == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_Summary = file_InputRootFile->mkdir("Summary");
    } //End Case: Directory did not exist in file, CREATE
    
    //Check to see if dir_TimeAbs directory exists already, if not create it
    TDirectory *dir_TimeAbs = file_InputRootFile->GetDirectory("TimeAbsolute", false, "GetDirectory" );
    if (dir_TimeAbs == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_TimeAbs = file_InputRootFile->mkdir("TimeAbsolute");
    } //End Case: Directory did not exist in file, CREATE
    
    //Store Event Level Fits
    //-------------------------------------
    //Placeholder
    
    //Loop Over the Histogram Matrix and store All fits
    //-------------------------------------
    //Level - Superchamber
    for (auto iterSC = m_mtrxHistos.m_map_HistosSC_All.begin(); iterSC != m_mtrxHistos.m_map_HistosSC_All.end(); ++iterSC) { //Loop Over Superchamber Fits
        //Get Directory
        //-------------------------------------
        //Check to see if dir_Superchamber directory exists already, if not create it
        TDirectory *dir_Superchamber = dir_TimeAbs->GetDirectory( ( (*iterSC).second.m_strName ).c_str(), false, "GetDirectory" );
        if (dir_Superchamber == nullptr) { //Case: Directory did not exist in file, CREATE
            dir_Superchamber = dir_TimeAbs->mkdir(  ( (*iterSC).second.m_strName ).c_str() );
        } //End Case: Directory did not exist in file, CREATE
        
        //Store Fits - Superchamber Level
        //-------------------------------------
        //Placeholder
        
        //Level - Detector
        map<string, TDirectory *> map_dirDet;
        map<string, TDirectory *> map_dirDetChan;
        for (auto iterDet = (*iterSC).second.m_map_HistosDet.begin(); iterDet != (*iterSC).second.m_map_HistosDet.end(); ++iterDet) { //Loop Over this Superchambers' Detector Fits
            //Get Directory
            //-------------------------------------
            //Check to see if dir_Detector directory exists already, if not create it
            TDirectory *dir_Detector = dir_Superchamber->GetDirectory( ( (*iterDet).first ).c_str(), false, "GetDirectory" );
            if (dir_Detector == nullptr) { //Case: Directory did not exist in file, CREATE
                dir_Detector = dir_Superchamber->mkdir(  ( (*iterDet).first ).c_str() );
            } //End Case: Directory did not exist in file, CREATE
            map_dirDet[(*iterDet).first]=dir_Detector;
            
            //Check to see if dir_Chan directory exists already, if not create it
            TDirectory *dir_Chan = map_dirDet[(*iterDet).first]->GetDirectory( "Channels", false, "GetDirectory" );
            if (dir_Chan == nullptr) { //Case: Directory did not exist in file, CREATE
                dir_Chan = map_dirDet[(*iterDet).first]->mkdir( "Channels" );
            } //End Case: Directory did not exist in file, CREATE
            map_dirDetChan[(*iterDet).first]=dir_Chan;
            
            //Store Fits - Detector Level
            //-------------------------------------
            //Placeholder
            
            //Level - Channel
            for (auto iterChan = (*iterDet).second.m_map_fitChan.begin(); iterChan != (*iterDet).second.m_map_fitChan.end(); ++iterChan) { //Loop Over this Detector's Channel Fits
                //Store Fits - Detector Level
                //-------------------------------------
                map_dirDetChan[(*iterDet).first]->cd();
                (*iterChan).second->Write();
            } //End Loop Over this Detector's Channel Fits
            
            map_dirDet.clear();
            map_dirDetChan.clear();
        } //End Loop Over this Superchambers' Detector Fits
    } //End Loop Over Superchamber Fits
    
    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End AnalyzeTiming::storeFits()

//Stores booked histograms (for those histograms that are non-null)
//Takes a std::string which stores the physical filename as input
void AnalyzeTiming::storeHistos( string & strOutputROOTFileName, std::string strOption){
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("AnalyzeTiming","storeHistos","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("AnalyzeTiming","storeHistos", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("AnalyzeTiming","storeHistos", "\tExiting; No Histograms have been stored!\n" );
        
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
void AnalyzeTiming::storeHistos(TFile * file_InputRootFile){
    
    //Check if File Failed to Open Correctly
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("AnalyzeTiming","storeHistos","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("AnalyzeTiming","storeHistos", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("AnalyzeTiming","storeHistos", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Get Base Directories
    //-------------------------------------
    //Check to see if dir_Summary directory exists already, if not create it
    TDirectory *dir_Summary = file_InputRootFile->GetDirectory("Summary", false, "GetDirectory" );
    if (dir_Summary == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_Summary = file_InputRootFile->mkdir("Summary");
    } //End Case: Directory did not exist in file, CREATE
    
    //Check to see if dir_TimeAbs directory exists already, if not create it
    TDirectory *dir_TimeAbs = file_InputRootFile->GetDirectory("TimeAbsolute", false, "GetDirectory" );
    if (dir_TimeAbs == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_TimeAbs = file_InputRootFile->mkdir("TimeAbsolute");
    } //End Case: Directory did not exist in file, CREATE
    
    //Store Event Level Histograms
    //-------------------------------------
    dir_Summary->cd();
    m_mtrxHistos.m_hMultiDUT->Write();
    m_mtrxHistos.m_hMultiTrig->Write();
    
    //Loop Over the Histogram Matrix and store All histos
    //-------------------------------------
    //Level - Superchamber
    for (auto iterSC = m_mtrxHistos.m_map_HistosSC_All.begin(); iterSC != m_mtrxHistos.m_map_HistosSC_All.end(); ++iterSC) { //Loop Over Superchamber Histos
        //Get Directory
        //-------------------------------------
        //Check to see if dir_Superchamber directory exists already, if not create it
        TDirectory *dir_Superchamber = dir_TimeAbs->GetDirectory( ( (*iterSC).second.m_strName ).c_str(), false, "GetDirectory" );
        if (dir_Superchamber == nullptr) { //Case: Directory did not exist in file, CREATE
            dir_Superchamber = dir_TimeAbs->mkdir(  ( (*iterSC).second.m_strName ).c_str() );
        } //End Case: Directory did not exist in file, CREATE
        
        //Store Histograms - Superchamber Level
        //-------------------------------------
        dir_Superchamber->cd();
        (*iterSC).second.m_hAll->Write();
        (*iterSC).second.m_hAND->Write();
        (*iterSC).second.m_hDelta->Write();
        (*iterSC).second.m_hMulti->Write();
        (*iterSC).second.m_hOR->Write();
        (*iterSC).second.m_hCorr->Write();
        //cout<<"(*iterSC).second.m_hAll->GetName() = " << (*iterSC).second.m_hAll->GetName() << endl;
        //cout<<"(*iterSC).second.m_hAll->GetEntries() = " << (*iterSC).second.m_hAll->GetEntries() << endl;
        
        //Level - Detector
        map<string, TDirectory *> map_dirDet;
        map<string, TDirectory *> map_dirDetChan;
        for (auto iterDet = (*iterSC).second.m_map_HistosDet.begin(); iterDet != (*iterSC).second.m_map_HistosDet.end(); ++iterDet) { //Loop Over this Superchambers' Detector Histos
            //Get Directory
            //-------------------------------------
            //Check to see if dir_Detector directory exists already, if not create it
            TDirectory *dir_Detector = dir_Superchamber->GetDirectory( ( (*iterDet).first ).c_str(), false, "GetDirectory" );
            if (dir_Detector == nullptr) { //Case: Directory did not exist in file, CREATE
                dir_Detector = dir_Superchamber->mkdir(  ( (*iterDet).first ).c_str() );
            } //End Case: Directory did not exist in file, CREATE
            map_dirDet[(*iterDet).first]=dir_Detector;
            
            //Check to see if dir_Chan directory exists already, if not create it
            TDirectory *dir_Chan = map_dirDet[(*iterDet).first]->GetDirectory( "Channels", false, "GetDirectory" );
            if (dir_Chan == nullptr) { //Case: Directory did not exist in file, CREATE
                dir_Chan = map_dirDet[(*iterDet).first]->mkdir( "Channels" );
            } //End Case: Directory did not exist in file, CREATE
            map_dirDetChan[(*iterDet).first]=dir_Chan;
            
            //Store Histograms - Detector Level
            //-------------------------------------
            map_dirDet[(*iterDet).first]->cd();
            (*iterDet).second.m_hAll->Write();
            (*iterDet).second.m_hMulti->Write();
            
            //Level - Channel
            for (auto iterChan = (*iterDet).second.m_map_hChan.begin(); iterChan != (*iterDet).second.m_map_hChan.end(); ++iterChan) { //Loop Over this Detector's Channel Histos
                //Store Histograms - Detector Level
                //-------------------------------------
                map_dirDetChan[(*iterDet).first]->cd();
                (*iterChan).second->Write();
            } //End Loop Over this Detector's Channel Histos
            
            map_dirDet.clear();
            map_dirDetChan.clear();
        } //End Loop Over this Superchambers' Detector Histos
    } //End Loop Over Superchamber Histos
    
    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End AnalyzeTiming::storeHistos()

//Resets attributes of this class
void AnalyzeTiming::reset(){
    m_mtrxHistos.clear();
}

TF1 AnalyzeTiming::getFit(int iChan, std::string strLogic, InfoFit & setupFit, shared_ptr<TH1F> hInput, TSpectrum &specInput){
    //Fit Declaration
    TF1 ret_Fit( Analyzer::getFit(setupFit, hInput, specInput) );
    
    //Set Name for this convention
    ret_Fit.SetName( (getNameByChanNum(iChan, "fit", setupFit.m_strFit_Name) + strLogic).c_str() );
    
    return ret_Fit;
} //End AnalyzeTiming::getFit()

TH1F AnalyzeTiming::getHistogram(int iChan, std::string strLogic, Timing::HistoSetup setupHisto){
    //Histo Declaration
    TH1F ret_Histo( Analyzer::getHistogram(setupHisto) );
    
    //Set Name for this convention
    ret_Histo.SetName( (getNameByChanNum(iChan, "h", setupHisto.strHisto_Name) + strLogic).c_str() );
    
    return ret_Histo;
} //End AnalyzeTiming::getHistogram()

TH2F AnalyzeTiming::getHistogram2D(int iChan, Timing::HistoSetup &setupHisto_X, Timing::HistoSetup &setupHisto_Y){
    //Histo Declaration
    TH2F ret_Histo( Analyzer::getHistogram2D(setupHisto_X, setupHisto_Y) );
    
    //Set Name for this convention
    ret_Histo.SetName( getNameByChanNum(iChan, "h", setupHisto_Y.strHisto_Name + "_v_" + setupHisto_X.strHisto_Name).c_str() );
    
    return ret_Histo;
} //End AnalyzeTiming::getHistogram2D()

std::string AnalyzeTiming::getNameByChanNum(int iChan, std::string strPrefix, std::string strName){
    //Variable Declaration
    string ret_Name = "";
    
    if (iChan > -1) { //Case: Specific Channel
        ret_Name = strPrefix + strName + "_Ch" + getString(iChan);
    } //End Case: Specific Channel
    else{ //Case: All Channels
        ret_Name = strPrefix + strName + "_ChAll";
    } //End Case: All Channels
    
    return ret_Name;
} //End AnalyzeTiming::getNameByChanNum()




