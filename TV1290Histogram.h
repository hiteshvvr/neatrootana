#ifndef TV1290Histograms_h
#define TV1290Histograms_h

#include <string>
#include "THistogramArrayBase.h"
#define RES_1290N 0.0245

/// Class for making histograms of V1290 TDC data.
class TV1290Histograms : public THistogramArrayBase {
 public:
  TV1290Histograms();
  virtual ~TV1290Histograms(){};
  
  /// Update the histograms for this canvas.
  void UpdateHistograms(TDataContainer& dataContainer);

  /// Take actions at begin run
  void BeginRun(int transition,int run,int time);

  /// Take actions at end run  
  void EndRun(int transition,int run,int time);

private:

  void CreateHistograms();
    
};

#endif


