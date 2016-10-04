#include "SRSCluster.h"
#include <cmath>
//====================================================================================================================
SRSCluster::SRSCluster(int minClusterSize, int maxClusterSize, std::string isMaximumOrTotalADCs) {
  fNbOfHits = 0;
  fstrip = 0;
  
  fClusterSumADCs = 0;
  fClusterPeakADCs = 0;
  
  fClusterTimeBinADC = 0 ;
  fClusterTimeBin = 0 ;
  fClusterPeakTimeBin = 0 ;
  
  fposition = 0;
  fclusterCentralStrip = 0;
  
  fMinClusterSize = minClusterSize;
  fMaxClusterSize = maxClusterSize;
  fIsClusterMaxOrSumADCs = isMaximumOrTotalADCs ;
  fPlaneSize = 512. ;
  fPlane     = "GEM1X" ;
  fNbAPVsOnPlane = 10 ;
  //    fArrayOfHits = new TObjArray(maxClusterSize);
  fIsGoodCluster = true ;
  fIsCluserPosCorrection = false ;
  //  printf("=======SRSCluster\n") ;
}

//====================================================================================================================
SRSCluster::~SRSCluster() {
  fArrayOfHits.clear();
  //    delete fArrayOfHits;
  fClusterTimeBinADCs.clear() ;
}

//====================================================================================================================
int SRSCluster::GetClusterTimeBin() {
  float q;
  //  std::vector<SRSHit* > temp = fArrayOfHits;
  int nbofhits =  this->GetNbOfHits() ;
  fClusterTimeBinADC = 0 ;
  
  for (int i = 0; i < nbofhits; i++) {
    std::vector< float > timeBinADCs = (fArrayOfHits[i])->GetTimeBinADCs() ;
    int nbOfTimeBins = timeBinADCs.size() ;
    fClusterTimeBinADCs.resize(nbOfTimeBins) ;
    for (int k = 0; k < nbOfTimeBins; k++) {
      fClusterTimeBinADCs[k] += timeBinADCs[k] ;
    }
    timeBinADCs.clear() ;
  }
  
  this->Timing() ;
  return fClusterTimeBin ;
}

//============================================================================================
void SRSCluster::Timing() {
  //  bool timingStatus = true ;
  fClusterTimeBin=-1;
  int vmax = -1;
  for (int k = 0; k < fClusterTimeBinADCs.size(); k++) {
    if (fClusterTimeBinADCs[k]>vmax) {
      fClusterTimeBin=k;
      vmax = fClusterTimeBinADCs[k];
    }
  }
}


  //====================================================================================================================
void SRSCluster::ClusterPositionPulseHeghtWeight() {  // Calculate the fposition and the total fClusterSumADCs
  float hitposition, q;
  int nbofhits =  this->GetNbOfHits() ;
  for (int i = 0; i < nbofhits; i++) {
    q  = (fArrayOfHits[i])->GetHitADCs() ;
    hitposition = (fArrayOfHits[i])->GetStripPosition() ;
    fClusterSumADCs += q ;
    fposition += q * hitposition ;
    
    if (q > fClusterPeakADCs) {
      fClusterPeakTimeBin = (fArrayOfHits[i])->GetSignalPeakBinNumber() ;
      fClusterPeakADCs = q ;
    }
  }
  fposition /= fClusterSumADCs;
  //  printf("   SRSCluster::ClusterPositionPulseHeghtWeight => clusterPosition = %f\n", fposition) ;
}

//====================================================================================================================
void SRSCluster::ClusterPositionHistoMean() {  // Calculate the fposition and the total fClusterSumADCs
  float hitposition, q;
  int nbofhits =  this->GetNbOfHits() ;
  int indexLastHist = nbofhits - 1 ;
  float pitch = fPlaneSize / (NCH * fNbAPVsOnPlane) ;
  float min = (fArrayOfHits[0])->GetStripPosition() -  pitch;
  float max = (fArrayOfHits[indexLastHist])->GetStripPosition() + pitch ;
  int nbin = nbofhits + 2 ;
  float pos=0.;
  for (int i = 0; i < nbofhits; i++) {
    q  = (fArrayOfHits[i])->GetHitADCs() ;
    hitposition = (fArrayOfHits[i])->GetStripPosition() ;
    pos +=hitposition*q;
    fClusterSumADCs += q ;
    if (q > fClusterPeakADCs) fClusterPeakADCs = q ;
  }
  fposition = pos/fClusterSumADCs;
  printf("   SRSCluster::ClusterPositionGausFitMean => clusterPosition = %f\n", fposition) ;
}
/*
//====================================================================================================================
void SRSCluster::ClusterPositionGausFitMean() {  // Calculate the fposition and the total fClusterSumADCs
float hitposition, q;
TObjArray &temp = *fArrayOfHits;
int nbofhits =  GetNbOfHits() ;
int indexLastHist = nbofhits - 1 ;
float pitch = fPlaneSize / (NCH * fNbAPVsOnPlane) ;
float min = ((SRSHit*)temp[0])->GetStripPosition() -  pitch;
    float max = ((SRSHit*)temp[indexLastHist])->GetStripPosition() + pitch ;
    int nbin = nbofhits + 2 ;
    TH1F * h = new TH1F("coordHisto", "coordinate" , nbin, min, max) ;
    for (int i = 0; i < nbofhits; i++) {
        q  = ((SRSHit*)temp[i])->GetHitADCs() ;
        hitposition = ((SRSHit*)temp[i])->GetStripPosition() ;
        h->Fill(hitposition , q) ;
        fClusterSumADCs += q ;
        if (q > fClusterPeakADCs) fClusterPeakADCs = q ;
    }
    h->Fit("gaus", "Q", "", min, max) ;
    fposition = h->GetFunction("gaus")->GetParameter(1) ;
    delete  h ;
    //  printf("   SRSCluster::ClusterPositionGausFitMean => clusterPosition = %f\n", fposition) ;
}
 */
//====================================================================================================================
void SRSCluster::ClusterCentralStrip() {
  float p, dp ;
  float dpmin = 99;
  int nbofhits =  this->GetNbOfHits() ;
  for (int i = 0; i < nbofhits; i++) {
    p  = (fArrayOfHits[i])->GetStripPosition();
    dp = fabs(fposition - p);
    if (dp <= dpmin) {
      fclusterCentralStrip = p;
      dpmin = dp;
    }
  }
}

//====================================================================================================================
float SRSCluster::GetClusterADCs() {
  float adcs = 0 ;
  if (fIsClusterMaxOrSumADCs == "maximumADCs") {
    adcs = fClusterPeakADCs ;
  }
  else {
    adcs = fClusterSumADCs ;
  }
  return adcs ;
}

//====================================================================================================================
void SRSCluster::Dump() {
  std::cout << "*** APV Cluster dump ***" << std::endl;
  //  TObject::Dump();
  std::cout << std::endl;
}

//====================================================================================================================
void SRSCluster::AddHit(SRSHit *h) {
  fArrayOfHits.push_back(h);
}

//====================================================================================================================
void SRSCluster::ClearArrayOfHits() {
  fArrayOfHits.clear();
}

//====================================================================================================================
bool SRSCluster::IsGoodCluster() {
  fIsGoodCluster = true ;
  fNbOfHits = fArrayOfHits.size() ;
  if ( (fNbOfHits > fMaxClusterSize) || (fNbOfHits < fMinClusterSize) ) {
    this->ClearArrayOfHits() ;
    fIsGoodCluster  = false ;
    fNbOfHits = fArrayOfHits.size() ;
  }
  return fIsGoodCluster ;
}

//====================================================================================================================
int SRSCluster::Compare(SRSCluster *obj) const {

  int compare = 0;
  if (fClusterSumADCs < obj->GetClusterADCs())
    compare = 1 ;
  else
    compare = -1;
  
  return compare;
}
    
//-------------------------------------------------------------------------------------------------
void SRSCluster::ComputeClusterPositionWithoutCorrection() {
  this->ClusterPositionPulseHeghtWeight() ;
  this->ClusterCentralStrip() ;
}

/**
 //-------------------------------------------------------------------------------------------------
 void SRSCluster::ComputeClusterPositionWithCorrection(const char * filename) {
 ComputeClusterPositionWithoutCorrection() ;
 
 if(fNbOfHits > 1)  {
 TFile * file =  new TFile(filename, "read") ;
 if (!file->IsOpen())   printf("SRSEventBuilder::LoadPositionCorrectionRootFile()==> ERROR Can not open file %s \n",filename) ;
 
 float pitch    = fPlaneSize / (NCH * fNbAPVsOnPlane) ;
 if(fPlane.Contains("EIC")) pitch = 2 * pitch ;
 float eta = (fposition - fclusterCentralStrip) / pitch ;
 
 std::string histoName = "etaFunction" + fPlane ;
 
 if(eta > 0) {
 if(fNbOfHits == 2) histoName = "eta2FunctionPos" + fPlane ;
 if(fNbOfHits == 3) histoName = "eta3FunctionPos" + fPlane ;
 if(fNbOfHits == 4) histoName = "eta4FunctionPos" + fPlane ;
 if(fNbOfHits == 5) histoName = "eta5FunctionPos" + fPlane ;
 if(fNbOfHits > 5) histoName = "eta6PlusFunctionPos" + fPlane ;
 }
 
 if(eta < 0) {
 if(fNbOfHits == 2) histoName = "eta2FunctionNeg" + fPlane ;
 if(fNbOfHits == 3) histoName = "eta3FunctionNeg" + fPlane ;
 if(fNbOfHits == 4) histoName = "eta4FunctionNeg" + fPlane ;
 if(fNbOfHits == 5) histoName = "eta5FunctionNeg" + fPlane ;
 if(fNbOfHits > 5) histoName = "eta6PlusFunctionNeg" + fPlane ;
 }
 
 TH1F * posCorrectionHistos =  (TH1F *) file->Get(histoName) ;
 
 std::string afterhistoname = posCorrectionHistos->GetName() ;
 std::string fitName = afterhistoname+"_FIT" ;
 TF1 * correctionFunctionFit = (TF1 *) posCorrectionHistos->GetFunction(fitName) ;
 float posCorrecFuncution = 0 ;
 if (eta > 0) posCorrecFuncution  = correctionFunctionFit->Integral(0, eta) / correctionFunctionFit->Integral(0,0.5) ;
 if (eta < 0) posCorrecFuncution  = correctionFunctionFit->Integral(eta, 0) / correctionFunctionFit->Integral(-0.5,0) ;
 delete correctionFunctionFit ;
 
 fposition = fclusterCentralStrip - (pitch * (0.5 - posCorrecFuncution)) ;
 delete posCorrectionHistos ;
 delete file ;
 }
 }
 */

/*
//-------------------------------------------------------------------------------------------------
void SRSCluster::ComputeClusterPositionWithCorrection(const char * filename) {
    ComputeClusterPositionWithoutCorrection() ;
    
    if(fNbOfHits > 1)  {
        TFile * file =  new TFile(filename, "read") ;
        if (!file->IsOpen())   printf("SRSEventBuilder::LoadPositionCorrectionRootFile()==> ERROR Can not open file %s \n",filename) ;
        
        float pitch    = fPlaneSize / (NCH * fNbAPVsOnPlane) ;
        if(fPlane.Contains("EIC")) pitch = 2 * pitch ;
        float eta = (fposition - fclusterCentralStrip) / pitch ;
        
        std::string histoNamePos = "etaFunction" + fPlane ;
        std::string histoNameNeg = "etaFunction" + fPlane ;
        
        
        if(fNbOfHits == 2) {
            histoNamePos = "eta2FunctionPos" + fPlane ;
            histoNameNeg = "eta2FunctionNeg" + fPlane ;
        }
        
        if(fNbOfHits == 3) {
            histoNamePos = "eta3FunctionPos" + fPlane ;
            histoNameNeg = "eta3FunctionNeg" + fPlane ;
        }
        if(fNbOfHits == 4) {
            histoNamePos = "eta4FunctionPos" + fPlane ;
            histoNameNeg = "eta4FunctionNeg" + fPlane ;
        }
        if(fNbOfHits == 5) {
            histoNamePos = "eta5FunctionPos" + fPlane ;
            histoNameNeg = "eta5FunctionNeg" + fPlane ;
        }
        if(fNbOfHits > 5) {
            histoNamePos = "eta6PlusFunctionPos" + fPlane ;
            histoNameNeg = "eta6PlusFunctionNeg" + fPlane ;
        }
        
        TH1F * posCorrectionPosHistos =  (TH1F *) file->Get(histoNamePos) ;
        TH1F * posCorrectionNegHistos =  (TH1F *) file->Get(histoNameNeg) ;
        float posCorrecFuncution = 0 ;
        
        int firstBin = posCorrectionNegHistos->FindBin(-0.5) ;
        int zeroBin  = posCorrectionPosHistos->FindBin(0) ;
        int etaBin   = posCorrectionPosHistos->FindBin(eta) ;
        if (eta < 0) etaBin   = posCorrectionNegHistos->FindBin(eta) ;
        
        float integralPos = posCorrectionPosHistos->Integral() ;
        float integralNeg = posCorrectionPosHistos->Integral() ;
        float integralSum = integralNeg + integralPos ;
        
        if (eta > 0) posCorrecFuncution = ( integralNeg + posCorrectionPosHistos->Integral(zeroBin,etaBin) )/ integralSum ;
        if (eta < 0) posCorrecFuncution = ( posCorrectionPosHistos->Integral(firstBin, etaBin) ) / integralSum ;
        
        
        // std::string afterhistonamePos = posCorrectionPosHistos->GetName() ;
        // std::string fitNamePos = afterhistonamePos + "_FIT" ;
        // TF1 * correctionFunctionPosFit = (TF1 *) posCorrectionPosHistos->GetFunction(fitNamePos) ;
         
        // std::string afterhistonameNeg = posCorrectionNegHistos->GetName() ;
        // std::stringg fitNameNeg = afterhistonameNeg + "_FIT" ;
        // TF1 * correctionFunctionNegFit = (TF1 *) posCorrectionNegHistos->GetFunction(fitNameNeg) ;
         
        // float integralPos = correctionFunctionPosFit->Integral(0,0.5)  ;
        // float integralNeg = correctionFunctionNegFit->Integral(-0.5,0) ;
        // float integralSum = integralNeg + integralPos ;
         
        // if (eta > 0) posCorrecFuncution = ( integralNeg + correctionFunctionPosFit->Integral(0, eta) ) / integralSum ; 
        // if (eta < 0) posCorrecFuncution  =( correctionFunctionNegFit->Integral(-0.5, eta) ) / integralSum ; 
        // delete correctionFunctionPosFit ;
        // delete correctionFunctionNegFit ;
         
        
        fposition = fclusterCentralStrip - (pitch * (0.5 - posCorrecFuncution)) ; 
        delete posCorrectionPosHistos ;
        delete posCorrectionNegHistos ;
        delete file ; 
    }
}
*/
