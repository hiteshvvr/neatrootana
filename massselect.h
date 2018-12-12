#ifndef massselect_h
#define massselect_h

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
};

#endif


