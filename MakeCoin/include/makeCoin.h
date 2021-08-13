
struct PARTICLE{
   Int_t           id;
   ULong64_t       ts;
   Int_t           dT;
   Float_t         dL;
   Float_t         dR;
   Float_t         thetaL;
   Float_t         thetaR;
   Float_t         phiL;
   Float_t         phiR;
};

struct GAMMAGT{
   Float_t         x;
   Float_t         y;
   Float_t         z;
   Float_t         e;
   Float_t         theta;
   Float_t         phi;
   ULong64_t       ts;
   Int_t           cid;
};

struct GAMMATK{
   Float_t         e;
   Int_t           ndet;
   Float_t         fom;
   ULong64_t       ts;
   Float_t         x0;
   Float_t         y0;
   Float_t         z0;
   Float_t         e0;
   Float_t         x1;
   Float_t         y1;
   Float_t         z1;
   Float_t         e1;
   Int_t           fid;
   Float_t         theta;
   Float_t         phi;
};
