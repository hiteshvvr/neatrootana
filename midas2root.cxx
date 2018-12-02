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

class Analyzer : public TRootanaEventLoop
{
public:
    // An analysis manager.  Define and fill histograms in
    // analysis manager.
    TAnaManager *anaManager;

    // The tree to fill.
    TTree *fTree;
    TGraph *gr;


    // Getting raw data if required Generates large files
    int getrawdata = 1;
    std::ofstream outfile;

#ifdef USE_V1720
    // CAEN V1720 tree variables
/*    int midasid;
    uint32_t timetag;
    int chan_no;
    int maxadcvalue;
    uint32_t maxtimetag;
    std::vector<float> pulse;
    std::vector<float> ptime;
    double chx;
    double chy;
*/
    struct psdEvent {
        int midasid;
        uint32_t timetag;
        int maxadc[5];
        int maxtimetag[5];
        float base[5];
        int ch0data[4096];
        int ch1data[4096];
        int ch2data[4096];
        int ch3data[4096];
        int ch4data[4096];
        double posx;
        double posy;
    } psevent;


// Define the Histograms
    TH1D *a =  new TH1D("QuadA", "QuadA", 1000, 00, 5000);
    TH1D *b =  new TH1D("QuadB", "QuadB", 1000, 00, 5000);
    TH1D *c =  new TH1D("QuadC", "QuadC", 1000, 00, 5000);
    TH1D *d =  new TH1D("QuadD", "QuadD", 1000, 00, 5000);
    TH1D *hsum =  new TH1D("Sum of Channels", "hsum", 4000, 00, 12000);
    TH2D *complete = new TH2D("complete", "complete", 200, 0,1, 200,0,1);
    TH2D *focused = new TH2D("focused", "focused", 200, 0.1,0.6, 200,0.4,0.6);
#endif


    Analyzer() {};

    virtual ~Analyzer() {};

    void Initialize() {}

    void BeginRun(int transition, int run, int time)
    {
        // Create a TTree
        fTree = new TTree("v1720Data", "v1720 Data");

        if (getrawdata == 1)
        {
            TString csvfile = Form("outfiles/run_csv%05d.csv",run);
            outfile.open(csvfile);
        }

#ifdef USE_V1720
        fTree->Branch("midasid", &psevent.midasid, "midasid/I");
        fTree->Branch("timetag", &psevent.timetag, "timetag/i");
        fTree->Branch("maxadcvalue", psevent.maxadc, "maxadc[5]/I");
        fTree->Branch("maxtimetag", psevent.maxtimetag, "maxtimetag[5]/i");
        fTree->Branch("base", psevent.base, "base[5]/F");
        fTree->Branch("ch0data", psevent.ch0data,"ch0data[4096]/I");
        fTree->Branch("ch1data", psevent.ch1data,"ch1data[4096]/I");
        fTree->Branch("ch2data", psevent.ch2data,"ch2data[4096]/I");
        fTree->Branch("ch3data", psevent.ch3data,"ch3data[4096]/I");
        fTree->Branch("ch4data", psevent.ch4data,"ch4data[4096]/I");
        fTree->Branch("XhitPosition", &psevent.posx,"posx/F");
        fTree->Branch("YhitPosition", &psevent.posy,"posy/F");
#endif
    }

    void EndRun(int transition, int run, int time) {

    a->SetXTitle("ADC Value");
    a->SetYTitle("Counts");
    a->Write("Quadrand A histogram");

    b->SetXTitle("ADC Value");
    b->SetYTitle("Counts");
    b->Write("Quadrand B histogram");

    c->SetXTitle("ADC Value");
    c->SetYTitle("Counts");
    c->Write("Quadrand C histogram");
    d->SetXTitle("ADC Value");
    d->SetYTitle("Counts");
    d->Write("Quadrand D histogram");

    hsum->SetXTitle("ADC Value");
    hsum->SetYTitle("Counts");
    hsum->Write("Sum of all Quadrand histogram");

    complete->SetXTitle("X Coordinate");
    complete->SetYTitle("Y Coordinate");
    complete->Write("PSD hit postition histogram");

    focused->SetXTitle("X Coordinate");
    focused->SetYTitle("Y Coordinate");
    complete->Write("PSD hit focused histogram");
    
    
//    gr->Write("Single Sample Pulse");



        if (getrawdata == 1)
            outfile.close();
        printf("\n");
    }

    // Main work here; create ttree events for every sequenced event in
    // Lecroy data packets.
    bool ProcessMidasEvent(TDataContainer &dataContainer)
    {
        psevent.midasid = dataContainer.GetMidasEvent().GetSerialNumber();
        // if (midasid % 10 == 0) printf(".");
        // int i,k;
        int i, numsamples, j;

#ifdef USE_V1720
        TV1720RawData *v1720 = dataContainer.GetEventData<TV1720RawData>("DG01");

        if (v1720 && !v1720->IsZLECompressed())
        {
            psevent.timetag = v1720->GetTriggerTag();
            int channelmask = v1720->GetChannelMask();
            double maxch[4];

            std::bitset<8> chmaskbit(channelmask);   // Getting channelmask as an array
            for (i = 0; i < 8; i++)
            {
                if (chmaskbit[i] == 1)
                {
                    int chan_no = i;
                    TV1720RawChannel channelData = v1720->GetChannelData(i);
                    numsamples = channelData.GetNSamples();
                    if(numsamples <=  0) {maxch[i] = 0; continue;}
                    if(numsamples >=  4096) {maxch[i] = 0; printf("There is problem\n"); continue;}
                    int maxadc = 0;
                    int maxtime = -1;
                    double adc[4096];
                    for (j = 0; j < numsamples; j++)
                    {
                        adc[j] = channelData.GetADCSample(j);

                        if (adc[j] > maxadc)
                        {
                            maxadc = adc[j];
                            maxtime = j * 4;
                        }
                        if (getrawdata == 57)
                            outfile << psevent.timetag + i * 4 << "    " << adc << "\n";
                    }

                        if(i == 0)for(j=0;j<numsamples;j++)psevent.ch0data[j]=adc[j];
                        if(i == 1)for(j=0;j<numsamples;j++)psevent.ch1data[j]=adc[j];
                        if(i == 2)for(j=0;j<numsamples;j++)psevent.ch2data[j]=adc[j];
                        if(i == 3)for(j=0;j<numsamples;j++)psevent.ch3data[j]=adc[j];
                        if(i == 4)for(j=0;j<numsamples;j++)psevent.ch4data[j]=adc[j];

                    // getting the baseline
                    double base = 0;
                    int baselinesamples = 350;      // Number of samples for creating the baseline
                    for(int j = 0 ; j<baselinesamples; j++)
                        base = base + channelData.GetADCSample(j);

                    base =  base/baselinesamples;

                    psevent.maxadc[i] =  maxadc;
                    psevent.maxtimetag[i] = maxtime;
                    psevent.base[i] = base;
                    
                    maxch[i] = maxadc - base;

                    // Fill Max Histograms
                    if(i == 0) a->Fill(maxadc - base);
                    if(i == 1) b->Fill(maxadc - base);
                    if(i == 2) c->Fill(maxadc - base);
                    if(i == 3) d->Fill(maxadc - base);


                    int plotevent = 100;
                    if (psevent.midasid == plotevent)
                    {
                        TH1D *h  = new TH1D("h", "Normal histogram", numsamples, 0, numsamples - 1);
                        for (j = 0; j < numsamples; j++) h->SetBinContent (j, channelData.GetADCSample(j));
                        h->Write("Single Sample Pulse");

                   //     gr = new TGraph(numsamples, &ptime[0], &pulse[0]);
                    }
                    // std::cout << pulse.size() << '\n';
                }
            }

            double sum = maxch[0] + maxch[1] + maxch[2] + maxch[3];

            psevent.posx = (maxch[0] + maxch[1])/sum;
            psevent.posy = (maxch[1] + maxch[2])/sum;

            hsum->Fill(sum);
            complete->Fill(psevent.posx, psevent.posy);
            focused->Fill(psevent.posx, psevent.posy);
            
            if (getrawdata == 1)
                outfile <<psevent.midasid<< "\t"<<psevent.timetag<<"\t"<<maxch[0]<<"\t";
                outfile <<maxch[1]<<"\t"<<maxch[2]<<"\t"<<maxch[3]<<"\t";
                outfile <<psevent.posx<<"\t"<<psevent.posy<<"\t"<<"\n";

            fTree->Fill();
        }
#endif


        return true;
    };

    // Complicated method to set correct filename when dealing with subruns.
    std::string SetFullOutputFileName(int run, std::string midasFilename) {
        char buff[128];
        Int_t in_num = 0, part = 0;
        Int_t num[2] = {0, 0};  // run and subrun values
        // get run/subrun numbers from file name
        for (int i = 0;; ++i) {
            char ch = midasFilename[i];
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
                std::cerr << "File name run number (" << num[0]
                          << ") disagrees with MIDAS run (" << run << ")" << std::endl;
                exit(1);
            }
            sprintf(buff, "output_%.6d_%.4d.root", run, num[1]);
            printf("Using filename %s\n", buff);
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


