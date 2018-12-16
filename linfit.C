{
    Float_t x[3] = {0};
    Float_t y[3] = {0};
    const int64_t n = 3;
    
    y[0] = 2.014;
    y[1] = 18;
    y[2] = 20.0;

    for(int i = 0; i<3;i++)
    {
        cout << "\nMass Hydrogen Molecule:\t";
        cout << y[i];
        cout << "\tTime:\t";
        cin >> x[i];
    }
     for(int i = 0; i<3;i++) y[i] = sqrt(y[i]);
    TGraph *m = new TGraph(n,y,x);
    m->Fit("pol1");
    m->Draw("AP*");

}
