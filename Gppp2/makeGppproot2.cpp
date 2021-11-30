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
Short_t ex_buf[10000],ey_buf[10000],ez_buf[10000];
Short_t cosPx_buf[10000],cosTx_buf[10000];
Short_t cosPy_buf[10000],cosTy_buf[10000];
Short_t cosPz_buf[10000],cosTz_buf[10000];

// Branches for new ROOT file
Short_t np;     // number of gamma coincident
Short_t ex,ey,ez;   // energy, doppler corrected
Short_t cosPx,cosTx,cosPy,cosTy,cosPz,cosTz; // cos value times 10000
Short_t pP_new,pT_new;

int main(int argc, char** argv)
{
    if (argc != 3){
        printf("USAGE: ./makeGppproot2 runid_1 runid_2");
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
    
    const char *filename = Form("ROOT/Gppp2_%d_%d.root",atoi(argv[1]),atoi(argv[2]));
    TFile *opf = new TFile(filename,"RECREATE");
    TTree *opt = new TTree("tree","tree");
    
    opt->Branch("np",&np,"np/S");
    opt->Branch("ex",&ex,"ex/S");
    opt->Branch("ey",&ey,"ey/S");
    opt->Branch("ez",&ez,"ez/S");
    opt->Branch("cosPx",&cosPx,"cosPx/S");
    opt->Branch("cosTx",&cosTx,"cosTx/S");
    opt->Branch("cosPy",&cosPy,"cosPy/S");
    opt->Branch("cosTy",&cosTy,"cosTy/S");
    opt->Branch("cosPz",&cosPz,"cosPz/S");
    opt->Branch("cosTz",&cosTz,"cosTz/S");
    opt->Branch("pP",&pP_new,"pP/S");
    opt->Branch("pT",&pT_new,"pT/S");
    
    clock_t start=clock(),stop=clock();
    
    long long int nentries = tree->GetEntries();
    for (long long int jentry=0; jentry<nentries; jentry++){
        tree->GetEntry(jentry);
        pP_new = pP;
        pT_new = pT;
        
        int nevent = 0;
        for (int xhit=0; xhit<nG; xhit++)
            for (int yhit=xhit+1; yhit<nG; yhit++)
                for (int zhit=yhit+1; zhit<nG; zhit++){
                    if (Gfom[xhit]>0.8 || Gfom[yhit]>0.8 || Gfom[zhit]>0.8) continue;
                    if (Ge[xhit]>2000 || Ge[yhit]>2000 || Ge[zhit]>2000) continue;
                    if (abs(Gdt[xhit])>30 || abs(Gdt[yhit])>30 || abs(Gdt[zhit])>30) continue;  // prompt
                    if (abs(Gdt[xhit]-Gdt[yhit])>20 || abs(Gdt[xhit]-Gdt[zhit])>20 || abs(Gdt[yhit]-Gdt[zhit])>20) continue;

                    ex_buf[nevent] = Ge[xhit];
                    ey_buf[nevent] = Ge[yhit];
                    ez_buf[nevent] = Ge[zhit];
                    cosPx_buf[nevent] = GcosP[xhit]*10000;
                    cosTx_buf[nevent] = GcosT[xhit]*10000;
                    cosPy_buf[nevent] = GcosP[yhit]*10000;
                    cosTy_buf[nevent] = GcosT[yhit]*10000;
                    cosPz_buf[nevent] = GcosP[zhit]*10000;
                    cosTz_buf[nevent] = GcosT[zhit]*10000;
                    nevent++;
                }
        
        for (int n=0; n<nevent; n++){
            // if(eTx_buf[n]>=401 && eTx_buf[n]<=411) continue;
            // if(eTx_buf[n]>=572 && eTx_buf[n]<=582) continue;
            // if(eTx_buf[n]>=353 && eTx_buf[n]<=358) continue;
            // if(eTx_buf[n]>=364 && eTx_buf[n]<=370) continue;
            // if(eTy_buf[n]>=401 && eTy_buf[n]<=411) continue;
            // if(eTy_buf[n]>=572 && eTy_buf[n]<=582) continue;
            // if(eTy_buf[n]>=353 && eTy_buf[n]<=358) continue;
            // if(eTy_buf[n]>=364 && eTy_buf[n]<=370) continue;
            
            np = nevent;
            ex = ex_buf[n];
            ey = ey_buf[n];
            ez = ez_buf[n];
            cosPx = cosPx_buf[n];
            cosTx = cosTx_buf[n];
            cosPy = cosPy_buf[n];
            cosTy = cosTy_buf[n];
            cosPz = cosPz_buf[n];
            cosTz = cosTz_buf[n];
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
