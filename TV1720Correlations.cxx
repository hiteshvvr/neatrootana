#include "TV1720Correlations.h"

#include "TDirectory.h"
#include "TV1720RawData.h"

#include "TInterestingEventManager.hxx"

void TV1720Correlations::CreateHistograms() {
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
    
    TH1D *a= new TH1D("QuadA", "QuadA", 1000, 00, 5000);
    a->SetXTitle("ADC Value");
    a->SetYTitle("Counts");
    push_back(a);

    TH1D *b= new TH1D("QuadB", "QuadB", 1000, 00, 5000);
    b->SetXTitle("ADC Value");
    b->SetYTitle("Counts");
    push_back(b);

    TH1D *c= new TH1D("QuadC", "QuadC", 1000, 00, 5000);
    c->SetXTitle("ADC Value");
    c->SetYTitle("Counts");
    push_back(c);

    TH1D *d= new TH1D("QuadD", "QuadD", 1000, 00, 5000);
    d->SetXTitle("ADC Value");
    d->SetYTitle("Counts");
    push_back(d);

    TH1D *sum= new TH1D("sum", "sum", 4000, 00, 12000);
    sum->SetXTitle("ADC Value");
    sum->SetYTitle("Counts");
    push_back(sum);

    TH2D *complete= new TH2D("complete", "complete", 200, 0,1, 200,0,1);
    complete->SetXTitle("X Coordinate");
    complete->SetYTitle("Y Coordinate");
    push_back(complete);

    TH2D *focused= new TH2D("focused", "focused", 200, 0.1,0.6, 200,0.4,0.6);
    focused->SetXTitle("X Coordinate");
    focused->SetYTitle("Y Coordinate");
    push_back(focused);
}

void TV1720Correlations::UpdateHistograms(TDataContainer &dataContainer) {
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

    double maxch[4];
    for (int i = 0; i < 4; i++) {  // loop over channels

      TV1720RawChannel channelData = v1720->GetChannelData(i);
      if (channelData.GetNSamples() <= 0) continue;

   //   double minadc = 4000;
      double maxadc = 0;
      // Find Max
      for (int j = 0; j < channelData.GetNSamples(); j++) {
        double adc = channelData.GetADCSample(j);
        if (adc > maxadc) {
          maxadc = adc;
        }
      }
      // Find BAseline
      double base=0;
      for (int j = 0; j < 350; j++)
       {
        base = base+channelData.GetADCSample(j);
       }
       base=base/350;


     maxch[i]=maxadc-base;

      // As test, set any event where time for max bin < 200 as 'interesting'
      // if (max_adc_time < 400) iem_t::instance()->SetInteresting();
    GetHistogram(i)->Fill(maxch[i]);
    }

    // double sum = minch[0]+minch[1]+minch[2]+minch[3];
    double sum = maxch[0]+maxch[1]+maxch[2]+maxch[3];
    double chx = (maxch[0]+maxch[1])/sum;    
//    double chx = 0;
    double chy = (maxch[1]+maxch[2])/sum;    
//    double chy = 0;
    
    GetHistogram(4)->Fill(sum);
    GetHistogram(5)->Fill(chx,chy);
    GetHistogram(6)->Fill(chx,chy);
  }
}

void TV1720Correlations::Reset() {
  for (int i = 0; i < 8; i++) {  // loop over channels
    GetHistogram(i)->Reset();
  }
}
