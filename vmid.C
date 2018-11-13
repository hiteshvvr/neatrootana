void vmid(char* runfile){
    TFile *fin = new TFile(runfile);
    TTree *T = (TTree*)fin->Get("v1720Data");
    TNtuple* ntuple = (TNtuple*)fin->Get("ntuple");
    TCanvas* c1 = new TCanvas("c1","c1",0,0,1200,800);
    // TGraph *graph = new TGraph(nselected, gsy, grep);
    c1->Divide(3,2);
    c1->cd(1);
    T->Draw("midasid");
    c1->cd(2);
    T->Draw("timetag");
    c1->cd(3);
    T->Draw("chan_no");
    c1->cd(4);
    T->Draw("maxadcvalue");
    c1->cd(5);
    T->Draw("maxtimetag");
    c1->cd(6);
    T->Draw("maxtimetag");
// graph->Draw("ac*");
}