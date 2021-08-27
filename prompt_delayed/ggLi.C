#define gg_cxx
#include "gg.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void gg::Loop()
{
   if (fChain == 0) return;
   Short_t npx,ndy;
   Short_t px,dy,pgt,dgt,qval;
   Short_t thetat;

   TFile *f1=new TFile(Form("pd%d_%d.root",run1,run2),"RECREATE");
   f1->cd();
   TTree *opt1=new TTree("tree","tree");
   opt1->Branch("px",&px,"px/S");
   opt1->Branch("dy",&dy,"dy/S");
   opt1->Branch("npx",&npx,"npx/S");
   opt1->Branch("ndy",&ndy,"ndy/S");
   opt1->Branch("dgt",&dgt,"dgt/S");
   opt1->Branch("pgt",&pgt,"pgt/S");
   opt1->Branch("qval",&qval,"qval/S");
   opt1->Branch("thetat",&thetat,"thetat/S");
   Long64_t nentries = fChain->GetEntries();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      float GGeP[1000],GGeT[1000],GGe[1000],GGt[1000];
      float GGed[1000],GGtd[1000];
      int nGG=0,nGGd=0;
      for(int i=0;i<nGam;i++) {
	if(Gdt[i]>-20 && Gdt[i]<20 && Gfom[i]<0.8 && GeP[i]<2000) {//prompt
	  GGeP[nGG]=GeP[i];
	  GGeT[nGG]=GeT[i];
	  GGt[nGG]=Gdt[i];
	  GGe[nGG]=Ge[i];
	  nGG++;
	}
	if(Gdt[i]>30 && Gfom[i]<0.8 && Ge[i]<2000) {//delayed
	  GGed[nGGd]=Ge[i];
	  GGtd[nGGd]=Gdt[i];
	  nGGd++;
	}
      }  
      qval=Qval1;
      thetat=thetaT*180/3.14159;
      if(nGG==1 && nGGd==1) {//nGG>0 && nGG<3;M2
	for(int i=0;i<nGG;i++){
	  for(int j=0;j<nGGd;j++){
	    ndy=nGGd;
	    npx=nGG;
	    if(GGeT[i]>=401 && GGeT[i]<=411) continue;
	    if(GGeT[i]>=572 && GGeT[i]<=582) continue;
	    // if(GGeT[i]>=353 && GGeT[i]<=358) continue;
	    // if(GGeT[i]>=364 && GGeT[i]<=370) continue;
	    px=GGeP[i];
            pgt=GGt[i];
	    dy=GGed[j];
	    dgt=GGtd[j];
	    opt1->Fill();
	  }
	}
      } 

      if(jentry % 10000 == 0) {
	printf("Process %.2f \%, %dk / %dk\r",
	       Double_t(jentry)/nentries*100.,
	       jentry/1000,nentries/1000);
	fflush(stdout);
      }
      
   }
   cout<<endl;
   f1->cd();
   opt1->Write();
 }
