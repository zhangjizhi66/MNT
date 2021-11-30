void gmat()
{
    TFile *fin=new TFile("ROOT/Gpd.root");
    TTree *tree=(TTree*)fin->Get("tree");

    TFile *fout=new TFile("ROOT/Gpdmat.root","RECREATE");
    
    TCut cut = "";
  
    TH2S *ggP,*ggT;
    
    tree->Draw("epT:ed>>ggT(1000,0,2000,1000,0,2000)",cut,"colz");
    tree->Draw("epP:ed>>ggP(1000,0,2000,1000,0,2000)",cut,"colz");
    ggT = (TH2S*)gROOT->FindObject("ggT");
    ggP = (TH2S*)gROOT->FindObject("ggP");

    fout->cd();
    ggT->Write();
    ggP->Write();
    fout->Close();
}
