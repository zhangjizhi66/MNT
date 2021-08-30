#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include <fstream>
#include <stdio.h>
#include <time.h>

// Branches for input ROOT file
int nG,Gdt[10000];
float Ge[10000],GeP[10000],GeT[10000],Gfom[10000];
float pseuQ,thetaT;

// buffer branches 
Short_t ePx_buf[10000],ePy_buf[10000];
Short_t eTx_buf[10000],eTy_buf[10000];

// Branches for new ROOT file
Short_t np;     // number of delayed gamma coincident
Short_t ePx,ePy,eTx,eTy;   // energy, doppler corrected
Short_t pseuQ_new,thetaT_new;

int main(int argc, char** argv)
{
    if (argc != 3){
        printf("USAGE: ./makeGpproot runid_1 runid_2");
        exit(EXIT_FAILURE);
    }
    
    printf("\nmaking ROOT files %d to %d ......\n",atoi(argv[1]),atoi(argv[2]));
    
    std::ifstream ifs("../validID.txt");
    int runid;
    TChain *tree = new TChain("tree");
    while (ifs>>runid)
        if (runid>=atoi(argv[1]) && runid<=atoi(argv[2]))
            tree->Add(Form("../COIN_ROOT/coin%04d.root",runid));
    
    tree->SetBranchAddress("nG",&nG);
    tree->SetBranchAddress("GeP",&GeP);
    tree->SetBranchAddress("GeT",&GeT);
    tree->SetBranchAddress("Gdt",&Gdt);
    tree->SetBranchAddress("Gfom",&Gfom);
    tree->SetBranchAddress("pseuQ",&pseuQ);
    tree->SetBranchAddress("thetaT",&thetaT);
    
    const char *filename = Form("ROOT/Gpp_%d_%d.root",atoi(argv[1]),atoi(argv[2]));
    TFile *opf = new TFile(filename,"RECREATE");
    TTree *opt = new TTree("tree","tree");
    
    opt->Branch("np",&np,"np/S");
    opt->Branch("ePx",&ePx,"ePx/S");
    opt->Branch("ePy",&ePy,"ePy/S");
    opt->Branch("eTx",&eTx,"eTx/S");
    opt->Branch("eTy",&eTy,"eTy/S");
    opt->Branch("pseuQ",&pseuQ_new,"pseuQ/S");
    opt->Branch("thetaT",&thetaT_new,"thetaT/S");
    
    clock_t start=clock(),stop=clock();
    
    long long int nentries = tree->GetEntries();
    for (long long int jentry=0; jentry<nentries; jentry++){
        tree->GetEntry(jentry);
        pseuQ_new = pseuQ;
        thetaT_new = thetaT*180/3.14159;
        
        int nevent = 0;
        for (int xhit=0; xhit<nG; xhit++)
            for (int yhit=0; yhit<nG; yhit++){
                if (xhit == yhit) continue;
                if (Gfom[xhit]>0.8 || Gfom[yhit]>0.8) continue;
                if (Ge[xhit]>2000 || Ge[yhit]>2000) continue;
                if (abs(Gdt[xhit])>30 || abs(Gdt[yhit])>30) continue;  // prompt
                if (abs(Gdt[xhit]-Gdt[yhit])>20) continue;  // coincident
                
                ePx_buf[nevent] = GeP[xhit];
                ePy_buf[nevent] = GeP[yhit];
                eTx_buf[nevent] = GeT[xhit];
                eTy_buf[nevent] = GeT[yhit];
                nevent++;
            }
        if (nevent >= 12) continue;  // ignore 4 or more multiplicity coincidence, must be fake
        
        for (int n=0; n<nevent; n++){
            //if(eTx_buf[n]>=401 && eTx_buf[n]<=411) continue;
            //if(eTx_buf[n]>=572 && eTx_buf[n]<=582) continue;
            // if(eTx_buf[n]>=353 && eTx_buf[n]<=358) continue;
            // if(eTx_buf[n]>=364 && eTx_buf[n]<=370) continue;
            //if(eTy_buf[n]>=401 && eTy_buf[n]<=411) continue;
            //if(eTy_buf[n]>=572 && eTy_buf[n]<=582) continue;
            // if(eTy_buf[n]>=353 && eTy_buf[n]<=358) continue;
            //if(eTy_buf[n]>=364 && eTy_buf[n]<=370) continue;
            
            np = nevent;
            ePx = ePx_buf[n];
            ePy = ePy_buf[n];
            eTx = eTx_buf[n];
            eTy = eTy_buf[n];
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
