#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include <fstream>
#include <stdio.h>
#include <time.h>

// Branches for input ROOT file
float thetaP,massP,pP;
float thetaT,massT,pT;
float pseuQ;

// Branches for new ROOT file
Short_t thetaP_s,massP_s,ekP_s;  // scattering angle, mass, kinetic energy of PLF
Short_t thetaT_s,massT_s,ekT_s;  // scattering angle, mass, kinetic energy of TLF
Short_t pseuQ_s;

int main(int argc, char** argv)
{
    if (argc != 3){
        printf("USAGE: ./makeGddroot runid_1 runid_2");
        exit(EXIT_FAILURE);
    }
    
    printf("\nmaking ROOT files %d to %d ......\n",atoi(argv[1]),atoi(argv[2]));
    
    std::ifstream ifs("../validID.txt");  // read valid run id
    int runid;
    TChain *tree = new TChain("tree");
    while (ifs>>runid)
        if (runid>=atoi(argv[1]) && runid<=atoi(argv[2]))
            tree->Add(Form("../COIN_ROOT/coin%04d.root",runid));

    tree->SetBranchAddress("thetaP",&thetaP);
    tree->SetBranchAddress("massP",&massP);
    tree->SetBranchAddress("pP",&pP);
    tree->SetBranchAddress("thetaT",&thetaT);
    tree->SetBranchAddress("massT",&massT);
    tree->SetBranchAddress("pT",&pT);
    tree->SetBranchAddress("pseuQ",&pseuQ);
    
    const char *filename = Form("ROOT/chico_%d_%d.root",atoi(argv[1]),atoi(argv[2]));
    TFile *opf = new TFile(filename,"RECREATE");
    TTree *opt = new TTree("tree","tree");

    opt->Branch("thetaP",&thetaP_s,"thetaP/S");
    opt->Branch("massP",&massP_s,"massP/S");
    opt->Branch("ekP",&ekP_s,"ekP/S");
    opt->Branch("thetaT",&thetaT_s,"thetaT/S");
    opt->Branch("massT",&massT_s,"massT/S");
    opt->Branch("ekT",&ekT_s,"ekT/S");
    opt->Branch("pseuQ",&pseuQ_s,"pseuQ/S");
    
    clock_t start=clock(),stop=clock();
    
    long long int nentries = tree->GetEntries();
    for (long long int jentry=0; jentry<nentries; jentry++){
        tree->GetEntry(jentry);
        
        thetaP_s = int(thetaP/M_PI*180);
        massP_s = int(massP);
        ekP_s = int(pP*pP/massP/2/931.4);
        
        thetaT_s = int(thetaT/M_PI*180);
        massT_s = int(massT);
        ekT_s = int(pT*pT/massT/2/931.4);
        
        pseuQ_s = int(pseuQ);
        
        opt->Fill();
        
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
