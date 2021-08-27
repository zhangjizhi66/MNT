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
Short_t ed_buf[10000],epP_buf[10000],epT_buf[10000];
Short_t td_buf[10000],tp_buf[10000];

// Branches for new ROOT file
Short_t nd,np;     // number of gamma coincident
Short_t ed,epP,epT;   // energy, prompt corrected
Short_t td,tp;  // time with respect to Chico event
Short_t pseuQ_new,thetaT_new;

int main(int argc, char** argv)
{
    if (argc != 3){
        printf("USAGE: ./makeGpdroot runid_1 runid_2");
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
    tree->SetBranchAddress("GeT",&GeT);
    tree->SetBranchAddress("GeP",&GeP);
    tree->SetBranchAddress("Gdt",&Gdt);
    tree->SetBranchAddress("Gfom",&Gfom);
    tree->SetBranchAddress("pseuQ",&pseuQ);
    tree->SetBranchAddress("thetaT",&thetaT);
    
    const char *filename = Form("ROOT/Gpd_%d_%d.root",atoi(argv[1]),atoi(argv[2]));
    TFile *opf = new TFile(filename,"RECREATE");
    TTree *opt = new TTree("tree","tree");
    
    opt->Branch("nd",&nd,"nd/S");
    opt->Branch("np",&np,"nd/S");
    opt->Branch("ed",&ed,"ed/S");
    opt->Branch("epP",&epP,"epP/S");
    opt->Branch("epT",&epT,"epT/S");
    opt->Branch("td",&td,"td/S");
    opt->Branch("tp",&tp,"tp/S");
    opt->Branch("pseuQ",&pseuQ_new,"pseuQ/S");
    opt->Branch("thetaT",&thetaT_new,"thetaT/S");
    
    clock_t start=clock(),stop=clock();
    
    long long int nentries = tree->GetEntries();
    for (long long int jentry=0; jentry<nentries; jentry++){
        tree->GetEntry(jentry);
        pseuQ_new = pseuQ;
        thetaT_new = thetaT*180/3.14159;
        
        np=0;
        for (int phit=0; phit<nG; phit++){
            if (Gfom[phit]>0.8) continue;
            if (GeP[phit]>2000) continue;
            if (abs(Gdt[phit])>20) continue;  // prompt
                
            epP_buf[np] = GeP[phit];
            epT_buf[np] = GeT[phit];
            tp_buf[np] = Gdt[phit];
            np++;
        }
        nd=0;
        for (int dhit=0; dhit<nG; dhit++){
            if (Gfom[dhit]>0.8) continue;
            if (Ge[dhit]>2000) continue;
            if (Gdt[dhit]<30) continue;  // delayed
                
            ed_buf[nd] = Ge[dhit];
            td_buf[nd] = Gdt[dhit];
            nd++;
        }
        if (nd>2 || np>2) continue;  // ignore 3 or more multiplicity coincidence
        
        for (int p=0; p<np; p++)
            for (int d=0; d<nd; d++){
                //if(epT_buf[p]>=401 && epT_buf[p]<=411) continue;
                //if(epT_buf[p]>=572 && epT_buf[p]<=582) continue;
                // if(epT_buf[p]>=353 && epT_buf[p]<=358) continue;
                // if(epT_buf[p]>=364 && epT_buf[p]<=370) continue;
                ed = ed_buf[d];
                epP = epP_buf[p];
                epT = epT_buf[p];
                td = td_buf[d];
                tp = tp_buf[p];
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
