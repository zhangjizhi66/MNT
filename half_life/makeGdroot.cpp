#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include <fstream>
#include <stdio.h>
#include <time.h>

// Branches for input ROOT file
int nG,Gdt[10000];
float Ge[10000],Gfom[10000];

// buffer branches 
Short_t ed_buf[10000],td_buf[10000];

// Branches for new ROOT file
Short_t nd;     // number of gamma coincident
Short_t e;   // energy
Short_t t;  // time with respect to Chico event

int main(int argc, char** argv)
{
    if (argc != 3){
        printf("USAGE: ./makeGdroot runid_1 runid_2");
        exit(EXIT_FAILURE);
    }
    
    std::ifstream ifs("../validID.txt");
    int runid;
    TChain *tree = new TChain("tree");
    while (ifs>>runid)
        if (runid>=atoi(argv[1]) && runid<=atoi(argv[2]))
            tree->Add(Form("../COIN_ROOT/coin%04d.root",runid));
    
    tree->SetBranchAddress("nG",&nG);
    tree->SetBranchAddress("Ge",&Ge);
    tree->SetBranchAddress("Gdt",&Gdt);
    tree->SetBranchAddress("Gfom",&Gfom);
    
    const char *filename = Form("ROOT/Gd_%d_%d.root",atoi(argv[1]),atoi(argv[2]));
    TFile *opf = new TFile(filename,"RECREATE");
    TTree *opt = new TTree("tree","tree");
    
    opt->Branch("nd",&nd,"nd/S");
    opt->Branch("e",&e,"e/S");
    opt->Branch("t",&t,"t/S");
    
    clock_t start=clock(),stop=clock();
    
    long long int nentries = tree->GetEntries();
    for (long long int jentry=0; jentry<nentries; jentry++){
        tree->GetEntry(jentry);
        
        nd=0;
        for (int dhit=0; dhit<nG; dhit++){
            if (Gfom[dhit]>0.8) continue;
            if (Ge[dhit]>2000) continue;
            if (Gdt[dhit]<=30) continue;  // delayed
                
            ed_buf[nd] = Ge[dhit];
            td_buf[nd] = Gdt[dhit];
            nd++;
        }
        if (nd>4) continue;  // ignore 5 or more multiplicity coincidence
        
        for (int d=0; d<nd; d++){
            e = ed_buf[d];
            t = td_buf[d];
            opt->Fill();
        }
        
        // display progress and time needed
        if (jentry%50000 == 49999){
            stop = clock();
            printf("Process %.3f %%  Time remaining %02d min %02d s        \r",double(jentry)/double(nentries)*100.,
                int((stop-start)*(nentries-jentry)/jentry/1e6/60),
                int((stop-start)*(nentries-jentry)/jentry/1e6)%60);
            fflush(stdout);
        }
    }
    stop = clock();
    printf("Process %.3f %%  Total Time %02d min %02d s        \n",100.,int((stop-start)/1e6/60),int((stop-start)/1e6)%60);
    
    opf->cd();
    opt->Write();
    opf->Close();
}
