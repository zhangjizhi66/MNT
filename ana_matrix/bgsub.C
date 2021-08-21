// subtract background for TH1

// return: background subtracted spectrum TH1D

// parameters:
// TH1* h: input spectrum
// double smooth: smoothness of background

TH1D* bgsub(TH1* h, double smooth=8){
    int nbins = h->GetXaxis()->GetNbins();
    double emax = h->GetXaxis()->GetXmax();
    double emin = h->GetXaxis()->GetXmin();
    
    TH1D *bg; //background spectrum
    TSpectrum *tspec = new TSpectrum(500);
    bg = (TH1D*)tspec->Background(h,smooth,"nosmoothing same");

    TH1D *hpeak = new TH1D(Form("sbg_%.0f",h->GetBinContent(nbins/2)),"hpeak",nbins,emin,emax);
    hpeak->Add(h,bg,1,-1);
    return hpeak;
}