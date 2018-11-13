void testtree() {
   TFile f1("./files/output_000003.root");
   TGraph *gr;
   TH1D *h;
   h = new TH1D("hp0", "HIST", 1025, 0, 1025);
   std::vector<float> adcdata;
   std::vector<float> timedata;

   TTreeReader myReader("v1720Data", &f1);
   TTreeReaderValue<std::vector<float>> myVectorRV(myReader, "pulse");
   unsigned int evtCounter = 0;
   int j = 0;
   int numsamples;
   while (myReader.Next()) {
      // cout << "Event " << evtCounter << endl;
      evtCounter++;
      if (evtCounter == 100)
      {
         for (auto && value : *myVectorRV) 
         {
            // cout << value << endl;
            adcdata.emplace_back(value);
            timedata.emplace_back(j);
            // h->SetBinContent(j, value);
            h->Fill(value);
            j++;
            numsamples = j;
         }
      }
      gr = new TGraph(numsamples, &timedata[0], &adcdata[0]);
   }
   gr->Draw();
}