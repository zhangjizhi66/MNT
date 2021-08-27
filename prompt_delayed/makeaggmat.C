//make delayed gamma-gamma asymptotic matrix using RADWARE approch.
//agg.C
#include <iostream>
#include <sstream>
#include "TString.h"
#include "TSpectrum.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TChain.h"

TString rootfile="ROOT/Gpd.root";//input
TString ggfile="ROOT/Gpdmat.root";//output
void makeaggmat()
{
    cout<<"Generate gg matix to ["<<ggfile<<"] from ["<<rootfile<<"]"<<endl;
    TFile *fin=TFile::Open(rootfile);
    TTree *tree=(TTree*)fin->Get("tree");
  
    tree->Draw("ed:epP>>gg(2000,0,2000,2000,0,2000)","np==1 && nd==1","colz");
    auto hgg=(TH2D*)gROOT->FindObject("gg");
    auto hx=(TH1D*)hgg->ProjectionX("xTpj");

    TSpectrum *sx= new TSpectrum(500);
    Int_t nfoundx=sx->Search(hx,2,"",0.1);
    auto hbx=sx->Background(hx,10,"same");
    hbx->SetName("xTpjBg");
    hbx->SetTitle("xTpjBg");
    auto hpeakx=(TH2D*)hbx->Clone("xTpjPeak");
    hpeakx->Add(hx,hbx,1,-1);
    hpeakx->Draw("same");
  
    auto hy=(TH1D*)hgg->ProjectionY("yTpj");
    TSpectrum *sy= new TSpectrum(500);
    Int_t nfoundy=sy->Search(hy,2,"",0.1);
    auto hby=sy->Background(hy,10,"same");
    hby->SetName("yTpjBg");
    hby->SetTitle("yTpjBg");
    auto hpeaky=(TH2D*)hby->Clone("yTpjPeak");
    hpeaky->Add(hy,hby,1,-1);
    hpeaky->Draw("same");
  
    TH2D *hggb=new TH2D("ggbmat","bgmat for gg",2000,0,2000,2000,0,2000);
    hggb->Reset();
    Double_t T,Pi,Pj,pi,pj,bi,bj,Bij,x,y;
    T=hx->Integral();
    for(int i=0;i<hgg->GetNbinsX();i++) {
      for(int j=0;j<hgg->GetNbinsY();j++) {
        // Pi=h->GetBinContent(i+1);
        // Pj=h->GetBinContent(j+1);
        pi=hpeakx->GetBinContent(i+1);
        pj=hpeaky->GetBinContent(j+1);
        bi=hbx->GetBinContent(i+1);
        bj=hby->GetBinContent(j+1);
        Bij=(bi*pj+bi*bj)/T;//asymetric matix
        x=hx->GetBinCenter(i+1);
        y=hy->GetBinCenter(j+1);
        hggb->Fill(x,y,Bij);
      }
    }
    TH2D *hggmat=new TH2D("ggmat","gg with backsub",2000,0,2000,2000,0,2000);  
    hggmat->Add(hgg,hggb,1,-1);
 
    TFile *fout=new TFile(ggfile,"RECREATE");
    hgg->Write();
    hx->Write();
    hbx->Write();
    hpeakx->Write();
    hy->Write();
    hby->Write();
    hpeaky->Write();
    hggb->Write();
    hggmat->Write();
    fout->Write();
    fout->Close(); 
}

