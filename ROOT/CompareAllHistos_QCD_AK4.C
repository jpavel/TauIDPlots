#include <TFile.h>
#include <TKey.h>
#include <TH1D.h>
#include <TF1.h>
#include <TLegend.h>
#include <TPostScript.h>
#include <TPaveText.h>
#include <TProfile.h>
#include <TProfile2D.h>


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

int CompareAllHistos(TString input1 = "../../ak4/output.root", TString input2 = "../../ak5/output.root") {
	
	gROOT->Reset();             
  //SetAtlasStyle();
  setTDRStyle();
  gStyle->SetPalette(1);
  gStyle->SetErrorX(0.5);
  
  TString Names[2] = { input1, input2};
  
  TString outdir = "AK4/QCD/";
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
  const uint nProfile12=8;
  const uint nProfile=4;
  const uint nWP=3;
  const uint n123=3;
  const uint n12=2;
  
  TString dirNames[1] = { "tauDifferenceAnalyzer"}; //"mmet_zh","mmmt_zh","mmme_zh","eett_zh","eemt_zh","eeet_zh","eeem_zh"};
  TString profileNames_12[nProfile12] = {"pt","eta","vx","phi","id_pt","id_eta","id_vx","id_phi"};
  TString prefix_12 = "h_eff_";
  TString suffix_12[n12] = {"_1","_2"};
  
  TString profileX[nProfile] = {"pt","eta","vx","phi"};
  TString profile123_prefix="h_eff_id_";
  TString profile123_WP[nWP]={"_loose","_medium","_tight"};
  TString profile123_suffix[n123]={"","_2","_3"};
  
  TProfile* profileHist1[nProfile12][nFiles];
  TProfile* profileHist2[nProfile12][nFiles];
  
  TProfile* profileHist[n123][nProfile*nWP][nFiles];
  //~ TProfile* profileHistB[nProfile*nWP][nFiles];
  //~ TProfile* profileHistC[nProfile*nWP][nFiles];
  //~ 
  
  const uint nIsoSum = 4;
  TH1D* isoSumHist1[nIsoSum][nFiles];
  TH1D* isoSumHist2[nIsoSum][nFiles];
  
  const uint nIsoSumProfile = 2;
  TProfile* isoSumProfile1[nIsoSumProfile][nFiles];
  TProfile* isoSumProfile2[nIsoSumProfile][nFiles];
  
  
  TString isoSumNames[nIsoSum]={"h_discPU","h_discRaw_ctrl","h_discRawN_ctrl","h_discRawCH_ctrl"};
  TString isoSumProfileNames[nIsoSumProfile]={"h_discRawN_dR","h_discRawCH_dR"};
  
  TProfile2D* isoSumProfile2D1[nIsoSumProfile][nFiles];
  TProfile2D* isoSumProfile2D2[nIsoSumProfile][nFiles];
  
  TString isoSumProfile2DNames[nIsoSumProfile]={"h_discRawN_dPhidEta","h_discRawCH_dPhidEta"};
  
  
 // std::stringstream name;
  for(uint iFile = 0; iFile < nFiles; iFile++)
  {
	for(uint iProf12=0; iProf12< nProfile12; iProf12++)
	{
		
		profileHist1[iProf12][iFile]=(TProfile*)f[iFile]->Get(TString(dirNames[0]+"/"+prefix_12+profileNames_12[iProf12]+suffix_12[0]));
		profileHist2[iProf12][iFile]=(TProfile*)f[iFile]->Get(TString(dirNames[0]+"/"+prefix_12+profileNames_12[iProf12]+suffix_12[1]));
	}
  }
  
  
  for(uint iFile = 0; iFile < nFiles; iFile++)
  {
	for(uint iIsoSum=0; iIsoSum< nIsoSum; iIsoSum++)
	{
		isoSumHist1[iIsoSum][iFile]=(TH1D*)f[iFile]->Get(TString(dirNames[0]+"/"+isoSumNames[iIsoSum]+"1"));
		isoSumHist2[iIsoSum][iFile]=(TH1D*)f[iFile]->Get(TString(dirNames[0]+"/"+isoSumNames[iIsoSum]+"2"));
	}
	for(uint iIsoSum=0; iIsoSum< nIsoSumProfile; iIsoSum++)
	{
		isoSumProfile1[iIsoSum][iFile]=(TProfile*)f[iFile]->Get(TString(dirNames[0]+"/"+isoSumProfileNames[iIsoSum]+"1"));
		isoSumProfile2[iIsoSum][iFile]=(TProfile*)f[iFile]->Get(TString(dirNames[0]+"/"+isoSumProfileNames[iIsoSum]+"2"));
	}
	
	for(uint iIsoSum=0; iIsoSum< nIsoSumProfile; iIsoSum++)
	{
		isoSumProfile2D1[iIsoSum][iFile]=(TProfile2D*)f[iFile]->Get(TString(dirNames[0]+"/"+isoSumProfile2DNames[iIsoSum]+"1"));
		isoSumProfile2D2[iIsoSum][iFile]=(TProfile2D*)f[iFile]->Get(TString(dirNames[0]+"/"+isoSumProfile2DNames[iIsoSum]+"2"));
	}
  }
  
  
  
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
  
	TString XTitle[4] = {"True visible P_{T} [GeV]","True visible #eta", "nVx","True visible #phi"};
    
	for(uint iProf12=0; iProf12< nProfile12; iProf12++)
	{
		 int title_id=iProf12%4;
		
		 profileHist2[iProf12][1]->Rebin(10);
		 profileHist1[iProf12][1]->Rebin(10);
	
	
		 if(title_id==0) profileHist2[iProf12][1]->GetXaxis()->SetRangeUser(10,100);
		 profileHist2[iProf12][1]->GetXaxis()->SetLabelOffset(0.1);		
		 profileHist2[iProf12][1]->GetXaxis()->SetTitle(XTitle[title_id]);
		 profileHist2[iProf12][1]->GetYaxis()->SetTitle("Fake Rate");
		 profileHist2[iProf12][1]->SetMarkerStyle(20);
		 profileHist2[iProf12][1]->SetMarkerSize(2);
		 profileHist2[iProf12][1]->SetLineWidth(2);
		 
		 profileHist1[iProf12][1]->SetMarkerColor(kRed);
		 profileHist1[iProf12][1]->SetMarkerStyle(25);
		 profileHist1[iProf12][1]->SetMarkerSize(2);
		 profileHist1[iProf12][1]->SetLineWidth(2);
		 profileHist1[iProf12][1]->SetLineStyle(2);
		 profileHist1[iProf12][1]->SetLineColor(kRed);
		
		 int binmax = profileHist2[iProf12][1]->GetMaximumBin();
	     double max = 1.05*(profileHist2[iProf12][1]->GetBinContent(binmax)+profileHist2[iProf12][1]->GetBinError(binmax));
	     int binmax2 = profileHist1[iProf12][1]->GetMaximumBin();
	     double max2 = 1.05*(profileHist1[iProf12][1]->GetBinContent(binmax2)+profileHist1[iProf12][1]->GetBinError(binmax2));
	     max = max2 > max ? max2 : max;
	 
	     profileHist2[iProf12][1]->SetMaximum(max);
		 
		 histPad->cd();	
		histPad->SetLogy();
		 profileHist2[iProf12][1]->Draw();
		 profileHist1[iProf12][1]->Draw("same");	
		 
		TLegend* leg = new TLegend(0.8,0.80,0.9,0.9,NULL,"brNDC");
		leg->SetFillColor(0);
		leg->SetTextSize(0.035);
		leg->SetBorderSize(0);
		
		leg->AddEntry(profileHist2[iProf12][1], "AK4", "lp");
		leg->AddEntry(profileHist1[iProf12][1], "AK5", "lp");
		
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
	TString text = "RelValQCD";
	ll->AddText(0.01,0.7,text);
	text = "#sqrt{s} = 13 TeV";
	text = text + lumist;
	//  ll->SetTextAlign(32); // align right
	ll->AddText(0.7, 0.5, text);
	ll->Draw("same");
	  
	
	c1->Update();
	
	TH1D* h_diff=(TH1D*)profileHist2[iProf12][1]->Clone();
	h_diff->Divide(profileHist1[iProf12][1]);
	diffPad->cd();
	diffPad->SetGridy();
	diffPad->SetGridx();
	
	h_diff->GetYaxis()->SetLabelSize(0.1);
    h_diff->GetYaxis()->SetTitleOffset(0.55);
    h_diff->GetYaxis()->SetTitleSize(0.12);
    h_diff->GetYaxis()->SetTitle("AK4/AK5");
    h_diff->GetXaxis()->SetLabelSize(0.12);
    h_diff->GetXaxis()->SetLabelOffset(0.01);
    h_diff->GetXaxis()->SetTitleSize(0.15);
	h_diff->GetYaxis()->SetNdivisions(509);
	h_diff->SetMinimum(0.9);
	h_diff->SetMaximum(2.05);
	
	h_diff->Draw("hist");
	c1->Print(outdir+"pdf/"+profileHist2[iProf12][1]->GetName()+".pdf");
	c1->Print(outdir+"png/"+profileHist2[iProf12][1]->GetName()+".png");
	
	
	}
	
	TString XTitleSum[4] = {"PU isolation sum [GeV]","Combined #Delta#beta corrected isolation sum[GeV]", "Neutral isolation sum[GeV]","Charged isolation sum[GeV]"};
	TString Sumsuffix[nFiles] = {"_Mike", "RECO"};
	TString legendRerun[nFiles]={"AK4+iso05","AK5"};
  for(uint iFile=0; iFile < nFiles; iFile++){
	for(uint iIsoSum=0; iIsoSum< nIsoSum; iIsoSum++)
	{
		 isoSumHist2[iIsoSum][iFile]->Rebin(20);
		 isoSumHist1[iIsoSum][iFile]->Rebin(20);
	//~ 
	
		 isoSumHist2[iIsoSum][iFile]->GetXaxis()->SetRangeUser(0,20);
		 isoSumHist2[iIsoSum][iFile]->GetXaxis()->SetLabelOffset(0.1);		
		 isoSumHist2[iIsoSum][iFile]->GetXaxis()->SetTitle(XTitleSum[iIsoSum]);
		 isoSumHist2[iIsoSum][iFile]->SetMarkerStyle(20);
		 isoSumHist2[iIsoSum][iFile]->SetMarkerSize(2);
		 isoSumHist2[iIsoSum][iFile]->SetLineWidth(2);
		 
		 isoSumHist1[iIsoSum][iFile]->SetMarkerColor(kRed);
		 isoSumHist1[iIsoSum][iFile]->SetMarkerStyle(25);
		 isoSumHist1[iIsoSum][iFile]->SetMarkerSize(2);
		 isoSumHist1[iIsoSum][iFile]->SetLineWidth(2);
		 isoSumHist1[iIsoSum][iFile]->SetLineStyle(2);
		 isoSumHist1[iIsoSum][iFile]->SetLineColor(kRed);
		
		 int binmax = isoSumHist2[iIsoSum][iFile]->GetMaximumBin();
	     double max = 1.05*(isoSumHist2[iIsoSum][iFile]->GetBinContent(binmax)+isoSumHist2[iIsoSum][iFile]->GetBinError(binmax));
	     int binmax2 = isoSumHist1[iIsoSum][iFile]->GetMaximumBin();
	     double max2 = 1.05*(isoSumHist1[iIsoSum][iFile]->GetBinContent(binmax2)+isoSumHist1[iIsoSum][iFile]->GetBinError(binmax2));
	     max = max2 > max ? max2 : max;
	 
	     isoSumHist2[iIsoSum][iFile]->SetMaximum(max);
		 
		 histPad->cd();	
		 histPad->SetLogy();
		
		 isoSumHist2[iIsoSum][iFile]->Draw();
		 isoSumHist1[iIsoSum][iFile]->Draw("same");	
		 
		TLegend* leg = new TLegend(0.8,0.80,0.9,0.9,NULL,"brNDC");
		leg->SetFillColor(0);
		leg->SetTextSize(0.035);
		leg->SetBorderSize(0);
		
		leg->AddEntry(isoSumHist2[iIsoSum][iFile], "AK4", "lp");
		leg->AddEntry(isoSumHist1[iIsoSum][iFile], legendRerun[iFile], "lp");
		
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
	TString text = "RelValQCD";
	ll->AddText(0.01,0.7,text);
	text = "#sqrt{s} = 13 TeV";
	text = text + lumist;
	//  ll->SetTextAlign(32); // align right
	ll->AddText(0.7, 0.5, text);
	ll->Draw("same");
	  
	
	c1->Update();
	
	TH1D* h_diff=(TH1D*)isoSumHist2[iIsoSum][iFile]->Clone();
	h_diff->Divide(isoSumHist1[iIsoSum][iFile]);
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
	h_diff->SetMinimum(0.5);
	h_diff->SetMaximum(2.05);
	
	h_diff->Draw("hist");
	c1->Print(outdir+"pdf/"+isoSumHist2[iIsoSum][iFile]->GetName()+Sumsuffix[iFile]+".pdf");
	c1->Print(outdir+"png/"+isoSumHist2[iIsoSum][iFile]->GetName()+Sumsuffix[iFile]+".png");
	}
}

for(uint iIsoSum=0; iIsoSum< nIsoSum; iIsoSum++)
	{
		
	//~ 
	
		 isoSumHist1[iIsoSum][1]->GetXaxis()->SetRangeUser(0,20);
		 isoSumHist1[iIsoSum][1]->GetXaxis()->SetLabelOffset(0.1);		
		 isoSumHist1[iIsoSum][1]->GetXaxis()->SetTitle(XTitleSum[iIsoSum]);
		 isoSumHist1[iIsoSum][1]->SetMarkerStyle(20);
		 isoSumHist1[iIsoSum][1]->SetMarkerSize(2);
		 isoSumHist1[iIsoSum][1]->SetMarkerColor(kBlack);
		 isoSumHist1[iIsoSum][1]->SetLineWidth(2);
		 isoSumHist1[iIsoSum][1]->SetLineColor(kBlack);
		 isoSumHist1[iIsoSum][1]->SetLineStyle(0);
		 
		 isoSumHist1[iIsoSum][0]->SetMarkerColor(kRed);
		 isoSumHist1[iIsoSum][0]->SetMarkerStyle(25);
		 isoSumHist1[iIsoSum][0]->SetMarkerSize(2);
		 isoSumHist1[iIsoSum][0]->SetLineWidth(2);
		 isoSumHist1[iIsoSum][0]->SetLineStyle(2);
		 isoSumHist1[iIsoSum][0]->SetLineColor(kRed);
		
		 int binmax = isoSumHist1[iIsoSum][1]->GetMaximumBin();
	     double max = 1.05*(isoSumHist1[iIsoSum][1]->GetBinContent(binmax)+isoSumHist1[iIsoSum][1]->GetBinError(binmax));
	     int binmax2 = isoSumHist1[iIsoSum][0]->GetMaximumBin();
	     double max2 = 1.05*(isoSumHist1[iIsoSum][0]->GetBinContent(binmax2)+isoSumHist1[iIsoSum][0]->GetBinError(binmax2));
	     max = max2 > max ? max2 : max;
	 
	     isoSumHist1[iIsoSum][1]->SetMaximum(max);
		 
		 histPad->cd();	
		 histPad->SetLogy();
		
		 isoSumHist1[iIsoSum][1]->Draw();
		 isoSumHist1[iIsoSum][0]->Draw("same");	
		 
		TLegend* leg = new TLegend(0.8,0.80,0.9,0.9,NULL,"brNDC");
		leg->SetFillColor(0);
		leg->SetTextSize(0.035);
		leg->SetBorderSize(0);
		
		leg->AddEntry(isoSumHist1[iIsoSum][1], legendRerun[1], "lp");
		leg->AddEntry(isoSumHist1[iIsoSum][0], legendRerun[0], "lp");
		
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
	TString text = "RelValQCD";
	ll->AddText(0.01,0.7,text);
	text = "#sqrt{s} = 13 TeV";
	text = text + lumist;
	//  ll->SetTextAlign(32); // align right
	ll->AddText(0.7, 0.5, text);
	ll->Draw("same");
	  
	
	c1->Update();
	
	TH1D* h_diff=(TH1D*)isoSumHist1[iIsoSum][1]->Clone();
	h_diff->Divide(isoSumHist1[iIsoSum][0]);
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
	h_diff->SetMinimum(0.5);
	h_diff->SetMaximum(2.05);
	
	h_diff->Draw("hist");
	c1->Print(outdir+"pdf/"+isoSumHist1[iIsoSum][1]->GetName()+".pdf");
	c1->Print(outdir+"png/"+isoSumHist1[iIsoSum][1]->GetName()+".png");
}
  
  TString XTitleProfileSum[2] = {"#DeltaR from tau axis","#DeltaR from tau axis"};
  TString YTitleProfileSum[2] = {"Average neutral isolation","Average charged isolation"};
  
  for(uint iFile=0; iFile < nFiles; iFile++){
	for(uint iIsoSum=0; iIsoSum< nIsoSumProfile; iIsoSum++)
	{
		 isoSumProfile2[iIsoSum][iFile]->Rebin(10);
		 isoSumProfile1[iIsoSum][iFile]->Rebin(10);
	//~ 
	
		 //isoSumProfile2[iIsoSum][iFile]->GetXaxis()->SetRangeUser(0,20);
		 isoSumProfile2[iIsoSum][iFile]->GetXaxis()->SetLabelOffset(0.1);		
		 isoSumProfile2[iIsoSum][iFile]->GetXaxis()->SetTitle(XTitleProfileSum[iIsoSum]);
		 isoSumProfile2[iIsoSum][iFile]->SetMarkerStyle(20);
		 isoSumProfile2[iIsoSum][iFile]->SetMarkerSize(2);
		 isoSumProfile2[iIsoSum][iFile]->SetLineWidth(2);
		 
		 isoSumProfile1[iIsoSum][iFile]->SetMarkerColor(kRed);
		 isoSumProfile1[iIsoSum][iFile]->SetMarkerStyle(25);
		 isoSumProfile1[iIsoSum][iFile]->SetMarkerSize(2);
		 isoSumProfile1[iIsoSum][iFile]->SetLineWidth(2);
		 isoSumProfile1[iIsoSum][iFile]->SetLineStyle(2);
		 isoSumProfile1[iIsoSum][iFile]->SetLineColor(kRed);
		
		 int binmax = isoSumProfile2[iIsoSum][iFile]->GetMaximumBin();
	     double max = 1.05*(isoSumProfile2[iIsoSum][iFile]->GetBinContent(binmax)+isoSumProfile2[iIsoSum][iFile]->GetBinError(binmax));
	     int binmax2 = isoSumProfile1[iIsoSum][iFile]->GetMaximumBin();
	     double max2 = 1.05*(isoSumProfile1[iIsoSum][iFile]->GetBinContent(binmax2)+isoSumProfile1[iIsoSum][iFile]->GetBinError(binmax2));
	     max = max2 > max ? max2 : max;
	 
	     isoSumProfile2[iIsoSum][iFile]->SetMaximum(max);
		 
		 histPad->cd();	
		 histPad->SetLogy();
		
		 isoSumProfile2[iIsoSum][iFile]->Draw();
		 isoSumProfile1[iIsoSum][iFile]->Draw("same");	
		 
		TLegend* leg = new TLegend(0.8,0.80,0.9,0.9,NULL,"brNDC");
		leg->SetFillColor(0);
		leg->SetTextSize(0.035);
		leg->SetBorderSize(0);
		
		leg->AddEntry(isoSumProfile2[iIsoSum][iFile], "AK4", "lp");
		leg->AddEntry(isoSumProfile1[iIsoSum][iFile], legendRerun[iFile], "lp");
		
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
	TString text = "RelValQCD";
	ll->AddText(0.01,0.7,text);
	text = "#sqrt{s} = 13 TeV";
	text = text + lumist;
	//  ll->SetTextAlign(32); // align right
	ll->AddText(0.7, 0.5, text);
	ll->Draw("same");
	  
	
	c1->Update();
	
	TProfile* h_diff=(TProfile*)isoSumProfile2[iIsoSum][iFile]->Clone();
	h_diff->Divide(isoSumProfile1[iIsoSum][iFile]);
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
	h_diff->SetMinimum(0.5);
	h_diff->SetMaximum(2.05);
	
	h_diff->Draw("hist");
	c1->Print(outdir+"pdf/"+isoSumProfile2[iIsoSum][iFile]->GetName()+Sumsuffix[iFile]+".pdf");
	c1->Print(outdir+"png/"+isoSumProfile2[iIsoSum][iFile]->GetName()+Sumsuffix[iFile]+".png");
	}
}

	for(uint iIsoSum=0; iIsoSum< nIsoSumProfile; iIsoSum++)
	{
		
	
		 //isoSumProfile1[iIsoSum][1]->GetXaxis()->SetRangeUser(0,20);
		 isoSumProfile1[iIsoSum][1]->GetXaxis()->SetLabelOffset(0.1);		
		 isoSumProfile1[iIsoSum][1]->GetXaxis()->SetTitle(XTitleProfileSum[iIsoSum]);
		 isoSumProfile1[iIsoSum][1]->SetMarkerStyle(20);
		 isoSumProfile1[iIsoSum][1]->SetMarkerSize(2);
		 isoSumProfile1[iIsoSum][1]->SetMarkerColor(kBlack);
		 isoSumProfile1[iIsoSum][1]->SetLineColor(kBlack);
		 isoSumProfile1[iIsoSum][1]->SetLineWidth(2);
		 isoSumProfile1[iIsoSum][1]->SetLineStyle(0);
		 
		 
		 isoSumProfile1[iIsoSum][0]->SetMarkerColor(kRed);
		 isoSumProfile1[iIsoSum][0]->SetMarkerStyle(25);
		 isoSumProfile1[iIsoSum][0]->SetMarkerSize(2);
		 isoSumProfile1[iIsoSum][0]->SetLineWidth(2);
		 isoSumProfile1[iIsoSum][0]->SetLineStyle(2);
		 isoSumProfile1[iIsoSum][0]->SetLineColor(kRed);
		
		 int binmax = isoSumProfile1[iIsoSum][1]->GetMaximumBin();
	     double max = 1.05*(isoSumProfile1[iIsoSum][1]->GetBinContent(binmax)+isoSumProfile1[iIsoSum][1]->GetBinError(binmax));
	     int binmax2 = isoSumProfile1[iIsoSum][0]->GetMaximumBin();
	     double max2 = 1.05*(isoSumProfile1[iIsoSum][0]->GetBinContent(binmax2)+isoSumProfile1[iIsoSum][0]->GetBinError(binmax2));
	     max = max2 > max ? max2 : max;
	 
	     isoSumProfile1[iIsoSum][1]->SetMaximum(max);
		 
		 histPad->cd();	
		 histPad->SetLogy();
		
		 isoSumProfile1[iIsoSum][1]->Draw();
		 isoSumProfile1[iIsoSum][0]->Draw("same");	
		 
		TLegend* leg = new TLegend(0.8,0.80,0.9,0.9,NULL,"brNDC");
		leg->SetFillColor(0);
		leg->SetTextSize(0.035);
		leg->SetBorderSize(0);
		
		leg->AddEntry(isoSumProfile1[iIsoSum][1], legendRerun[1], "lp");
		leg->AddEntry(isoSumProfile1[iIsoSum][0], legendRerun[0], "lp");
		
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
	TString text = "RelValQCD";
	ll->AddText(0.01,0.7,text);
	text = "#sqrt{s} = 13 TeV";
	text = text + lumist;
	//  ll->SetTextAlign(32); // align right
	ll->AddText(0.7, 0.5, text);
	ll->Draw("same");
	  
	
	c1->Update();
	
	TProfile* h_diff=(TProfile*)isoSumProfile1[iIsoSum][1]->Clone();
	h_diff->Divide(isoSumProfile1[iIsoSum][0]);
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
	h_diff->SetMinimum(0.5);
	h_diff->SetMaximum(2.05);
	
	h_diff->Draw("hist");
	c1->Print(outdir+"pdf/"+isoSumProfile1[iIsoSum][1]->GetName()+".pdf");
	c1->Print(outdir+"png/"+isoSumProfile1[iIsoSum][1]->GetName()+".png");
	}
  
  TCanvas *c2 = new TCanvas("c2","",5,30,1500,1024);
  c2->SetGrid(0,0);
  c2->SetFillStyle(4000);
  c2->SetFillColor(10);
  c2->SetTicky();
  c2->SetObjectStat(0);
  c2->SetRightMargin(0.2);
 
 for(uint iFile =0; iFile < nFiles; iFile++){
	for(uint iIsoSum=0; iIsoSum< nIsoSumProfile; iIsoSum++)
	{
		isoSumProfile2D1[iIsoSum][iFile]->GetXaxis()->SetRangeUser(-1,1);
		isoSumProfile2D1[iIsoSum][iFile]->GetYaxis()->SetRangeUser(-0.8,0.8);
		isoSumProfile2D1[iIsoSum][iFile]->Draw("colz");
		
		TLegend* leg = new TLegend(0.6,0.80,0.7,0.9,NULL,"brNDC");
		leg->SetFillColor(0);
		leg->SetTextSize(0.035);
		leg->SetBorderSize(0);
		
		leg->AddEntry(isoSumProfile2D1[iIsoSum][iFile], legendRerun[iFile], "");
		
		leg->Draw();
		
		   TString lumist="";
	TPaveText *ll = new TPaveText(0.15, 0.95, 0.95, 0.99, "NDC");
	ll->SetTextSize(0.03);
	ll->SetTextFont(62);
	ll->SetFillColor(0);
	ll->SetBorderSize(0);
	ll->SetMargin(0.01);
	ll->SetTextAlign(12); // align left
	TString text = "RelValQCD";
	ll->AddText(0.01,0.7,text);
	text = "#sqrt{s} = 13 TeV";
	text = text + lumist;
	//  ll->SetTextAlign(32); // align right
	ll->AddText(0.7, 0.5, text);
	ll->Draw("same");
	  
		
		c2->Print(outdir+"pdf/"+isoSumProfile2D1[iIsoSum][1]->GetName()+Sumsuffix[iFile]+".pdf");
	    c2->Print(outdir+"png/"+isoSumProfile2D1[iIsoSum][1]->GetName()+Sumsuffix[iFile]+".png");
	}
 }
 TString Ztitle[2]={"Neutral isolation ratio","Charged isolation ratio"};
 
 for(uint iIsoSum=0; iIsoSum< nIsoSumProfile; iIsoSum++)
	{
		isoSumProfile2D2[iIsoSum][1]->GetXaxis()->SetRangeUser(-1,1);
		isoSumProfile2D2[iIsoSum][1]->GetYaxis()->SetRangeUser(-0.8,0.8);
		isoSumProfile2D2[iIsoSum][1]->Draw("colz");
		
		TLegend* leg = new TLegend(0.6,0.80,0.7,0.9,NULL,"brNDC");
		leg->SetFillColor(0);
		leg->SetTextSize(0.035);
		leg->SetBorderSize(0);
		
		leg->AddEntry(isoSumProfile2D1[iIsoSum][1], "AK4", "");
		
		leg->Draw();
		
		   TString lumist="";
	TPaveText *ll = new TPaveText(0.15, 0.95, 0.95, 0.99, "NDC");
	ll->SetTextSize(0.03);
	ll->SetTextFont(62);
	ll->SetFillColor(0);
	ll->SetBorderSize(0);
	ll->SetMargin(0.01);
	ll->SetTextAlign(12); // align left
	TString text = "RelValQCD";
	ll->AddText(0.01,0.7,text);
	text = "#sqrt{s} = 13 TeV";
	text = text + lumist;
	//  ll->SetTextAlign(32); // align right
	ll->AddText(0.7, 0.5, text);
	ll->Draw("same");
	  
		
		c2->Print(outdir+"pdf/"+isoSumProfile2D2[iIsoSum][1]->GetName()+".pdf");
	    c2->Print(outdir+"png/"+isoSumProfile2D2[iIsoSum][1]->GetName()+".png");
	    
	    TProfile2D* h_diff2=(TProfile2D*)isoSumProfile2D1[iIsoSum][1]->Clone();
		h_diff2->Divide(isoSumProfile2D1[iIsoSum][0]);
		h_diff2->RebinY(2);
		h_diff2->GetZaxis()->SetRangeUser(0.7,1.3);
	
		h_diff2->GetYaxis()->SetRangeUser(-0.8,0.8);
		h_diff2->GetXaxis()->SetRangeUser(-1,1);
		h_diff2->Draw("colz");
		TLegend* leg2 = new TLegend(0.5,0.80,0.7,0.9,NULL,"brNDC");
		leg2->SetFillColor(0);
		leg2->SetTextSize(0.035);
		leg2->SetBorderSize(0);
		
		leg2->AddEntry(isoSumProfile2D1[iIsoSum][1], "AK5/AK4+isoCone0.5", "");
		
		leg2->Draw();
		ll->Draw("same");
		
		c2->Print(outdir+"pdf/"+isoSumProfile2D2[iIsoSum][1]->GetName()+"_ratio.pdf");
	    c2->Print(outdir+"png/"+isoSumProfile2D2[iIsoSum][1]->GetName()+"_ratio.png");
	    
		
	}
	
	
	return 0;
}
