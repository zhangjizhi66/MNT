
#include <stdio.h>
#include <time.h>

#include <TFile.h>
#include <TTree.h>
#include <TH2.h>

int main(int argc, char **argv)
{
    if (argc < 2){
        printf("USAGE: ./promptprompt RunID\n");
        exit(EXIT_FAILURE);
    }

    int runid = atoi(argv[1]);
    printf("drawing prompt-prompt gamma matrix %04d ......\n",runid);

    TFile* ipf = new TFile(Form("../../COIN_ROOT/coin%04d.root",runid));
    TTree* tree = (TTree*)ipf->Get("tree");

    const char *filename = Form("../../MATRIX/promptprompt/ppmat%04d.root",runid);
    TFile *opf=new TFile(filename,"RECREATE");

    int ngam,dt[10000];
    float e[10000],eP[10000];
    tree->SetBranchAddress("ng",&ngam);
    tree->SetBranchAddress("ge",&e);
    tree->SetBranchAddress("geP",&eP);
    tree->SetBranchAddress("gdt",&dt);

    TH2F *pp = new TH2F("pp","prompt-prompt matrix gated on 136Ba delayed gamma",2000,0,2000,2000,0,2000);

    clock_t start=clock(),stop=clock();
    int nentries = tree->GetEntries();
    for (int jentry=0; jentry<nentries; jentry++)
    {
        tree->GetEntry(jentry);
        if (ngam < 3) continue;
        for (int ihit=0; ihit<ngam; ihit++)//prompt
            for (int jhit=0; jhit<ngam; jhit++)//prompt
                for (int khit=0; khit<ngam; khit++)//delayed
                {
                    if ( ihit == jhit || ihit == khit || jhit == khit ) continue;
                    if ( dt[ihit]>30 || dt[ihit]<-30 ) continue;    //prompt
                    if ( dt[jhit]>30 || dt[jhit]<-30 ) continue;    //prompt
                    if ( dt[khit]<30 ) continue;                    //delayed

                    double delayed[6] = {340.8,363.0,787.1,818.6,1048.0,1235.2};
                    bool is_136Ba = 0;
                    for (int i=0; i<6; i++)
                        if (e[khit]>delayed[i]-2 && e[khit]<delayed[i]+2){
                            is_136Ba = 1;
                            break;
                        }
                    if (is_136Ba) pp->Fill(eP[ihit],eP[jhit]);
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
