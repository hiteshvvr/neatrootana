#include "TV1290Histogram.h"

#include "TV1290Data.hxx"
#include "TDirectory.h"

const int Nchannels = 16;

/// Reset the histograms for this canvas
TV1290Histograms::TV1290Histograms() {

    CreateHistograms();
}


void TV1290Histograms::CreateHistograms() {


    // Otherwise make histograms
    clear();

    std::cout << "Create Histos" << std::endl;
    int i = 0;

    TH1D *alltdiff = new TH1D("TotalSpectrum", "TotalSpectrum", 10000, -50000, 50000);
    alltdiff->SetXTitle("Time in ns");
    alltdiff->SetYTitle("Counts");
    push_back(alltdiff);

    TH1D *shits = new TH1D("singleHit", "singleHit", 10000, 0, 5000);
    shits->SetXTitle("Time in ns");
    shits->SetYTitle("Counts");
    push_back(shits);

    TH1D *mhits = new TH1D("multiHit", "multiHit", 10000, -5000, 5000);
    mhits->SetXTitle("Time in ns");
    mhits->SetYTitle("Counts");
    push_back(mhits);

    TH1I *nuofhits = new TH1I("NuOfHits", "NumberOfHits", 36, -2, 10);
    nuofhits->SetXTitle("Number of Hits in single Event");
    nuofhits->SetYTitle("Counts");
    push_back(nuofhits);

    TH1I *hitcounts = new TH1I("NuOfHits", "HitDistribution", 50 , -1, 20);
    hitcounts->SetXTitle("Hits");
    hitcounts->SetYTitle("Counts");
    push_back(hitcounts);


    TH1F *tripdis = new TH1F("TripleDis", "Triple distributed 0-eee, 1-eei, 2-eie, 3-eii, 4-iee, 5-iei, 6-iie, 7-iii", 1000, -5, 10);
    tripdis->SetXTitle("Triple Counts");
    tripdis->SetYTitle("Counts");
    push_back(tripdis);


    TH1D *triples2ion = new TH1D("triples2ion", "Triple With Two Ions", 5000, -7000, 7000);
    triples2ion->SetXTitle("time");
    triples2ion->SetYTitle("Counts");
    push_back(triples2ion);

    TH1D *triples2electrs = new TH1D("triples2electrs", "Triples With Two Electrons", 5000, -7000, 7000);
    triples2electrs->SetXTitle("time");
    triples2electrs->SetYTitle("Counts");
    push_back(triples2electrs);




    for ( i = 1; i < Nchannels; i++) { // loop over channels
        // for(int i = 0; i < 3; i++){ // loop over channels
        char name[100];
        char title[100];
        sprintf(name, "V1290_%i_%i", 0, i);

        // Delete old histograms, if we already have them
        TH1D *old = (TH1D*)gDirectory->Get(name);
        if (old) {delete old;}

        // Create new histograms
        // printf("%s\n", name );
        // std::cout << "Name of Histogram " << i << ":::::::::" <<  name << std::endl;
        sprintf(title, "V1290 histogram for channel=%i", i);
        TH1D *tmp = new TH1D(name, title, 5000, -7000, 7000);
        tmp->SetXTitle("TDC value");
        tmp->SetYTitle("Number of Entries");
        push_back(tmp);
    }

}

/// Update the histograms for this canvas.
void TV1290Histograms::UpdateHistograms(TDataContainer& dataContainer) {

    int i;
    int numhit;
    int startchannel = 0;
    int stopchannel = 1;
    double stoptime = 0;
    double starttime = 0;
    int startchancount = 0;
    int stopchancount = 0;


    TV1290Data *data = dataContainer.GetEventData<TV1290Data>("TDC0");   //TDC0 is the name of databank
    if (!data) return;

    /// Get the Vector of ADC Measurements.
    std::vector<TDCV1290Measurement> measurements = data->GetMeasurements();
    numhit = measurements.size();
    for (i = 0; i < numhit ; i++) // loop over measurements
    {
        int channo = measurements[i].GetChannel();
        GetHistogram(channo + 8)->Fill(measurements[i].GetMeasurement() * RES_1290N);
        if (channo == stopchannel)
        {
            stoptime = measurements[i].GetMeasurement() * RES_1290N;
            stopchancount++;
        }
        if (channo == startchannel)
        {
            starttime = measurements[i].GetMeasurement() * RES_1290N;
            startchancount++;
        }
    }

    GetHistogram(3)->Fill(numhit);
    GetHistogram(4)->Fill(numhit * 10 + stopchancount);

    if (numhit == 2)
    {
        double stopdata = 0;
        double startdata = 0;

        for (i = 0; i < numhit ; i++) // loop over measurements
        {
            int chan = measurements[i].GetChannel();
            if (chan == stopchannel)
                stopdata  = measurements[i].GetMeasurement() * RES_1290N;
            if (chan == startchannel)
                startdata = measurements[i].GetMeasurement() * RES_1290N;
        }

        float tdiff = stopdata - startdata;
        GetHistogram(0)->Fill(tdiff);
        GetHistogram(1)->Fill(tdiff);
    }

    if (numhit == 3)
    {


        for (i = 0; i < numhit; i++)
        {
            int chan = measurements[i].GetChannel();
            double meas = measurements[i].GetMeasurement() * RES_1290N;
            if (startchancount == 2 && chan != stopchannel)
            {
                float ttdiff = stoptime - meas;
                GetHistogram(7)->Fill(ttdiff);
                GetHistogram(0)->Fill(ttdiff);
            }
            if (stopchancount == 2 && chan != startchannel)
            {
                float ttdiff = meas - starttime;
                GetHistogram(6)->Fill(ttdiff);
                GetHistogram(0)->Fill(ttdiff);
            }
        }


        int chan0 = measurements[0].GetChannel();
        // double meas0 = measurements[0].GetMeasurement() * RES_1290N;
        int chan1 = measurements[1].GetChannel();
        // double meas1 = measurements[1].GetMeasurement() * RES_1290N;
        int chan2 = measurements[2].GetChannel();
        // double meas2 = measurements[2].GetMeasurement() * RES_1290N;

        int tdis = 4 * chan0 + 2 * chan1 + 1 * chan2;
        GetHistogram(5)->Fill(tdis);

    }





    if (numhit > 3)
    {
        for (i = 0; i < numhit; i++)
        {
            int chan = measurements[i].GetChannel();
            if (chan != stopchannel)
            {
                float mtdiff = stoptime - measurements[i].GetMeasurement();
                GetHistogram(0)->Fill(mtdiff);
                GetHistogram(2)->Fill(mtdiff);
            }
        }
    }


}



/// Take actions at begin run
void TV1290Histograms::BeginRun(int transition, int run, int time) {

    CreateHistograms();

}

/// Take actions at end run
void TV1290Histograms::EndRun(int transition, int run, int time) {

}
