void vmid(TString runfile){
    TFile *fin = new TFile(runfile);
    TTree *T = (TTree*)fin->Get("NeatData");
    TNtuple* ntuple = (TNtuple*)fin->Get("ntuple");
    TCanvas* c1 = new TCanvas("c1","c1",0,0,1200,800);
    // TGraph *graph = new TGraph(nselected, gsy, grep);
    c1->Divide(2,3);
    c1->cd(1);
    T->Draw("posx:posy>>psd(100,0,1,100,0,1)","","colz");
    c1->cd(2);
    T->Draw("adcoverrange");
    c1->cd(3);
    T->Draw("base[0]>>basehist(1000,50,350)");
    c1->cd(4);
    T->Draw("base[1]>>basehist1(1000,50,350)");
    c1->cd(5);
    T->Draw("base[2]>>basehist2(1000,50,350)");
    c1->cd(6);
    T->Draw("base[3]>>basehist3(1000,50,350)");
    // T->Draw("maxadcvalue");
    // c1->cd(5);
    // T->Draw("maxtimetag");
    // c1->cd(6);
    // T->Draw("maxtimetag");
// graph->Draw("ac*");
}
