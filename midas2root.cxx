// Example Program for converting MIDAS format to ROOT format.
//
// T. Lindner (Jan 2016)
//
// Example is for the CAEN V792 ADC module

#include <stdio.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <bitset>

#include "TFile.h"
#include "TRootanaEventLoop.hxx"
#include "TTree.h"
#include "TGraph.h"
#include "TNtuple.h"

#include <fstream>
// using namespace std;

#include "TAnaManager.hxx"

#ifdef USE_V792
#include "TV792Data.hxx"
#endif

#ifdef USE_V1720
#include "TV1720RawData.h"
#endif

#ifdef USE_V1290
#include "TV1290Data.hxx"
#endif


class Analyzer : public TRootanaEventLoop
{
public:
    // An analysis manager.  Define and fill histograms in
    // analysis manager.
    TAnaManager *anaManager;

    int getrawdata = 1;
    std::ofstream outfile;

#ifdef USE_V1720
    // The tree to fill.
    TTree *f1720Tree;
    TGraph *gr1720;

    // Getting raw data if required Generates large files

    // CAEN V1720 tree variables
    int midasid;
    uint32_t timetag;
    int chan_no;
    int maxadcvalue;
    uint32_t maxtimetag;
    std::vector<float> pulse;
    std::vector<float> ptime;
    double chx;
    double chy;
    /*    struct psdEvent {
            int midasid;
            uint32_t timetag;
            int chan_no;
            int maxadc;
            int base;
            std::vector<float> pulse;
            std::vector<float> ptime;
            double x;
            double y;
        } psevent;
    */
// Define the Histograms
    TH1D *a =  new TH1D("QuadA", "QuadA", 1000, 00, 5000);
    TH1D *b =  new TH1D("QuadB", "QuadB", 1000, 00, 5000);
    TH1D *c =  new TH1D("QuadC", "QuadC", 1000, 00, 5000);
    TH1D *d =  new TH1D("QuadD", "QuadD", 1000, 00, 5000);
    TH1D *hsum =  new TH1D("Sum of Channels", "hsum", 4000, 00, 12000);
    TH2D *complete = new TH2D("complete", "complete", 200, 0, 1, 200, 0, 1);
    TH2D *focused = new TH2D("focused", "focused", 200, 0.1, 0.6, 200, 0.4, 0.6);
#endif

#ifdef USE_V1290
    TTree *f1290Tree;
    TH1D *h1290;
    TGraph *gr1290;
//    float RES_1290N = 0.0245;

    TH1D *alltdiff = new TH1D("TotalSpectrum", "TotalSpectrum", 10000, -5000, 5000);
    TH1D *shits = new TH1D("singleHit", "singleHit", 10000, 0, 5000);
    TH1D *mhits = new TH1D("multiHit", "multiHit", 5000, -5000, 5000);
    TH1D *triples2ion = new TH1D("triples2ion", "Triple With Two Ions", 5000, -7000, 7000);
    TH1D *triples2electrs = new TH1D("triples2electrs", "Triples With Two Electrons", 5000, -7000, 7000);
    TH1I *nuofhits = new TH1I("NuOfHits", "NumberOfHits", 300, -1, 100);
    TH1I *hitcounts = new TH1I("NuOfHits", "HitDistribution", 50 , -1, 20);
    TH1F *tripdis = new TH1F("TripleDis", "Triple distributed 0-eee, 1-eei, 2-eie, 3-eii, 4-iee, 5-iei, 6-iie, 7-iii", 1000, -5, 10);

    // Getting raw data if required Generates large files

    // CAEN V1290 TREE VARIABLES
    struct v1290Event {
        int midasid;
        int eventid;
        int num_of_hits;
        unsigned int timestampdata[5];
        int channodata[5];
        unsigned int chan0Data;
        unsigned int chan1Data;
    } event;

//    std::vector<unsigned int> timestampdata;   // vector containing timestamp for each hit.
//    std::vector<unsigned int> channodata;     // vector containing channel number for each hit.
#endif


    Analyzer() {};

    virtual ~Analyzer() {};

    void Initialize() {}

    void BeginRun(int transition, int run, int time)
    {

        if (getrawdata == 1)
        {
            TString csvfile = Form("outfiles/run_csv%05d.csv", run);
            outfile.open(csvfile);
        }

        // Create a TTree
#ifdef USE_V1720
        f1720Tree = new TTree("v1720Data", "v1720 Data");

        f1720Tree->Branch("midasid", &midasid, "midasid/I");
        f1720Tree->Branch("timetag", &timetag, "timetag/i");
        f1720Tree->Branch("chan_no", &chan_no, "chan_no/I");
        f1720Tree->Branch("maxadcvalue", &maxadcvalue, "maxadcvalue/I");
        f1720Tree->Branch("maxtimetag", &maxtimetag, "maxtimetag/i");
        f1720Tree->Branch("pulse", &pulse);
        f1720Tree->Branch("XhitPosition", &chx);
        f1720Tree->Branch("YhitPosition", &chy);
#endif

#ifdef USE_V1290

        f1290Tree = new TTree("v1290Data", "v1290Data");

        f1290Tree->Branch("midasid", &event.midasid, "midasid/I");
        f1290Tree->Branch("eventid", &event.eventid, "eventid/I");
        f1290Tree->Branch("nu_of_hits", &event.num_of_hits, "num_of_hits/I");
        f1290Tree->Branch("timestampdata", event.timestampdata, "timestampdata[num_of_hits]/I");
        f1290Tree->Branch("ChannelNumData", event.channodata, "channodata[num_of_hits]/I");
        f1290Tree->Branch("Channel0Data", &event.chan0Data, "chan0Data/I");
        f1290Tree->Branch("Channel1Data", &event.chan1Data, "chan1Data/I");
#endif

    }

    void endrun(int transition, int run, int time) {

#ifdef use_v1720
        a->setxtitle("adc value");
        a->setytitle("counts");
        a->write("quadrand a histogram");

        b->setxtitle("adc value");
        b->setytitle("counts");
        b->write("quadrand b histogram");

        c->setxtitle("adc value");
        c->setytitle("counts");
        c->write("quadrand c histogram");
        d->setxtitle("adc value");
        d->setytitle("counts");
        d->write("quadrand d histogram");

        hsum->setxtitle("adc value");
        hsum->setytitle("counts");
        hsum->write("sum of all quadrand histogram");

        complete->setxtitle("x coordinate");
        complete->setytitle("y coordinate");
        complete->write("psd hit postition histogram");

        focused->setxtitle("x coordinate");
        focused->setytitle("y coordinate");
        complete->write("psd hit focused histogram");


        gr->write("single sample pulse");
#endif

#ifdef use_v1290
        shits->write("singlehits");
        mhits->write("multiplehits");
        alltdiff->write("completespectrum");
        nuofhits->write("numberofhits");
        hitcounts->write("hitdistribution");
        triples2ion->write("triples with two ions");
        triples2electrs->write("triples with two electrons");
        tripdis->write("triples distribution");
#endif

        if (getrawdata == 1)
            outfile.close();
        printf("\n");
    }

    // main work here; create ttree events for every sequenced event in
    // lecroy data packets.
    bool ProcessMidasEvent(TDataContainer &dataContainer)
    {
        midasid = dataContainer.GetMidasEvent().GetSerialNumber();
        // if (midasid % 10 == 0) printf(".");
        // int i,k;
        int i, numsamples, j;
        if(getrawdata == 1)
            outfile << "!!!" << "\n" << midasid << "\n";

#ifdef USE_V1720
        TV1720RawData *v1720data = dataContainer.GetEventData<TV1720RawData>("DG01");
        if (v1720data && !v1720data->IsZLECompressed())
        {
            timetag = v1720data->GetTriggerTag();
            int channelmask = v1720data->GetChannelMask();
            double maxch[4];

            std::bitset<8> chmaskbit(channelmask);   // getting channelmask as an array
            for (i = 0; i < 8; i++)
    {
                if (chmaskbit[i] == 1)
                {
                    chan_no = i;
                    TV1720RawChannel channelData  = v1720data->GetChannelData(i);
                    numsamples = channelData.GetNSamples();
                    if (numsamples <=  0) {maxch[i] = 0; continue;}
                    int maxadc = 0;
                    int maxtime = -1;
                    pulse.clear();
                    ptime.clear();
                    for (j = 0; j < numsamples; j++)
                    {
                        double adc;
                        adc = channelData.GetADCSample(j);
                        pulse.emplace_back(adc);
                        ptime.emplace_back(j * 4);
                        if (adc > maxadc)
                        {
                            maxadc = adc;
                            maxtime = j * 4;
                        }
                        if (getrawdata == 57)
                            outfile << timetag + i * 4 << "    " << adc << "\n";
                    }

                    // getting the baseline
                    double base = 0;
                    int baselinesamples = 350;      // number of samples for creating the baseline
                    for (int j = 0 ; j < baselinesamples; j++)
                        base = base + channelData.GetADCSample(j);

                    base =  base / baselinesamples;

                    maxadcvalue = maxadc;
                    maxtimetag = maxtime;
                    // std::cout << timetag << '\n';
                    // maxtimetag = timetag + max_time;
                    //
                    maxch[i] = maxadc - base;

                    // fill max histograms
                    if (i == 0) a->Fill(maxadc - base);
                    if (i == 1) b->Fill(maxadc - base);
                    if (i == 2) c->Fill(maxadc - base);
                    if (i == 3) d->Fill(maxadc - base);


                    int plotevent = 100;
                    if (midasid == plotevent)
                    {
                        TH1D *h1720  = new TH1D("h1720", "normal histogram", numsamples, 0, numsamples - 1);
                        for (j = 0; j < numsamples; j++) h1720->SetBinContent(j, channelData.GetADCSample(j));
                        h1720->Write("single sample pulse");

                        gr1720 = new TGraph(numsamples, &ptime[0], &pulse[0]);
                    }
                    // std::cout << pulse.size() << '\n';
                }
            }

            double sum = maxch[0] + maxch[1] + maxch[2] + maxch[3];

            chx = (maxch[0] + maxch[1]) / sum;
            chy = (maxch[1] + maxch[2]) / sum;

            hsum->Fill(sum);
            complete->Fill(chx, chy);
            focused->Fill(chx, chy);

            if (getrawdata == 1)
                outfile << midasid << "\t" << timetag << "\t" << maxch[0] << "\t";
            outfile << maxch[1] << "\t" << maxch[2] << "\t" << maxch[3] << "\t";
            outfile << chx << "\t" << chy << "\t" << "\n";

            f1720Tree->Fill();
        }
#endif

#ifdef USE_V1290
        TV1290Data *v1290data= dataContainer.GetEventData<TV1290Data>("TDC0");
        if (!v1290data) return 0 ;
        if (v1290data) {

            int numhit;
            int startchancount = 0;
            int stopchancount = 0;
            double stoptime = 0;
            double starttime = 0;
            int startchannel = 0;
            int stopchannel = 1;

            std::vector<TDCV1290Measurement> measurements = v1290data->GetMeasurements();

//       eventid = data->GetEventID();
            numhit = measurements.size();
//        std::cout<< eventid<<std::endl;
            //      std::cout<< event.midasid<<std::endl;


            event.num_of_hits = numhit;
            if (numhit > 5)
                numhit = 5;

            for (i = 0; i < numhit ; i++)
            {
                int channo = measurements[i].GetChannel();
                unsigned int chandata = measurements[i].GetMeasurement();
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

                event.channodata[i] = channo;
                event.timestampdata[i] = chandata;

                if (channo == 0)
                    event.chan0Data = chandata;
                if (channo == 1)
                    event.chan1Data = chandata;

                if (getrawdata == 1)
                    outfile << channo << "\t" << chandata << "\n";
            }
            hitcounts->Fill(numhit * 10 + stopchancount);
            nuofhits->Fill(numhit);


            if (numhit == 2)
            {
                double stopdata = 0;
                double startdata = 0;

                for (i = 0; i < numhit; i++) // loop over measurements
                {
                    int chan = measurements[i].GetChannel();
                    if (chan == stopchannel)
                        stopdata  = measurements[i].GetMeasurement() * RES_1290N;
                    if (chan == startchannel)
                        startdata = measurements[i].GetMeasurement() * RES_1290N;
                }

                float tdiff = stopdata - startdata;
                shits->Fill(tdiff);
                alltdiff->Fill(tdiff);
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
                        triples2electrs->Fill(ttdiff);
                        alltdiff->Fill(ttdiff);
                    }
                    if (stopchancount == 2 && chan != startchannel)
                    {
                        float ttdiff = meas - starttime;
                        triples2ion->Fill(ttdiff);
                        alltdiff->Fill(ttdiff);
                    }
                }

                int chan0 = measurements[0].GetChannel();
            // double meas0 = measurements[0].GetMeasurement() * RES_1290N;
                int chan1 = measurements[1].GetChannel();
                // double meas1 = measurements[1].GetMeasurement() * RES_1290N;
                int chan2 = measurements[2].GetChannel();
                // double meas2 = measurements[2].GetMeasurement() * RES_1290N;

                int tdis = 4 * chan0 + 2 * chan1 + 1 * chan2;
                tripdis->Fill(tdis);

            }

            if (numhit > 3)
            {
                for (i = 0; i < numhit; i++)
                {
                    int chan = measurements[i].GetChannel();
                    if (chan != stopchannel)
                    {
                        float mtdiff = stoptime - measurements[i].GetMeasurement();
                        mhits->Fill(mtdiff);
                        alltdiff->Fill(mtdiff);
                    }
                }
            }

            f1290Tree->Fill();
        }
#endif

        return true;
    };

    // complicated method to set correct filename when dealing with subruns.
    std::string setfulloutputfilename(int run, std::string midasfilename) {
        char buff[128];
        Int_t in_num = 0, part = 0;
        Int_t num[2] = {0, 0};  // run and subrun values
        // get run/subrun numbers from file name
        for (int i = 0;; ++i) {
            char ch = midasfilename[i];
            if (!ch) break;
            if (ch == '/') {
                // skip numbers in the directory name
                num[0] = num[1] = in_num = part = 0;
            } else if (ch >= '0' && ch <= '9' && part < 2) {
                num[part] = num[part] * 10 + (ch - '0');
                in_num = 1;
            } else if (in_num) {
                in_num = 0;
                ++part;
            }
        }
        if (part == 2) {
            if (run != num[0]) {
                std::cerr << "file name run number (" << num[0]
                          << ") disagrees with midas run (" << run << ")" << std::endl;
                exit(1);
            }
            sprintf(buff, "output_%.6d_%.4d.root", run, num[1]);
            printf("using filename %s\n", buff);
        } else {
            sprintf(buff, "outfiles/run_root%.5d.root", run);
        }
        return std::string(buff);
    };
};

int main(int argc, char *argv[]) {
    Analyzer::CreateSingleton<Analyzer>();
    return Analyzer::Get().ExecuteLoop(argc, argv);
}



// tgenericdata *v1720a = datacontainer.geteventdata<tgenericdata>("dg01");
// if(v1720){

// const uint32_t *fdata = v1720a->getdata32();
// int fglobalheader1 = v1720a->getdata32()[1];
// int fglobalheader2 = v1720->getdata32()[2];
// int fglobalheader3 = v1720->getdata32()[3];

// printf("%ld %ld %ld %ld \n",fdata[0],fdata[1],fdata[2],fdata[3]);

//  // printf("data exists\n");
//  const uint32_t *a = v1720->getdata32();

//  if(id==35){
//  printf("%d\n",v1720->getsize());
//  k=a[1] & 0xff;
//  printf("%d\n", k );
//  // for(i=0;i<55;i++)
//  // printf ("a[%i]:%d\n", i, *(a++));
//  int n32samples = (v1720->geteventsize() - 4)/ 1;

//  }
// }
