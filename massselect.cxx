#include "massselect.h"

#include "TDirectory.h"
#include "TV1720RawData.h"
#include "TV1290Data.hxx"

#include "TInterestingEventManager.hxx"

void massselect::CreateHistograms() {
  // check if we already have histogramss.
/*  char tname[100];
  sprintf(tname, "V1720_Correlations_%i", 0);

  TH2D *tmp = (TH2D *)gDirectory->Get(tname);
  if (tmp) return;
*/

  // Otherwise make histograms
  clear();

//  sprintf(name, "V1720_Correlations_%i", i);
//  sprintf(title, "V1720 Max ADC vs Max ADC time ch=%i", i);
 
    TH1D *masshist = new TH1D("masshist", "Mass Histogram", 10000, 0, 50);
    masshist->SetXTitle("Mass in amu");
    masshist->SetYTitle("Counts");
    push_back(masshist);

    TH2D *hatom = new TH2D("hatom", "Hydrogen Atom", 50, 0,1, 50,0,1);
    hatom->SetXTitle("X Position");
    hatom->SetYTitle("Y Position");
    push_back(hatom);

    TH2D *hmol = new TH2D("hmol", "Hydrogen Molecule", 50, 0,1, 50,0,1);
    hmol->SetXTitle("X Position");
    hmol->SetYTitle("Y Position");
    push_back(hmol);

    TH2D *ohmol = new TH2D("ohmol", "OH Molecule", 50, 0,1, 50,0,1);
    ohmol->SetXTitle("X Position");
    ohmol->SetYTitle("Y Position");
    push_back(ohmol);

    TH2D *water = new TH2D("water", "Water", 50, 0,1, 50,0,1);
    water->SetXTitle("X Position");
    water->SetYTitle("Y Position");
    push_back(water);

    TH2D *neon = new TH2D("neon", "Neon", 50, 0,1, 50,0,1);
    neon->SetXTitle("X Position");
    neon->SetYTitle("Y Position");
    push_back(neon);

    TH2D *neondim = new TH2D("neondim", "Neon Dimer", 50, 0,1, 50,0,1);
    neondim->SetXTitle("X Position");
    neondim->SetYTitle("Y Position");
    push_back(neondim);

    // TH1D *base = new TH1D("baseline", "BaseLine", 2000, 00, 8000);
    // base->SetXTitle("Sort of Timingi");
    // base->SetYTitle("Counts");
    // push_back(base);

}

void massselect::UpdateHistograms(TDataContainer &dataContainer) {

    TV1290Data *data = dataContainer.GetEventData<TV1290Data>("TDC0");   //TDC0 is the name of databank
    if (!data) return;

    /// Get the Vector of ADC Measurements.
    std::vector<TDCV1290Measurement> measurements = data->GetMeasurements();
    numhit = measurements.size();
    if(numhit != 2) return;

    int startchannel = 0;
    int stopchannel = 1;
    double stopdata = 0;
    double startdata = 0;

    for (int i = 0; i < numhit ; i++) // loop over measurements
        {
            int chan = measurements[i].GetChannel();
            if (chan == stopchannel)
                stopdata  = measurements[i].GetMeasurement() * RES_1290N;
            if (chan == startchannel)
                startdata = measurements[i].GetMeasurement() * RES_1290N;
        }
    float tdiff = stopdata - startdata;




  TV1720RawData *v1720 = dataContainer.GetEventData<TV1720RawData>("DG01");

  if (v1720 && v1720->IsZLECompressed()) {
    // NOTHING
    // can't handle compressed data yet...
  }

  //  if(v1720 && !v1720->IsZLECompressed()){

  //    for(int i = 0; i < 8; i++){ // loop over channels

  //      TV1720RawChannel channelData = v1720->GetChannelData(i);
  //      if(channelData.GetNSamples() <= 0) continue;

  //      double max_adc_value = -1.0;
  //      double max_adc_time = -1;

  //      for(int j = 0; j < channelData.GetNSamples(); j++){
  // double adc = channelData.GetADCSample(j);
  // if(adc > max_adc_value){
  //   max_adc_value = adc;
  //   max_adc_time = j * 4.0; // 4ns per bin
  // }

  //      }

  //      GetHistogram(i)->Fill(max_adc_time,max_adc_value);

  //      // As test, set any event where time for max bin < 200 as
  //      'interesting' if(max_adc_time < 400)
  //      iem_t::instance()->SetInteresting();

  //    }
  //  }

  if (v1720 && !v1720->IsZLECompressed()) {

    TV1720RawChannel channelDatasum = v1720->GetChannelData(4);
    double sadc, smaxadc = 0;
    int k, smaxpos = 0;
    int doubleeventflag = 0;
    int numsam = channelDatasum.GetNSamples();
    int windowmin = 500 , windowmax = 1500;
    for (k = 0; k < numsam ; k++) {
        sadc = channelDatasum.GetADCSample(k);
        if (sadc > smaxadc) {
          smaxadc = sadc;
          smaxpos = k; 
        }
      }
    
 //   printf("smaxpos= %d\n",smaxpos);
    if(windowmin < smaxpos && smaxpos < windowmax)
    {
    double smaxfac = smaxadc/2.5;
    for (k = windowmin; k < smaxpos-70; k++) 
    {
        sadc = channelDatasum.GetADCSample(k);
        if (sadc > smaxfac ) doubleeventflag = 1;
    }
    
    for (k = smaxpos+70; k < windowmax; k++) 
    {
        sadc = channelDatasum.GetADCSample(k);
        if (sadc > smaxfac ) doubleeventflag = 1;
    }

//    printf("doubleeventflag= %d\n",doubleeventflag);
    if (doubleeventflag == 0)
    {

    double maxch[4];
    for (int i = 0; i < 4; i++) {  // loop over channels

      TV1720RawChannel channelData = v1720->GetChannelData(i);
      if (channelData.GetNSamples() <= 0) continue;

   //   double minadc = 4000;
      double maxadc = 0;
      int maxadcpos = 0;
      // Find Max
      for (int j = smaxpos; j < smaxpos + 500; j++) {
        double adc = channelData.GetADCSample(j);
        if (adc > maxadc) {
          maxadc = adc;
          maxadcpos = j;
        }
      }
      // Find BAseline
      double base = 0;
      if(smaxpos < 4000)
      {
        for (int j = maxadcpos+450; j < numsam ; j++)
        {
            base = base+channelData.GetADCSample(j);
        }
       base=base/(numsam-maxadcpos-450);
       maxch[i] = maxadc-base;
      }

      if(smaxpos >= 4000)
      {
        for (int j = 0; j < maxadcpos-250; j++)
        {
            base = base+channelData.GetADCSample(j);
        }
       base=base/(maxadcpos-250);
       maxch[i] = maxadc-base;
      }
    }
    double sum = maxch[0]+maxch[1]+maxch[2]+maxch[3];
    double chx = (maxch[0]+maxch[1])/sum;    
    double chy = (maxch[1]+maxch[2])/sum;    


//    float m = 0;
//    float c = 0;
//    m = (wattim - hmoltim)/(watmass - hmolmass);
//    c = wattim - m*watmass;

    mass = (tdiff - intercept)/slope;
    mass = mass*mass;

    GetHistogram(0)->Fill(mass);
    if(hatommass-dmass1<mass && mass<hatommass+dmass1)
        GetHistogram(1)->Fill(chx,chy);
    if(hmolmass-dmass1<mass && mass<hmolmass+dmass1)
        GetHistogram(2)->Fill(chx,chy);
    if(ohmass-dmass1<mass && mass<ohmass+dmass1)
        GetHistogram(3)->Fill(chx,chy);
    if(watmass-dmass1<mass && mass<watmass+dmass1)
        GetHistogram(4)->Fill(chx,chy);
    if(neonmass-dmass1<mass && mass<neonmass+dmass1)
        GetHistogram(5)->Fill(chx,chy);
    if(neondimmass-dmass1<mass && mass<neondimmass+dmass1)
        GetHistogram(6)->Fill(chx,chy);
 
  }
    }
  }
}

void massselect::Reset() {
  for (int i = 0; i < 8; i++) {  // loop over channels
    GetHistogram(i)->Reset();
  }
}
