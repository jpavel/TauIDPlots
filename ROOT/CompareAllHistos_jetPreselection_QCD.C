#include <TFile.h>
#include <TKey.h>
#include <TH1D.h>
#include <TF1.h>
#include <TLegend.h>
#include <TPostScript.h>
#include <TPaveText.h>
#include <TProfile.h>

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "./plotStyles/AtlasStyle.C"
#include "./plotStyles/AtlasUtils.h"
#include "./plotStyles/AtlasUtils.C"
#include "./plotStyles/TriggerPerfPaperConsts.h"
#include "./plotStyles/tdrstyle.C"

int CompareAllHistos(TString input1 = "../../QCD_ref.root", TString input2 = "../../QCD_filter.root", TString outdir="../plots/QCD/", TString leg1="no cut", TString leg2="jet filter", TString name="RelValQCD", bool MakeTotal=false) {
	
	gROOT->Reset();             
  //SetAtlasStyle();
  setTDRStyle();
  gStyle->SetPalette(1);
  gStyle->SetErrorX(0.5);
  gROOT->ProcessLine(".!mkdir -p "+outdir);
  gROOT->ProcessLine(".!mkdir -p "+outdir+"/pdf");
  gROOT->ProcessLine(".!mkdir -p "+outdir+"/png");

  
  TString Names[2] = { input1, input2};
  
//  TString outdir = "../plots/QCD/";
  const uint nFiles = 2;
  TFile * f[nFiles];  	

  
  std::stringstream indexes;
  for(uint iFile = 0; iFile < nFiles; iFile++)
  {
	indexes.str("");
  	indexes << Names[iFile];
  	
	std::string input_file=indexes.str();
	f[iFile] = TFile::Open(input_file.c_str());
	if(!f[iFile]) {
		std::cerr << "Error: file " << input_file << " could not be opened." << std::endl; 
    return 1;
	}
	else std::cout << "File " << input_file << " succesfully opened!" << std::endl;
  
  }
 // const uint nProfile12=8;
  const uint nProfile=4;
 // const uint nWP=3;
 // const uint n123=3;
  const uint n12=5;
  
  TString dirNames[1] = { "tauDifferenceAnalyzer"}; //"mmet_zh","mmmt_zh","mmme_zh","eett_zh","eemt_zh","eeet_zh","eeem_zh"};
 // TString profileNames_12[nProfile12] = {"pt_DM","eta_DM","vx_DM","phi_DM","pt_DMall","eta_DMall","vx_DMall","phi_DMall"};
  TString prefix_12 = "h_eff_id_";
  TString suffix_12[n12] = {"_DM","_DMall","_loose","_medium","_tight"};
  
  TString profileX[nProfile] = {"pt","eta","vx","phi"};
  const uint nTotal=nProfile*n12;
  //~ TString profile123_prefix="h_eff_id_";
  //~ TString profile123_WP[nWP]={"_loose","_medium","_tight"};
  //~ TString profile123_suffix[n123]={"","_2","_3"};
  //~ 
  TProfile* profileHist1[nTotal][nFiles];
// TProfile* profileHist2[nTotal][nFiles];
  
  //TProfile* profileHist[n123][nProfile*nWP][nFiles];
  //~ TProfile* profileHistB[nProfile*nWP][nFiles];
  //~ TProfile* profileHistC[nProfile*nWP][nFiles];
  //~ 
  
  
 // std::stringstream name;
 for(uint iFile = 0; iFile < nFiles; iFile++)
  {
	for(uint iSuff=0; iSuff < n12; iSuff++)
	{		
		for(uint iProf12=0; iProf12< nProfile; iProf12++)
		{
			profileHist1[iProf12+iSuff*nProfile][iFile]=(TProfile*)f[iFile]->Get(TString(dirNames[0]+"/"+prefix_12+profileX[iProf12]+suffix_12[iSuff]));
		//	profileHist2[iProf12+iSuff*nProfile][iFile]=(TProfile*)f[iFile]->Get(TString(dirNames[0]+"/"+prefix_12+profileX[iProf12]+suffix_12[iSuff]));
		}
	}
  }
  
  //~ for(uint iFile = 0; iFile < nFiles; iFile++)
  //~ {
	//~ for(uint iWP=0; iWP<nWP; iWP++)
		//~ {
			//~ for(uint iProf123=0; iProf123< nProfile; iProf123++)
			//~ {
				//~ for(uint i123=0; i123 < n123; i123++)
				//~ {
					//~ std::cout << dirNames[0]+"/"+profile123_prefix+profileX[iProf123]+profile123_WP[iWP]+profile123_suffix[i123] << std::endl;
					//~ profileHist[i123][iWP*nProfile+iProf123][iFile]=(TProfile*)f[iFile]->Get(TString(dirNames[0]+"/"+profile123_prefix+profileX[iProf123]+profile123_WP[iWP]+profile123_suffix[i123]));
				//~ }
			//~ }
		//~ }
  //~ }
  
  
  
  
  TCanvas *c1 = new TCanvas("c1","",5,30,1024,1024);
  c1->SetGrid(0,0);
  c1->SetFillStyle(4000);
  c1->SetFillColor(10);
  c1->SetTicky();
  c1->SetObjectStat(0);
  
  TPad* histPad = new TPad("histPad","histPad",0.01,0.25,0.99,0.99);
  histPad->SetBottomMargin(0.02);
  histPad->Draw();
  
  TPad* diffPad = new TPad("diffPad","diffPad",0.01,0.01,0.99,0.25);
  diffPad->SetBottomMargin(0.3);
  diffPad->SetTopMargin(0.0);
  diffPad->Draw();
  
  // plotting
  
	TString XTitle[4] = {"True P_{T} [GeV]","True #eta", "nVx","True #phi"};
   // c1->SetLogy();
	std::cout << "hej hola " << std::endl;
	for(uint iProf12=0; iProf12< nTotal; iProf12++)
	{
		 int title_id=iProf12%4;
		
		 if(title_id!=2){
			 profileHist1[iProf12][0]->Rebin(5);
			 profileHist1[iProf12][1]->Rebin(5);
		 }
	
		 TH1D* baseHist = (TH1D*)profileHist1[iProf12][0]->Clone();
		 TH1D* overHist = (TH1D*)profileHist1[iProf12][1]->Clone();
		 
		 if(MakeTotal && iProf12>7){ 
			 double binLow = baseHist->GetXaxis()->GetBinLowEdge(baseHist->GetXaxis()->GetFirst());
			 double binUp = baseHist->GetXaxis()->GetBinUpEdge(baseHist->GetXaxis()->GetLast());
			 int nBins = baseHist->GetXaxis()->GetNbins();
			 
			 TH1D* newBaseHist = new TH1D(TString(baseHist->GetName())+"TOT",baseHist->GetTitle(),nBins,binLow,binUp);
			 TH1D* newDMHist = new TH1D("newDMHist","",nBins,binLow,binUp);
			 TH1D* newOverHist = new TH1D(TString(overHist->GetName())+"TOTover",baseHist->GetTitle(),nBins,binLow,binUp);
			 TH1D* newDMHist2 = new TH1D("newDMHist2","",nBins,binLow,binUp);
			 
			 
			 for(int iBin=0; iBin <= baseHist->GetNbinsX(); iBin++)
			 {
				newBaseHist->SetBinContent(iBin,baseHist->GetBinContent(iBin));
				newBaseHist->SetBinError(iBin,baseHist->GetBinError(iBin));
				newDMHist->SetBinContent(iBin,profileHist1[title_id][0]->GetBinContent(iBin));
				newDMHist->SetBinError(iBin,profileHist1[title_id][0]->GetBinError(iBin));
				newOverHist->SetBinContent(iBin,overHist->GetBinContent(iBin));
				newOverHist->SetBinError(iBin,overHist->GetBinError(iBin));
				newDMHist2->SetBinContent(iBin,profileHist1[title_id][1]->GetBinContent(iBin));
				newDMHist2->SetBinError(iBin,profileHist1[title_id][1]->GetBinError(iBin));	
				
			 }
			 newBaseHist->Multiply(newDMHist);
			 newOverHist->Multiply(newDMHist2);
			 baseHist=newBaseHist;
			 overHist=newOverHist;
			 delete newDMHist;
			 delete newDMHist2;
		 }
	
		 if(title_id==0) baseHist->GetXaxis()->SetRangeUser(0,100);
		 baseHist->GetXaxis()->SetLabelOffset(0.1);		
		 baseHist->GetXaxis()->SetTitle(XTitle[title_id]);
		 baseHist->GetYaxis()->SetTitle("Fake rate");
		 baseHist->SetMarkerStyle(20);
		 baseHist->SetMarkerSize(2);
		 baseHist->SetLineWidth(2);
		 
		 overHist->SetMarkerColor(kRed);
		 overHist->SetMarkerStyle(25);
		 overHist->SetMarkerSize(2);
		 overHist->SetLineWidth(2);
		 overHist->SetLineStyle(2);
		 overHist->SetLineColor(kRed);
		
		 int binmax = baseHist->GetMaximumBin();
	     double max = 1.2*(baseHist->GetBinContent(binmax)+baseHist->GetBinError(binmax));
	     int binmax2 = overHist->GetMaximumBin();
	     double max2 = 1.2*(overHist->GetBinContent(binmax2)+overHist->GetBinError(binmax2));
	     max = max2 > max ? max2 : max;
	 
	     baseHist->SetMaximum(10.0);
	     baseHist->SetMinimum(1e-3);
	     
		 
		 histPad->cd();	
			 histPad->SetLogy();
		 baseHist->Draw();
		 overHist->Draw("same");	
		 
		TLegend* leg = new TLegend(0.8,0.80,0.9,0.9,NULL,"brNDC");
		leg->SetFillColor(0);
		leg->SetTextSize(0.035);
		leg->SetBorderSize(0);
		
		leg->AddEntry(baseHist, leg1, "lp");
		leg->AddEntry(overHist, leg2, "lp");
		
		leg->Draw();
		 //~ 
		 
    TString lumist="";
	TPaveText *ll = new TPaveText(0.15, 0.95, 0.95, 0.99, "NDC");
	ll->SetTextSize(0.03);
	ll->SetTextFont(62);
	ll->SetFillColor(0);
	ll->SetBorderSize(0);
	ll->SetMargin(0.01);
	ll->SetTextAlign(12); // align left
	TString text = name;
	ll->AddText(0.01,0.7,text);
	text = "#sqrt{s} = 13 TeV";
	text = text + lumist;
	//  ll->SetTextAlign(32); // align right
	ll->AddText(0.7, 0.5, text);
	ll->Draw("same");
	  
	
	c1->Update();
	
	TH1D* h_diff=(TH1D*)baseHist->Clone();
	h_diff->Divide(overHist);
	diffPad->cd();
	diffPad->SetGridy();
	diffPad->SetGridx();
	
	h_diff->GetYaxis()->SetLabelSize(0.1);
    h_diff->GetYaxis()->SetTitleOffset(0.55);
    h_diff->GetYaxis()->SetTitleSize(0.12);
    h_diff->GetYaxis()->SetTitle("Ratio");
    h_diff->GetXaxis()->SetLabelSize(0.12);
    h_diff->GetXaxis()->SetLabelOffset(0.01);
    h_diff->GetXaxis()->SetTitleSize(0.15);
	h_diff->GetYaxis()->SetNdivisions(509);
	h_diff->SetMinimum(0.9);
	h_diff->SetMaximum(1.1);
	
	h_diff->Draw("hist");
	c1->Print(outdir+"/pdf/"+profileHist1[iProf12][0]->GetName()+".pdf");
	c1->Print(outdir+"/png/"+profileHist1[iProf12][0]->GetName()+".png");
	
	
	}
  
  
	return 0;
}
