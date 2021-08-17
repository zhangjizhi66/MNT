
#include <stdio.h>
#include <time.h>

#include <TFile.h>
#include <TTree.h>
#include <TH2.h>

int main(int argc, char **argv)
{
    if (argc < 2){
        printf("USAGE: ./delaydelay RunID\n");
        exit(EXIT_FAILURE);
    }

    int runid = atoi(argv[1]);
    printf("drawing delayed-delayed gamma matrix %04d ......\n",runid);

    TFile* ipf = new TFile(Form("../../COIN_ROOT/coin%04d.root",runid));
    TTree* tree = (TTree*)ipf->Get("tree");

    const char *filename = Form("../../MATRIX/delaydelay/ddmat%04d.root",runid);
    TFile *opf=new TFile(filename,"RECREATE");

    int ngam,dt[10000];
    float e[10000],fom[10000];
    tree->SetBranchAddress("ntk",&ngam);
    tree->SetBranchAddress("tke",&e);
    tree->SetBranchAddress("tkdt",&dt);
    tree->SetBranchAddress("tkfom",&fom);

    TH2F *dd = new TH2F("dd","delayed-delayed g-g matrix",2000,0,2000,2000,0,2000);

    clock_t start=clock(),stop=clock();
    int nentries = tree->GetEntries();
    for (int jentry=0; jentry<nentries; jentry++)
    {
        tree->GetEntry(jentry);
        for (int ihit=0; ihit<ngam; ihit++)
            for (int jhit=0; jhit<ngam; jhit++)
            {
                if ( ihit == jhit ) continue;
                if ( fom[ihit]>0.8 || fom[jhit]>0.8 ) continue;
                if ( dt[ihit]<30 || dt[jhit]<30 ) continue;
                if ( abs(dt[ihit]-dt[jhit]) > 20 ) continue;

                dd->Fill(e[ihit],e[jhit]);
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
    printf("Process %.3f %%  Time remaining %02d min %02d s\r",100.,0,0);

    opf->Write(NULL, TObject::kOverwrite);
    opf->Close();

    printf("\nDone! Output file in %s\n",filename);
}
