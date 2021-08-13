
#include <stdio.h>
#include <time.h>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TH2.h>

int main(int argc, char **argv)
{
    if (argc < 4){
        printf("USAGE: RunID TimestampCut_btm_ns>-1000 TimestampCut_top_ns<2000 (EnergyRange_top_keV=1500) (Width_Coin_ns=20) (gt/tk)\n");
        exit(EXIT_FAILURE);
    }

    int runid = atoi(argv[1]);
    printf("drawing delayed-delayed gamma matrix %04d ......",runid);

    TFile* ipf = new TFile(Form("../COIN_ROOT/coin%04d.root",runid));
    TTree* tree = (TTree*)ipf->Get("tree");

    const char *filename = Form("../../MATRIX/delaydelay/ddmat%04d.root",runid);
    TFile *opf=new TFile(filename,"RECREATE");

    int ngam,dt[10000];
    float e[10000];
    std::string gamma_type;
    if (argc >= 7) gamma_type = argv[6];
    else gamma_type = "gt";

    tree->SetBranchAddress(("n"+gamma_type).c_str(),&ngam);
    tree->SetBranchAddress((gamma_type+"e").c_str(),&e);
    tree->SetBranchAddress((gamma_type+"dt").c_str(),&dt);

    int dt_btm=atoi(argv[2]),dt_top=atoi(argv[3]);
    int egam_top = 1500;
    if (argc >= 5) egam_top = atoi(argv[4]);
    TH2F *dgg = new TH2F("dgg",Form("delayed g-g matrix (%d<dt<%d)",dt_btm,dt_top),egam_top,0,egam_top,egam_top,0,egam_top);

    int coin_wid = 20;
    if (argc >= 6) egam_top = atoi(argv[5]);

    clock_t start=clock(),stop=clock();
    int nentries = tree->GetEntries();
    for (long int jentry=0; jentry<nentries; jentry++)
    {
        tree->GetEntry(jentry);
        for (int ihit=0; ihit<ngam; ihit++)
            for (int jhit=0; jhit<ihit; jhit++)
            {
                if ( dt[ihit]<dt_btm || dt[ihit]>dt_top || dt[jhit]<dt_btm || dt[jhit]>dt_top )continue;
                if ( abs(dt[ihit]-dt[jhit]) > coin_wid ) continue;

                dgg->Fill(e[ihit],e[jhit]);
                dgg->Fill(e[jhit],e[ihit]);
            }

        //show progress and time needed
        if (jentry%50000 == 49999 || jentry == nentries-1){
            stop = clock();
            printf("Process %.3f %%  Time remaining %02d min %02d s\r",double(jentry)/double(nentries)*100.,
                int((stop-start)*(nentries-jentry)/jentry/1e6/60),
                int((stop-start)*(nentries-jentry)/jentry/1e6)%60);
            fflush(stdout);
        }
    }

    opf->Write(NULL, TObject::kOverwrite);
    opf->Close();

    printf("\nDone! Output file in %s\n",filename);
}
