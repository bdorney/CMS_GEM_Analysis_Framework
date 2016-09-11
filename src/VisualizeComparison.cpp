//
//  VisualizeComparison.cpp
//  
//
//  Created by Brian L Dorney on 10/09/16.
//
//

#include "VisualizeComparison.h"

using std::cout;
using std::endl;
using std::make_shared;
using std::map;
using std::shared_ptr;
using std::string;
using std::vector;

using QualityControl::Timing::getCharSeparatedList;
using QualityControl::Timing::getCyclicColor;
using QualityControl::Timing::getString;
using QualityControl::Timing::printROOTFileStatus;

using namespace QualityControl::Uniformity;

//Default Constructor
VisualizeComparison::VisualizeComparison(){
    m_iEta = m_iPhi = m_iSlice = -1;
} //End Default Constructor

void VisualizeComparison::storeCanvasComparisonHisto(std::string strOutputROOTFileName, std::string strOption, std::string strObsName){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("VisualizeComparison","storeCanvasComparisonHisto","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("VisualizeComparison","storeCanvasComparisonHisto", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeComparison","storeCanvasComparisonHisto", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Call the method below
    //------------------------------------------------------
    storeCanvasComparisonHisto(ptr_fileOutput, strObsName);
    
    //Close the File
    //------------------------------------------------------
    ptr_fileOutput->Close();
    
    return;
} //End VisualizeComparison::storeCanvasComparisonHisto()

void VisualizeComparison::storeCanvasComparisonHisto(TFile * file_InputRootFile, std::string strObsName){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);

    //Make the Canvas
    //------------------------------------------------------
    TCanvas canv_ObsComp( ("canv_" + m_strIdent + "_" + strObsName ).c_str(), ( strObsName + " for " + m_strIdent ).c_str(), 600, 600);
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("VisualizeComparison","storeCanvasComparisonHisto","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("VisualizeComparison","storeCanvasComparisonHisto", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeComparison","storeCanvasComparisonHisto", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Get the distributions to be compared, exit if there are none
    //------------------------------------------------------
    map<string, shared_ptr<TH1F> > map_hObs = getObsHistoMap(strObsName);
    if ( map_hObs.size() == 0 ) {
        printClassMethodMsg("VisualizeComparison","storeCanvasComparisonHisto", ("\tExiting; Did not find any distributons for " + strObsName + "!\n" ).c_str() );
        printClassMethodMsg("VisualizeComparison","storeCanvasComparisonHisto", "\tPlease check your input files again, exiting!!!\n" );
        
        return;
    }
    
    //Setup the Legend
    //------------------------------------------------------
    TLegend * legObs = new TLegend(0.2,0.2,0.6,0.4);
    legObs->SetNColumns(2);
    legObs->SetFillColor(kWhite);
    legObs->SetLineColor(kWhite);
    
    //Draw each element of map_hObs
    //------------------------------------------------------
    canv_ObsComp.cd();
    for (auto iterObs = map_hObs.begin(); iterObs != map_hObs.end(); ++iterObs) { //Loop Over map_hObs
        int iIdx = std::distance(map_hObs.begin(), iterObs );
        
	(*iterObs).second->SetFillColor( getCyclicColor(iIdx) );
	(*iterObs).second->SetFillStyle( 3244 );
        (*iterObs).second->SetLineColor( getCyclicColor(iIdx) );
        (*iterObs).second->SetMarkerColor( getCyclicColor(iIdx) );
        legObs->AddEntry( (*iterObs).second.get(), (*iterObs).first.c_str(), "LPE" );
        
        if (iterObs == map_hObs.begin()) {
            (*iterObs).second->Draw();
        }
        else{
            (*iterObs).second->Draw("same");
        }
    } //End Loop Over map_hObs
    
    //Draw the legend & the logo
    //-----------------------------------------------------
    legObs->Draw("same");
    TLatex latex_CMSPrelim;
    latex_CMSPrelim.SetTextSize(0.05);
    latex_CMSPrelim.DrawLatexNDC(0.1, 0.905, "CMS Preliminary" );
    
    //Setup the output directories
    //-----------------------------------------------------
    //Check to see if dir_Ident exists already, if not create it
    TDirectory *dir_Ident = file_InputRootFile->GetDirectory( m_strIdent.c_str(), false, "GetDirectory" );
    if (dir_Ident == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_Ident = file_InputRootFile->mkdir( m_strIdent.c_str() );
    } //End Case: Directory did not exist in file, CREATE
    
    //Check to see if dir_ObsName exists already, if not create it
    TDirectory *dir_ObsName = dir_Ident->GetDirectory( strObsName.c_str(), false, "GetDirectory" );
    if (dir_ObsName == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_ObsName = dir_Ident->mkdir( strObsName.c_str() );
    } //End Case: Directory did not exist in file, CREATE
    
    //Write the TObjects to the File
    //------------------------------------------------------
    dir_ObsName->cd();
    canv_ObsComp.Write();
    if (bSaveCanvases) { save2png(canv_ObsComp); }
    for (auto iterObs = map_hObs.begin(); iterObs != map_hObs.end(); ++iterObs) { //Loop Over map_hObs
        (*iterObs).second->Write();
    } //End Loop Over map_hObs

    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End VisualizeComparison::storeCanvasComparisonHisto()

//Returns a map of identifier (key) and observable histogram
shared_ptr<TH1F> VisualizeComparison::getObsHisto(TFile * file_InputRootFile, string strObsName){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    shared_ptr<TH1F> ret_hObs;
    
    string strRootFilePath = "";
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("VisualizeComparison","getObsHisto","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("VisualizeComparison","getObsHisto", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeComparison","getObsHisto", "\tExiting; No Histograms have been stored!\n" );
        
        return ret_hObs;
    } //End Check if File Failed to Open Correctly

    //Setup the filepath
    //------------------------------------------------------
    if (m_iSlice > -1) {
        strRootFilePath = "SectorEta" + getString(m_iEta) + "/SectorPhi" + getString(m_iPhi) + "/Slice" + getString(m_iSlice) + "/";
    }
    else if (m_iPhi > -1){ //Case: Specific (iEta,iPhi) sector
        strRootFilePath = "SectorEta" + getString(m_iEta) + "/SectorPhi" + getString(m_iPhi) + "/";
    } //End Case: Specific (iEta,iPhi) sector
    else if (m_iEta > -1){
        strRootFilePath = "SectorEta" + getString(m_iEta) +  "/";
    }
    else{ //Case: iEta Sector, sum over sector's iPhi
        strRootFilePath = "Summary/";
    } //End Case: iEta Sector, sum over sector's iPhi
    
    strRootFilePath = strRootFilePath + getNameByIndex(m_iEta, m_iPhi, m_iSlice, "h", strObsName);
    
    //Get the TH1F from the file
    //------------------------------------------------------
    
    ret_hObs = make_shared<TH1F>( *((TH1F*) file_InputRootFile->Get( strRootFilePath.c_str() ) ) );
    
    return ret_hObs;
} //End VisualizeComparison::getObsHisto()

map<string, shared_ptr<TH1F> > VisualizeComparison::getObsHistoMap(std::string strObsName){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    map<string, shared_ptr<TH1F> > ret_map_hObs;
    
    shared_ptr<TH1F> hObs;
    
    string strHistoName = "";
    string strKey = "";
    string strFilenameNoSlashes = ""; //for string manipulation
    
    vector<string> vec_strParsedName;
    
    for (int i=0; i<m_vec_strFileList.size(); ++i) { //Loop Over m_vec_strFileList
        strFilenameNoSlashes = m_vec_strFileList[i];
        
        //Get the file
        TFile * file_InputRootFile = new TFile(m_vec_strFileList[i].c_str(), "READ","",1);
        
        //Strip the file path
        std::size_t posLastSlash = strFilenameNoSlashes.find_last_of("/");
        if (posLastSlash != std::string::npos) {
            strFilenameNoSlashes.erase(0,posLastSlash+1);
            //strFilenameNoSlashes = strFilenameNoSlashes(posLastSlash+1);
            
        }
        
        //Get the underscore separated list of fields
        vec_strParsedName = getCharSeparatedList(strFilenameNoSlashes, '_');
        
        //Look for the identifier, if not found skip this file
        auto iterStr = std::find_if(vec_strParsedName.begin(), vec_strParsedName.end(), contains(m_strIdent) );
        if ( iterStr == vec_strParsedName.end() ) {
            file_InputRootFile->Close();
            continue;
        }
        
        //Get the histogram, if null skip this file
        hObs = getObsHisto(file_InputRootFile, strObsName);
        if ( hObs == nullptr) {
            file_InputRootFile->Close();
            continue;
        }
        
        //Append the name of the histogram
        strHistoName = hObs->GetName();
        strHistoName = strHistoName + "_" + (*iterStr);
        hObs->SetName( strHistoName.c_str() );
        
        //Normalize the histogram?
        if (m_bNormalize) {
            hObs->Scale( 1. / hObs->Integral() );
        }
        
        //Store this histogram and close the file
        ret_map_hObs[(*iterStr)] = hObs;
        file_InputRootFile->Close();
        delete file_InputRootFile;
    } //End Loop Over m_vec_strFileList
    
    return ret_map_hObs;
} //End VisualizeComparison::getObsHistoMap()
