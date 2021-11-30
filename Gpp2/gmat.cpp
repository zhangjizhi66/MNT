#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <time.h>

// Branches for input ROOT file
Short_t np;        // number of delayed gamma coincident
Short_t ex,ey;     // energy, doppler corrected
Short_t cosx,cosy; // cos value times 10000
Short_t p;         // momentum of fragments

void gmat(int mass)
{
    TFile *fin=new TFile("ROOT/Gpp2.root");
    TTree *tree=(TTree*)fin->Get("tree");
    
    tree->SetBranchAddress("np",&np);
    tree->SetBranchAddress("ex",&ex);
    tree->SetBranchAddress("ey",&ey);
    if (mass>167){
        tree->SetBranchAddress("cosTx",&cosx);
        tree->SetBranchAddress("cosTy",&cosy);
        tree->SetBranchAddress("pT",&p);
    }
    else {
        tree->SetBranchAddress("cosPx",&cosx);
        tree->SetBranchAddress("cosPy",&cosy);
        tree->SetBranchAddress("pP",&p);
    }
    
    TH2I *gg = new TH2I("gg",Form("mass=%d",mass),1000,0,2000,1000,0,2000);
    
    clock_t start=clock(),stop=clock();
    
    int nentries = tree->GetEntries();
    for (int jentry=0; jentry<nentries; jentry++){
        tree->GetEntry(jentry);
        // if (np<=6) continue;
        
        int exx = ex * (1.-1.0658e-7*p/mass*cosx)/sqrt(1.-powf(1.0658e-3*p/mass,2.));
        int eyy = ey * (1.-1.0658e-7*p/mass*cosy)/sqrt(1.-powf(1.0658e-3*p/mass,2.));
        
        gg->Fill(exx,eyy);
        gg->Fill(eyy,exx);
        
        // display progress and time needed
        if (jentry%1000000 == 999999){
            stop = clock();
            printf("Process %.3f %%  Time remaining %02d min %02d s        \r",double(jentry)/double(nentries)*100.,
                int((stop-start)*(nentries-jentry)/jentry/1e6/60),
                int((stop-start)*(nentries-jentry)/jentry/1e6)%60);
            fflush(stdout);
        }
    }
    stop = clock();
    printf("Process %.3f %%  Total Time %02d min %02d s        \n",100.,int((stop-start)/1e6/60),int((stop-start)/1e6)%60);

    TFile *fout = new TFile(Form("ROOT/Gppmat2_%d.root",mass),"RECREATE");
    fout->cd();
    gg->Write();
    fout->Close();
}
