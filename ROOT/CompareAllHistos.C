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

int CompareAllHistos(TString input1 = "../../ak4/output.root", TString input2 = "../../ak5/output.root") {
	
	gROOT->Reset();             
  //SetAtlasStyle();
  setTDRStyle();
  gStyle->SetPalette(1);
  gStyle->SetErrorX(0.5);
  
  TString Names[2] = { input1, input2};
  
  TString outdir = "";
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
	for(uint iWP=0; iWP<nWP; iWP++)
		{
			for(uint iProf123=0; iProf123< nProfile; iProf123++)
			{
				for(uint i123=0; i123 < n123; i123++)
				{
					std::cout << dirNames[0]+"/"+profile123_prefix+profileX[iProf123]+profile123_WP[iWP]+profile123_suffix[i123] << std::endl;
					profileHist[i123][iWP*nProfile+iProf123][iFile]=(TProfile*)f[iFile]->Get(TString(dirNames[0]+"/"+profile123_prefix+profileX[iProf123]+profile123_WP[iWP]+profile123_suffix[i123]));
				}
			}
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
  
	TString XTitle[4] = {"True P_{T} [GeV]","True #eta", "nVx","True #phi"};
 c1->SetLogy();
	for(uint iProf12=0; iProf12< nProfile12; iProf12++)
	{
		 int title_id=iProf12%4;
		
		 profileHist1[iProf12][0]->Rebin(10);
		 profileHist1[iProf12][1]->Rebin(10);
	
	
		 if(title_id==0) profileHist1[iProf12][0]->GetXaxis()->SetRangeUser(10,100);
		 profileHist1[iProf12][0]->GetXaxis()->SetLabelOffset(0.1);		
		 profileHist1[iProf12][0]->GetXaxis()->SetTitle(XTitle[title_id]);
		 profileHist1[iProf12][0]->GetYaxis()->SetTitle("Fake Rate");
		 profileHist1[iProf12][0]->SetMarkerStyle(20);
		 profileHist1[iProf12][0]->SetMarkerSize(2);
		 profileHist1[iProf12][0]->SetLineWidth(2);
		 
		 profileHist1[iProf12][1]->SetMarkerColor(kRed);
		 profileHist1[iProf12][1]->SetMarkerStyle(25);
		 profileHist1[iProf12][1]->SetMarkerSize(2);
		 profileHist1[iProf12][1]->SetLineWidth(2);
		 profileHist1[iProf12][1]->SetLineStyle(2);
		 profileHist1[iProf12][1]->SetLineColor(kRed);
		
		 int binmax = profileHist1[iProf12][0]->GetMaximumBin();
	     double max = 1.2*(profileHist1[iProf12][0]->GetBinContent(binmax)+profileHist1[iProf12][0]->GetBinError(binmax));
	     int binmax2 = profileHist1[iProf12][1]->GetMaximumBin();
	     double max2 = 1.2*(profileHist1[iProf12][1]->GetBinContent(binmax2)+profileHist1[iProf12][1]->GetBinError(binmax2));
	     max = max2 > max ? max2 : max;
	 
	     profileHist1[iProf12][0]->SetMaximum(max);
		 
		 histPad->cd();	
		 histPad->SetLogy();
		 profileHist1[iProf12][0]->Draw();
		 profileHist1[iProf12][1]->Draw("same");	
		 
		TLegend* leg = new TLegend(0.8,0.80,0.9,0.9,NULL,"brNDC");
		leg->SetFillColor(0);
		leg->SetTextSize(0.035);
		leg->SetBorderSize(0);
		
		leg->AddEntry(profileHist1[iProf12][0], "AK4", "lp");
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
	
	TH1D* h_diff=(TH1D*)profileHist1[iProf12][0]->Clone();
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
	h_diff->SetMaximum(1.05);
	
	h_diff->Draw("hist");
	c1->Print(outdir+"pdf/"+profileHist1[iProf12][0]->GetName()+".pdf");
	c1->Print(outdir+"png/"+profileHist1[iProf12][0]->GetName()+".png");
	
	
	}
  
  for(uint iWP=0; iWP<nWP; iWP++)
		{
			for(uint iProf123=0; iProf123< nProfile; iProf123++)
			{
				for(uint i123=0; i123 < n123; i123++)
				{
					int title_id=iProf123%4;
		
					profileHist[i123][iWP*nProfile+iProf123][0]->Rebin(10);
				    profileHist[i123][iWP*nProfile+iProf123][1]->Rebin(10);
		//~ 
					if(title_id==0) profileHist[i123][iWP*nProfile+iProf123][0]->GetXaxis()->SetRangeUser(10,100);
					
					 profileHist[i123][iWP*nProfile+iProf123][0]->GetXaxis()->SetLabelOffset(0.1);		
					 profileHist[i123][iWP*nProfile+iProf123][0]->GetXaxis()->SetTitle(XTitle[title_id]);
					 profileHist[i123][iWP*nProfile+iProf123][0]->GetYaxis()->SetTitle("Fake Rate");
					 profileHist[i123][iWP*nProfile+iProf123][0]->SetMarkerStyle(20);
					 profileHist[i123][iWP*nProfile+iProf123][0]->SetMarkerSize(2);
					 profileHist[i123][iWP*nProfile+iProf123][0]->SetLineWidth(2);
					 
					 profileHist[i123][iWP*nProfile+iProf123][1]->SetMarkerColor(kRed);
					 profileHist[i123][iWP*nProfile+iProf123][1]->SetMarkerStyle(25);
					 profileHist[i123][iWP*nProfile+iProf123][1]->SetMarkerSize(2);
					 profileHist[i123][iWP*nProfile+iProf123][1]->SetLineWidth(2);
					 profileHist[i123][iWP*nProfile+iProf123][1]->SetLineStyle(2);
					 profileHist[i123][iWP*nProfile+iProf123][1]->SetLineColor(kRed);
					 
					  int binmax = profileHist[i123][iWP*nProfile+iProf123][0]->GetMaximumBin();
				     double max = 1.2*(profileHist[i123][iWP*nProfile+iProf123][0]->GetBinContent(binmax)+profileHist[i123][iWP*nProfile+iProf123][0]->GetBinError(binmax));
				     int binmax2 = profileHist[i123][iWP*nProfile+iProf123][1]->GetMaximumBin();
				     double max2 = 1.2*(profileHist[i123][iWP*nProfile+iProf123][1]->GetBinContent(binmax2)+profileHist[i123][iWP*nProfile+iProf123][1]->GetBinError(binmax2));
				     max = max2 > max ? max2 : max;
				 
				     profileHist[i123][iWP*nProfile+iProf123][0]->SetMaximum(max);
					 
					 histPad->cd();	
					 histPad->SetLogy();
					 profileHist[i123][iWP*nProfile+iProf123][0]->Draw();
					 profileHist[i123][iWP*nProfile+iProf123][1]->Draw("same");	
					 
					 TLegend* leg = new TLegend(0.8,0.80,0.9,0.9,NULL,"brNDC");
						leg->SetFillColor(0);
						leg->SetTextSize(0.035);
						leg->SetBorderSize(0);
						
						leg->AddEntry(profileHist[i123][iWP*nProfile+iProf123][0], "AK4", "lp");
						leg->AddEntry(profileHist[i123][iWP*nProfile+iProf123][1], "AK5", "lp");
						
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
					
					TH1D* h_diff=(TH1D*)profileHist[i123][iWP*nProfile+iProf123][0]->Clone();
					h_diff->Divide(profileHist[i123][iWP*nProfile+iProf123][1]);
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
					h_diff->SetMaximum(1.05);
					
					h_diff->Draw("hist");
					c1->Print(outdir+"pdf/"+profileHist[i123][iWP*nProfile+iProf123][0]->GetName()+".pdf");
					c1->Print(outdir+"png/"+profileHist[i123][iWP*nProfile+iProf123][0]->GetName()+".png");
		 
					
				}
			}
		}
  
  
  //~ 
  //~ TPostScript *ps[8];
  //~ for(int iDir=0; iDir < 8; iDir++)
  //~ { 
   //~ ps[iDir] = new TPostScript(dirNames[iDir]+".ps",112);
   //~ f[0]->cd(dirNames[iDir]);
   //~ gDirectory->GetListOfKeys()->Print();
   //~ TIter next(gDirectory->GetListOfKeys());
   //~ TKey* key;
  	//~ 
  //~ while ( (key = (TKey*)next())){
	//~ std::cout << key->GetName() << std::endl;
	//~ 
	//~ TH1D* hist = (TH1D*)f[0]->Get(dirNames[iDir]+"/"+key->GetName());
	//~ TH1D* hist2 = (TH1D*)f[1]->Get(dirNames[iDir]+"/"+key->GetName());
	//~ 
	//~ std::cout << hist << " " << hist2 << std::endl;
	//~ if(!hist2) continue;
	//~ int binmax = hist->GetMaximumBin();
	//~ double max = 1.2*(hist->GetBinContent(binmax)+hist->GetBinError(binmax));
	//~ int binmax2 = hist2->GetMaximumBin();
	//~ double max2 = 1.2*(hist2->GetBinContent(binmax2)+hist2->GetBinError(binmax2));
	//~ max = max2 > max ? max2 : max;
	//~ 
	//~ hist->SetMaximum(max);
	//~ hist->SetMarkerStyle(20);
	//~ hist->SetMarkerSize(2);
	//~ hist->SetLineWidth(2);
	//~ 
	//~ ps[iDir]->NewPage();
	//~ 
	//~ 
	//~ 
	//~ hist->Draw("e1");
	//~ hist->GetXaxis()->SetTitle("m_{H}[GeV]");
	//~ hist->GetYaxis()->SetTitle("Events/20 GeV");
	//~ 
	
	//~ 
	//~ hist2->SetLineColor(kRed);
	//~ hist2->SetLineStyle(2);
	//~ hist2->SetLineWidth(2);
	//~ hist2->SetMarkerColor(kRed);
	//~ hist2->SetMarkerStyle(25);
	//~ hist2->SetMarkerSize(2);
	//~ 
	//~ 
	//~ hist2->Draw("e1same");
	//~ 
	//~ TString lumist="4.9 fb^{-1}";
	//~ TPaveText *ll = new TPaveText(0.15, 0.95, 0.95, 0.99, "NDC");
	//~ ll->SetTextSize(0.03);
	//~ ll->SetTextFont(62);
	//~ ll->SetFillColor(0);
	//~ ll->SetBorderSize(0);
	//~ ll->SetMargin(0.01);
	//~ ll->SetTextAlign(12); // align left
	//~ TString text = key->GetName();
	//~ ll->AddText(0.01,0.7,text);
	//~ text = "#sqrt{s} = 7 TeV  L = ";
	//~ text = text + lumist;
	//~ //  ll->SetTextAlign(32); // align right
	//~ ll->AddText(0.7, 0.5, text);
	//~ ll->Draw("same");
	  //~ 
	//~ 
	//~ c1->Update();
  //~ }
  //~ 
  //~ ps[iDir]->Close();
	//~ }
	//~ 
	return 0;
}
