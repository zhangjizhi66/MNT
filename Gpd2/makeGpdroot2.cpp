#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include <fstream>
#include <stdio.h>
#include <time.h>

// Branches for input ROOT file
int nG,Gdt[10000];
float Ge[10000],Gfom[10000];
float GcosP[10000],GcosT[10000];
float pP,pT;

// buffer branches 
Short_t ed_buf[10000],ep_buf[10000];
Short_t td_buf[10000],tp_buf[10000];
Short_t cosP_buf[10000],cosT_buf[10000];

// Branches for new ROOT file
Short_t nd,np;     // number of gamma coincident
Short_t ed,ep;     // energy, prompt corrected
Short_t td,tp;     // time with respect to Chico event
Short_t cosP,cosT; // cos value times 10000
Short_t pP_new,pT_new;

int main(int argc, char** argv)
{
    if (argc != 3){
        printf("USAGE: ./makeGpdroot2 runid_1 runid_2\n");
        exit(EXIT_FAILURE);
    }
    
    printf("\nmaking ROOT files %d to %d ......\n",atoi(argv[1]),atoi(argv[2]));
    
    std::ifstream ifs("../validID.txt");
    int runid;
    TChain *tree = new TChain("tree");
    while (ifs>>runid)
        if (runid>=atoi(argv[1]) && runid<=atoi(argv[2]))
            tree->Add(Form("../COIN_ROOT2/coin%04d.root",runid));
    
    tree->SetBranchAddress("nG",&nG);
    tree->SetBranchAddress("Ge",&Ge);
    tree->SetBranchAddress("Gdt",&Gdt);
    tree->SetBranchAddress("Gfom",&Gfom);
    tree->SetBranchAddress("GcosP",&GcosP);
    tree->SetBranchAddress("GcosT",&GcosT);
    tree->SetBranchAddress("pP",&pP);
    tree->SetBranchAddress("pT",&pT);
    
    const char *filename = Form("ROOT/Gpd2_%d_%d.root",atoi(argv[1]),atoi(argv[2]));
    TFile *opf = new TFile(filename,"RECREATE");
    TTree *opt = new TTree("tree","tree");
    
    opt->Branch("nd",&nd,"nd/S");
    opt->Branch("np",&np,"nd/S");
    opt->Branch("ed",&ed,"ed/S");
    opt->Branch("ep",&ep,"ep/S");
    opt->Branch("td",&td,"td/S");
    opt->Branch("tp",&tp,"tp/S");
    opt->Branch("cosP",&cosP,"cosP/S");
    opt->Branch("cosT",&cosT,"cosT/S");
    opt->Branch("pP",&pP_new,"pP/S");
    opt->Branch("pT",&pT_new,"pT/S");
    
    clock_t start=clock(),stop=clock();
    
    long long int nentries = tree->GetEntries();
    for (long long int jentry=0; jentry<nentries; jentry++){
        tree->GetEntry(jentry);
        pP_new = pP;
        pT_new = pT;
  
        np=0;
        nd=0;
        for (int hit=0; hit<nG; hit++){
            if (Gfom[hit]>0.8) continue;
            if (Ge[hit]>2000) continue;
            if (abs(Gdt[hit])<=20){  // prompt   
                ep_buf[np] = Ge[hit];
                tp_buf[np] = Gdt[hit];
                cosP_buf[np] = GcosP[hit]*10000;
                cosT_buf[np] = GcosT[hit]*10000;
                np++;
            }
            else if (Gdt[hit]>30){  // delayed
                ed_buf[nd] = Ge[hit];
                td_buf[nd] = Gdt[hit];
                nd++;
            }
        }
        
        for (int p=0; p<np; p++)
            for (int d=0; d<nd; d++){
                // if(epT_buf[p]>=401 && epT_buf[p]<=411) continue;
                // if(epT_buf[p]>=572 && epT_buf[p]<=582) continue;
                // if(epT_buf[p]>=353 && epT_buf[p]<=358) continue;
                // if(epT_buf[p]>=364 && epT_buf[p]<=370) continue;
                ed = ed_buf[d];
                ep = ep_buf[p];
                td = td_buf[d];
                tp = tp_buf[p];
                cosP = cosP_buf[p];
                cosT = cosT_buf[p];
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
