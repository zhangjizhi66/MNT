
#include <stdio.h>
#include <time.h>

#include <TFile.h>
#include <TTree.h>
#include <TH3.h>

int main(int argc, char **argv)
{
    if (argc < 2){
        printf("USAGE: ./promptdelaytime RunID\n");
        exit(EXIT_FAILURE);
    }

    int runid = atoi(argv[1]);
    printf("drawing prompt-delayed-time gamma cube %04d ......\n",runid);

    TFile* ipf = new TFile(Form("../../COIN_ROOT/coin%04d.root",runid));
    TTree* tree = (TTree*)ipf->Get("tree");

    const char *filename = Form("../../MATRIX/promptdelaytime/pdtmat%04d.root",runid);
    TFile *opf=new TFile(filename,"RECREATE");

    int ngam,dt[10000];
    float e[10000],eP[10000];
    tree->SetBranchAddress("ng",&ngam);
    tree->SetBranchAddress("ge",&e);
    tree->SetBranchAddress("geP",&eP);
    tree->SetBranchAddress("gdt",&dt);

    // x:prompt y:delayed z:time
    /// note: bin number 1500*1500*200 is known to overflow
    TH3F *pdt = new TH3F("pdt","prompt-delayed-time gamma cube",1500,0,1500,1500,0,1500,100,0,1000);

    clock_t start=clock(),stop=clock();
    int nentries = tree->GetEntries();
    for (int jentry=0; jentry<nentries; jentry++)
    {
        tree->GetEntry(jentry);
        for (int ihit=0; ihit<ngam; ihit++)
            for (int jhit=0; jhit<ngam; jhit++)
            {
                if ( ihit == jhit ) continue;
                if ( dt[ihit]>30 || dt[ihit]<-30 ) continue;    //prompt
                if ( dt[jhit]<30 ) continue;                    //delayed

                pdt->Fill(eP[ihit],e[jhit],dt[jhit]-dt[ihit]);
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
