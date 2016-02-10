#include "TAxis.h"
#include <vector.h>
#include <algorithm>
#include <stdio.h>
#include <iostream>
#include "Riostream.h"
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "TTree.h"
#include "TBranch.h"
#include "TFrame.h"
#include "TCanvas.h"
#include "TPaveLabel.h"
#include "TPaveText.h"
#include "TFile.h"
#include "TString.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include "math.h"
#include "time.h"
#include "TRandom.h"
#include "TSpectrum.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include <TROOT.h>
#include <TStyle.h>
#define bin 100

using namespace std;

int main(int argc, char* argv[]) {
    
    // FIT variables - second peak
    Int_t amplitude=50, amplitude_min=10, amplitude_max=600;
    Int_t mean=4300, mean_min=2500, mean_max=7000;
    Int_t sigma=1800, sigma_min=1000, sigma_max=2000;
    
    // FIT variables - saturation peak
    Int_t pamplitude=50, pamplitude_min=10, pamplitude_max=600;
    Int_t pmean=1400, pmean_min=1000, pmean_max=2500;
    Int_t psigma=680, psigma_min=300, psigma_max=1000;
    
    // FIT variables - window
    Int_t min_fit=900, max_fit=10000;
    
    // HISTO SETTINGS
    Int_t bmin=0, bmax=10000, minstat=500;
    
    string file = argv[1];
    Int_t Nslices = atoi( argv[2]);
    Int_t start_slice = atoi( argv[3]);
    Int_t stop_slice = atoi( argv[4]);
    string instance = argv[5];
    
    // Variables
    Int_t i=0,j=0,k=0;
    Float_t Interval1=0, Interval2=0, Interval3=0, Interval4=0, Interval5=0, Interval6=0, Interval7=0, Interval8=0;
    Float_t PosMax1=0,PosMin1=0, PosMax2=0,PosMin2=0,PosMax3=0,PosMin3=0,PosMax4=0,PosMin4=0,PosMax5=0,PosMin5=0,PosMax6=0,PosMin6=0,PosMax7=0,PosMin7=0,PosMax8=0,PosMin8=0;
    Float_t rposition1, rmean1, rerrmean1, rsigma1, rerrsigma1, rchi2ndof1, rposition2, rmean2, rerrmean2, rsigma2, rerrsigma2, rchi2ndof2, rposition3, rmean3, rerrmean3, rsigma3, rerrsigma3, rchi2ndof3, rposition4, rmean4, rerrmean4, rsigma4, rerrsigma4, rchi2ndof4, rposition5, rmean5, rerrmean5, rsigma5, rerrsigma5, rchi2ndof5, rposition6, rmean6, rerrmean6, rsigma6, rerrsigma6, rchi2ndof6, rposition7, rmean7, rerrmean7, rsigma7, rerrsigma7, rchi2ndof7, rposition8, rmean8, rerrmean8, rsigma8, rerrsigma8, rchi2ndof8, rms1, mean1, rms2, mean2, rms3, mean3, rms4, mean4, rms5, mean5, rms6, mean6, rms7, mean7, rms8, mean8;
    
    Float_t VectPosEta1[Nslices],VectPosEta2[Nslices],VectPosEta3[Nslices],VectPosEta4[Nslices],VectPosEta5[Nslices],VectPosEta6[Nslices],VectPosEta7[Nslices],VectPosEta8[Nslices];
    Float_t VectFitMeanEta1[Nslices],VectFitMeanErrEta1[Nslices],VectFitSigmaEta1[Nslices],VectFitSigmaErrEta1[Nslices], VectFitEta1chi2ndof[Nslices];
    Float_t VectFitMeanEta2[Nslices],VectFitMeanErrEta2[Nslices],VectFitSigmaEta2[Nslices],VectFitSigmaErrEta2[Nslices], VectFitEta2chi2ndof[Nslices];
    Float_t VectFitMeanEta3[Nslices],VectFitMeanErrEta3[Nslices],VectFitSigmaEta3[Nslices],VectFitSigmaErrEta3[Nslices], VectFitEta3chi2ndof[Nslices];
    Float_t VectFitMeanEta4[Nslices],VectFitMeanErrEta4[Nslices],VectFitSigmaEta4[Nslices],VectFitSigmaErrEta4[Nslices], VectFitEta4chi2ndof[Nslices];
    Float_t VectFitMeanEta5[Nslices],VectFitMeanErrEta5[Nslices],VectFitSigmaEta5[Nslices],VectFitSigmaErrEta5[Nslices], VectFitEta5chi2ndof[Nslices];
    Float_t VectFitMeanEta6[Nslices],VectFitMeanErrEta6[Nslices],VectFitSigmaEta6[Nslices],VectFitSigmaErrEta6[Nslices], VectFitEta6chi2ndof[Nslices];
    Float_t VectFitMeanEta7[Nslices],VectFitMeanErrEta7[Nslices],VectFitSigmaEta7[Nslices],VectFitSigmaErrEta7[Nslices], VectFitEta7chi2ndof[Nslices];
    Float_t VectFitMeanEta8[Nslices],VectFitMeanErrEta8[Nslices],VectFitSigmaEta8[Nslices],VectFitSigmaErrEta8[Nslices], VectFitEta8chi2ndof[Nslices];
    
    
    /************************************* READ the ROOT FILE *****************************************/
    const char *cstr = file.c_str();
    TFile *MyFile=new TFile(cstr,"READ");
    /************************************* READ the ROOT Tree *****************************************/
    gStyle->SetOptFit(1);
    
    TTree *tree = 0;
    tree = (TTree*)gDirectory->Get("TCluster");
    Int_t N=tree->GetEntries();
    Int_t evtID, nclust;
    Int_t planeID[155], detID[155], clustSize[155], clustTimebin[155];
    Float_t etaSector[155], clustPos[155], clustADCs[155];
    
    TBranch        *b_evtID;
   	TBranch        *b_nclust;
   	TBranch        *b_clustPos;
   	TBranch        *b_clustSize;
   	TBranch        *b_clustADCs;
   	TBranch        *b_detID;
   	TBranch        *b_planeID;
    TBranch        *b_etaSector;
    TBranch        *b_clustTimebin;
    
    tree->SetMakeClass(1);
    tree->SetBranchAddress("detID", &detID, &b_detID);
    tree->SetBranchAddress("nclust", &nclust, &b_nclust);
    tree->SetBranchAddress("planeID", &planeID, &b_planeID);
    tree->SetBranchAddress("clustPos", &clustPos, &b_clustPos);
    tree->SetBranchAddress("clustTimebin", &clustTimebin, &b_clustTimebin);
    tree->SetBranchAddress("clustSize", &clustSize, &b_clustSize);
    tree->SetBranchAddress("clustADCs", &clustADCs, &b_clustADCs);
    
    /******************* Calculate the min and max positions for each eta sectors ************************/
    for (i=0;i<N;i++)//loop on events
    {
        b_nclust->GetEntry(i);// get number of clusters for this event
        b_planeID->GetEntry(i);
        b_clustPos->GetEntry(i);
        
        for (j=0;j<nclust;j++)//loop on clusters
        {
            if( planeID[j] < 200)//etaSector1 selection
            {
                if(clustPos[j] > PosMax1){PosMax1=clustPos[j];}
                if(clustPos[j] < PosMin1){PosMin1=clustPos[j];}
            }//end of etaSector1 selection
            
            if( planeID[j] > 200 && planeID[j] < 300 )//etaSector2 selection
            {
                if(clustPos[j] > PosMax2){PosMax2=clustPos[j];}
                if(clustPos[j] < PosMin2){PosMin2=clustPos[j];}
            }//end of etaSector2 selection
            
            if( planeID[j] > 300 && planeID[j] < 500 )//etaSector3 selection
            {
                if(clustPos[j] > PosMax3){PosMax3=clustPos[j];}
                if(clustPos[j] < PosMin3){PosMin3=clustPos[j];}
            }//end of etaSector3 selection
            
            if( planeID[j] > 500 && planeID[j] < 700 )//etaSector4 selection
            {
                if(clustPos[j] > PosMax4){PosMax4=clustPos[j];}
                if(clustPos[j] < PosMin4){PosMin4=clustPos[j];}
            }//end of etaSector4 selection
            
            if( planeID[j] > 700 && planeID[j] < 800 )//etaSector5 selection
            {
                if(clustPos[j] > PosMax5){PosMax5=clustPos[j];}
                if(clustPos[j] < PosMin5){PosMin5=clustPos[j];}
            }//end of etaSector5 selection
            
            if( planeID[j] > 800 && planeID[j] < 1000 )//etaSector6 selection
            {
                if(clustPos[j] > PosMax6){PosMax6=clustPos[j];}
                if(clustPos[j] < PosMin6){PosMin6=clustPos[j];}
            }//end of etaSector6 selection
            
            if( planeID[j] > 1000 && planeID[j] < 1100 )//etaSector7 selection
            {
                if(clustPos[j] > PosMax7){PosMax7=clustPos[j];}
                if(clustPos[j] < PosMin7){PosMin7=clustPos[j];}
            }//end of etaSector7 selection
            
            if( planeID[j] > 1100 )//etaSector8 selection
            {
                if(clustPos[j] > PosMax8){PosMax8=clustPos[j];}
                if(clustPos[j] < PosMin8){PosMin8=clustPos[j];}
            }//end of etaSector8 selection
            
        }// end of loop on clusters
        
    }
    /*
     cout << "*********Eta sector 1*********" << endl;
     cout << "Processed " << i+1 << " events" << endl;
     cout << "Min position eta 1: " << PosMin1 << endl;
     cout << "Max position eta 2: " << PosMax1 << endl;
     cout << "******************************" << endl;
     */
    /************* End of calculating the min and max positions for each eta sector *******************/
    
    
    
    /***************** Calculate the positions of the slices for each eta sector **********************/
    Interval1=(PosMax1-PosMin1)/Nslices;
    Interval2=(PosMax2-PosMin2)/Nslices;
    Interval3=(PosMax3-PosMin3)/Nslices;
    Interval4=(PosMax4-PosMin4)/Nslices;
    Interval5=(PosMax5-PosMin5)/Nslices;
    Interval6=(PosMax6-PosMin6)/Nslices;
    Interval7=(PosMax7-PosMin7)/Nslices;
    Interval8=(PosMax8-PosMin8)/Nslices;
    // calculated the width of the slices
    
    //cout << "Nslices= " << Nslices << " / Interval1= " << Interval1 << "mm" << endl;
    
    for(i=start_slice;i<stop_slice;i++) {
        VectPosEta1[i]=PosMin1+(i+0.5)*Interval1;
        VectPosEta2[i]=PosMin2+(i+0.5)*Interval2;
        VectPosEta3[i]=PosMin3+(i+0.5)*Interval3;
        VectPosEta4[i]=PosMin4+(i+0.5)*Interval4;
        VectPosEta5[i]=PosMin5+(i+0.5)*Interval5;
        VectPosEta6[i]=PosMin6+(i+0.5)*Interval6;
        VectPosEta7[i]=PosMin7+(i+0.5)*Interval7;
        VectPosEta8[i]=PosMin8+(i+0.5)*Interval8;
    }
    /************** End of calculating the positions of the slices for each eta sector *****************/
    
    
    /***************************** Process the event selection and fit *********************************/
    
    TCanvas *C1 = new TCanvas("C1","Eta",1400,800);
    C1->Divide(4,2);
    
    TH1F *hSlice1 = new TH1F("hSlice1","Slice",bin, bmin, bmax);
    TH1F *hSlice2 = new TH1F("hSlice2","Slice",bin, bmin, bmax);
    TH1F *hSlice3 = new TH1F("hSlice3","Slice",bin, bmin, bmax);
    TH1F *hSlice4 = new TH1F("hSlice4","Slice",bin, bmin, bmax);
    TH1F *hSlice5 = new TH1F("hSlice5","Slice",bin, bmin, bmax);
    TH1F *hSlice6 = new TH1F("hSlice6","Slice",bin, bmin, bmax);
    TH1F *hSlice7 = new TH1F("hSlice7","Slice",bin, bmin, bmax);
    TH1F *hSlice8 = new TH1F("hSlice8","Slice",bin, bmin, bmax);
    
    TF1 *fit1=new TF1("fit1","gaus(0)+gaus(3)",2000,4000);
    TF1 *fit2=new TF1("fit2","gaus(0)+gaus(3)",2000,4000);
    TF1 *fit3=new TF1("fit3","gaus(0)+gaus(3)",2000,4000);
    TF1 *fit4=new TF1("fit4","gaus(0)+gaus(3)",2000,4000);
    TF1 *fit5=new TF1("fit5","gaus(0)+gaus(3)",2000,4000);
    TF1 *fit6=new TF1("fit6","gaus(0)+gaus(3)",2000,4000);
    TF1 *fit7=new TF1("fit7","gaus(0)+gaus(3)",2000,4000);
    TF1 *fit8=new TF1("fit8","gaus(0)+gaus(3)",2000,4000);
    
    
    // SETTING FIT PARAMETERS
    fit1->SetParameters(0,amplitude);
    fit1->SetParLimits(0,amplitude_min,amplitude_max);
    fit1->SetParameters(1,mean);
    fit1->SetParLimits(1,mean_min,mean_max);
    fit1->SetParameters(2,sigma);
    fit1->SetParLimits(2,sigma_min,sigma_max);
    fit1->SetParameters(3,pamplitude);
    fit1->SetParLimits(3,pamplitude_min,pamplitude_max);
    fit1->SetParameters(4,pmean);
    fit1->SetParLimits(4,pmean_min,pmean_max);
    fit1->SetParameters(5,psigma);
    fit1->SetParLimits(5,psigma_min,psigma_max);
    fit1->SetLineColor(2);
    
    fit2->SetParameters(0,amplitude);
    fit2->SetParLimits(0,amplitude_min,amplitude_max);
    fit2->SetParameters(1,mean);
    fit2->SetParLimits(1,mean_min,mean_max);
    fit2->SetParameters(2,sigma);
    fit2->SetParLimits(2,sigma_min,sigma_max);
    fit2->SetParameters(3,pamplitude);
    fit2->SetParLimits(3,pamplitude_min,pamplitude_max);
    fit2->SetParameters(4,pmean);
    fit2->SetParLimits(4,pmean_min,pmean_max);
    fit2->SetParameters(5,psigma);
    fit2->SetParLimits(5,psigma_min,psigma_max);
    fit2->SetLineColor(2);
    
    fit3->SetParameters(0,amplitude);
    fit3->SetParLimits(0,amplitude_min,amplitude_max);
    fit3->SetParameters(1,mean);
    fit3->SetParLimits(1,mean_min,mean_max);
    fit3->SetParameters(2,sigma);
    fit3->SetParLimits(2,sigma_min,sigma_max);
    fit3->SetParameters(3,pamplitude);
    fit3->SetParLimits(3,pamplitude_min,pamplitude_max);
    fit3->SetParameters(4,pmean);
    fit3->SetParLimits(4,pmean_min,pmean_max);
    fit3->SetParameters(5,psigma);
    fit3->SetParLimits(5,psigma_min,psigma_max);
    fit3->SetLineColor(2);
    
    fit4->SetParameters(0,amplitude);
    fit4->SetParLimits(0,amplitude_min,amplitude_max);
    fit4->SetParameters(1,mean);
    fit4->SetParLimits(1,mean_min,mean_max);
    fit4->SetParameters(2,sigma);
    fit4->SetParLimits(2,sigma_min,sigma_max);
    fit4->SetParameters(3,pamplitude);
    fit4->SetParLimits(3,pamplitude_min,pamplitude_max);
    fit4->SetParameters(4,pmean);
    fit4->SetParLimits(4,pmean_min,pmean_max);
    fit4->SetParameters(5,psigma);
    fit4->SetParLimits(5,psigma_min,psigma_max);
    fit4->SetLineColor(2);
    
    fit5->SetParameters(0,amplitude);
    fit5->SetParLimits(0,amplitude_min,amplitude_max);
    fit5->SetParameters(1,mean);
    fit5->SetParLimits(1,mean_min,mean_max);
    fit5->SetParameters(2,sigma);
    fit5->SetParLimits(2,sigma_min,sigma_max);
    fit5->SetParameters(3,pamplitude);
    fit5->SetParLimits(3,pamplitude_min,pamplitude_max);
    fit5->SetParameters(4,pmean);
    fit5->SetParLimits(4,pmean_min,pmean_max);
    fit5->SetParameters(5,psigma);
    fit5->SetParLimits(5,psigma_min,psigma_max);
    fit5->SetLineColor(2);
    
    fit6->SetParameters(0,amplitude);
    fit6->SetParLimits(0,amplitude_min,amplitude_max);
    fit6->SetParameters(1,mean);
    fit6->SetParLimits(1,mean_min,mean_max);
    fit6->SetParameters(2,sigma);
    fit6->SetParLimits(2,sigma_min,sigma_max);
    fit6->SetParameters(3,pamplitude);
    fit6->SetParLimits(3,pamplitude_min,pamplitude_max);
    fit6->SetParameters(4,pmean);
    fit6->SetParLimits(4,pmean_min,pmean_max);
    fit6->SetParameters(5,psigma);
    fit6->SetParLimits(5,psigma_min,psigma_max);
    fit6->SetLineColor(2);
    
    fit7->SetParameters(0,amplitude);
    fit7->SetParLimits(0,amplitude_min,amplitude_max);
    fit7->SetParameters(1,mean);
    fit7->SetParLimits(1,mean_min,mean_max);
    fit7->SetParameters(2,sigma);
    fit7->SetParLimits(2,sigma_min,sigma_max);
    fit7->SetParameters(3,pamplitude);
    fit7->SetParLimits(3,pamplitude_min,pamplitude_max);
    fit7->SetParameters(4,pmean);
    fit7->SetParLimits(4,pmean_min,pmean_max);
    fit7->SetParameters(5,psigma);
    fit7->SetParLimits(5,psigma_min,psigma_max);
    fit7->SetLineColor(2);
    
    fit8->SetParameters(0,amplitude);
    fit8->SetParLimits(0,amplitude_min,amplitude_max);
    fit8->SetParameters(1,mean);
    fit8->SetParLimits(1,mean_min,mean_max);
    fit8->SetParameters(2,sigma);
    fit8->SetParLimits(2,sigma_min,sigma_max);
    fit8->SetParameters(3,pamplitude);
    fit8->SetParLimits(3,pamplitude_min,pamplitude_max);
    fit8->SetParameters(4,pmean);
    fit8->SetParLimits(4,pmean_min,pmean_max);
    fit8->SetParameters(5,psigma);
    fit8->SetParLimits(5,psigma_min,psigma_max);
    fit8->SetLineColor(2);
    
    //cout << "*** Processing the selection for each slices ***" << endl;
    //cout << "************************************************" << endl;
    
    TFile *hfile = 0;
    TString rootfilename = instance.c_str();
    hfile = TFile::Open(rootfilename+".root","RECREATE");
    
    // Tree and Branch definitions
    TTree *cmsgem = new TTree("CMSGEM","OFFLINE ANALYSIS");
    
    cmsgem->Branch("Slice",&j,"j/I");
    cmsgem->Branch("VectPosEta1",&rposition1,"rposition1/F");
    cmsgem->Branch("VectFitMeanEta1",&rmean1,"rmean1/F");
    cmsgem->Branch("VectFitMeanErrEta1",&rerrmean1,"rerrmean1/F");
    cmsgem->Branch("VectFitSigmaEta1",&rsigma1,"rsigma1/F");
    cmsgem->Branch("VectFitSigmaErrEta1",&rerrsigma1,"rerrsigma1/F");
    cmsgem->Branch("VectFitEta1chi2ndof",&rchi2ndof1,"rchi2ndof1/F");
    
    cmsgem->Branch("VectPosEta2",&rposition2,"rposition2/F");
    cmsgem->Branch("VectFitMeanEta2",&rmean2,"rmean2/F");
    cmsgem->Branch("VectFitMeanErrEta2",&rerrmean2,"rerrmean2/F");
    cmsgem->Branch("VectFitSigmaEta2",&rsigma2,"rsigma2/F");
    cmsgem->Branch("VectFitSigmaErrEta2",&rerrsigma2,"rerrsigma2/F");
    cmsgem->Branch("VectFitEta2chi2ndof",&rchi2ndof2,"rchi2ndof2/F");
    
    cmsgem->Branch("VectPosEta3",&rposition3,"rposition3/F");
    cmsgem->Branch("VectFitMeanEta3",&rmean3,"rmean3/F");
    cmsgem->Branch("VectFitMeanErrEta3",&rerrmean3,"rerrmean3/F");
    cmsgem->Branch("VectFitSigmaEta3",&rsigma3,"rsigma3/F");
    cmsgem->Branch("VectFitSigmaErrEta3",&rerrsigma3,"rerrsigma3/F");
    cmsgem->Branch("VectFitEta3chi2ndof",&rchi2ndof3,"rchi2ndof3/F");
    
    cmsgem->Branch("VectPosEta4",&rposition4,"rposition4/F");
    cmsgem->Branch("VectFitMeanEta4",&rmean4,"rmean4/F");
    cmsgem->Branch("VectFitMeanErrEta4",&rerrmean4,"rerrmean4/F");
    cmsgem->Branch("VectFitSigmaEta4",&rsigma4,"rsigma4/F");
    cmsgem->Branch("VectFitSigmaErrEta4",&rerrsigma4,"rerrsigma4/F");
    cmsgem->Branch("VectFitEta4chi2ndof",&rchi2ndof4,"rchi2ndof4/F");
    
    cmsgem->Branch("VectPosEta5",&rposition5,"rposition5/F");
    cmsgem->Branch("VectFitMeanEta5",&rmean5,"rmean5/F");
    cmsgem->Branch("VectFitMeanErrEta5",&rerrmean5,"rerrmean5/F");
    cmsgem->Branch("VectFitSigmaEta5",&rsigma5,"rsigma5/F");
    cmsgem->Branch("VectFitSigmaErrEta5",&rerrsigma5,"rerrsigma5/F");
    cmsgem->Branch("VectFitEta5chi2ndof",&rchi2ndof5,"rchi2ndof5/F");
    
    cmsgem->Branch("VectPosEta6",&rposition6,"rposition6/F");
    cmsgem->Branch("VectFitMeanEta6",&rmean6,"rmean6/F");
    cmsgem->Branch("VectFitMeanErrEta6",&rerrmean6,"rerrmean6/F");
    cmsgem->Branch("VectFitSigmaEta6",&rsigma6,"rsigma6/F");
    cmsgem->Branch("VectFitSigmaErrEta6",&rerrsigma6,"rerrsigma6/F");
    cmsgem->Branch("VectFitEta6chi2ndof",&rchi2ndof6,"rchi2ndof6/F");
    
    cmsgem->Branch("VectPosEta7",&rposition7,"rposition7/F");
    cmsgem->Branch("VectFitMeanEta7",&rmean7,"rmean7/F");
    cmsgem->Branch("VectFitMeanErrEta7",&rerrmean7,"rerrmean7/F");
    cmsgem->Branch("VectFitSigmaEta7",&rsigma7,"rsigma7/F");
    cmsgem->Branch("VectFitSigmaErrEta7",&rerrsigma7,"rerrsigma7/F");
    cmsgem->Branch("VectFitEta7chi2ndof",&rchi2ndof7,"rchi2ndof7/F");
    
    cmsgem->Branch("VectPosEta8",&rposition8,"rposition8/F");
    cmsgem->Branch("VectFitMeanEta8",&rmean8,"rmean8/F");
    cmsgem->Branch("VectFitMeanErrEta8",&rerrmean8,"rerrmean8/F");
    cmsgem->Branch("VectFitSigmaEta8",&rsigma8,"rsigma8/F");
    cmsgem->Branch("VectFitSigmaErrEta8",&rerrsigma8,"rerrsigma8/F");
    cmsgem->Branch("VectFitEta8chi2ndof",&rchi2ndof8,"rchi2ndof8/F");

    cmsgem->Branch("mean1",&mean1,"mean1/F");
    cmsgem->Branch("mean2",&mean2,"mean2/F");
    cmsgem->Branch("mean3",&mean3,"mean3/F");
    cmsgem->Branch("mean4",&mean4,"mean4/F");
    cmsgem->Branch("mean5",&mean5,"mean5/F");
    cmsgem->Branch("mean6",&mean6,"mean6/F");
    cmsgem->Branch("mean7",&mean7,"mean7/F");
    cmsgem->Branch("mean8",&mean8,"mean8/F");
    
    cmsgem->Branch("rms1",&rms1,"rms1/F");
    cmsgem->Branch("rms2",&rms2,"rms2/F");
    cmsgem->Branch("rms3",&rms3,"rms3/F");
    cmsgem->Branch("rms4",&rms4,"rms4/F");
    cmsgem->Branch("rms5",&rms5,"rms5/F");
    cmsgem->Branch("rms6",&rms6,"rms6/F");
    cmsgem->Branch("rms7",&rms7,"rms7/F");
    cmsgem->Branch("rms8",&rms8,"rms8/F");

    for (j=start_slice;j<stop_slice;j++)// loop on slices
    {
        
        hSlice1->Reset();
        hSlice2->Reset();
        hSlice3->Reset();
        hSlice4->Reset();
        hSlice5->Reset();
        hSlice6->Reset();
        hSlice7->Reset();
        hSlice8->Reset();
        
        for (i=0;i<N;i++)//loop on events
        {
            b_nclust->GetEntry(i);// get number of clusters for this event
            b_planeID->GetEntry(i);
            b_clustPos->GetEntry(i);
            b_clustTimebin->GetEntry(i);
            b_clustSize->GetEntry(i);
            b_clustADCs->GetEntry(i);
            
            for (k=0;k<nclust;k++)//loop on clusters
            {
                if(planeID[k] < 200)//etaSector1 selection
                {
                    if( clustPos[k] > PosMin1+j*Interval1 && clustPos[k] < PosMin1+(j+1)*Interval1 )// position selection
                    {
                        if(clustADCs[k] > 700 && clustSize[k] >= 1 && clustTimebin[k] > 1 && clustTimebin[k] < 22){hSlice1->Fill(clustADCs[k]);}// event selection
                    }// end of event selection
                    
                }// end of etaSector1 selection
                
                if( planeID[k] > 200 && planeID[k] < 300 )//etaSector2 selection
                {
                    if( clustPos[k] > PosMin2+j*Interval2 && clustPos[k] < PosMin2+(j+1)*Interval2 )// position selection
                    {
                        if(clustADCs[k] > 700 && clustSize[k] >= 1 && clustTimebin[k] > 1 && clustTimebin[k] < 22){hSlice2->Fill(clustADCs[k]);}// event selection
                    }// end of event selection
                    
                }// end of etaSector2 selection
                
                if( planeID[k] > 300 && planeID[k] < 500 )//etaSector3 selection
                {
                    if( clustPos[k] > PosMin3+j*Interval3 && clustPos[k] < PosMin3+(j+1)*Interval3 )// position selection
                    {
                        if(clustADCs[k] > 700 && clustSize[k] >= 1 && clustTimebin[k] > 1 && clustTimebin[k] < 22){hSlice3->Fill(clustADCs[k]);}// event selection
                    }// end of event selection
                    
                }// end of etaSector3 selection
                
                if( planeID[k] > 500 && planeID[k] < 700 )//etaSector4 selection
                {
                    if( clustPos[k] > PosMin4+j*Interval4 && clustPos[k] < PosMin4+(j+1)*Interval4 )// position selection
                    {
                        if(clustADCs[k] > 700 && clustSize[k] >= 1 && clustTimebin[k] > 1 && clustTimebin[k] < 22){hSlice4->Fill(clustADCs[k]);}// event selection
                    }// end of event selection
                    
                }// end of etaSector4 selection
                
                if( planeID[k] > 700 && planeID[k] < 800 )//etaSector5 selection
                {
                    if( clustPos[k] > PosMin5+j*Interval5 && clustPos[k] < PosMin5+(j+1)*Interval5 )// position selection
                    {
                        if(clustADCs[k] > 700 && clustSize[k] >= 1 && clustTimebin[k] > 1 && clustTimebin[k] < 22){hSlice5->Fill(clustADCs[k]);}// event selection
                    }// end of event selection
                    
                }// end of etaSector5 selection
                
                if( planeID[k] > 800 && planeID[k] < 1000 )//etaSector6 selection
                {
                    if( clustPos[k] > PosMin6+j*Interval6 && clustPos[k] < PosMin6+(j+1)*Interval6 )// position selection
                    {
                        if(clustADCs[k] > 700 && clustSize[k] >= 1 && clustTimebin[k] > 1 && clustTimebin[k] < 22){hSlice6->Fill(clustADCs[k]);}// event selection
                    }// end of event selection
                    
                }// end of etaSector6 selection
                
                if( planeID[k] > 1000 && planeID[k] < 1100 )//etaSector7 selection
                {
                    if( clustPos[k] > PosMin7+j*Interval7 && clustPos[k] < PosMin7+(j+1)*Interval7 )// position selection
                    {
                        if(clustADCs[k] > 700 && clustSize[k] >= 1 && clustTimebin[k] > 1 && clustTimebin[k] < 22){hSlice7->Fill(clustADCs[k]);}// event selection
                    }// end of event selection
                    
                }// end of etaSector7 selection
                
                if( planeID[k] > 1100 )//etaSector8 selection
                {
                    if( clustPos[k] > PosMin8+j*Interval8 && clustPos[k] < PosMin8+(j+1)*Interval8 )// position selection
                    {
                        if(clustADCs[k] > 700 && clustSize[k] >= 1 && clustTimebin[k] > 1 && clustTimebin[k] < 22){hSlice8->Fill(clustADCs[k]);}// event selection
                    }// end of event selection
                    
                }// end of etaSector8 selection
                
            }// end of loop on clusters
            
        }// end of loop on events
        
        
        stringstream temp;
        temp.str("");
        
        C1->cd(1);
        hSlice1->SetLineColor(1);
        temp << "eta1_slice_" << j;
        TString Title1 = temp.str();
        hSlice1->SetTitle(Title1);
        hSlice1->Draw();
        if ( (hSlice1->Integral(bmin,bmax))> minstat) {
            hSlice1->Fit("fit1","QM","",min_fit,max_fit);
            VectFitMeanEta1[j] = fit1->GetParameter(1);
            VectFitMeanErrEta1[j] = fit1->GetParError(1);
            VectFitSigmaEta1[j] = fit1->GetParameter(2);
            VectFitSigmaErrEta1[j] = fit1->GetParError(2);
            if (fit1->GetNDF()>0)  {
                VectFitEta1chi2ndof[j] = (fit1->GetChisquare()) / (fit1->GetNDF());
            }
            else {
                VectFitEta1chi2ndof[j]=9999;
            }
        }
        else {
            VectFitMeanEta1[j] = 0;//fit1->GetParameter(1);
            VectFitMeanErrEta1[j] = 0;//fit1->GetParError(1);
            VectFitSigmaEta1[j] = 0;//fit1->GetParameter(2);
            VectFitSigmaErrEta1[j] = 0;//fit1->GetParError(2);
            VectFitEta1chi2ndof[j] = 0;//(fit1->GetChisquare()) / (fit1->GetNDF());
        }
        C1->cd(2);
        hSlice2->SetLineColor(1);
        temp.str("");
        temp << "eta2_slice_" << j;
        TString Title2 = temp.str();
        hSlice2->SetTitle(Title2);
        hSlice2->Draw();
        if ( (hSlice2->Integral(bmin,bmax))> minstat) {
            hSlice2->Fit("fit2","QM","",min_fit,max_fit);
            VectFitMeanEta2[j] = fit2->GetParameter(1);
            VectFitMeanErrEta2[j] = fit2->GetParError(1);
            VectFitSigmaEta2[j] = fit2->GetParameter(2);
            VectFitSigmaErrEta2[j] = fit2->GetParError(2);
            if (fit2->GetNDF()>0)  {
                VectFitEta2chi2ndof[j] = (fit2->GetChisquare()) / (fit2->GetNDF());
            }
            else {
                VectFitEta2chi2ndof[j]=9999;
            }
        }
        else {
            VectFitMeanEta2[j] = 0;
            VectFitMeanErrEta2[j] = 0;
            VectFitSigmaEta2[j] = 0;
            VectFitSigmaErrEta2[j] = 0;
            VectFitEta2chi2ndof[j] = 0;
        }
        
        C1->cd(3);
        hSlice3->SetLineColor(1);
        temp.str("");
        temp << "eta3_slice_" << j;
        TString Title3 = temp.str();
        hSlice3->SetTitle(Title3);
        hSlice3->Draw();
        if ( (hSlice3->Integral(bmin,bmax))> minstat) {
            hSlice3->Fit("fit3","QM","",min_fit,max_fit);
            VectFitMeanEta3[j] = fit3->GetParameter(1);
            VectFitMeanErrEta3[j] = fit3->GetParError(1);
            VectFitSigmaEta3[j] = fit3->GetParameter(2);
            VectFitSigmaErrEta3[j] = fit3->GetParError(2);
            if (fit3->GetNDF()>0)  {
                VectFitEta3chi2ndof[j] = (fit3->GetChisquare()) / (fit3->GetNDF());
            }
            else {
                VectFitEta3chi2ndof[j]=9999;
            }
        }
        else {
            VectFitMeanEta3[j] = 0;
            VectFitMeanErrEta3[j] = 0;
            VectFitSigmaEta3[j] = 0;
            VectFitSigmaErrEta3[j] = 0;
            VectFitEta3chi2ndof[j] = 0;
        }
        C1->cd(4);
        hSlice4->SetLineColor(1);
        temp.str("");
        temp << "eta4_slice_" << j;
        TString Title4 = temp.str();
        hSlice4->SetTitle(Title4);
        hSlice4->Draw();
        if ( (hSlice4->Integral(bmin,bmax))> minstat) {
            hSlice4->Fit("fit4","QM","",min_fit,max_fit);
            VectFitMeanEta4[j] = fit4->GetParameter(1);
            VectFitMeanErrEta4[j] = fit4->GetParError(1);
            VectFitSigmaEta4[j] = fit4->GetParameter(2);
            VectFitSigmaErrEta4[j] = fit4->GetParError(2);
            if (fit4->GetNDF()>0)  {
                VectFitEta4chi2ndof[j] = (fit4->GetChisquare()) / (fit4->GetNDF());
            }
            else {
                VectFitEta4chi2ndof[j]=9999;
            }
        }
        else {
            VectFitMeanEta4[j] = 0;
            VectFitMeanErrEta4[j] = 0;
            VectFitSigmaEta4[j] = 0;
            VectFitSigmaErrEta4[j] = 0;
            VectFitEta4chi2ndof[j] = 0;
        }
        C1->cd(5);
        hSlice5->SetLineColor(1);
        temp.str("");
        temp << "eta5_slice_" << j;
        TString Title5 = temp.str();
        hSlice5->SetTitle(Title5);
        hSlice5->Draw();
        if ( (hSlice5->Integral(bmin,bmax))> minstat) {
            hSlice5->Fit("fit5","QM","",min_fit,max_fit);
            VectFitMeanEta5[j] = fit5->GetParameter(1);
            VectFitMeanErrEta5[j] = fit5->GetParError(1);
            VectFitSigmaEta5[j] = fit5->GetParameter(2);
            VectFitSigmaErrEta5[j] = fit5->GetParError(2);
            if (fit5->GetNDF()>0)  {
                VectFitEta5chi2ndof[j] = (fit5->GetChisquare()) / (fit5->GetNDF());
            }
            else {
                VectFitEta5chi2ndof[j]=9999;
            }
        }
        else {
            VectFitMeanEta5[j] = 0;
            VectFitMeanErrEta5[j] = 0;
            VectFitSigmaEta5[j] = 0;
            VectFitSigmaErrEta5[j] = 0;
            VectFitEta5chi2ndof[j] = 0;
        }
        C1->cd(6);
        hSlice6->SetLineColor(1);
        temp.str("");
        temp << "eta6_slice_" << j;
        TString Title6 = temp.str();
        hSlice6->SetTitle(Title6);
        hSlice6->Draw();
        if ( (hSlice6->Integral(bmin,bmax))> minstat) {
            hSlice6->Fit("fit6","QM","",min_fit,max_fit);
            VectFitMeanEta6[j] = fit6->GetParameter(1);
            VectFitMeanErrEta6[j] = fit6->GetParError(1);
            VectFitSigmaEta6[j] = fit6->GetParameter(2);
            VectFitSigmaErrEta6[j] = fit6->GetParError(2);
            if (fit6->GetNDF()>0)  {
                VectFitEta6chi2ndof[j] = (fit6->GetChisquare()) / (fit6->GetNDF());
            }
            else {
                VectFitEta6chi2ndof[j]=9999;
            }
        }
        else {
            VectFitMeanEta6[j] = 0;
            VectFitMeanErrEta6[j] = 0;
            VectFitSigmaEta6[j] = 0;
            VectFitSigmaErrEta6[j] = 0;
            VectFitEta6chi2ndof[j] = 0;
        }
        C1->cd(7);
        hSlice7->SetLineColor(1);
        temp.str("");
        temp << "eta7_slice_" << j;
        TString Title7 = temp.str();
        hSlice7->SetTitle(Title7);
        hSlice7->Draw();
        if ( (hSlice6->Integral(bmin,bmax))> minstat) {
            hSlice7->Fit("fit7","QM","",min_fit,max_fit);
            VectFitMeanEta7[j] = fit7->GetParameter(1);
            VectFitMeanErrEta7[j] = fit7->GetParError(1);
            VectFitSigmaEta7[j] = fit7->GetParameter(2);
            VectFitSigmaErrEta7[j] = fit7->GetParError(2);
            if (fit7->GetNDF()>0)  {
                VectFitEta7chi2ndof[j] = (fit7->GetChisquare()) / (fit7->GetNDF());
            }
            else {
                VectFitEta7chi2ndof[j]=9999;
            }
        }
        else {
            VectFitMeanEta7[j] = 0;
            VectFitMeanErrEta7[j] = 0;
            VectFitSigmaEta7[j] = 0;
            VectFitSigmaErrEta7[j] = 0;
            VectFitEta7chi2ndof[j] = 0;
        }
        C1->cd(8);
        hSlice8->SetLineColor(1);
        temp.str("");
        temp << "eta8_slice_" << j;
        TString Title8 = temp.str();
        hSlice8->SetTitle(Title8);
        hSlice8->Draw();
        if ( (hSlice6->Integral(bmin,bmax))> minstat) {
            hSlice8->Fit("fit8","QM","",min_fit,max_fit);
            VectFitMeanEta8[j] = fit8->GetParameter(1);
            VectFitMeanErrEta8[j] = fit8->GetParError(1);
            VectFitSigmaEta8[j] = fit8->GetParameter(2);
            VectFitSigmaErrEta8[j] = fit8->GetParError(2);
            if (fit8->GetNDF()>0)  {
                VectFitEta8chi2ndof[j] = (fit1->GetChisquare()) / (fit8->GetNDF());
            }
            else {
                VectFitEta8chi2ndof[j]=9999;
            }
        }
        else {
            VectFitMeanEta8[j] = 0;
            VectFitMeanErrEta8[j] = 0;
            VectFitSigmaEta8[j] = 0;
            VectFitSigmaErrEta8[j] = 0;
            VectFitEta8chi2ndof[j] = 0;
        }
        
        ofstream myfile;
        const char *cstr1 = instance.c_str();
        myfile.open (cstr1,ios::app);
        myfile << j  << " 1 " << VectPosEta1[j] << " " << VectFitMeanEta1[j] << " " << VectFitMeanErrEta1[j] << " " << VectFitSigmaEta1[j] << " " << VectFitSigmaErrEta1[j] << " " << VectFitEta1chi2ndof[j] << endl;
        myfile << j  << " 2 " << VectPosEta2[j] << " " << VectFitMeanEta2[j] << " " << VectFitMeanErrEta2[j] << " " << VectFitSigmaEta2[j] << " " << VectFitSigmaErrEta2[j] << " " << VectFitEta2chi2ndof[j] << endl;
        myfile << j  << " 3 " << VectPosEta3[j] << " " << VectFitMeanEta3[j] << " " << VectFitMeanErrEta3[j] << " " << VectFitSigmaEta3[j] << " " << VectFitSigmaErrEta3[j] << " " << VectFitEta3chi2ndof[j] << endl;
        myfile << j  << " 4 " << VectPosEta4[j] << " " << VectFitMeanEta4[j] << " " << VectFitMeanErrEta4[j] << " " << VectFitSigmaEta4[j] << " " << VectFitSigmaErrEta4[j] << " " << VectFitEta4chi2ndof[j] << endl;
        myfile << j  << " 5 " << VectPosEta5[j] << " " << VectFitMeanEta5[j] << " " << VectFitMeanErrEta5[j] << " " << VectFitSigmaEta5[j] << " " << VectFitSigmaErrEta1[j] << " " << VectFitEta5chi2ndof[j] << endl;
        myfile << j  << " 6 " << VectPosEta6[j] << " " << VectFitMeanEta6[j] << " " << VectFitMeanErrEta6[j] << " " << VectFitSigmaEta6[j] << " " << VectFitSigmaErrEta6[j] << " " << VectFitEta6chi2ndof[j] << endl;
        myfile << j  << " 7 " << VectPosEta7[j] << " " << VectFitMeanEta7[j] << " " << VectFitMeanErrEta7[j] << " " << VectFitSigmaEta7[j] << " " << VectFitSigmaErrEta7[j] << " " << VectFitEta7chi2ndof[j] << endl;
        myfile << j  << " 8 " << VectPosEta8[j] << " " << VectFitMeanEta8[j] << " " << VectFitMeanErrEta8[j] << " " << VectFitSigmaEta8[j] << " " << VectFitSigmaErrEta8[j] << " " << VectFitEta8chi2ndof[j] << endl;
        myfile.close();
        
        
        rposition1 = VectPosEta1[j];
        rmean1 = VectFitMeanEta1[j];
        rerrmean1 = VectFitMeanErrEta1[j];
        rsigma1 = VectFitSigmaEta1[j];
        rerrsigma1 = VectFitSigmaErrEta1[j];
        rchi2ndof1 = VectFitEta1chi2ndof[j];
        mean1 = hSlice1->GetMean();
        rms1 = hSlice1->GetRMS();

        rposition2 = VectPosEta2[j];
        rmean2 = VectFitMeanEta2[j];
        rerrmean2 = VectFitMeanErrEta2[j];
        rsigma2 = VectFitSigmaEta2[j];
        rerrsigma2 = VectFitSigmaErrEta2[j];
        rchi2ndof2 = VectFitEta2chi2ndof[j];
        mean2 = hSlice2->GetMean();
        rms2 = hSlice2->GetRMS();
        
        rposition3 = VectPosEta3[j];
        rmean3 = VectFitMeanEta3[j];
        rerrmean3 = VectFitMeanErrEta3[j];
        rsigma3 = VectFitSigmaEta3[j];
        rerrsigma3 = VectFitSigmaErrEta3[j];
        rchi2ndof3 = VectFitEta3chi2ndof[j];
        mean3 = hSlice3->GetMean();
        rms3 = hSlice3->GetRMS();

        rposition4 = VectPosEta4[j];
        rmean4 = VectFitMeanEta4[j];
        rerrmean4 = VectFitMeanErrEta4[j];
        rsigma4 = VectFitSigmaEta4[j];
        rerrsigma4 = VectFitSigmaErrEta4[j];
        rchi2ndof4 = VectFitEta4chi2ndof[j];
        mean4 = hSlice4->GetMean();
        rms4 = hSlice4->GetRMS();

        rposition5 = VectPosEta5[j];
        rmean5 = VectFitMeanEta5[j];
        rerrmean5 = VectFitMeanErrEta5[j];
        rsigma5 = VectFitSigmaEta5[j];
        rerrsigma5 = VectFitSigmaErrEta5[j];
        rchi2ndof5 = VectFitEta5chi2ndof[j];
        mean5 = hSlice5->GetMean();
        rms5 = hSlice5->GetRMS();

        rposition6 = VectPosEta6[j];
        rmean6 = VectFitMeanEta6[j];
        rerrmean6 = VectFitMeanErrEta6[j];
        rsigma6 = VectFitSigmaEta6[j];
        rerrsigma6 = VectFitSigmaErrEta6[j];
        rchi2ndof6 = VectFitEta6chi2ndof[j];
        mean6 = hSlice6->GetMean();
        rms6 = hSlice6->GetRMS();

        rposition7 = VectPosEta7[j];
        rmean7 = VectFitMeanEta7[j];
        rerrmean7 = VectFitMeanErrEta7[j];
        rsigma7 = VectFitSigmaEta7[j];
        rerrsigma7 = VectFitSigmaErrEta7[j];
        rchi2ndof7 = VectFitEta7chi2ndof[j];
        mean7 = hSlice7->GetMean();
        rms7 = hSlice7->GetRMS();

        rposition8 = VectPosEta8[j];
        rmean8 = VectFitMeanEta8[j];
        rerrmean8 = VectFitMeanErrEta8[j];
        rsigma8 = VectFitSigmaEta8[j];
        rerrsigma8 = VectFitSigmaErrEta8[j];
        rchi2ndof8 = VectFitEta8chi2ndof[j];
        mean8 = hSlice8->GetMean();
        rms8 = hSlice8->GetRMS();

        cmsgem->Fill();
        cmsgem->Write();
        
        /*
         cout <<"Eta 1 Slice " << j+1 <<" position : " << VectPosEta1[j] <<  " mean : " << VectFitMeanEta1[j] << " +/- " << VectFitMeanErrEta1[j] << " sigma : " << VectFitSigmaEta1[j] << " +/- " << VectFitSigmaErrEta1[j] << endl;
         cout <<"Eta 2 Slice " << j+1 <<" position : " << VectPosEta2[j] <<  " mean : " << VectFitMeanEta2[j] << " +/- " << VectFitMeanErrEta2[j] << " sigma : " << VectFitSigmaEta2[j] << " +/- " << VectFitSigmaErrEta2[j] << endl;
         cout <<"Eta 3 Slice " << j+1 <<" position : " << VectPosEta3[j] <<  " mean : " << VectFitMeanEta3[j] << " +/- " << VectFitMeanErrEta3[j] << " sigma : " << VectFitSigmaEta3[j] << " +/- " << VectFitSigmaErrEta3[j] << endl;
         cout <<"Eta 4 Slice " << j+1 <<" position : " << VectPosEta4[j] <<  " mean : " << VectFitMeanEta4[j] << " +/- " << VectFitMeanErrEta4[j] << " sigma : " << VectFitSigmaEta4[j] << " +/- " << VectFitSigmaErrEta4[j] << endl;
         cout <<"Eta 5 Slice " << j+1 <<" position : " << VectPosEta5[j] <<  " mean : " << VectFitMeanEta5[j] << " +/- " << VectFitMeanErrEta5[j] << " sigma : " << VectFitSigmaEta5[j] << " +/- " << VectFitSigmaErrEta5[j] << endl;
         cout <<"Eta 6 Slice " << j+1 <<" position : " << VectPosEta6[j] <<  " mean : " << VectFitMeanEta6[j] << " +/- " << VectFitMeanErrEta6[j] << " sigma : " << VectFitSigmaEta6[j] << " +/- " << VectFitSigmaErrEta6[j] << endl;
         cout <<"Eta 7 Slice " << j+1 <<" position : " << VectPosEta7[j] <<  " mean : " << VectFitMeanEta7[j] << " +/- " << VectFitMeanErrEta7[j] << " sigma : " << VectFitSigmaEta7[j] << " +/- " << VectFitSigmaErrEta7[j] << endl;
         cout <<"Eta 8 Slice " << j+1 <<" position : " << VectPosEta8[j] <<  " mean : " << VectFitMeanEta8[j] << " +/- " << VectFitMeanErrEta8[j] << " sigma : " << VectFitSigmaEta8[j] << " +/- " << VectFitSigmaErrEta8[j] << endl;
         cout << "************************************************" << endl;
         */
        C1->Modified();
        C1->Update();
        stringstream imgName;
        imgName << "slice_" << j << ".png";
        TString imgName2 = imgName.str();//"slice_"+ to_string(j) + ".png"; // saving histograms as .png
        C1->SaveAs(imgName2);
        
    }// end of loop on slices
    
    //cout << "************************************************" << endl;
    
    /********************** End of processing the event selection and fit **************************/
    
    return(0);
}
