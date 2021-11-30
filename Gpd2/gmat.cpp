#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <time.h>

// Branches for input ROOT file
Short_t np,nd;           // number of gamma coincident
Short_t ep,ed;           // energy, doppler corrected
Short_t cosp;             // cos value times 10000
Short_t p;               // momentum of fragment

void gmat(int mass)
{
    TFile *ipf=new TFile("ROOT/Gpd2.root");
    TTree *tree=(TTree*)ipf->Get("tree");

    tree->SetBranchAddress("np",&np);
    tree->SetBranchAddress("nd",&nd);
    tree->SetBranchAddress("ep",&ep);
    tree->SetBranchAddress("ed",&ed);
    if (mass>167){
        tree->SetBranchAddress("cosT",&cosp);
        tree->SetBranchAddress("pT",&p);
    }
    else {
        tree->SetBranchAddress("cosP",&cosp);
        tree->SetBranchAddress("pP",&p);
    }
    
    TH2I *gg = new TH2I("gg",Form("mass = %d",mass),2000,0,2000,1000,0,2000);
    clock_t start=clock(),stop=clock();
    
    int nentries = tree->GetEntries();
    for (int jentry=0; jentry<nentries; jentry++){
        tree->GetEntry(jentry);
        // if (np<=6) continue;
        
        ep = ep * (1.-1.0658e-7*p/mass*cosp)/sqrt(1.-powf(1.0658e-3*p/mass,2.));
        if (ep>=2000) continue;
        
        gg->Fill(ed,ep);
        
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
    printf("Process %.3f %%  Total Time %02d min %02d s        \n\n",100.,int((stop-start)/1e6/60),int((stop-start)/1e6)%60);

    TFile *fout = new TFile(Form("ROOT/Gpdmat2_%d.root",mass),"RECREATE");
    fout->cd();
    gg->Write();
    fout->Close();
}
