
void mass_theta_mat()
{
    TH2F *htm = new TH2F("htm","theta:mass",350,0,350,400,10,90);
    
    TChain *tree = new TChain("tree");
    std::ifstream ifs("../validID.txt");
    int runid;
    while(ifs>>runid)
        tree->Add(Form("../CHICO_ROOT/Chico%04d.root",runid));
        
    Int_t dT;
    Float_t dL,dR,thetaL,thetaR;
        
    tree->SetBranchAddress("dT",&dT);  // tL-tR
    tree->SetBranchAddress("dL",&dL);
    tree->SetBranchAddress("dR",&dR);
    tree->SetBranchAddress("thetaL",&thetaL);
    tree->SetBranchAddress("thetaR",&thetaR);
        
    int nentries = tree->GetEntries();
    for (int jentry=0; jentry<nentries; jentry++){
        tree->GetEntry(jentry);
        float pL = 16602.2*sinf(thetaR)/sinf(thetaL+thetaR);
        float pR = 16602.2*sinf(thetaL)/sinf(thetaL+thetaR);
        float massR = (-0.0032206*dT + 334.*dL/pL)/(dR/pR+dL/pL);
        float massL = ( 0.0032206*dT + 334.*dR/pR)/(dR/pR+dL/pL);

        htm->Fill(massL,thetaL/M_PI*180);
        htm->Fill(massR,thetaR/M_PI*180);
            
        // display progress and time needed
        if (jentry%500000 == 499999){
            printf("Process %.3f %%  \r",double(jentry)/double(nentries)*100.);
            fflush(stdout);
        }
    }
    printf("Process 100.000 %%\n");
    
    TFile *opf = new TFile("mass_theta_mat.root","RECREATE");
    opf->cd();
    htm->Write();
    opf->Close();
    
    printf("Done!\n");
}