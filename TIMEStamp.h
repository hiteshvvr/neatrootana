#ifndef TIMEStamp_hxx_seen
#define TIMEStamp_hxx_seen

#include <vector>

#include "TGenericData.hxx"


/// This decoder is for getting the absolute timestamps from computer time
class TIMEStamp: public TGenericData {

public:

  /// Constructor
  TIMEStamp(int bklen, int bktype, const char* name, void *pdata);

  /// Get the Time in second
  uint32_t GetUnixSecondTime() const {return (timeinsec & 0xffffffff);};

  /// Get the NanoSecond Time
  uint32_t GetNanoSecTime() const {return (timeinnanosec & 0xffffffff);};

  /// Get Total Time in Nanosecond Precision
  uint64_t GetTotalTime() const {return ((timeinsec & 0xffffffff) * 1000000000 + (timeinnanosec & 0xffffffff));};


private:

  uint32_t timeinsec;
  uint32_t timeinnanosec;
  

};

#endif
