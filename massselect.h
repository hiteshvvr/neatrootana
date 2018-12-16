#ifndef massselect_h
#define massselect_h
#define RES_1290N 0.0245
#include <string>
#include "THistogramArrayBase.h"
#include "TH2D.h"
/// Class for making 2D histogram of correlation
/// between V1720 pulse height and bin
class massselect : public THistogramArrayBase {
public:
  massselect(){
    CreateHistograms();
  };
  virtual ~massselect(){};

  void UpdateHistograms(TDataContainer& dataContainer);

  // Reset the histograms; needed before we re-fill each histo.
  void Reset();
  
  void CreateHistograms();
  
  /// Take actions at begin run
  void BeginRun(int transition,int run,int time){		
    CreateHistograms();		
  }

private:
int numhit;
float wattim = 1765;
float hmoltim = 545;
float watmass = 18.02;
float hmolmass = 2.014;
float hatommass = 1.007;
float ohmass = watmass - hatommass;
float neonmass = 20;
float neondimmass = 40;
float dmass1 = 0.5;
float dmass2 = 1;
float mass = 0;

// INSERT THESE VALUES
float intercept = 1;
float slope = 1;


};

#endif

