// gate for gamma matrix

// return: gated spectrum TH1D

// parameters:
// TH2* hgg: input gamma matrix
// double ge: gamma energy to gate on
// int cwid: width of gate, namely bin range (cbin-cwid,cbin+cwid), where cbin is the number of bin on which gamma energy is located
// int left1,left2,right1,right2: range of background gate, namely (cbin+left1,cbin+left2) (cbin+right1,cbin+right2)
// char axis: which axis to project on; should be assigned only by 'X' or 'Y'

// note: if background subtraction is not expected, keep default value of left1,left2,right1,right2
// note: input matrix is not necessary to be symmetric

TH1D* gate(TH2* hgg, double ge, int cwid, int left1=2, int left2=1, int right1=2, int right2=1, char axis='X')
{
    int nbins;
    double emax,emin;
    
    int cbin;
    TH1D *hgc,*hgl,*hgr;
    if (axis=='Y'){
        nbins = hgg->GetYaxis()->GetNbins();
        emax = hgg->GetYaxis()->GetXmax();
        emin = hgg->GetYaxis()->GetXmin();
        
        cbin = hgg->GetXaxis()->FindBin(ge);
        hgc = hgg->ProjectionY("hgc",cbin-cwid,cbin+cwid); // central gate spectrum
        hgl = hgg->ProjectionY("hgl",cbin+left1,cbin+left2); // left background spectrum
        hgr = hgg->ProjectionY("hgr",cbin+right1,cbin+right2); //right background sprctrum
    }
    else if (axis=='X'){
        nbins = hgg->GetXaxis()->GetNbins();
        emax = hgg->GetXaxis()->GetXmax();
        emin = hgg->GetXaxis()->GetXmin();
        
        cbin = hgg->GetYaxis()->FindBin(ge);
        hgc = hgg->ProjectionX("hgc",cbin-cwid,cbin+cwid);
        hgl = hgg->ProjectionX("hgl",cbin+left1,cbin+left2);
        hgr = hgg->ProjectionX("hgr",cbin+right1,cbin+right2);
    }
    else{
        printf("the 8th parameter should be 'X' or 'Y', depending on which axis to project on\n");
        return NULL;
    }

    int n1 = left2-left1+1;
    int n2 = right2-right1+1;
    TH1D *hgate = new TH1D(Form("%d_keV gate %d",(int)ge,left2),Form("gate on %d keV",(int)ge),nbins,emin,emax);
    hgate->Add(hgate,hgc);
    if (n1>=0 && n2>=0 && n1+n2>0){
        hgate->Add(hgate,hgl,1,-(double)(2*cwid+1)/(n1+n2));
        hgate->Add(hgate,hgr,1,-(double)(2*cwid+1)/(n1+n2));
    }
    hgate->Sumw2(0); // toggle error bar
    return hgate;
}