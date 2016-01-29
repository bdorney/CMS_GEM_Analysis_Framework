#include "TAxis.h"
#include "TH2.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TNtupleD.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector.h>
#include <algorithm>
#include <iostream>
#include "math.h"
#include <string>
#include "TH1F.h"
#include "TH2F.h"
#include "TH1.h"
#include "TF1.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Riostream.h"
#include <fstream>
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
#include "TLorentzVector.h"
#include "math.h"
#include "time.h"
#include "TRandom.h"
#include "TSpectrum.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"


#define bin 100

using namespace std;

float getdata(int i, const char* dataname, TNtuple* TCluster) ;

float getdata(int i, const char* dataname, TNtuple* TCluster) 
{

	
	Float_t data;
	TBranch *dataBranch;
	
	TCluster->SetMakeClass(1);
	TCluster->SetBranchAddress(dataname, &data, &dataBranch);
	dataBranch->GetEntry(i);

	return data;
}


int AnalysisCode()
{

	/************************************* READ the ROOT FILE *****************************************/

	char fileName[256];

	cout << "Please enter the ROOT file's name (fileName.root) : ";
  	cin.getline (fileName,256);

	TFile *MyFile=new TFile(fileName,"READ");


	/************************************* READ the ROOT Tree *****************************************/

	TNtuple *TCluster=TCluster;
	Int_t N=TCluster->GetEntries();
	cout << "Number of entries : " << N << endl;

	Int_t evtID, nclust;
	Int_t planeID[55],detID[55],clustSize[55],clustTimebin[55]; // [#] is the max Nb of cluster for a given events --> is defined in amore.cfg and given by nclust in the tree
	Float_t etaSector[55],clustPos[55],clustADCs[55];

	TBranch        *b_evtID;   
   	TBranch        *b_nclust;   
   	TBranch        *b_clustPos;   
   	TBranch        *b_clustSize;   
   	TBranch        *b_clustADCs;   
   	TBranch        *b_detID;   
   	TBranch        *b_planeID;   
	TBranch        *b_etaSector;
	TBranch        *b_clustTimebin;

	TCluster->SetMakeClass(1);

	TCluster->SetBranchAddress("detID", &detID, &b_detID);
	TCluster->SetBranchAddress("nclust", &nclust, &b_nclust);
	TCluster->SetBranchAddress("planeID", &planeID, &b_planeID);
	TCluster->SetBranchAddress("clustPos", &clustPos, &b_clustPos);
	TCluster->SetBranchAddress("clustTimebin", &clustTimebin, &b_clustTimebin);
	TCluster->SetBranchAddress("clustSize", &clustSize, &b_clustSize);
	TCluster->SetBranchAddress("clustADCs", &clustADCs, &b_clustADCs);

	/******************* Calculate the min and max positions for each eta sectors ************************/

	Int_t i=0,j=0,k=0;
	Float_t PosMax1=0,PosMin1=0;
	Float_t PosMax2=0,PosMin2=0;
	Float_t PosMax3=0,PosMin3=0;
	Float_t PosMax4=0,PosMin4=0;
	Float_t PosMax5=0,PosMin5=0;
	Float_t PosMax6=0,PosMin6=0;
	Float_t PosMax7=0,PosMin7=0;
	Float_t PosMax8=0,PosMin8=0;

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

	}// end of loop on events
	
	cout << "*********Eta sector 1*********" << endl;
	cout << "Processed " << i+1 << " events" << endl;
	cout << "Min position eta 1: " << PosMin1 << endl;
	cout << "Max position eta 2: " << PosMax1 << endl;
	cout << "******************************" << endl;

	/************* End of calculating the min and max positions for each eta sector *******************/


	
	/***************** Calculate the positions of the slices for each eta sector **********************/

	Int_t Nslices=1;
	Float_t Interval1=0;
	Float_t Interval2=0;
	Float_t Interval3=0;
	Float_t Interval4=0;
	Float_t Interval5=0;
	Float_t Interval6=0;
	Float_t Interval7=0;
	Float_t Interval8=0;
	Float_t *VectPosEta1=NULL;
	Float_t *VectPosEta2=NULL;
	Float_t *VectPosEta3=NULL;
	Float_t *VectPosEta4=NULL;
	Float_t *VectPosEta5=NULL;
	Float_t *VectPosEta6=NULL;
	Float_t *VectPosEta7=NULL;
	Float_t *VectPosEta8=NULL;

	cout << "Please enter the number of slices/sector you want (1 - 384) " << endl;
	cin>>Nslices;

	VectPosEta1 = malloc(Nslices*sizeof(Float_t));	
	VectPosEta2 = malloc(Nslices*sizeof(Float_t));	
	VectPosEta3 = malloc(Nslices*sizeof(Float_t));	
	VectPosEta4 = malloc(Nslices*sizeof(Float_t));	
	VectPosEta5 = malloc(Nslices*sizeof(Float_t));	
	VectPosEta6 = malloc(Nslices*sizeof(Float_t));	
	VectPosEta7 = malloc(Nslices*sizeof(Float_t));	
	VectPosEta8 = malloc(Nslices*sizeof(Float_t));	

	Interval1=(PosMax1-PosMin1)/Nslices;// calculate the width of the slices
	Interval2=(PosMax2-PosMin2)/Nslices;
	Interval3=(PosMax3-PosMin3)/Nslices;
	Interval4=(PosMax4-PosMin4)/Nslices;
	Interval5=(PosMax5-PosMin5)/Nslices;
	Interval6=(PosMax6-PosMin6)/Nslices;
	Interval7=(PosMax7-PosMin7)/Nslices;
	Interval8=(PosMax8-PosMin8)/Nslices;
	
	cout << "Nslices= " << Nslices << " / Interval1= " << Interval1 << "mm" << endl;

	for(i=0;i<Nslices;i++)
	{
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

	hSlice1 = new TH1F("hSlice1","Slice",bin, 0, 10000);
	hSlice2 = new TH1F("hSlice2","Slice",bin, 0, 10000);
	hSlice3 = new TH1F("hSlice3","Slice",bin, 0, 10000);
	hSlice4 = new TH1F("hSlice4","Slice",bin, 0, 10000);
	hSlice5 = new TH1F("hSlice5","Slice",bin, 0, 10000);
	hSlice6 = new TH1F("hSlice6","Slice",bin, 0, 10000);
	hSlice7 = new TH1F("hSlice7","Slice",bin, 0, 10000);
	hSlice8 = new TH1F("hSlice8","Slice",bin, 0, 10000);

	int nfound1=0;// define the TSpectrum parameter 1
	int nfound2=0;
	int nfound3=0;
	int nfound4=0;
	int nfound5=0;
	int nfound6=0;
	int nfound7=0;
	int nfound8=0;

	float threshold=0.5;
	float sigma=5;
	float window=600;

	float *xpeaks1;// define the TSpectrum parameter 2
	float *xpeaks2;
	float *xpeaks3;
	float *xpeaks4;
	float *xpeaks5;
	float *xpeaks6;
	float *xpeaks7;
	float *xpeaks8;

	TSpectrum *s1 = new TSpectrum(2);// define the TSpectrum parameter 3
	TSpectrum *s2 = new TSpectrum(2);
	TSpectrum *s3 = new TSpectrum(2);
	TSpectrum *s4 = new TSpectrum(2);
	TSpectrum *s5 = new TSpectrum(2);
	TSpectrum *s6 = new TSpectrum(2);
	TSpectrum *s7 = new TSpectrum(2);
	TSpectrum *s8 = new TSpectrum(2);

	TF1 *fit1=new TF1("fit1","gaus",2000,4000);
	TF1 *fit2=new TF1("fit2","gaus",2000,4000);
	TF1 *fit3=new TF1("fit3","gaus",2000,4000);
	TF1 *fit4=new TF1("fit4","gaus",2000,4000);
	TF1 *fit5=new TF1("fit5","gaus",2000,4000);
	TF1 *fit6=new TF1("fit6","gaus",2000,4000);
	TF1 *fit7=new TF1("fit7","gaus",2000,4000);
	TF1 *fit8=new TF1("fit8","gaus",2000,4000);

	fit1->SetParameters(0,1000);// set the fit parameters
	fit1->SetParameters(1,3000);
	fit1->SetParameters(2,200);
	fit1->SetLineColor(2);

	fit2->SetParameters(0,1000);// set the fit parameters
	fit2->SetParameters(1,3000);
	fit2->SetParameters(2,200);
	fit2->SetLineColor(2);

	fit3->SetParameters(0,1000);// set the fit parameters
	fit3->SetParameters(1,3000);
	fit3->SetParameters(2,200);
	fit3->SetLineColor(2);

	fit4->SetParameters(0,1000);// set the fit parameters
	fit4->SetParameters(1,3000);
	fit4->SetParameters(2,200);
	fit4->SetLineColor(2);

	fit5->SetParameters(0,1000);// set the fit parameters
	fit5->SetParameters(1,3000);
	fit5->SetParameters(2,200);
	fit5->SetLineColor(2);

	fit6->SetParameters(0,1000);// set the fit parameters
	fit6->SetParameters(1,3000);
	fit6->SetParameters(2,200);
	fit6->SetLineColor(2);

	fit7->SetParameters(0,1000);// set the fit parameters
	fit7->SetParameters(1,3000);
	fit7->SetParameters(2,200);
	fit7->SetLineColor(2);

	fit8->SetParameters(0,1000);// set the fit parameters
	fit8->SetParameters(1,3000);
	fit8->SetParameters(2,200);
	fit8->SetLineColor(2);


	Float_t *VectFitMeanEta1=NULL,*VectFitMeanErrEta1=NULL;
	Float_t *VectFitSigmaEta1=NULL,*VectFitSigmaErrEta1=NULL;
	
	Float_t *VectFitMeanEta2=NULL,*VectFitMeanErrEta2=NULL;
	Float_t *VectFitSigmaEta2=NULL,*VectFitSigmaErrEta2=NULL;
	
	Float_t *VectFitMeanEta3=NULL,*VectFitMeanErrEta3=NULL;
	Float_t *VectFitSigmaEta3=NULL,*VectFitSigmaErrEta3=NULL;

	Float_t *VectFitMeanEta4=NULL,*VectFitMeanErrEta4=NULL;
	Float_t *VectFitSigmaEta4=NULL,*VectFitSigmaErrEta4=NULL;

	Float_t *VectFitMeanEta5=NULL,*VectFitMeanErrEta5=NULL;
	Float_t *VectFitSigmaEta5=NULL,*VectFitSigmaErrEta5=NULL;

	Float_t *VectFitMeanEta6=NULL,*VectFitMeanErrEta6=NULL;
	Float_t *VectFitSigmaEta6=NULL,*VectFitSigmaErrEta6=NULL;

	Float_t *VectFitMeanEta7=NULL,*VectFitMeanErrEta7=NULL;
	Float_t *VectFitSigmaEta7=NULL,*VectFitSigmaErrEta7=NULL;

	Float_t *VectFitMeanEta8=NULL,*VectFitMeanErrEta8=NULL;
	Float_t *VectFitSigmaEta8=NULL,*VectFitSigmaErrEta8=NULL;
	

	VectFitMeanEta1 = malloc(Nslices*sizeof(Float_t));
	VectFitMeanErrEta1 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaEta1 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaErrEta1 = malloc(Nslices*sizeof(Float_t));

	VectFitMeanEta2 = malloc(Nslices*sizeof(Float_t));
	VectFitMeanErrEta2 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaEta2 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaErrEta2 = malloc(Nslices*sizeof(Float_t));

	VectFitMeanEta3 = malloc(Nslices*sizeof(Float_t));
	VectFitMeanErrEta3 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaEta3 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaErrEta3 = malloc(Nslices*sizeof(Float_t));

	VectFitMeanEta4 = malloc(Nslices*sizeof(Float_t));
	VectFitMeanErrEta4 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaEta4 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaErrEta4 = malloc(Nslices*sizeof(Float_t));

	VectFitMeanEta5 = malloc(Nslices*sizeof(Float_t));
	VectFitMeanErrEta5 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaEta5 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaErrEta5 = malloc(Nslices*sizeof(Float_t));

	VectFitMeanEta6 = malloc(Nslices*sizeof(Float_t));
	VectFitMeanErrEta6 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaEta6 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaErrEta6 = malloc(Nslices*sizeof(Float_t));

	VectFitMeanEta7 = malloc(Nslices*sizeof(Float_t));
	VectFitMeanErrEta7 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaEta7 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaErrEta7 = malloc(Nslices*sizeof(Float_t));

	VectFitMeanEta8 = malloc(Nslices*sizeof(Float_t));
	VectFitMeanErrEta8 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaEta8 = malloc(Nslices*sizeof(Float_t));
	VectFitSigmaErrEta8 = malloc(Nslices*sizeof(Float_t));


	cout << "*** Processing the selection for each slices ***" << endl;
	cout << "************************************************" << endl;

	for (j=0;j<Nslices;j++)// loop on slices
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
						if(clustADCs[k] > 700 && clustSize[k] > 1 && clustTimebin[k] > 1 && clustTimebin[k] < 14){hSlice1->Fill(clustADCs[k]);}// event selection
					}// end of event selection

				}// end of etaSector1 selection 

				if( planeID[k] > 200 && planeID[k] < 300 )//etaSector2 selection 
				{		
					if( clustPos[k] > PosMin2+j*Interval2 && clustPos[k] < PosMin2+(j+1)*Interval2 )// position selection
					{
						if(clustADCs[k] > 700 && clustSize[k] > 1 && clustTimebin[k] > 1 && clustTimebin[k] < 14){hSlice2->Fill(clustADCs[k]);}// event selection
					}// end of event selection

				}// end of etaSector2 selection 

				if( planeID[k] > 300 && planeID[k] < 500 )//etaSector3 selection 
				{		
					if( clustPos[k] > PosMin3+j*Interval3 && clustPos[k] < PosMin3+(j+1)*Interval3 )// position selection
					{
						if(clustADCs[k] > 700 && clustSize[k] > 1 && clustTimebin[k] > 1 && clustTimebin[k] < 14){hSlice3->Fill(clustADCs[k]);}// event selection
					}// end of event selection

				}// end of etaSector3 selection 

				if( planeID[k] > 500 && planeID[k] < 700 )//etaSector4 selection 
				{		
					if( clustPos[k] > PosMin4+j*Interval4 && clustPos[k] < PosMin4+(j+1)*Interval4 )// position selection
					{
						if(clustADCs[k] > 700 && clustSize[k] > 1 && clustTimebin[k] > 1 && clustTimebin[k] < 14){hSlice4->Fill(clustADCs[k]);}// event selection
					}// end of event selection

				}// end of etaSector4 selection 

				if( planeID[k] > 700 && planeID[k] < 800 )//etaSector5 selection 
				{		
					if( clustPos[k] > PosMin5+j*Interval5 && clustPos[k] < PosMin5+(j+1)*Interval5 )// position selection
					{
						if(clustADCs[k] > 700 && clustSize[k] > 1 && clustTimebin[k] > 1 && clustTimebin[k] < 14){hSlice5->Fill(clustADCs[k]);}// event selection
					}// end of event selection

				}// end of etaSector5 selection 

				if( planeID[k] > 800 && planeID[k] < 1000 )//etaSector6 selection 
				{		
					if( clustPos[k] > PosMin6+j*Interval6 && clustPos[k] < PosMin6+(j+1)*Interval6 )// position selection
					{
						if(clustADCs[k] > 700 && clustSize[k] > 1 && clustTimebin[k] > 1 && clustTimebin[k] < 14){hSlice6->Fill(clustADCs[k]);}// event selection
					}// end of event selection

				}// end of etaSector6 selection 

				if( planeID[k] > 1000 && planeID[k] < 1100 )//etaSector7 selection 
				{		
					if( clustPos[k] > PosMin7+j*Interval7 && clustPos[k] < PosMin7+(j+1)*Interval7 )// position selection
					{
						if(clustADCs[k] > 700 && clustSize[k] > 1 && clustTimebin[k] > 1 && clustTimebin[k] < 14){hSlice7->Fill(clustADCs[k]);}// event selection
					}// end of event selection

				}// end of etaSector7 selection 

				if( planeID[k] > 1100 )//etaSector8 selection 
				{		
					if( clustPos[k] > PosMin8+j*Interval8 && clustPos[k] < PosMin8+(j+1)*Interval8 )// position selection
					{
						if(clustADCs[k] > 700 && clustSize[k] > 1 && clustTimebin[k] > 1 && clustTimebin[k] < 14){hSlice8->Fill(clustADCs[k]);}// event selection
					}// end of event selection

				}// end of etaSector8 selection 

			}// end of loop on clusters

		}// end of loop on events
	

		C1->cd(1);
		hSlice1->SetLineColor(1);
		TString Title1 = "eta1_slice_"+ IntToStr(j);
		hSlice1->SetTitle(Title1);
		hSlice1->Draw();

		nfound1=s1->Search(hSlice1,sigma,"nobackground",threshold);
		xpeaks1=s1->GetPositionX();
		hSlice1->Fit("fit1","QM","",(float)(xpeaks1[0]-window),(float)(xpeaks1[0]+window));

		VectFitMeanEta1[j] = fit1->GetParameter(1);
		VectFitMeanErrEta1[j] = fit1->GetParError(1);
		VectFitSigmaEta1[j] = fit1->GetParameter(2);
		VectFitSigmaErrEta1[j] = fit1->GetParError(2);

		C1->cd(2);
		hSlice2->SetLineColor(1);
		TString Title2 = "eta2_slice_"+ IntToStr(j);
		hSlice2->SetTitle(Title2);
		hSlice2->Draw();

		nfound2=s2->Search(hSlice2,sigma,"nobackground",threshold);
		xpeaks2=s2->GetPositionX();
		hSlice2->Fit("fit2","QM","",(float)(xpeaks2[0]-window),(float)(xpeaks2[0]+window));

		VectFitMeanEta2[j] = fit2->GetParameter(1);
		VectFitMeanErrEta2[j] = fit2->GetParError(1);
		VectFitSigmaEta2[j] = fit2->GetParameter(2);
		VectFitSigmaErrEta2[j] = fit2->GetParError(2);

		C1->cd(3);
		hSlice3->SetLineColor(1);
		TString Title3 = "eta3_slice_"+ IntToStr(j);
		hSlice3->SetTitle(Title3);
		hSlice3->Draw();

		nfound3=s3->Search(hSlice3,sigma,"nobackground",threshold);
		xpeaks3=s3->GetPositionX();
		hSlice3->Fit("fit3","QM","",(float)(xpeaks3[0]-window),(float)(xpeaks3[0]+window));

		VectFitMeanEta3[j] = fit3->GetParameter(1);
		VectFitMeanErrEta3[j] = fit3->GetParError(1);
		VectFitSigmaEta3[j] = fit3->GetParameter(2);
		VectFitSigmaErrEta3[j] = fit3->GetParError(2);

		C1->cd(4);
		hSlice4->SetLineColor(1);
		TString Title4 = "eta4_slice_"+ IntToStr(j);
		hSlice4->SetTitle(Title4);
		hSlice4->Draw();

		nfound4=s4->Search(hSlice4,sigma,"nobackground",threshold);
		xpeaks4=s4->GetPositionX();
		hSlice4->Fit("fit4","QM","",(float)(xpeaks4[0]-window),(float)(xpeaks4[0]+window));

		VectFitMeanEta4[j] = fit4->GetParameter(1);
		VectFitMeanErrEta4[j] = fit4->GetParError(1);
		VectFitSigmaEta4[j] = fit4->GetParameter(2);
		VectFitSigmaErrEta4[j] = fit4->GetParError(2);

		C1->cd(5);
		hSlice5->SetLineColor(1);
		TString Title5 = "eta5_slice_"+ IntToStr(j);
		hSlice5->SetTitle(Title5);
		hSlice5->Draw();

		nfound5=s5->Search(hSlice5,sigma,"nobackground",threshold);
		xpeaks5=s5->GetPositionX();
		hSlice5->Fit("fit5","QM","",(float)(xpeaks5[0]-window),(float)(xpeaks5[0]+window));

		VectFitMeanEta5[j] = fit5->GetParameter(1);
		VectFitMeanErrEta5[j] = fit5->GetParError(1);
		VectFitSigmaEta5[j] = fit5->GetParameter(2);
		VectFitSigmaErrEta5[j] = fit5->GetParError(2);

		C1->cd(6);
		hSlice6->SetLineColor(1);
		TString Title6 = "eta6_slice_"+ IntToStr(j);
		hSlice6->SetTitle(Title6);
		hSlice6->Draw();

		nfound6=s6->Search(hSlice6,sigma,"nobackground",threshold);
		xpeaks6=s6->GetPositionX();
		hSlice6->Fit("fit6","QM","",(float)(xpeaks6[0]-window),(float)(xpeaks6[0]+window));

		VectFitMeanEta6[j] = fit6->GetParameter(1);
		VectFitMeanErrEta6[j] = fit6->GetParError(1);
		VectFitSigmaEta6[j] = fit6->GetParameter(2);
		VectFitSigmaErrEta6[j] = fit6->GetParError(2);

		C1->cd(7);
		hSlice7->SetLineColor(1);
		TString Title7 = "eta7_slice_"+ IntToStr(j);
		hSlice7->SetTitle(Title7);
		hSlice7->Draw();

		nfound7=s7->Search(hSlice7,sigma,"nobackground",threshold);
		xpeaks7=s7->GetPositionX();
		hSlice7->Fit("fit7","QM","",(float)(xpeaks7[0]-window),(float)(xpeaks7[0]+window));

		VectFitMeanEta7[j] = fit7->GetParameter(1);
		VectFitMeanErrEta7[j] = fit7->GetParError(1);
		VectFitSigmaEta7[j] = fit7->GetParameter(2);
		VectFitSigmaErrEta7[j] = fit7->GetParError(2);

		C1->cd(8);
		hSlice8->SetLineColor(1);
		TString Title8 = "eta8_slice_"+ IntToStr(j);
		hSlice8->SetTitle(Title8);
		hSlice8->Draw();

		nfound8=s8->Search(hSlice8,sigma,"nobackground",threshold);
		xpeaks8=s8->GetPositionX();
		hSlice8->Fit("fit8","QM","",(float)(xpeaks8[0]-window),(float)(xpeaks8[0]+window));

		VectFitMeanEta8[j] = fit8->GetParameter(1);
		VectFitMeanErrEta8[j] = fit8->GetParError(1);
		VectFitSigmaEta8[j] = fit8->GetParameter(2);
		VectFitSigmaErrEta8[j] = fit8->GetParError(2);


		cout << "************************************************" << endl;

		cout <<"Eta 1 Slice " << j+1 <<" position : " << VectPosEta1[j] <<  " mean : " << VectFitMeanEta1[j] << " +/- " << VectFitMeanErrEta1[j] << " sigma : " << VectFitSigmaEta1[j] << " +/- " << VectFitSigmaErrEta1[j] << endl;

		cout <<"Eta 2 Slice " << j+1 <<" position : " << VectPosEta2[j] <<  " mean : " << VectFitMeanEta2[j] << " +/- " << VectFitMeanErrEta2[j] << " sigma : " << VectFitSigmaEta2[j] << " +/- " << VectFitSigmaErrEta2[j] << endl;

		cout <<"Eta 3 Slice " << j+1 <<" position : " << VectPosEta3[j] <<  " mean : " << VectFitMeanEta3[j] << " +/- " << VectFitMeanErrEta3[j] << " sigma : " << VectFitSigmaEta3[j] << " +/- " << VectFitSigmaErrEta3[j] << endl;

		cout <<"Eta 4 Slice " << j+1 <<" position : " << VectPosEta4[j] <<  " mean : " << VectFitMeanEta4[j] << " +/- " << VectFitMeanErrEta4[j] << " sigma : " << VectFitSigmaEta4[j] << " +/- " << VectFitSigmaErrEta4[j] << endl;

		cout <<"Eta 5 Slice " << j+1 <<" position : " << VectPosEta5[j] <<  " mean : " << VectFitMeanEta5[j] << " +/- " << VectFitMeanErrEta5[j] << " sigma : " << VectFitSigmaEta5[j] << " +/- " << VectFitSigmaErrEta5[j] << endl;

		cout <<"Eta 6 Slice " << j+1 <<" position : " << VectPosEta6[j] <<  " mean : " << VectFitMeanEta6[j] << " +/- " << VectFitMeanErrEta6[j] << " sigma : " << VectFitSigmaEta6[j] << " +/- " << VectFitSigmaErrEta6[j] << endl;

		cout <<"Eta 7 Slice " << j+1 <<" position : " << VectPosEta7[j] <<  " mean : " << VectFitMeanEta7[j] << " +/- " << VectFitMeanErrEta7[j] << " sigma : " << VectFitSigmaEta7[j] << " +/- " << VectFitSigmaErrEta7[j] << endl;

		cout <<"Eta 8 Slice " << j+1 <<" position : " << VectPosEta8[j] <<  " mean : " << VectFitMeanEta8[j] << " +/- " << VectFitMeanErrEta8[j] << " sigma : " << VectFitSigmaEta8[j] << " +/- " << VectFitSigmaErrEta8[j] << endl;

		cout << "************************************************" << endl;

		//*********************************************************************************************************************************
		TString fileN = "Results_" + IntToStr(Nslices) + "slices" ;// write parameters in txt file

		ofstream fileData(fileN, ios::out | ios::app);
		if(fileData)
		{
			fileData 	<< j+1 << " " 

					<< VectPosEta1[j] << " " 
					<< VectFitMeanEta1[j] << " " 
					<< VectFitMeanErrEta1[j] << " " 
					<< VectFitSigmaEta1[j] << " " 
					<< VectFitSigmaErrEta1[j] << " "

					<< VectPosEta2[j] << " " 
					<< VectFitMeanEta2[j] << " " 
					<< VectFitMeanErrEta2[j] << " " 
					<< VectFitSigmaEta2[j] << " " 
					<< VectFitSigmaErrEta2[j] << " "

					<< VectPosEta3[j] << " " 
					<< VectFitMeanEta3[j] << " " 
					<< VectFitMeanErrEta3[j] << " " 
					<< VectFitSigmaEta3[j] << " " 
					<< VectFitSigmaErrEta3[j] << " "

					<< VectPosEta4[j] << " " 
					<< VectFitMeanEta4[j] << " " 
					<< VectFitMeanErrEta4[j] << " " 
					<< VectFitSigmaEta4[j] << " " 
					<< VectFitSigmaErrEta4[j] << " "

					<< VectPosEta5[j] << " " 
					<< VectFitMeanEta5[j] << " " 
					<< VectFitMeanErrEta5[j] << " " 
					<< VectFitSigmaEta5[j] << " " 
					<< VectFitSigmaErrEta5[j] << " "

					<< VectPosEta6[j] << " " 
					<< VectFitMeanEta6[j] << " " 
					<< VectFitMeanErrEta6[j] << " " 
					<< VectFitSigmaEta6[j] << " " 
					<< VectFitSigmaErrEta6[j] << " "

					<< VectPosEta7[j] << " " 
					<< VectFitMeanEta7[j] << " " 
					<< VectFitMeanErrEta7[j] << " " 
					<< VectFitSigmaEta7[j] << " " 
					<< VectFitSigmaErrEta7[j] << " "

					<< VectPosEta8[j] << " " 
					<< VectFitMeanEta8[j] << " " 
					<< VectFitMeanErrEta8[j] << " " 
					<< VectFitSigmaEta8[j] << " " 
					<< VectFitSigmaErrEta8[j] << " " 

					<< endl;
			fileData.close();
		}
		else {cerr << "File doesn't openned" << endl;} // end of writing parameters in txt file			
		//*********************************************************************************************************************************
	
		C1->Modified();
	  	C1->Update();

		TString imgName = "slice_"+ IntToStr(j) + ".png"; // saving histograms as .png
		C1->SaveAs(imgName);

	}// end of loop on slices

	cout << "************************************************" << endl;

	/********************** End of processing the event selection and fit **************************/

	/************************** Display the Uniformity of the chamber ******************************/

	TCanvas *C2 = new TCanvas("C2","Eta1:Uniformity",1400,800);
	TGraphErrors *Uniformity1 = new TGraphErrors(Nslices,VectPosEta1,VectFitMeanEta1,0,VectFitMeanErrEta1);
	TGraphErrors *Uniformity2 = new TGraphErrors(Nslices,VectPosEta2,VectFitMeanEta2,0,VectFitMeanErrEta2);
	TGraphErrors *Uniformity3 = new TGraphErrors(Nslices,VectPosEta3,VectFitMeanEta3,0,VectFitMeanErrEta3);
	TGraphErrors *Uniformity4 = new TGraphErrors(Nslices,VectPosEta4,VectFitMeanEta4,0,VectFitMeanErrEta4);
	TGraphErrors *Uniformity5 = new TGraphErrors(Nslices,VectPosEta5,VectFitMeanEta5,0,VectFitMeanErrEta5);
	TGraphErrors *Uniformity6 = new TGraphErrors(Nslices,VectPosEta6,VectFitMeanEta6,0,VectFitMeanErrEta6);
	TGraphErrors *Uniformity7 = new TGraphErrors(Nslices,VectPosEta7,VectFitMeanEta7,0,VectFitMeanErrEta7);
	TGraphErrors *Uniformity8 = new TGraphErrors(Nslices,VectPosEta8,VectFitMeanEta8,0,VectFitMeanErrEta8);

	C2->Divide(4,2);

	C2->cd(1);

	Uniformity1->SetMarkerStyle(7);
	Uniformity1->SetMarkerColor(0);
	Uniformity1->SetLineColor(1);
	Uniformity1->SetLineWidth(1);
	Uniformity1->SetTitle("Eta 1 Uniformity");
	Uniformity1->GetYaxis()->SetTitle("Mean [ADC channel]");
	Uniformity1->GetYaxis()->SetRangeUser(1000,5000);
	Uniformity1->GetXaxis()->SetTitle("Position [mm]");
	Uniformity1->Draw("ALP");

	C2->cd(2);

	Uniformity2->SetMarkerStyle(7);
	Uniformity2->SetMarkerColor(0);
	Uniformity2->SetLineColor(1);
	Uniformity2->SetLineWidth(1);
	Uniformity2->SetTitle("Eta 2 Uniformity");
	Uniformity2->GetYaxis()->SetTitle("Mean [ADC channel]");
	Uniformity2->GetYaxis()->SetRangeUser(1000,5000);
	Uniformity2->GetXaxis()->SetTitle("Position [mm]");
	Uniformity2->Draw("ALP");

	C2->cd(3);

	Uniformity3->SetMarkerStyle(7);
	Uniformity3->SetMarkerColor(0);
	Uniformity3->SetLineColor(1);
	Uniformity3->SetLineWidth(1);
	Uniformity3->SetTitle("Eta 3 Uniformity");
	Uniformity3->GetYaxis()->SetTitle("Mean [ADC channel]");
	Uniformity3->GetYaxis()->SetRangeUser(1000,5000);
	Uniformity3->GetXaxis()->SetTitle("Position [mm]");
	Uniformity3->Draw("ALP");

	C2->cd(4);

	Uniformity4->SetMarkerStyle(7);
	Uniformity4->SetMarkerColor(0);
	Uniformity4->SetLineColor(1);
	Uniformity4->SetLineWidth(1);
	Uniformity4->SetTitle("Eta 4 Uniformity");
	Uniformity4->GetYaxis()->SetTitle("Mean [ADC channel]");
	Uniformity4->GetYaxis()->SetRangeUser(1000,5000);
	Uniformity4->GetXaxis()->SetTitle("Position [mm]");
	Uniformity4->Draw("ALP");

	C2->cd(5);

	Uniformity5->SetMarkerStyle(7);
	Uniformity5->SetMarkerColor(0);
	Uniformity5->SetLineColor(1);
	Uniformity5->SetLineWidth(1);
	Uniformity5->SetTitle("Eta 5 Uniformity");
	Uniformity5->GetYaxis()->SetTitle("Mean [ADC channel]");
	Uniformity5->GetYaxis()->SetRangeUser(1000,5000);
	Uniformity5->GetXaxis()->SetTitle("Position [mm]");
	Uniformity5->Draw("ALP");

	C2->cd(6);

	Uniformity6->SetMarkerStyle(7);
	Uniformity6->SetMarkerColor(0);
	Uniformity6->SetLineColor(1);
	Uniformity6->SetLineWidth(1);
	Uniformity6->SetTitle("Eta 6 Uniformity");
	Uniformity6->GetYaxis()->SetTitle("Mean [ADC channel]");
	Uniformity6->GetYaxis()->SetRangeUser(1000,5000);
	Uniformity6->GetXaxis()->SetTitle("Position [mm]");
	Uniformity6->Draw("ALP");

	C2->cd(7);

	Uniformity7->SetMarkerStyle(7);
	Uniformity7->SetMarkerColor(0);
	Uniformity7->SetLineColor(1);
	Uniformity7->SetLineWidth(1);
	Uniformity7->SetTitle("Eta 7 Uniformity");
	Uniformity7->GetYaxis()->SetTitle("Mean [ADC channel]");
	Uniformity7->GetYaxis()->SetRangeUser(1000,5000);
	Uniformity7->GetXaxis()->SetTitle("Position [mm]");
	Uniformity7->Draw("ALP");

	C2->cd(8);

	Uniformity8->SetMarkerStyle(7);
	Uniformity8->SetMarkerColor(0);
	Uniformity8->SetLineColor(1);
	Uniformity8->SetLineWidth(1);
	Uniformity8->SetTitle("Eta 8 Uniformity");
	Uniformity8->GetYaxis()->SetTitle("Mean [ADC channel]");
	Uniformity8->GetYaxis()->SetRangeUser(1000,5000);
	Uniformity8->GetXaxis()->SetTitle("Position [mm]");
	Uniformity8->Draw("ALP");

	C2->Modified();
	C2->Update();

	TString imgNameFinal = "UniformityFinalTimeBin.png"; // saving histogram as .png
	C2->SaveAs(imgNameFinal);

	/*********************** End of displaying uniformity of the chamber ***************************/

	/********************************* Display the 3D gain maps *************************************/

	int dim=Nslices*8;
	TCanvas *C3 = new TCanvas("C3","GE1/1:3D Uniformity",700,400);
	TGraph2D *Gg = new TGraph2D(dim);

	for (j=0;j<Nslices;j++)// loop on slices
	{		
		Gg->SetPoint(j*8,VectPosEta1[j],139,VectFitMeanEta1[j]);
		Gg->SetPoint(j*8+1,VectPosEta2[j],236,VectFitMeanEta2[j]);
		Gg->SetPoint(j*8+2,VectPosEta3[j],409,VectFitMeanEta3[j]);
		Gg->SetPoint(j*8+3,VectPosEta4[j],571,VectFitMeanEta4[j]);
		Gg->SetPoint(j*8+4,VectPosEta5[j],733,VectFitMeanEta5[j]);
		Gg->SetPoint(j*8+5,VectPosEta6[j],867,VectFitMeanEta6[j]);
		Gg->SetPoint(j*8+6,VectPosEta7[j],1056,VectFitMeanEta7[j]);
		Gg->SetPoint(j*8+7,VectPosEta8[j],1169,VectFitMeanEta8[j]);
	}// end of loop on slices

	C3->cd();

	Gg->Draw("pcol");

	C3->Modified();
	C3->Update();

	TString imgName3D = "3D_Uniformity.png"; // saving plot as .png
	C3->SaveAs(imgName3D);

	TCanvas *C4 = new TCanvas("C4","GE1/1:3D Uniformity",700,400);
	TH2D* h1 = new TH2D("h1", " GE1/1 gain map (histo)",Nslices-1,1.0,(float)Nslices,7,1.0,8.0);

	for (int m=1; m < Nslices; m++) // loop on slices
	{ 
			h1->SetBinContent(m,1,VectFitMeanEta1[m]);
			h1->SetBinContent(m,2,VectFitMeanEta2[m]);
			h1->SetBinContent(m,3,VectFitMeanEta3[m]);
			h1->SetBinContent(m,4,VectFitMeanEta4[m]);
			h1->SetBinContent(m,5,VectFitMeanEta5[m]);
			h1->SetBinContent(m,6,VectFitMeanEta6[m]);
			h1->SetBinContent(m,7,VectFitMeanEta7[m]);
			h1->SetBinContent(m,8,VectFitMeanEta8[m]);	
	}// end of loop on slices

	C4->cd();
	h1->Draw("COLZ");
	C4->Modified();
	C4->Update();

	TString imgName3D1 = "3D_Uniformity_histo.png"; // saving plot as .png
	C4->SaveAs(imgName3D1);

	TCanvas *C5 = new TCanvas("C5","GE1/1:3D Uniformity",700,400);
	TH2D* h2 = new TH2D("h2", " GE1/1 Energy resolution map (histo)",Nslices-1,1.0,(float)Nslices,7,1.0,8.0);

	for (int m=1; m < Nslices; m++) // loop on slices
	{ 
			h2->SetBinContent(m,1,VectFitSigmaEta1[m]/VectFitMeanEta1[m]);
			h2->SetBinContent(m,2,VectFitSigmaEta2[m]/VectFitMeanEta2[m]);
			h2->SetBinContent(m,3,VectFitSigmaEta3[m]/VectFitMeanEta3[m]);
			h2->SetBinContent(m,4,VectFitSigmaEta4[m]/VectFitMeanEta4[m]);
			h2->SetBinContent(m,5,VectFitSigmaEta5[m]/VectFitMeanEta5[m]);
			h2->SetBinContent(m,6,VectFitSigmaEta6[m]/VectFitMeanEta6[m]);
			h2->SetBinContent(m,7,VectFitSigmaEta7[m]/VectFitMeanEta7[m]);
			h2->SetBinContent(m,8,VectFitSigmaEta8[m]/VectFitMeanEta8[m]);	
	}// end of loop on slices

	C5->cd();
	h2->Draw("COLZ");
	C5->Modified();
	C5->Update();

	TString imgName3D2 = "3D_EnergyRes_histo.png"; // saving plot as .png
	C5->SaveAs(imgName3D2);


	/***************************** End of displaying the 3D gain maps *******************************/

	
	cout << "End of program" << endl;


	free(VectPosEta1);
	free(VectFitMeanEta1);
	free(VectFitMeanErrEta1);
	free(VectFitSigmaEta1);
	free(VectFitSigmaErrEta1);

	free(VectPosEta2);
	free(VectFitMeanEta2);
	free(VectFitMeanErrEta2);
	free(VectFitSigmaEta2);
	free(VectFitSigmaErrEta2);

	free(VectPosEta3);
	free(VectFitMeanEta3);
	free(VectFitMeanErrEta3);
	free(VectFitSigmaEta3);
	free(VectFitSigmaErrEta3);

	free(VectPosEta4);
	free(VectFitMeanEta4);
	free(VectFitMeanErrEta4);
	free(VectFitSigmaEta4);
	free(VectFitSigmaErrEta4);

	free(VectPosEta5);
	free(VectFitMeanEta5);
	free(VectFitMeanErrEta5);
	free(VectFitSigmaEta5);
	free(VectFitSigmaErrEta5);

	free(VectPosEta6);
	free(VectFitMeanEta6);
	free(VectFitMeanErrEta6);
	free(VectFitSigmaEta6);
	free(VectFitSigmaErrEta6);

	free(VectPosEta7);
	free(VectFitMeanEta7);
	free(VectFitMeanErrEta7);
	free(VectFitSigmaEta7);
	free(VectFitSigmaErrEta7);

	free(VectPosEta8);
	free(VectFitMeanEta8);
	free(VectFitMeanErrEta8);
	free(VectFitSigmaEta8);
	free(VectFitSigmaErrEta8);

	return(0);
}

TString IntToStr(int n)
{
    stringstream result;
    result << n;
    return result.str();
}
