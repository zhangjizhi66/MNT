//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Jan  3 11:35:59 2021 by ROOT version 6.16/00
// from TTree tree/tree
// found on file: ../ANA_ROOT/g0090.root
//////////////////////////////////////////////////////////

#ifndef gg_h
#define gg_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class gg {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           id;
   ULong64_t       ts;
   Float_t         thetaL;
   Float_t         thetaR;
   Float_t         phiL;
   Float_t         phiR;
   Float_t         dL;
   Float_t         dR;
   Float_t         dT;
   Float_t         dphi;
   Float_t         MassT;
   Float_t         MassP;
   Float_t         BetaT;
   Float_t         BetaP;
   Float_t         MassL;
   Float_t         MassR;
   Float_t         BetaL;
   Float_t         BetaR;
   Float_t         Qval1;
   Float_t         Qval2;
   Float_t         thetaT;
   Float_t         thetaP;
   Float_t         phiT;
   Float_t         phiP;
   Int_t           ngam;
   Float_t         ge[330];   //[ngam]
   Float_t         ge0[330];   //[ngam]
   Float_t         ge1[330];   //[ngam]
   Float_t         ge2[330];   //[ngam]
   Float_t         gtheta[330];   //[ngam]
   Float_t         gphi[330];   //[ngam]
   Float_t         gdt[330];   //[ngam]
   Float_t         gx[330];   //[ngam]
   Float_t         gy[330];   //[ngam]
   Float_t         gz[330];   //[ngam]
   Int_t           gcid[330];   //[ngam]
   Float_t         gccosP[330];   //[ngam]
   Float_t         geP[330];   //[ngam]
   Float_t         gccosT[330];   //[ngam]
   Float_t         geT[330];   //[ngam]
   Int_t           nGam;
   Int_t           Gndet[270];   //[nGam]
   Float_t         Ge[270];   //[nGam]
   Float_t         Gfom[270];   //[nGam]
   Float_t         Gtheta[270];   //[nGam]
   Float_t         Gphi[270];   //[nGam]
   Float_t         Gdt[270];   //[nGam]
   Long64_t        Gts[270];   //[nGam]
   Float_t         Gx0[270];   //[nGam]
   Float_t         Gy0[270];   //[nGam]
   Float_t         Gz0[270];   //[nGam]
   Float_t         Gx1[270];   //[nGam]
   Float_t         Gy1[270];   //[nGam]
   Float_t         Gz1[270];   //[nGam]
   Float_t         Ge0[270];   //[nGam]
   Float_t         Ge1[270];   //[nGam]
   Int_t           Gfid[270];   //[nGam]
   Float_t         GccosP[270];   //[nGam]
   Float_t         GeP[270];   //[nGam]
   Float_t         GccosT[270];   //[nGam]
   Float_t         GeT[270];   //[nGam]

   // List of branches
   TBranch        *b_id;   //!
   TBranch        *b_ts;   //!
   TBranch        *b_thetaL;   //!
   TBranch        *b_thetaR;   //!
   TBranch        *b_phiL;   //!
   TBranch        *b_phiR;   //!
   TBranch        *b_dL;   //!
   TBranch        *b_dR;   //!
   TBranch        *b_dT;   //!
   TBranch        *b_dphi;   //!
   TBranch        *b_MassT;   //!
   TBranch        *b_MassP;   //!
   TBranch        *b_BetaT;   //!
   TBranch        *b_BetaP;   //!
   TBranch        *b_MassL;   //!
   TBranch        *b_MassR;   //!
   TBranch        *b_BetaL;   //!
   TBranch        *b_BetaR;   //!
   TBranch        *b_Qval1;   //!
   TBranch        *b_Qval2;   //!
   TBranch        *b_thetaT;   //!
   TBranch        *b_thetaP;   //!
   TBranch        *b_phiT;   //!
   TBranch        *b_phiP;   //!
   TBranch        *b_ngam;   //!
   TBranch        *b_ge;   //!
   TBranch        *b_ge0;   //!
   TBranch        *b_ge1;   //!
   TBranch        *b_ge2;   //!
   TBranch        *b_gtheta;   //!
   TBranch        *b_gphi;   //!
   TBranch        *b_gdt;   //!
   TBranch        *b_gx;   //!
   TBranch        *b_gy;   //!
   TBranch        *b_gz;   //!
   TBranch        *b_gcid;   //!
   TBranch        *b_gccosP;   //!
   TBranch        *b_geP;   //!
   TBranch        *b_gccosT;   //!
   TBranch        *b_geT;   //!
   TBranch        *b_nGam;   //!
   TBranch        *b_Gndet;   //!
   TBranch        *b_Ge;   //!
   TBranch        *b_Gfom;   //!
   TBranch        *b_Gtheta;   //!
   TBranch        *b_Gphi;   //!
   TBranch        *b_Gdt;   //!
   TBranch        *b_Gts;   //!
   TBranch        *b_Gx0;   //!
   TBranch        *b_Gy0;   //!
   TBranch        *b_Gz0;   //!
   TBranch        *b_Gx1;   //!
   TBranch        *b_Gy1;   //!
   TBranch        *b_Gz1;   //!
   TBranch        *b_Ge0;   //!
   TBranch        *b_Ge1;   //!
   TBranch        *b_Gfid;   //!
   TBranch        *b_GccosP;   //!
   TBranch        *b_GeP;   //!
   TBranch        *b_GccosT;   //!
   TBranch        *b_GeT;   //!
   int run1,run2;

   gg(int runa,int runb, TTree *tree=0);
   virtual ~gg();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef gg_cxx
gg::gg(int runa,int runb,  TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Memory Directory");
      if (!f || !f->IsOpen()) {
         f = new TFile("Memory Directory");
      }
      f->GetObject("tree",tree);

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain *chain = new TChain("tree","");
      run1=runa;
      run2=runb;
      for(int i=run1;i<=run2;i++) {//220
	TString sfile=Form("../../ANA_ROOT/g%04d.root",i);
	if(!gSystem->AccessPathName(sfile)) {
	  cout<<sfile.Data()<<endl;
	  chain->Add(Form("%s/tree",sfile.Data()));
    }
  }

      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

gg::~gg()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t gg::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t gg::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void gg::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("id", &id, &b_id);
   fChain->SetBranchAddress("ts", &ts, &b_ts);
   fChain->SetBranchAddress("thetaL", &thetaL, &b_thetaL);
   fChain->SetBranchAddress("thetaR", &thetaR, &b_thetaR);
   fChain->SetBranchAddress("phiL", &phiL, &b_phiL);
   fChain->SetBranchAddress("phiR", &phiR, &b_phiR);
   fChain->SetBranchAddress("dL", &dL, &b_dL);
   fChain->SetBranchAddress("dR", &dR, &b_dR);
   fChain->SetBranchAddress("dT", &dT, &b_dT);
   fChain->SetBranchAddress("dphi", &dphi, &b_dphi);
   fChain->SetBranchAddress("MassT", &MassT, &b_MassT);
   fChain->SetBranchAddress("MassP", &MassP, &b_MassP);
   fChain->SetBranchAddress("BetaT", &BetaT, &b_BetaT);
   fChain->SetBranchAddress("BetaP", &BetaP, &b_BetaP);
   fChain->SetBranchAddress("MassL", &MassL, &b_MassL);
   fChain->SetBranchAddress("MassR", &MassR, &b_MassR);
   fChain->SetBranchAddress("BetaL", &BetaL, &b_BetaL);
   fChain->SetBranchAddress("BetaR", &BetaR, &b_BetaR);
   fChain->SetBranchAddress("Qval1", &Qval1, &b_Qval1);
   fChain->SetBranchAddress("Qval2", &Qval2, &b_Qval2);
   fChain->SetBranchAddress("thetaT", &thetaT, &b_thetaT);
   fChain->SetBranchAddress("thetaP", &thetaP, &b_thetaP);
   fChain->SetBranchAddress("phiT", &phiT, &b_phiT);
   fChain->SetBranchAddress("phiP", &phiP, &b_phiP);
   fChain->SetBranchAddress("ngam", &ngam, &b_ngam);
   fChain->SetBranchAddress("ge", ge, &b_ge);
   fChain->SetBranchAddress("ge0", ge0, &b_ge0);
   fChain->SetBranchAddress("ge1", ge1, &b_ge1);
   fChain->SetBranchAddress("ge2", ge2, &b_ge2);
   fChain->SetBranchAddress("gtheta", gtheta, &b_gtheta);
   fChain->SetBranchAddress("gphi", gphi, &b_gphi);
   fChain->SetBranchAddress("gdt", gdt, &b_gdt);
   fChain->SetBranchAddress("gx", gx, &b_gx);
   fChain->SetBranchAddress("gy", gy, &b_gy);
   fChain->SetBranchAddress("gz", gz, &b_gz);
   fChain->SetBranchAddress("gcid", gcid, &b_gcid);
   fChain->SetBranchAddress("gccosP", gccosP, &b_gccosP);
   fChain->SetBranchAddress("geP", geP, &b_geP);
   fChain->SetBranchAddress("gccosT", gccosT, &b_gccosT);
   fChain->SetBranchAddress("geT", geT, &b_geT);
   fChain->SetBranchAddress("nGam", &nGam, &b_nGam);
   fChain->SetBranchAddress("Gndet", Gndet, &b_Gndet);
   fChain->SetBranchAddress("Ge", Ge, &b_Ge);
   fChain->SetBranchAddress("Gfom", Gfom, &b_Gfom);
   fChain->SetBranchAddress("Gtheta", Gtheta, &b_Gtheta);
   fChain->SetBranchAddress("Gphi", Gphi, &b_Gphi);
   fChain->SetBranchAddress("Gdt", Gdt, &b_Gdt);
   fChain->SetBranchAddress("Gts", Gts, &b_Gts);
   fChain->SetBranchAddress("Gx0", Gx0, &b_Gx0);
   fChain->SetBranchAddress("Gy0", Gy0, &b_Gy0);
   fChain->SetBranchAddress("Gz0", Gz0, &b_Gz0);
   fChain->SetBranchAddress("Gx1", Gx1, &b_Gx1);
   fChain->SetBranchAddress("Gy1", Gy1, &b_Gy1);
   fChain->SetBranchAddress("Gz1", Gz1, &b_Gz1);
   fChain->SetBranchAddress("Ge0", Ge0, &b_Ge0);
   fChain->SetBranchAddress("Ge1", Ge1, &b_Ge1);
   fChain->SetBranchAddress("Gfid", Gfid, &b_Gfid);
   fChain->SetBranchAddress("GccosP", GccosP, &b_GccosP);
   fChain->SetBranchAddress("GeP", GeP, &b_GeP);
   fChain->SetBranchAddress("GccosT", GccosT, &b_GccosT);
   fChain->SetBranchAddress("GeT", GeT, &b_GeT);
   Notify();
}

Bool_t gg::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void gg::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t gg::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef gg_cxx
