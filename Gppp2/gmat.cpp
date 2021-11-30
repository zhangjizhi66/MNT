#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <time.h>

// Branches for input ROOT file
Short_t np;              // number of gamma coincident
Short_t ex,ey,ez;        // energy, doppler corrected
Short_t cosx,cosy,cosz;  // cos value times 10000
Short_t p;               // momentum of fragment

void gmat(int mass)
{
    printf("initializing histograms ...\n");
    
    TFile *ipf=new TFile("ROOT/Gppp2.root");
    TTree *tree=(TTree*)ipf->Get("tree");

    tree->SetBranchAddress("np",&np);
    tree->SetBranchAddress("ex",&ex);
    tree->SetBranchAddress("ey",&ey);
    tree->SetBranchAddress("ez",&ez);
    if (mass>167){
        tree->SetBranchAddress("cosTx",&cosx);
        tree->SetBranchAddress("cosTy",&cosy);
        tree->SetBranchAddress("cosTz",&cosz);
        tree->SetBranchAddress("pT",&p);
    }
    else {
        tree->SetBranchAddress("cosPx",&cosx);
        tree->SetBranchAddress("cosPy",&cosy);
        tree->SetBranchAddress("cosPz",&cosz);
        tree->SetBranchAddress("pP",&p);
    }
    
    TH1I *g = new TH1I("g","Total projection of prompt gamma cube",1000,0,2000);
    
    TH2I *gg[1000];
    for (int i=0; i<1000; i++)
        gg[i] = new TH2I(Form("gg_%d",i*2+1),Form("gated on %d keV",i*2+1),1000,0,2000,1000,0,2000);
    
    printf("Filling histograms ...\n");
    clock_t start=clock(),stop=clock();
    
    int nentries = tree->GetEntries();
    for (int jentry=0; jentry<nentries; jentry++){
        tree->GetEntry(jentry);
        // if (np<=6) continue;
        
        ex = ex * (1.-1.0658e-7*p/mass*cosx)/sqrt(1.-powf(1.0658e-3*p/mass,2.));
        ey = ey * (1.-1.0658e-7*p/mass*cosy)/sqrt(1.-powf(1.0658e-3*p/mass,2.));
        ez = ez * (1.-1.0658e-7*p/mass*cosz)/sqrt(1.-powf(1.0658e-3*p/mass,2.));
        if (ex>=2000||ey>=2000||ez>=2000) continue;

        g->Fill(ex);
        
        gg[ex/2]->Fill(ey,ez);
        gg[ex/2]->Fill(ez,ey);

        gg[ey/2]->Fill(ex,ez);
        gg[ey/2]->Fill(ez,ex);

        gg[ez/2]->Fill(ex,ey);
        gg[ez/2]->Fill(ey,ex);
        
        // display progress and time needed
        if (jentry%500000 == 499999){
            stop = clock();
            printf("Process %.3f %%  Time remaining %02d min %02d s        \r",double(jentry)/double(nentries)*100.,
                int((stop-start)*(nentries-jentry)/jentry/1e6/60),
                int((stop-start)*(nentries-jentry)/jentry/1e6)%60);
            fflush(stdout);
        }
    }
    stop = clock();
    printf("Process %.3f %%  Total Time %02d min %02d s        \n",100.,int((stop-start)/1e6/60),int((stop-start)/1e6)%60);
    
    printf("Writing histograms to new root file ...\n");
    TFile *fout = new TFile(Form("ROOT/Gpppmats2_%d.root",mass),"RECREATE");
    fout->cd();
    g->Write();
    for (int i=0; i<1000; i++)
        gg[i]->Write();
    fout->Close();
    printf("Done!\n\n");
}
