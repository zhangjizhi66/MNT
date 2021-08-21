// subtract background for gamma matrix using radware method

// return: background subtracted matrix TH2D

// parameters:
// TH2* hgg: input gamma matrix
// double smooth: smoothness of background of total projection spectrum

// note: input matrix should be symmetric

TH2D* bgsub_radware(TH2* hgg, double smooth=8){
    int nbins = hgg->GetXaxis()->GetNbins();
    double emax = hgg->GetXaxis()->GetXmax();
    double emin = hgg->GetXaxis()->GetXmin();
    
    TH1D *hg = hgg->ProjectionX("hg");//一维投影谱

    TH1D *hbg;//一维本底谱
    TSpectrum *sa = new TSpectrum(200);
    hbg = (TH1D*)sa->Background(hg,smooth,"nosmoothing same");

    TH1D *hpeak = new TH1D("sbg","background subtracted",nbins,emin,emax);//一维减本底谱
    hpeak->Add(hg,hbg,1,-1);//peaks 

    TH2D* hggb = new TH2D("bgg","background matrix",nbins,emin,emax,nbins,emin,emax);//二维本底谱
    Double_t Pi,Pj,pi,pj,Bij,x,y;
    Double_t T = hg->Integral();
    for(int i=0; i<hgg->GetNbinsX(); i++)
        for(int j=0; j<hgg->GetNbinsY(); j++){
            Pi = hg->GetBinContent(i+1);
            Pj = hg->GetBinContent(j+1);
            pi = hpeak->GetBinContent(i+1);
            pj = hpeak->GetBinContent(j+1);
            Bij = (Pi*Pj-pi*pj)/T;
            x = hg->GetBinCenter(i+1);
            y = hg->GetBinCenter(j+1);
            hggb->Fill(x,y,Bij);
        }

    TH2D *hggsub = new TH2D("sbgg","radware sub matrix",nbins,emin,emax,nbins,emin,emax);
    hggsub->Add(hgg,hggb,1,-1);
    return hggsub;
}