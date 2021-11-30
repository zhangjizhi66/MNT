void gmatrix()
{
    TFile *fin=new TFile("ROOT/Gdd.root");
    TTree *tree=(TTree*)fin->Get("tree");

    TFile *fout=new TFile("ROOT/Gddmat.root","RECREATE");

    //matrix setting
    Double_t gmin=0;
    Double_t gmax=2000;
    int ngbin=2000;
    
    TCut cut = "nd==2";
  
    TH2S *hg2xy;
    TH1S *hg2x;
    
    TH1S *hg2xp=new TH1S("hg2xp","net peaks of total projection spectrum of hg2xy",ngbin,gmin,gmax);
    TH1S *hg2xb;
    TH2S *hg2xyb=new TH2S("hg2xyb","background gg-matrix",ngbin,gmin,gmax,ngbin,gmin,gmax); 
    TH2S *hg2xyp=new TH2S("hg2xyp","background subtracted gg-matrix",ngbin,gmin,gmax,ngbin,gmin,gmax); 

    TSpectrum *sa=new TSpectrum(200);
  
    tree->Draw("ex>>hg2x(2000,0,2000)",cut);
    hg2x = (TH1S*)gROOT->FindObject("hg2x");
    
    tree->Draw("ey:ex>>hg2xy(2000,0,2000,2000,0,2000)",cut);
    hg2xy = (TH2S*)gROOT->FindObject("hg2xy");

    //"nosmoothing" option to avoid overestimation of the background 
    hg2xb=(TH1S*)sa->Background(hg2x,8,"nosmoothing");
    hg2xb->SetName("hg2xb");
    hg2xb->SetTitle("background of total projection spectrum of hg2xy");
    hg2xp->Add(hg2x,hg2xb,1,-1);

    //make background matrix - RADWARE approch
    Double_t T,Pi,Pj,pi,pj,Bij;
    T=hg2x->Integral();
    int N=hg2x->GetNbinsX();
    for(int i=1; i<=N; i++) {
        for(int j=1; j<=N; j++) {
            Pi=hg2x->GetBinContent(i);
            Pj=hg2x->GetBinContent(j);
            pi=hg2xp->GetBinContent(i);
            pj=hg2xp->GetBinContent(j);
            Bij=(Pi*Pj-pi*pj)/T;
            Double_t x=hg2x->GetBinCenter(i);
            Double_t y=hg2x->GetBinCenter(j);       
            hg2xyb->Fill(x,y,Bij);
        }
    }
    hg2xyp->Add(hg2xy,hg2xyb,1,-1);

    fout->cd();
    hg2xy->Write();
    hg2xyb->Write();
    hg2xyp->Write();
    hg2x->Write();
    hg2xb->Write();
    hg2xp->Write();
    fout->Close();
}
