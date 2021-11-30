#include "TH3.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"

#include <fstream>
#include <iostream>

#define hfile   "../ROOT/Gddmat2.root"  // input ROOT file name
#define hname   "hg2xy"                 // histogram name in ROOT file
#define m4bfile "Gddmat2.m4b"           // output m4b file name

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int root2m4b()
{
  TFile *file = new TFile(hfile,"READ");//"RECREATE" "READ"

  TH2I *h = (TH2I*)file->Get(hname);
  h->Print();

  ofstream writefile;//fstream
  writefile.open(m4bfile,std::ios::binary);//ios::bin ios::app

  double raw;
  int data;
   for (int i = 0; i < 4096; ++i)
    for (int j = 0; j < 4096; ++j)
      {
	raw = h->GetBinContent(i+1, j+1);
	data = (int) raw;
	writefile.write((char*)&data,sizeof(int));
      }
   
  writefile.close();
  file->Close();
  return 0;
}
// 
// root2m4b.cc ends here
