void ggmat()
{
    TFile *fin=new TFile("ROOT/Gpp_80_89.root");
    TTree *tree=(TTree*)fin->Get("tree");

    TFile *fout=new TFile("ROOT/Gppmat.root","RECREATE");
    
    TCut cut = "";
  
    TH2S *ggP,*ggT;
    
    tree->Draw("eTy:eTx>>ggT(2000,0,2000,2000,0,2000)",cut);
    tree->Draw("ePy:ePx>>ggP(2000,0,2000,2000,0,2000)",cut);
    ggT = (TH2S*)gROOT->FindObject("ggT");
    ggP = (TH2S*)gROOT->FindObject("ggP");

    fout->cd();
    ggT->Write();
    ggP->Write();
    fout->Close();
}
