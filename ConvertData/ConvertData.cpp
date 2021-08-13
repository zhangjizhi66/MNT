#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <errno.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>

#include <TROOT.h>
#include <TFile.h>
#include <TObject.h>
#include <TCutG.h>
#include <TTree.h>

using namespace std;

#include "ConvertData.h"

#define Mvalue 512
#define PPAC_NUM 20
#define microsec 100
#define OverLap 1000
#define PGWINDL -150
#define PGWINDH -20
#define PGWINL -15
#define PGWINH 15
#define PGTKWINDL 52
#define PGTKWINDH 77
#define GTPosOffsetZ 5.334
#define GTPosOffsetY1 14.71
#define GTPosOffsetY2 -27.46
#define ch2ns 0.100
#define beamE 1088.0
//#define beamE 1028.5   //read run log
#define beamA 136.0
#define targetA 198.0
#define P0 16602.2
//#define P0 16141.8    // read run log
#define Mtot 334.0
#define BEAM 1
#define TARGET 2
#define LEFT 1
#define RIGHT 2
#define NGAM 1
#define FOM 0.8

FILE *fp1;
unsigned long long int tsFirst;

float offGT[QNum*4],gainGT[QNum*4],offdt[QNum*4];
float offGT2[QNum*4],gainGT2[QNum*4];
float gcidoff[QNum*4],gtkfidoff[120];

float offTheta[PPAC_NUM],gainTheta[PPAC_NUM];
float offPhi[PPAC_NUM],gainPhi[PPAC_NUM];
int anodemap[PPAC_NUM][PPAC_NUM];
int cathodemap[4*PPAC_NUM][4*PPAC_NUM];

TCutG *massLCut,*massRCut;
TCutG *etCut,*etCutTK;
TCutG *GT_TKCut;
TCutG *ThetaLRCut;

float crmat[MAXDETPOS+1][MAXCRYSTALNO+1][4][4];

GTEVENT2   GTEvent2;
CHICOEVENT ChicoEvent;
TRACKED_GAMMA_HIT GTEvent1;

TRACKED_GAMMA_RAY gammatk;
float thetaTK,phiTK;
GAMMA      gammagt;
int cidGT;
PARTICLE   particle;

void setbranchChico(TTree* treeChico){
    treeChico->Branch("id",&particle.id,"id/I");
    treeChico->Branch("ts",&particle.t,"ts/D");
    treeChico->Branch("dT",&particle.dT,"dT/I");
    treeChico->Branch("dL",&particle.dL,"dL/F");
    treeChico->Branch("dR",&particle.dR,"dR/F");
    treeChico->Branch("thetaL",&particle.fthetaL,"thetaL/F");
    treeChico->Branch("thetaR",&particle.fthetaR,"thetaR/F");
    treeChico->Branch("phiL",&particle.fphiL,"phiL/F");
    treeChico->Branch("phiR",&particle.fphiR,"phiR/F");
}
void setbranchGT(TTree* treeGT){
    treeGT->Branch("x",&gammagt.x,"x/F");
    treeGT->Branch("y",&gammagt.y,"y/F");
    treeGT->Branch("z",&gammagt.z,"z/F");
    treeGT->Branch("e",&gammagt.e,"e/F");
    treeGT->Branch("theta",&gammagt.theta,"theta/F");
    treeGT->Branch("phi",&gammagt.phi,"phi/F");
    treeGT->Branch("ts",&gammagt.t,"ts/D");
    treeGT->Branch("cid",&cidGT,"cid/I");
}
void setbranchTK(TTree* treeTK){
    treeTK->Branch("e",&gammatk.esum,"e/F");
    treeTK->Branch("ndet",&gammatk.ndet,"ndet/I");
    treeTK->Branch("fom",&gammatk.fom,"fom/F");
    treeTK->Branch("ts",&gammatk.timestamp,"ts/l");
    treeTK->Branch("x0",&gammatk.x0,"x0/F");
    treeTK->Branch("y0",&gammatk.y0,"y0/F");
    treeTK->Branch("z0",&gammatk.z0,"z0/F");
    treeTK->Branch("e0",&gammatk.e0,"e0/F");
    treeTK->Branch("x1",&gammatk.x1,"x1/F");
    treeTK->Branch("y1",&gammatk.y1,"y1/F");
    treeTK->Branch("z1",&gammatk.z1,"z1/F");
    treeTK->Branch("e1",&gammatk.e1,"e1/F");
    treeTK->Branch("fid",&gammatk.fhcrID,"fid/I");  //first hit crystal id
    treeTK->Branch("theta",&thetaTK,"theta/F");
    treeTK->Branch("phi",&phiTK,"phi/F");
}
//////////////////////////////////////////////////////////////////////
void RdCal(const char *fn1, const char *fn2, const char *fn3){
    ifstream GTCALFILE(fn1, ios::in);
    if(!GTCALFILE.is_open()) {
        cerr << "Error opening Gretina Calibration file!"<< endl;
        exit(1);
    }
    for(int i=0;i<QNum*4;i++)
        GTCALFILE >> offGT[i] >> gainGT[i] >> offGT2[i] >> gainGT2[i] >> offdt[i];
    GTCALFILE.close();

    ifstream GAMMAOFF(fn2, ios::in);
    if(!GAMMAOFF.is_open()) {
        cerr << "Error opening Gretina GAMMAOFF Calibration file!"<< endl;
        exit(1);
    }
    for(int i=0;i<QNum*4;i++){
        int k;
        char com;
        GAMMAOFF >> k >>com >> gcidoff[i];
    }
    GAMMAOFF.close();

    ifstream GTKOFF(fn3, ios::in);
    if(!GTKOFF.is_open()) {
        cerr << "Error opening Gretina GTKOFF Calibration file!"<< endl;
        exit(1);
    }
    for(int i=0;i<120;i++){
        int k;
        char com;
        GTKOFF >> k>>com  >> gtkfidoff[i];
    }
    GTKOFF.close();
}
//////////////////////////////////////////////////////////////////////
void RdCRMAT(const char *fn){
    FILE *fp;

    float f1, f2, f3, f4;
    int pos, xtal;
    int nn = 0;
    char *st, str[256];

    fp = fopen(fn, "r");
    if (fp == NULL) {
        printf("Could not open \"%s\".\n", fn);
        exit(1);
    }

    /* Read values. */
    nn = 0;
    st = fgets(str, 256, fp);
    while (st != NULL) {
        if (str[0] == 35) {}     /* '#' comment line, do nothing */
        else if (str[0] == 59) {}    /* ';' comment line, do nothing */
        else if (str[0] == 10) {}    /* Empty line, do nothing */
        else {
            sscanf(str, "%i %i", &pos, &xtal);
            for (int i=0; i<4; i++) {
                st = fgets(str, 256, fp);
                sscanf(str, "%f %f %f %f", &f1, &f2, &f3, &f4);
                crmat[pos-1][xtal][i][0] = f1;
                crmat[pos-1][xtal][i][1] = f2;
                crmat[pos-1][xtal][i][2] = f3;
                crmat[pos-1][xtal][i][3] = f4;
            }
            nn++;
        }

        /* Attempt to read the next line. */
        st = fgets(str, 256, fp);
    }

    fclose(fp);
}
//////////////////////////////////////////////////////////////////////
void setupcut(const char *fn){
    TFile *fcutg;
    char WinName[64];

    fcutg = new TFile(fn, "read");

    sprintf(WinName, "massLCut");
    massLCut = (TCutG*)fcutg->Get(WinName);
    if(massLCut == NULL){
        printf("Could Not Read 2d CutG File %s\n",WinName);
        exit(-1);
    }

    sprintf(WinName, "massRCut");
    massRCut = (TCutG*)fcutg->Get(WinName);
    if(massRCut == NULL){
        printf("Could Not Read 2d CutG File %s\n",WinName);
        exit(-1);
    }

    sprintf(WinName, "etCut");
    etCut = (TCutG*)fcutg->Get(WinName);
    if(etCut == NULL){
        printf("Could Not Read 2d CutG File %s\n",WinName);
        exit(-1);
    }

    sprintf(WinName, "etCutTK");
    etCutTK = (TCutG*)fcutg->Get(WinName);
    if(etCutTK == NULL){
        printf("Could Not Read 2d CutG File %s\n",WinName);
        exit(-1);
    }

    sprintf(WinName, "GT_TKCut");
    GT_TKCut = (TCutG*)fcutg->Get(WinName);
    if(GT_TKCut == NULL){
        printf("Could Not Read 2d CutG File %s\n",WinName);
        exit(-1);
    }

    sprintf(WinName, "ThetaLRCut");
    ThetaLRCut = (TCutG*)fcutg->Get(WinName);
    if(ThetaLRCut == NULL){
        printf("Could Not Read 2d CutG File %s\n",WinName);
        exit(-1);
    }

    fcutg->Close();
}
//////////////////////////////////////////////////////////////////////
static unsigned int* GetGEBEvBuf(GEBheader *Header, const char *FileName){
    /* function will read the gretina data file and */
    /* extract the event buffer following each GEB headr, but not interpret them. */

    unsigned int i;
    unsigned int t1, t2, t3, t4;
    unsigned int *TEMP;

    if(!(TEMP = (unsigned int*) malloc(GEBHDRLENBYTES))) {
        printf("\007  ERROR: Could not malloc data buffer %i bytes.\n",GEBHDRLENBYTES);
        exit(-1);
    }
    if(fread(TEMP,GEBHDRLENBYTES,1,fp1) != 1){
        if (feof(fp1)) return NULL;
        printf("file read error %s\n",FileName);
        return NULL;
    }
    memcpy(Header,TEMP,GEBHDRLENBYTES);
    free(TEMP);
    if(!(TEMP = (unsigned int*) malloc(Header->length))) {
        printf("\007  ERROR: Could not malloc data buffer %i bytes (length).\n",Header->length);
        exit(-1);
    }
    if(fread(TEMP,Header->length,1,fp1) != 1){
        if (feof(fp1)) return NULL;
        printf("file read error %s\n",FileName);
        return NULL;
    }
    if(Header->type == MODETHREE){
        for(i=0;i<Header->length/4;i++){
            t1 = (TEMP[i] & 0x000000ff) << 24;
            t2 = (TEMP[i] & 0x0000ff00) << 8;
            t3 = (TEMP[i] & 0x00ff0000) >> 8;
            t4 = (TEMP[i] & 0xff000000) >> 24;
            TEMP[i] = t1 + t2 + t3 + t4;
        }
    }
    return TEMP;
}
//////////////////////////////////////////////////////////////////////
unsigned int GetGamma3(unsigned int *DataRecord, GTEVENT3 *GTEvent3, unsigned int i){
    unsigned int tempE;
    unsigned int t1, t2, t3, t4;
    unsigned int k;

    for(int j=1;j<=4;j++){
        t1 = (DataRecord[i+j] & 0x000000ff) << 24;
        t2 = (DataRecord[i+j] & 0x0000ff00) << 8;
        t3 = (DataRecord[i+j] & 0x00ff0000) >> 8;
        t4 = (DataRecord[i+j] & 0xff000000) >> 24;
        DataRecord[i+j] = t1 + t2 + t3 + t4;
    }
    switch((DataRecord[i+1] >> 8) & 0xff){
        case Q1:
            GTEvent3->Qid = 1;
            break;
        case Q2:
            GTEvent3->Qid = 2;
            break;
        case Q3:
            GTEvent3->Qid = 3;
            break;
        case Q4:
            GTEvent3->Qid = 4;
            break;
        case Q5:
            GTEvent3->Qid = 5;
            break;
        case Q6:
            GTEvent3->Qid = 6;
            break;
        case Q7:
            GTEvent3->Qid = 7;
            break;
        case Q8:
            GTEvent3->Qid = 8;
            break;
        case Q9:
            GTEvent3->Qid = 9;
            break;
        case Q10:
            GTEvent3->Qid = 10;
            break;
        case Q11:
            GTEvent3->Qid = 11;
            break;
        case 31:
            GTEvent3->Qid = 31;
            break;
        default:
            break;
    }
    GTEvent3->id = ((DataRecord[i+1] >> 4) & 0xf)*10 + (DataRecord[i+1] & 0xf);

    tempE = (unsigned int)(((((unsigned int)DataRecord[i+4] & 0x00ff) << 16) +
            (unsigned int)((DataRecord[i+3] >> 16) & 0xffff)) << 8);
    GTEvent3->rawE = ((int)tempE)>>8;

    if(GTEvent3->rawE <= 0) GTEvent3->rawE=-GTEvent3->rawE;
    GTEvent3->ehi = (int)((float)GTEvent3->rawE/Mvalue);

    GTEvent3->LEDts  = (unsigned long long int) DataRecord[i+2];
    GTEvent3->LEDts += ((unsigned long long int) DataRecord[i+3] & 0xffff) << 32;
    t1 = (DataRecord[i+8] & 0x000000ff) << 24;
    t2 = (DataRecord[i+8] & 0x0000ff00) << 8;
    t3 = (DataRecord[i+8] & 0x00ff0000) >> 8;
    t4 = (DataRecord[i+8] & 0xff000000) >> 24;
    DataRecord[i+8] = t1 + t2 + t3 + t4;
    GTEvent3->base = (short int)(DataRecord[i+8] & 0xffff);
    k = (unsigned int)((DataRecord[i+1] >> 16) & 0x7ff) + 1;
    return k+i;
}
//////////////////////////////////////////////////////////////////////
int GetGamma(GTEVENT2 *GTEvent2, GAMMA *gamma){
    int nmax=0;
    float emax = 0.0;
    int detectorPosition,crystalNumber;
    float R,r;

    if(GTEvent2->pad != 0) return 0;
    for(int i=0;i<GTEvent2->num;i++)
        if(GTEvent2->intpts[i].e > emax)
            nmax = i;

    detectorPosition = ((GTEvent2->crystal_id & 0xfffc)>>2)-1;
    crystalNumber = (GTEvent2->crystal_id & 0x0003);

    gamma->id = crystalNumber;
    switch(detectorPosition+1){
        case Q1:
            gamma->cc_id = 1;
            break;
        case Q2:
            gamma->cc_id = 2;
            break;
        case Q3:
            gamma->cc_id = 3;
            break;
        case Q4:
            gamma->cc_id = 4;
            break;
        case Q5:
            gamma->cc_id = 5;
            break;
        case Q6:
            gamma->cc_id = 6;
            break;
        case Q7:
            gamma->cc_id = 7;
            break;
        case Q8:
            gamma->cc_id = 8;
            break;
        case Q9:
            gamma->cc_id = 9;
            break;
        case Q10:
            gamma->cc_id = 10;
            break;
        case Q11:
            gamma->cc_id = 11;
            break;
        default:
            break;
    }

    gamma->x = ( (crmat[detectorPosition][crystalNumber][0][0] * GTEvent2->intpts[nmax].x) +
                 (crmat[detectorPosition][crystalNumber][0][1] * GTEvent2->intpts[nmax].y) +
                 (crmat[detectorPosition][crystalNumber][0][2] * GTEvent2->intpts[nmax].z) +
                 (crmat[detectorPosition][crystalNumber][0][3]) );

    gamma->y = ( (crmat[detectorPosition][crystalNumber][1][0] * GTEvent2->intpts[nmax].x) +
                 (crmat[detectorPosition][crystalNumber][1][1] * GTEvent2->intpts[nmax].y) +
                 (crmat[detectorPosition][crystalNumber][1][2] * GTEvent2->intpts[nmax].z) +
                 (crmat[detectorPosition][crystalNumber][1][3]) );

    gamma->z = ( (crmat[detectorPosition][crystalNumber][2][0] * GTEvent2->intpts[nmax].x) +
                 (crmat[detectorPosition][crystalNumber][2][1] * GTEvent2->intpts[nmax].y) +
                 (crmat[detectorPosition][crystalNumber][2][2] * GTEvent2->intpts[nmax].z) +
                 (crmat[detectorPosition][crystalNumber][2][3]) );

    if(gamma->y<0) gamma->y -= GTPosOffsetY1;
    else if(gamma->y>=0) gamma->y -= GTPosOffsetY2;
    gamma->z -= GTPosOffsetZ;

    R = sqrtf( powf(gamma->x,2.0) + powf(gamma->y,2.0) + powf(gamma->z,2.0) );
    r = sqrtf( powf(gamma->x,2.0) + powf(gamma->y,2.0) );
    gamma->theta = acosf(gamma->z/R);
    gamma->phi = acosf(gamma->x/r);
    if(gamma->y < 0) gamma->phi = 2*M_PI - gamma->phi;

    gamma->e = GTEvent2->tot_e;
    gamma->e0 = GTEvent2->tot_e_FixedPickOff_thisEvent;
    gamma->e1 = GTEvent2->tot_e_FixedPickOff_priorEvent1;
    gamma->e2 = GTEvent2->tot_e_FixedPickOff_priorEvent2;

    //gamma->t = (double)GTEvent2->ts + (double)GTEvent2->t0/10.;
    gamma->t = (double)GTEvent2->timestamp;
    gamma->t0 = (double)GTEvent2->t0*2.0;
    //gamma->t0 = (double)GTEvent2->t0;

    return 1;
}
//////////////////////////////////////////////////////////////////////
void ChicoInit(int runid, const char *fn1, const char *fn2){
    int k;
    std::string OneLine;

    std::ifstream THETACALFILE(fn1, ios::in);
    if(!THETACALFILE.is_open()) {
        std::cerr << "Error opening Calibration file ppacTheta.cal! "<< std::endl;
        exit(1);
    }
    getline(THETACALFILE,OneLine);
    getline(THETACALFILE,OneLine);

    for(int i=0;i<PPAC_NUM;i++)
        THETACALFILE >> k >> offTheta[i] >> gainTheta[i] ;
    THETACALFILE.close();

    std::ifstream PHICALFILE(fn2, ios::in);
    if(!PHICALFILE.is_open()) {
        std::cerr << "Error opening Calibration file ppacPhi.cal! "<< std::endl;
        exit(1);
    }
    getline(PHICALFILE,OneLine);
    getline(PHICALFILE,OneLine);
    for(int i=0;i<PPAC_NUM;i++)
        PHICALFILE >> k >> offPhi[i] >> gainPhi[i];
    PHICALFILE.close();

    for(int i=0;i<PPAC_NUM;i++)
        for(int j=0;j<PPAC_NUM;j++)
            anodemap[i][j]=0;

    for(int i=0;i<PPAC_NUM/4;i++){
        anodemap[i][i+PPAC_NUM/4]=1;
        anodemap[i+PPAC_NUM/4][i]=-1;

        anodemap[i][i+PPAC_NUM/4*3]=2;
        anodemap[i+PPAC_NUM/4*3][i]=-2;
    }

    for(int i=PPAC_NUM/4;i<PPAC_NUM/2;i++){
        anodemap[i][i+PPAC_NUM/4]=2;
        anodemap[i+PPAC_NUM/4][i]=-2;
    }
      /*
      0  5  1  //left-right
      5  0 -1
      0 15  2  //front-left -back-right
     15  0 -2
      1  6  1
      6  1 -1
      1 16  2
     16  1 -2
      2  7  1
      7  2 -1
      2 17  2
      */
    for(int i=0;i<PPAC_NUM*4;i++)
        for(int j=0;j<PPAC_NUM*4;j++)
            cathodemap[i][j]=0;

    for(int i=0;i<PPAC_NUM*4;i++)
        for(int j=0;j<PPAC_NUM*4;j++){
            if((i%4)==0 && (j==i+1)){
                cathodemap[i][j]=1;
                cathodemap[j][i]=-1;
            }
            if((i%4)==2 && (j==i+1)){
                cathodemap[i][j]=2;
                cathodemap[j][i]=-2;
            }
        }
  /*
   0 1 1   1 0 -1
   2 3 2   3 2 -2
   4 5 1
   6 7 2
   */
/*=======between run82 to run119 ================*/
    if(runid >=82 && runid<=119) {
        cathodemap[24][25]=-2;
        cathodemap[25][24]=2;
        cathodemap[26][27]=-1;
        cathodemap[27][26]=1;

        cathodemap[28][29]=-2;
        cathodemap[29][28]=2;
        cathodemap[30][31]=-1;
        cathodemap[31][30]=1;

        cathodemap[32][33]=-2;
        cathodemap[33][32]=2;
        cathodemap[34][35]=-1;
        cathodemap[35][34]=1;

        cathodemap[36][37]=-2;
        cathodemap[37][36]=2;
        cathodemap[38][39]=-1;
        cathodemap[39][38]=1;
    }
/*=====================================================
else{
  cathodemap[56][57]=0;
  cathodemap[57][56]=0;
  cathodemap[58][59]=0;
  cathodemap[59][58]=0;

  cathodemap[56][59]=1;
  cathodemap[59][56]=-1;
  cathodemap[58][57]=2;
  cathodemap[57][58]=-2;
}
//=====================================================*/
}
//////////////////////////////////////////////////////////////////////
void InitChicoEvent(CHICOEVENT *chico){
    chico->LEDts=0;
    chico->cathode_tdc_num=0;
    chico->anode_tdc_num=0;
    chico->anode_qdc_num=0;
    for(int i=0;i<128;i++){
        chico->cathode_tdc_val[i]=0;
        chico->cathode_tdc_ch[i]=0;
        chico->anode_tdc_val[i]=0;
        chico->anode_tdc_ch[i]=0;
    }
    for(int i=0;i<32;i++){
        chico->anode_qdc_val[i]=0;
        chico->anode_qdc_ch[i]=0;
    }
    chico->status=0;
    chico->RF=0;
    chico->SINGLE=0;
}
//////////////////////////////////////////////////////////////////////
int GetChicoEvent(unsigned int *DataRecord, CHICOEVENT *ChicoEvent){
  unsigned short int chan=0;
  int val=0,refval=0;
  unsigned int NextInt;
  int seenTrailer=0;
  int i,j,k=0;
  int CH_Counter=0;
  static int multiAnodeTDCNum=0;
  static int multiCathodeTDCNum=0;

  InitChicoEvent(ChicoEvent);

  ChicoEvent->status = (unsigned short int)((DataRecord[0] & 0xffff0000)>>16);
  ChicoEvent->LEDts = (unsigned long long int) (DataRecord[1] & 0xffffffff);
  ChicoEvent->LEDts += ((unsigned long long int) (DataRecord[2] & 0xffff)<<32);
  j = 2;
  NextInt = ((DataRecord[j] & 0xffff0000) >> 16) +  ((DataRecord[j+1] & 0xffff) << 16);
  j++;
  if(NextInt != 0xffffffff){					//Anode_QDC
    //assert(((NextInt & QDCTYPEMASK) >> QDCTYPESHIFT) == QDCHEADER);
    if(((NextInt & QDCTYPEMASK) >> QDCTYPESHIFT)!=QDCHEADER)return 0;
    //assert(((NextInt & QDCGEOMASK) >> QDCGEOSHIFT) == ANODE_E_VSN);
    if(((NextInt & QDCGEOMASK) >> QDCGEOSHIFT) != ANODE_E_VSN)return 0;
    CH_Counter = (NextInt & COUNTMASK) >> COUNTSHIFT;
    k=0;
    for(i=0;i<CH_Counter;i++){
      NextInt = ((DataRecord[j] & 0xffff0000) >> 16) +  ((DataRecord[j+1] & 0xffff) << 16);
      j++;
      //assert(((NextInt & QDCTYPEMASK) >> QDCTYPESHIFT) == DATA);
      if(((NextInt & QDCTYPEMASK) >> QDCTYPESHIFT) != DATA)return 0;
      chan = (unsigned short int)((NextInt & QDCCHANMASK) >> QDCCHANSHIFT);
      val = (NextInt & QDCDATAMASK);
      if(chan<PPAC_NUM && val >0 && k<32){
        ChicoEvent->anode_qdc_ch[k] = chan;
        ChicoEvent->anode_qdc_val[k] = val;
        k++;
      }
    }
    NextInt = ((DataRecord[j] & 0xffff0000) >> 16) +  ((DataRecord[j+1] & 0xffff) << 16);
    j++;
    //assert(((NextInt & QDCTYPEMASK) >> QDCTYPESHIFT) == QDCTRAILER);
    if(((NextInt & QDCTYPEMASK) >> QDCTYPESHIFT) != QDCTRAILER)return 0;
    NextInt = ((DataRecord[j] & 0xffff0000) >> 16) +  ((DataRecord[j+1] & 0xffff) << 16);
    j++;
    if(NextInt != 0xffffffff){
      while (1){
        NextInt = ((DataRecord[j] & 0xffff0000) >> 16) +  ((DataRecord[j+1] & 0xffff) << 16);
        j++;
        if(NextInt == 0xffffffff)break;
      }
    }
  }
  ChicoEvent->anode_qdc_num = k;
  CH_Counter=0;
  chan=0;val=0;
  ChicoEvent->SINGLE = false;
  NextInt = ((DataRecord[j] & 0xffff0000) >> 16) +  ((DataRecord[j+1] & 0xffff) << 16);
  j++;
  if(NextInt != 0xffffffff){					//Anode_TDC
    //assert((NextInt & TDCTYPEMASK) == TDCHEADER);
    if((NextInt & TDCTYPEMASK) != TDCHEADER)return 0;
    //assert((NextInt & TDCGEOMASK) == ANODE_T_VSN);
    if((NextInt & TDCGEOMASK) != ANODE_T_VSN)return 0;
    while(!seenTrailer){
      NextInt = ((DataRecord[j] & 0xffff0000) >> 16) +  ((DataRecord[j+1] & 0xffff) << 16);
      j++;
      switch(NextInt & TDCTYPEMASK){
        case DATA:
          chan = (unsigned short int)((NextInt & TDCCHANMASK) >> TDCCHANSHIFT);
          val =(NextInt & TDCDATAMASK);
          if(chan != ANODE_REFCH && chan != RFCH && chan != SingleFlag){
            if(chan<PPAC_NUM && CH_Counter<128){
              ChicoEvent->anode_tdc_ch[CH_Counter] = chan;
              ChicoEvent->anode_tdc_val[CH_Counter] =val;
              CH_Counter++;
            }
          }
          else if (chan == RFCH){
            ChicoEvent->RF = val;
          }
          else if (chan == SingleFlag){
            ChicoEvent->SINGLE = true;
          }
          else if(chan == ANODE_REFCH){
            refval = (NextInt & TDCDATAMASK);
          }
          break;
        case TDCTRAILER:
          seenTrailer = 1;
          break;
        default:
          break;
      }
    }
    //if(refval > 0){
      for(i=0;i<CH_Counter;i++){
        ChicoEvent->anode_tdc_val[i] -= refval;//subtract TDC reference.
      }
      //ChicoEvent->RF -= refval;
    //}
    NextInt = ((DataRecord[j] & 0xffff0000) >> 16) +  ((DataRecord[j+1] & 0xffff) << 16);
    j++;
    if(NextInt != 0xffffffff) {
      multiAnodeTDCNum++;
      if((multiAnodeTDCNum%100000)==0)printf("*Warning* anode TDC package with multiple events: %i\n",multiAnodeTDCNum);
      while (1){
        NextInt = ((DataRecord[j] & 0xffff0000) >> 16) +  ((DataRecord[j+1] & 0xffff) << 16);
        j++;
        if(NextInt == 0xffffffff)break;
      }
    }
  }
  ChicoEvent->anode_tdc_num = CH_Counter;
  CH_Counter=0; seenTrailer=0;
  chan=0;val=0;
  NextInt = ((DataRecord[j] & 0xffff0000) >> 16) +  ((DataRecord[j+1] & 0xffff) << 16);
  j++;
  if(NextInt != 0xffffffff){					//Cathode_TDC
    //assert((NextInt & TDCTYPEMASK) == TDCHEADER);
    if((NextInt & TDCTYPEMASK) != TDCHEADER)return 0;
    //assert((NextInt & TDCGEOMASK) == CATHODE_T_VSN);
    if((NextInt & TDCGEOMASK) != CATHODE_T_VSN)return 0;
    while(!seenTrailer){
      NextInt = ((DataRecord[j] & 0xffff0000) >> 16) +  ((DataRecord[j+1] & 0xffff) << 16);
      j++;
      switch(NextInt & TDCTYPEMASK){
        case DATA:
          chan = (unsigned short int)((NextInt & TDCCHANMASK) >> TDCCHANSHIFT);
          val =(NextInt & TDCDATAMASK);
          if(chan != CATHODE_REFCH){
            if(chan < PPAC_NUM*4 && CH_Counter<128){
              ChicoEvent->cathode_tdc_ch[CH_Counter] = chan;
              ChicoEvent->cathode_tdc_val[CH_Counter] = val;
              CH_Counter++;
            }
          }
          else if(chan == CATHODE_REFCH){
            refval = (NextInt & TDCDATAMASK);
          }
          break;
        case TDCTRAILER:
          seenTrailer = 1;
          break;
        default:
          break;
      }
    }
    //if(refval > 0){
      //for(i=0;i<CH_Counter;i++){
      //  ChicoEvent->cathode_tdc_val[i] -= refval;
      //}
    //}
    NextInt = ((DataRecord[j] & 0xffff0000) >> 16) +  ((DataRecord[j+1] & 0xffff) << 16);
    j++;
    if(NextInt != 0xffffffff) {
      multiCathodeTDCNum++;
      if((multiCathodeTDCNum%100000)==0)printf("*Warning* cathode TDC package with multiple events: %i\n",multiCathodeTDCNum);
      while (1){
        NextInt = ((DataRecord[j] & 0xffff0000) >> 16) +  ((DataRecord[j+1] & 0xffff) << 16);
        j++;
        if(NextInt == 0xffffffff)break;
      }
    }
  }
  ChicoEvent->cathode_tdc_num = CH_Counter;
  return 1;
}
//////////////////////////////////////////////////////////////////////
int GetParticle(CHICOEVENT *ChicoEvent, PARTICLE *particle){
  static float d = (float) (RAND_MAX) + 1.0;
  int i,j,l;
  int dT=0;
  int anode[2]={160};
  //int valid = 0;
  int validTheta=0,validPhi=0;
  int validT[2] = {0};
  int validP[2] = {0};
  int theta[2]={0};
  int phi[2]={-500,-500}; // by ZL
  float ftheta[2]={0.};
  float fphi[2]={0.};
  float dL,dR;
  unsigned short int temp1=0,temp2=0;
  int vtemp1=0,vtemp2=0,doubleOK;
  int vanodemap=0,vcathodemap=0;
  int cathodesign;

  doubleOK = 0;
  if(ChicoEvent->anode_tdc_num >=2){
    for(i=0;i<ChicoEvent->anode_tdc_num-1;i++){
      for(j=i+1;j<ChicoEvent->anode_tdc_num;j++){
        vanodemap = anodemap[ChicoEvent->anode_tdc_ch[i]][ChicoEvent->anode_tdc_ch[j]];
        if(vanodemap!=0){
          temp1 = ChicoEvent->anode_tdc_ch[i];
          temp2 = ChicoEvent->anode_tdc_ch[j];
          vtemp1 = ChicoEvent->anode_tdc_val[i];
          vtemp2 = ChicoEvent->anode_tdc_val[j];
          doubleOK = 1;
        }
      }
    }
  }

  if(doubleOK == 1){
    ChicoEvent->anode_tdc_num = 2;
    ChicoEvent->anode_tdc_ch[0] = temp1;
    ChicoEvent->anode_tdc_ch[1] = temp2;
    ChicoEvent->anode_tdc_val[0] = vtemp1;
    ChicoEvent->anode_tdc_val[1] = vtemp2;
  }
  //anode[i]: ppac id, 0-left, 1-right
  //theta[i]: theta,
  //phi[i]: phi
  if(ChicoEvent->anode_tdc_num == 2){
    vanodemap = anodemap[ChicoEvent->anode_tdc_ch[0]][ChicoEvent->anode_tdc_ch[1]];
    if(vanodemap!=0){
      for(i=0;i<2;i++){// vanodemap  left-right 1, left-back 2,
        //anode[i] = ChicoEvent->anode_tdc_ch[(((i-vanodemap)+1)/2+i)%2];
        anode[i] = ChicoEvent->anode_tdc_ch[((vanodemap+1)/2==i)];      //anode chan:0: left; 1: right
      }
      dT = (ChicoEvent->anode_tdc_val[0]-ChicoEvent->anode_tdc_val[1])*vanodemap;  //Tleft-Tright
      for(i=0;i<ChicoEvent->cathode_tdc_num-1;i++){
        for(j=i+1;j<ChicoEvent->cathode_tdc_num;j++){
          vcathodemap=cathodemap[ChicoEvent->cathode_tdc_ch[i]][ChicoEvent->cathode_tdc_ch[j]];
          if(vcathodemap!=0){
            cathodesign=vcathodemap/abs(vcathodemap);
            for(l=0;l<2;l++){//left, right
              if(ChicoEvent->cathode_tdc_ch[i]/4==anode[l]){
                if(abs(vcathodemap)==1){
                  theta[l]=(ChicoEvent->cathode_tdc_val[i]-ChicoEvent->cathode_tdc_val[j])*cathodesign;
                  ftheta[l] = gainTheta[anode[l]]*(float)theta[l];
                  ftheta[l] += offTheta[anode[l]];
                  ftheta[l] += (float)rand()/d-0.5;
                  validT[l]=1;
                }
                if(abs(vcathodemap)==2){
                  phi[l]=(ChicoEvent->cathode_tdc_val[i]-ChicoEvent->cathode_tdc_val[j])*cathodesign;
                  fphi[l] = gainPhi[anode[l]]*(float)phi[l];
                  fphi[l] += offPhi[anode[l]];
                  fphi[l] += 36.0*(float)(anode[l]%10);       //10: back and front number of ppac
                  fphi[l] += (float)rand()/d-0.5;
                  validP[l]=1;
                }
              }
            }
          }
        }
      }
    }
  }

  //if((validT[0]*validP[0]*validT[1]*validP[1])!=0)valid = 1;
  if(validT[0] == 1 && validT[1] ==1)validTheta = 1;
  if((validP[0]+validP[1]) >= 1)validPhi = 1;
  //if((validP[0]*validP[1]) == 1)validPhi = 1;
  if(validTheta==1 && validPhi==1){
    particle->t= (double)ChicoEvent->LEDts;
    particle->rf = ((double)ChicoEvent->RF * ch2ns)*0.1 + (double)rand()/d-0.5;
    particle->id=anode[0];
    particle->dT=dT;
    particle->thetaL=theta[0];
    particle->thetaR=theta[1];
    particle->fthetaL=ftheta[0]*M_PI/180.;
    particle->fthetaR=ftheta[1]*M_PI/180.;
    particle->phiL=phi[0];
    particle->phiR=phi[1];
    if(validP[0]==1 && validP[1] == 0){
      if(fphi[0]<180.){
        particle->fphiL = fphi[0]*M_PI/180.;
        particle->fphiR = fphi[0]*M_PI/180. + M_PI;
      }
      else if(fphi[0]>=180.){
        particle->fphiL = fphi[0]*M_PI/180.;
        particle->fphiR = fphi[0]*M_PI/180. - M_PI;
      }
    }
    else if(validP[0]==0 && validP[1] == 1){
      if(fphi[1]<180.){
        particle->fphiR = fphi[1]*M_PI/180.;
        particle->fphiL = fphi[1]*M_PI/180. + M_PI;
      }
      else if(fphi[1]>=180.){
        particle->fphiR = fphi[1]*M_PI/180.;
        particle->fphiL = fphi[1]*M_PI/180. - M_PI;
      }
    }
    else if(validP[0]==1 && validP[1] == 1){
      if(fphi[0]<180.){
        particle->fphiL=(fphi[0]+fphi[1]-180.)*0.5*M_PI/180.;
        particle->fphiR=(fphi[0]+fphi[1]+180.)*0.5*M_PI/180.;
      }
      else if(fphi[0]>=180.){
        particle->fphiL=(fphi[0]+fphi[1]+180.)*0.5*M_PI/180.;
        particle->fphiR=(fphi[1]+fphi[0]-180.)*0.5*M_PI/180.;
      }
    }
    dL = 12.8/(0.75471*sinf(particle->fthetaL)*cosf(particle->fphiL-(18.0+(float)(anode[0]%10)*36.0)*M_PI/180.)
        +0.65606*cosf(particle->fthetaL));
    dR = 12.8/(0.75471*sinf(particle->fthetaR)*cosf(particle->fphiR-(18.0+(float)(anode[1]%10)*36.0)*M_PI/180.)
        +0.65606*cosf(particle->fthetaR));
    particle->dL = dL;
    particle->dR = dR;
    particle->single = ChicoEvent->SINGLE;
    particle->back = false;
    return 1;
  }
  else{
    return 0;
  }
}
//////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    unsigned int *EventBuf=NULL;
    GEBheader Header;

    if (argc!=2) {
        printf("USAGE: ./ConvertData Runid \n");
        exit(EXIT_FAILURE);
    }

    int runid = atoi(argv[1]);
    printf("converting tracked data %04d ......",runid);

    const char *FileName = Form("/data/d3/DataZhu/TRACKED_DATA/data%04d.dat",runid);
    fp1 = fopen64(FileName,"rb");
    int status;
    if(!fp1){
        status = errno;
        printf(" Unable to open file %s: %s\n",FileName ,strerror(status));
        exit(EXIT_FAILURE);
    }

    TFile *fChico=new TFile(Form("../CHICO_ROOT/Chico%04d.root",runid),"RECREATE");
    fChico->cd();
    TTree *treeChico=new TTree("tree","tree");
    setbranchChico(treeChico);

    TFile *fGT=new TFile(Form("../GAMMAGT_ROOT/GT%04d.root",runid),"RECREATE");
    fGT->cd();
    TTree *treeGT=new TTree("tree","tree");
    setbranchGT(treeGT);

    TFile *fTK=new TFile(Form("../GAMMATK_ROOT/TK%04d.root",runid),"RECREATE");
    fTK->cd();
    TTree *treeTK=new TTree("tree","tree");
    setbranchTK(treeTK);

    setupcut("CAL_DATA/ChicoCut");
    RdCal("CAL_DATA/CC.cal","CAL_DATA/gcidOff.txt","CAL_DATA/GfidOff.txt");
    RdCRMAT("CAL_DATA/crmat.dat");
    ChicoInit(runid,"CAL_DATA/ppacTheta.cal","CAL_DATA/ppacPhi.cal");

    //main loop
    while((EventBuf=GetGEBEvBuf(&Header,FileName))!=NULL){
        if(Header.type == MODETWO){  //GTEvent
            assert(Header.length == sizeof(GTEVENT2));
            memcpy(&GTEvent2,EventBuf,Header.length);
            if(GetGamma(&GTEvent2,&gammagt)!=0){  //gamma
                cidGT = gammagt.id+(gammagt.cc_id-1)*4;
                gammagt.t -= (int)gcidoff[cidGT];
                gammagt.e = gammagt.e*gainGT[cidGT] + offGT[cidGT];
                gammagt.e0 = gammagt.e0*gainGT2[cidGT] + offGT2[cidGT];
                gammagt.e1 = gammagt.e1*gainGT2[cidGT] + offGT2[cidGT];
                gammagt.e2 = gammagt.e2*gainGT2[cidGT] + offGT2[cidGT];
                treeGT->Fill();
            }
        }
        else if(Header.type == MODEONE){  //TKEvent
            memcpy(&GTEvent1,EventBuf,Header.length);
            for(int i=0;i<GTEvent1.ngam;i++){
                gammatk = GTEvent1.gr[i];
                gammatk.timestamp -= (long long int)gtkfidoff[gammatk.fhcrID];
                float R = sqrtf( powf(gammatk.x0,2.0) + powf(gammatk.y0,2.0) + powf(gammatk.z0,2.0) );
                float r = sqrtf( powf(gammatk.x0,2.0) + powf(gammatk.y0,2.0) );
                thetaTK = acosf(gammatk.z0/R);
                phiTK = acosf(gammatk.x0/r);
                if(gammatk.y0<0) phiTK = 2*M_PI - phiTK;
                treeTK->Fill();
            }
        }
        else if(Header.type == CHICOTYPE){  //ChicoEvent
            if(GetChicoEvent(EventBuf,&ChicoEvent)==1){
                if(GetParticle(&ChicoEvent,&particle)==1){  //particle
                    particle.t *= 10;     //convert to ns
                    treeChico->Fill();
                }
            }
        }
        free(EventBuf);
    }
    fclose(fp1);

    fChico->cd();
    treeChico->Write();
    fChico->Close();

    fGT->cd();
    treeGT->Write();
    fGT->Close();

    fTK->cd();
    treeTK->Write();
    fTK->Close();

    printf("Done! Output files in ../CHICO_ROOT/Chico%04d.root ../GAMMAGT_ROOT/GT%04d.root ../GAMMATK_ROOT/TK%04d.root\n",runid,runid,runid);

    exit(0);
}
