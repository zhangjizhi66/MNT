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
Short_t ex_buf[10000],ey_buf[10000];
Short_t tx_buf[10000],ty_buf[10000];

// Branches for new ROOT file
Short_t nd;     // number of delayed gamma coincident
Short_t ex,ey;   // energy
Short_t tx,ty;  // time with respect to Chico event

int main(int argc, char** argv)
{
    if (argc != 3){
        printf("USAGE: ./makeGddroot runid_1 runid_2");
        exit(EXIT_FAILURE);
    }
    
    printf("making ROOT files %d to %d ......\n",atoi(argv[1]),atoi(argv[2]));
    
    std::ifstream ifs("../validID.txt");  // read valid run id
    int runid;
    TChain *tree = new TChain("tree");
    while (ifs>>runid)
        if (runid>=atoi(argv[1]) && runid<=atoi(argv[2]))
            tree->Add(Form("../COIN_ROOT/coin%04d.root",runid));
    
    tree->SetBranchAddress("nG",&nG);
    tree->SetBranchAddress("Ge",&Ge);
    tree->SetBranchAddress("Gdt",&Gdt);
    tree->SetBranchAddress("Gfom",&Gfom);
    
    const char *filename = Form("ROOT/Gdd_%d_%d.root",atoi(argv[1]),atoi(argv[2]));
    TFile *opf = new TFile(filename,"RECREATE");
    TTree *opt = new TTree("tree","tree");
    
    opt->Branch("nd",&nd,"nd/S");
    opt->Branch("ex",&ex,"ex/S");
    opt->Branch("ey",&ey,"ey/S");
    opt->Branch("tx",&tx,"tx/S");
    opt->Branch("ty",&ty,"ty/S");
    
    clock_t start=clock(),stop=clock();
    
    long long int nentries = tree->GetEntries();
    for (long long int jentry=0; jentry<nentries; jentry++){
        tree->GetEntry(jentry);
        int nevent = 0;
        for (int xhit=0; xhit<nG; xhit++)
            for (int yhit=0; yhit<nG; yhit++){
                if (xhit == yhit) continue;
                if (Gfom[xhit]>0.8 || Gfom[yhit]>0.8) continue;
                if (Ge[xhit]>2000 || Ge[yhit]>2000) continue;
                if (Gdt[xhit]<30 || Gdt[yhit]<30) continue;  // delayed
                if (abs(Gdt[xhit]-Gdt[yhit])>20) continue;  // coincident
                
                ex_buf[nevent] = Ge[xhit];
                ey_buf[nevent] = Ge[yhit];
                tx_buf[nevent] = Gdt[xhit];
                ty_buf[nevent] = Gdt[yhit];
                nevent++;
            }
        if (nevent > 12) continue;  // ignore 3 or more multiplicity coincidence, must be fake
        
        for (int n=0; n<nevent; n++){
            nd = nevent;
            ex = ex_buf[n];
            ey = ey_buf[n];
            tx = tx_buf[n];
            ty = ty_buf[n];
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
