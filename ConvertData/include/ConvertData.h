/* $Id: GTSort.h,v 1.1 2010/05/19 16:22:06 tl Exp $ */
#define TRUE 1
#define FALSE 0
#define NOTDEF -1
#define STRLEN 256

#define MaxChNum 40
#define MaxGamNum 160
#define EOE 0xaaaaaaaa
#define GEBHDRLENBYTES 16
#define HDRLENBYTES  28
#define HDRLENWORDS  14
#define HDRLENINTS    7
#define MAXLENINTS  519
#define LENEOVWORDS  2

#define MAXDETPOS 30
#define MAXCRYSTALNO 3

#define QNum 11

#define Q1 6
#define Q2 7
#define Q3 22
#define Q4 14
#define Q5 26
#define Q6 17
#define Q7 19
#define Q8 29
#define Q9 24
#define Q10 25
#define Q11 21

#define ANODE_E_VSN 1
#define ANODE_T_VSN 2
#define CATHODE_T_VSN 3

#define DATA 0x00000000
#define INVALID 6
#define COUNTMASK 0x3f00
#define COUNTSHIFT 8

#define QDCTYPESHIFT 24
#define QDCGEOSHIFT 27
#define QDCHEADER 2
#define QDCTYPEMASK 0x7000000
#define QDCTRAILER 4
#define QDCGEOMASK 0xf8000000
#define QDCCHANMASK 0x3f0000
#define QDCCHANSHIFT 16
#define QDCDATAMASK 0x0fff

#define TDCTYPEMASK 0xf8000000
#define TDCGEOMASK 0x0000001f
#define TDCCHANMASK 0x03f80000
#define TDCCHANSHIFT 19
#define TDCDATAMASK 0x0007ffff
#define TDCHEADER 0x40000000
#define TDCTRAILER 0x80000000

#define ANODE_REFCH 24
#define CATHODE_REFCH 80
#define RFCH 25
#define SingleFlag 31
#define PHYSICSEv 30

#define CHICOTYPE 12
#define MODEONE 3
#define MODETWO 1
#define MODETHREE 2
#define BANK29 8

#define MaxGTNum 42
#define MaxChicoNum 3

typedef struct GEBheader_struct
{
  unsigned int           type;
  unsigned int           length; /* length of payload following the header, in bytes */
  unsigned long long int timestamp;
} GEBheader;

typedef struct CHICOEVENT_struct
{
  unsigned long long int  LEDts;
  unsigned short int      cathode_tdc_num;
  int                     cathode_tdc_val[128];
  unsigned short int      cathode_tdc_ch[128];
  unsigned short int      anode_tdc_num;
  int                     anode_tdc_val[128];
  unsigned short int      anode_tdc_ch[128];
  unsigned short int      anode_qdc_num;
  int                     anode_qdc_val[32];
  unsigned short int      anode_qdc_ch[32];
  unsigned int            status;
  int			  RF;
  bool			  SINGLE;
} CHICOEVENT;

typedef struct PARTICLE_struct
{
  int   id;
  double t;
  int    dT;  //left - right
  float  dL;
  float  dR;
  int  thetaL;
  int  phiL;
  int  thetaR;
  int  phiR;
  float  fthetaL;
  float  fphiL;
  float  fthetaR;
  float  fphiR;
  int    eL;
  int    eR;
  double rf;
  bool single;
  float Mass;
  bool back;
} PARTICLE;

#define MAX_INTPTS 16
//#define MAX_INTPTS 24

typedef struct intpts_struct
{
  float x,y,z,e; /* here e refers to the fraction */
  int seg; /* segment number hit */
  float seg_ener; /* energy of the hit segment */
} INTPTS;

typedef struct crys_intpts {
	int type; /* as of June 2012: abcd5678 */
	int crystal_id;
	int num; /* # of interaction points from decomp, or # of nets on decomp error */
	float tot_e; /* CC energy for the central contact selected for use in decomp (calibrated, and for 10 MeV CC channels, includes DNL correction. */
	int core_e[4]; /* 4 raw core energies from FPGA filter (uncalibrated) */
	long long int timestamp;
	float tot_e_FixedPickOff_priorEvent1;
	float tot_e_FixedPickOff_priorEvent2;
	float t0;
	unsigned short int deltaT_priorEvent1;
	unsigned short int deltaT_priorEvent2;
	float chisq;
	float norm_chisq;
	float tot_e_FixedPickOff_thisEvent;
	//FixedPickoff energies will be caliberated by CC gain, no offset
	float prestep; /* avg trace value before step (baseline) */
	float poststep; /* avg trace value after step (flat-top) */
	int pad; /* non-0 with a decomp error, value gives error type */
        INTPTS intpts[MAX_INTPTS];
} GTEVENT2;

typedef struct GTEVENT3_struct
{
    unsigned long long int  LEDts;
    int Qid;
    int id;
    short int base;
    int rawE;
    int ehi;
} GTEVENT3;

typedef struct GAMMA_struct
{
  float x,y,z,e,e0,e1,e2;
  float theta,phi;
  double t,t0;
  int cc_id,id;
} GAMMA;

/* structure for the tracked gamma rays */
/* written to the output with geb ID GEB_TYPE_TRACK==3 */

typedef struct TRACKED_GAMMA_RAY {
    float esum;              /* gamma ray energy */
    int ndet;                /* number of interactions */
    float fom;               /* figure of merit */
    int   tracked;           /* 1==if tracked */
    long long int timestamp; /* timestap of first interaction point */
    float x0, y0, z0, e0;    /* first interaction point */
    float x1, y1, z1, e1;    /* second interaction point */
    short int fhcrID;          /* first hit crystal ID */
} TRACKED_GAMMA_RAY;

typedef struct TRACKED_GAMMA_HIT {
      int ngam;
      int pad;// 0-valid decomposition
      TRACKED_GAMMA_RAY gr[MaxGTNum];
} TRACKED_GAMMA_HIT;
