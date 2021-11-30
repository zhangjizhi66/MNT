
#include <TFile.h>
#include <TCutG.h>
#include <TTree.h>
#include <TH2F.h>

#include <stdio.h>
#include <time.h>
#include <vector>

#include "MakeCoin2.h"

#define maxCoin 10000
#define winCoin 2000 //2us, according to /data/d3/zhangjizhi/MNT/ana_convert.ipynb @ 162.105.151.64
#define ch2ns 0.100
#define beamE 1088.0
#define beamA 136.0
#define targetA 198.0
#define P0 16602.2
#define Mtot 334.0

// Branches of timstamp
Double_t tsChico,tsGT;
ULong64_t tsTK;

// Branches of CHICO
Int_t pid,dT;
Long64_t ts;
Float_t dofP,dofT,thetaP,thetaT,phiP,phiT,massP,massT,pP,pT,pseuQ,realQ;

// Branches of GAMMAGT
Float_t gtx[maxCoin],gty[maxCoin],gtz[maxCoin],gte[maxCoin],gttheta[maxCoin],gtphi[maxCoin];
Float_t gtcosP[maxCoin],gtcosT[maxCoin];
Int_t gtdt[maxCoin],gtid[maxCoin];

// Branches of GAMMATK
Float_t tkx0[maxCoin],tkx1[maxCoin],tky0[maxCoin],tky1[maxCoin],tkz0[maxCoin],tkz1[maxCoin];
Float_t tke[maxCoin],tke0[maxCoin],tke1[maxCoin],tkfom[maxCoin],tktheta[maxCoin],tkphi[maxCoin];
Float_t tkcosP[maxCoin],tkcosT[maxCoin];
Int_t tkdt[maxCoin],tkndet[maxCoin],tkid[maxCoin];

// Number of gamma events coincident with Chico event
Int_t ngtCoin=0,ntkCoin=0;

PARTICLE particle;
GAMMAGT gammagt;
GAMMATK gammatk;

TCutG *massLCut,*massRCut;
TCutG *etCut,*etCutTK,*GT_TKCut,*ThetaLRCut;

void SetBranchAddressChico(TTree* treeChico){
    treeChico->SetBranchAddress("id",&particle.id);
    treeChico->SetBranchAddress("dT",&particle.dT);
    treeChico->SetBranchAddress("dL",&particle.dL);
    treeChico->SetBranchAddress("dR",&particle.dR);
    treeChico->SetBranchAddress("thetaL",&particle.thetaL);
    treeChico->SetBranchAddress("thetaR",&particle.thetaR);
    treeChico->SetBranchAddress("phiL",&particle.phiL);
    treeChico->SetBranchAddress("phiR",&particle.phiR);
}
void SetBranchAddressGT(TTree* treeGT){
    treeGT->SetBranchAddress("x",&gammagt.x);
    treeGT->SetBranchAddress("y",&gammagt.y);
    treeGT->SetBranchAddress("z",&gammagt.z);
    treeGT->SetBranchAddress("e",&gammagt.e);
    treeGT->SetBranchAddress("theta",&gammagt.theta);
    treeGT->SetBranchAddress("phi",&gammagt.phi);
    treeGT->SetBranchAddress("cid",&gammagt.cid);
}
void SetBranchAddressTK(TTree* treeTK){
    treeTK->SetBranchAddress("e",&gammatk.e);
    treeTK->SetBranchAddress("ndet",&gammatk.ndet);
    treeTK->SetBranchAddress("fom",&gammatk.fom);
    treeTK->SetBranchAddress("x0",&gammatk.x0);
    treeTK->SetBranchAddress("y0",&gammatk.y0);
    treeTK->SetBranchAddress("z0",&gammatk.z0);
    treeTK->SetBranchAddress("e0",&gammatk.e0);
    treeTK->SetBranchAddress("x1",&gammatk.x1);
    treeTK->SetBranchAddress("y1",&gammatk.y1);
    treeTK->SetBranchAddress("z1",&gammatk.z1);
    treeTK->SetBranchAddress("e1",&gammatk.e1);
    treeTK->SetBranchAddress("fid",&gammatk.fid);  //first hit crystal id
    treeTK->SetBranchAddress("theta",&gammatk.theta);
    treeTK->SetBranchAddress("phi",&gammatk.phi);
}

void SetBranch(TTree* tree){
    tree->Branch("pid",&pid,"pid/I");
    tree->Branch("ts",&ts,"ts/l");
    tree->Branch("dT",&dT,"dT/I");
    tree->Branch("dofP",&dofP,"dofP/F");
    tree->Branch("dofT",&dofT,"dofT/F");
    tree->Branch("thetaP",&thetaP,"thetaP/F");
    tree->Branch("thetaT",&thetaT,"thetaT/F");
    tree->Branch("phiP",&phiP,"phiP/F");
    tree->Branch("phiT",&phiT,"phiT/F");
    tree->Branch("massT",&massT,"massT/F");
    tree->Branch("massP",&massP,"massP/F");
    tree->Branch("pP",&pP,"pP/F");
    tree->Branch("pT",&pT,"pT/F");
    tree->Branch("pseuQ",&pseuQ,"pseuQ/F");
    tree->Branch("realQ",&realQ,"realQ/F");

    tree->Branch("ng",&ngtCoin,"ng/I");
    tree->Branch("gx",&gtx,"gx[ng]/F");
    tree->Branch("gy",&gty,"gy[ng]/F");
    tree->Branch("gz",&gtz,"gz[ng]/F");
    tree->Branch("ge",&gte,"ge[ng]/F");
    tree->Branch("gtheta",&gttheta,"gtheta[ng]/F");
    tree->Branch("gphi",&gtphi,"gphi[ng]/F");
    tree->Branch("gcosP",&gtcosP,"gcosP[ng]/F");
    tree->Branch("gcosT",&gtcosT,"gcosT[ng]/F");
    tree->Branch("gdt",&gtdt,"gdt[ng]/I");
    tree->Branch("gid",&gtid,"gid[ng]/I");

    tree->Branch("nG",&ntkCoin,"nG/I");
    tree->Branch("Ge",&tke,"Ge[nG]/F");
    tree->Branch("Gndet",&tkndet,"Gndet[nG]/I");
    tree->Branch("Gfom",&tkfom,"Gfom[nG]/F");
    tree->Branch("Gdt",&tkdt,"Gdt[nG]/I");
    tree->Branch("Gx0",&tkx0,"Gx0[nG]/F");
    tree->Branch("Gy0",&tky0,"Gy0[nG]/F");
    tree->Branch("Gz0",&tkz0,"Gz0[nG]/F");
    tree->Branch("Ge0",&tke0,"Ge0[nG]/F");
    tree->Branch("Gx1",&tkx1,"Gx1[nG]/F");
    tree->Branch("Gy1",&tky1,"Gy1[nG]/F");
    tree->Branch("Gz1",&tkz1,"Gz1[nG]/F");
    tree->Branch("Ge1",&tke1,"Ge1[nG]/F");
    tree->Branch("Gid",&tkid,"Gid[nG]/I");  //first hit crystal id
    tree->Branch("Gtheta",&tktheta,"Gtheta[nG]/F");
    tree->Branch("Gphi",&tkphi,"Gphi[nG]/F");
    tree->Branch("GcosP",&tkcosP,"GcosP[nG]/F");
    tree->Branch("GcosT",&tkcosT,"GcosT[nG]/F");
}

void copygt(int nCoin){
    gtx[nCoin] = gammagt.x;
    gty[nCoin] = gammagt.y;
    gtz[nCoin] = gammagt.z;
    gte[nCoin] = gammagt.e;
    gttheta[nCoin] = gammagt.theta;
    gtphi[nCoin] = gammagt.phi;
    gtid[nCoin] = gammagt.cid;
}
void copytk(int nCoin){
    tkx0[nCoin] = gammatk.x0;
    tky0[nCoin] = gammatk.y0;
    tkz0[nCoin] = gammatk.z0;
    tke0[nCoin] = gammatk.e0;
    tkx1[nCoin] = gammatk.x1;
    tky1[nCoin] = gammatk.y1;
    tkz1[nCoin] = gammatk.z1;
    tke1[nCoin] = gammatk.e1;
    tke[nCoin] = gammatk.e;
    tktheta[nCoin] = gammatk.theta;
    tkphi[nCoin] = gammatk.phi;
    tkid[nCoin] = gammatk.fid;
    tkndet[nCoin] = gammatk.ndet;
    tkfom[nCoin] = gammatk.fom;
}

float cos_solid(float theta1, float phi1, float theta2, float phi2){
    return sinf(theta1)*sinf(theta2)*cosf(phi1-phi2)+cosf(theta1)*cosf(theta2);
}

void assignChicoProjL(){    // assign Chico info if projectile-like fragment hit on left
    dT = particle.dT;
    pid = particle.id;
    ts = particle.ts;

    dofP = particle.dL;
    thetaP = particle.thetaL;
    phiP = particle.phiL;

    dofT = particle.dR;
    thetaT = particle.thetaR;
    phiT = particle.phiR;

    // correction for energy loss in target
    float fP = 1.0127 - 0.015567/cosf(thetaP) - 0.00027722/powf(cosf(thetaP),2.);  //1088 MeV;
    float fT = 1.0068 - 0.0040415/cosf(thetaT) - 0.0088646/powf(cosf(thetaT),2);  //1088 MeV;
    //float fP = 1.0132 - 0.016286/cosf(thetaP) - 0.00027716/powf(cosf(thetaP),2.);    //1028.5 MeV;
    //float fT = 0.83469 - 0.0037606/cosf(thetaT) - 0.0074352/powf(cosf(thetaT),2);    //1028.5 MeV;
    pP = P0*sinf(thetaT)/sinf(thetaP+thetaT)*fP;
    pT = P0*sinf(thetaP)/sinf(thetaP+thetaT)*fT;
    massP = (0.032206*dT*ch2ns + Mtot*dofT/pT)/(dofP/pP+dofT/pT);
    massT = (-0.032206*dT*ch2ns + Mtot*dofP/pP)/(dofP/pP+dofT/pT);

    float eP = powf(pP/P0,2)*beamE;
    float eT = powf(pT/P0,2)*beamE*beamA/targetA;
    pseuQ = eP + eT - beamE;
    realQ = powf(pP/P0,2)*(beamA/massP)*beamE + powf(pT/P0,2)*(beamA/massT)*beamE - beamE;
}
void assignChicoProjR(){      // assign Chico info if projectile-like fragment hit on right
    dT = particle.dT;
    pid = particle.id;
    ts = particle.ts;

    dofP = particle.dR;
    thetaP = particle.thetaR;
    phiP = particle.phiR;

    dofT = particle.dL;
    thetaT = particle.thetaL;
    phiT = particle.phiL;

    // correction for energy loss in target
    float fP = 1.0127 - 0.015567/cosf(thetaP) - 0.00027722/powf(cosf(thetaP),2);  //1088 MeV;
    float fT = 1.0068 - 0.0040415/cosf(thetaT) - 0.0088646/powf(cosf(thetaT),2);  //1088 MeV;
    //float fP = 1.0132 - 0.016286/cosf(thetaP) - 0.00027716/powf(cosf(thetaP),2.);    //1028.5 MeV;
    //float fT = 0.83469 - 0.0037606/cosf(thetaT) - 0.0074352/powf(cosf(thetaT),2);    //1028.5 MeV;
    pP = P0*sinf(thetaT)/sinf(thetaP+thetaT)*fP;
    pT = P0*sinf(thetaP)/sinf(thetaP+thetaT)*fT;
    massP = (-0.032206*dT*ch2ns + Mtot*dofT/pT)/(dofP/pP+dofT/pT);
    massT = (0.032206*dT*ch2ns + Mtot*dofP/pP)/(dofP/pP+dofT/pT);

    float eP = powf(pP/P0,2)*beamE;
    float eT = powf(pT/P0,2)*beamE*beamA/targetA;
    pseuQ = eP + eT - beamE;
    realQ = powf(pP/P0,2)*(beamA/massP)*beamE + powf(pT/P0,2)*(beamA/massT)*beamE - beamE;
}

int main(int argc, char **argv)
{
    if (argc!=2) {
        printf("USAGE: ./MakeCoin2 Runid \n");
        exit(EXIT_FAILURE);
    }

    int runid = atoi(argv[1]);

    TFile *fChico = new TFile(Form("../CHICO_ROOT/Chico%04d.root",runid));
    TTree *treeChico = (TTree*)fChico->Get("tree");
    treeChico->SetBranchAddress("ts",&tsChico);
    Int_t nentriesChico = treeChico->GetEntriesFast();

    TFile *fGT = new TFile(Form("../GAMMAGT_ROOT/GT%04d.root",runid));
    TTree *treeGT = (TTree*)fGT->Get("tree");
    treeGT->SetBranchAddress("ts",&tsGT);
    Int_t nentriesGT = treeGT->GetEntriesFast();

    TFile *fTK = new TFile(Form("../GAMMATK_ROOT/TK%04d.root",runid));
    TTree *treeTK = (TTree*)fTK->Get("tree");
    treeTK->SetBranchAddress("ts",&tsTK);
    Int_t nentriesTK = treeTK->GetEntriesFast();

    TFile *opf=new TFile(Form("../COIN_ROOT2/coin%04d.root",runid),"RECREATE");
    TTree *opt=new TTree("tree","tree");
    SetBranch(opt);

    clock_t start,stop;

    // store gamma entry number coincident with chico
    std::vector<std::vector<int>> vgt(nentriesChico,std::vector<int>());
    std::vector<std::vector<int>> vtk(nentriesChico,std::vector<int>());

    printf("making coincident event %04d ......\nsorting timestamp ......\n",runid);

    start = clock();
    Int_t ientry=0,jentry=0,kentry=0;
    while (ientry<nentriesChico)
    {
        treeChico->GetEntry(ientry);
        treeGT->GetEntry(jentry);

        bool coinGT = abs((Long64_t)tsGT-(Long64_t)tsChico)<winCoin;
        if (coinGT) vgt[ientry].push_back(jentry);

        if (tsGT<tsChico || coinGT) jentry++;
        else ientry++;

        if (jentry==nentriesGT) break;

        //show progress
        if (ientry%150000 == 149999){
            stop = clock();
            printf("Process %.2f %%  Time remaining %02d min %02d s\r",
                double(ientry)/double(nentriesChico)*50.,
                int((stop-start)*(2*nentriesChico-ientry)/ientry/1e6/60),
                int((stop-start)*(2*nentriesChico-ientry)/ientry/1e6)%60);
            fflush(stdout);
        }
    }

    ientry=0;
    while (ientry<nentriesChico)
    {
        treeChico->GetEntry(ientry);
        treeTK->GetEntry(kentry);

        bool coinTK = abs((Long64_t)tsTK-(Long64_t)tsChico)<winCoin;
        if (coinTK) vtk[ientry].push_back(kentry);

        if (tsTK<tsChico || coinTK) kentry++;
        else ientry++;

        if (kentry==nentriesTK) break;

        //show progress
        if (ientry%150000 == 149999){
            stop = clock();
            printf("Process %.2f %%  Time remaining %02d min %02d s\r",
                double(ientry)/double(nentriesChico)*50.+50.,
                int((stop-start)*(nentriesChico-ientry)/(nentriesChico+ientry)/1e6/60),
                int((stop-start)*(nentriesChico-ientry)/(nentriesChico+ientry)/1e6)%60);
            fflush(stdout);
        }
    }
    stop = clock();
    printf("Process %.3f %%  Total Time %02d min %02d s        \n",100.,int((stop-start)/1e6/60),int((stop-start)/1e6)%60);
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    printf("writing to new root file ......\n");

    // set branch address other than timestamp
    SetBranchAddressChico(treeChico);
    SetBranchAddressGT(treeGT);
    SetBranchAddressTK(treeTK);
    
    // read matrix file for cut
    TFile *ipf1 = new TFile("mass_theta_mat.root");
    TH2F *htm = (TH2F*)ipf1->Get("htm");

    start = clock();
    for (ientry=0; ientry<nentriesChico; ientry++)
    {
        treeChico->GetEntry(ientry);
        particle.ts = tsChico;          // to convert type from double to long long int
        
        double pL = P0*sinf(particle.thetaR)/sinf(particle.thetaL+particle.thetaR);
        double pR = P0*sinf(particle.thetaL)/sinf(particle.thetaL+particle.thetaR);

        double massL = ( 0.032206*particle.dT*ch2ns + Mtot*particle.dR/pR)/(particle.dR/pR+particle.dL/pL);
        double massR = (-0.032206*particle.dT*ch2ns + Mtot*particle.dL/pL)/(particle.dR/pR+particle.dL/pL);

        // to judge if projectile-like hit on left or right
        int countL = htm->GetBinContent(htm->FindBin(massL,particle.thetaL/M_PI*180));
        int countR = htm->GetBinContent(htm->FindBin(massR,particle.thetaR/M_PI*180));
        if (countL<20000 || countR<20000) continue;
        else if (particle.thetaL/M_PI*180<42 && particle.thetaR/M_PI*180>42) assignChicoProjL();
        else if (particle.thetaR/M_PI*180<42 && particle.thetaL/M_PI*180>42) assignChicoProjR();
        else continue;

        for (ngtCoin=0; ngtCoin<(int)vgt[ientry].size(); ngtCoin++){
            treeGT->GetEntry(vgt[ientry][ngtCoin]);
            gammagt.ts = tsGT;          // to convert type from double to long long int

            copygt(ngtCoin);
            gtdt[ngtCoin] = (Long64_t)gammagt.ts - (Long64_t)particle.ts + 207;  //calibrate and shift peak to zero
            
            // Doppler correction
            gtcosP[ngtCoin] = cos_solid(thetaP,phiP,gttheta[ngtCoin],gtphi[ngtCoin]);
            gtcosT[ngtCoin] = cos_solid(thetaT,phiT,gttheta[ngtCoin],gtphi[ngtCoin]);
        }

        for (ntkCoin=0; ntkCoin<(int)vtk[ientry].size(); ntkCoin++){
            treeTK->GetEntry(vtk[ientry][ntkCoin]);
            gammatk.ts = tsTK;

            copytk(ntkCoin);
            tkdt[ntkCoin] = (Long64_t)gammatk.ts - (Long64_t)particle.ts + 207;  //calibrate and shift peak to zero
            
            // Doppler correction
            tkcosP[ntkCoin] = cos_solid(thetaP,phiP,tktheta[ntkCoin],tkphi[ntkCoin]);
            tkcosT[ntkCoin] = cos_solid(thetaT,phiT,tktheta[ntkCoin],tkphi[ntkCoin]);
        }

        opt->Fill();

        if (ientry%50000 == 49999){
            stop = clock();
            printf("Process %.2f %%  Time remaining %02d min %02d s\r",
                double(ientry)/double(nentriesChico)*100.,
                int((stop-start)*(nentriesChico-ientry)/ientry/1e6/60),
                int((stop-start)*(nentriesChico-ientry)/ientry/1e6)%60);
            fflush(stdout);
        }
    }
    stop = clock();
    printf("Process %.3f %%  Total Time %02d min %02d s        \n",100.,int((stop-start)/1e6/60),int((stop-start)/1e6)%60);
   
    fChico->Close();
    fGT->Close();
    fTK->Close();

    opf->cd();
    opt->Write();
    opf->Close();

    printf("Done! Output file in ../COIN_ROOT2/coin%04d.root\n",runid);
}
