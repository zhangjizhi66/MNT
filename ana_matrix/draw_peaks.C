// find peaks for TH1 and draw on TCanvas

// parameters:
// int tough: define strictness of finding a peak
// double thres: define threshold of height of peaks

// note: TCanvas c1 should have been defined

void draw_peaks(TH1 *h, int tough=2, double thres=0.05)
{
    TSpectrum *s = new TSpectrum(500);
    int nfound = s->Search(h,tough,"",thres); 
    TPolyMarker *pm = (TPolyMarker *)h->GetListOfFunctions()->FindObject("TPolyMarker");
    pm->SetMarkerSize(0);
    h->Draw();
    
    double *xpeaks = s->GetPositionX();
    double *ypeaks = s->GetPositionY();
    for(int j=0; j<nfound; j++)
    {
        TLatex *tex=new TLatex(xpeaks[j],ypeaks[j],Form("%d",int(xpeaks[j])));
        tex->SetTextFont(13);
        tex->SetTextSize(14);
        tex->SetTextAlign(12);
        tex->SetTextAngle(90);
        tex->SetTextColor(kRed);
        tex->Draw();
    }
    c1->Draw();
}