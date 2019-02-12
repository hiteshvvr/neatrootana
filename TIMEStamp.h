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
  uint32_t GetUnixSecondTime() const {return (timeinsec & 0xfffffff);};

  /// Get the NanoSecond Time
  uint32_t GetNanoSecTime() const {return (timeinnanosec & 0xfffffff);};

  /// Get Total Time in Nanosecond Precision
  uint32_t GetTotalTime() const {return (timeinnanosec & 0xfffffff);};

  }
  

  void Print();


private:

  /// Helper method to handle uncompressed data.
  void HandlUncompressedData();

  /// The overall global headers
  uint32_t timeinsec;
  uint32_t fGlobalHeader1;
  uint32_t fGlobalHeader2;
  uint32_t fGlobalHeader3;
  

  /// Vector of V1740 measurements
  std::vector<TV1740RawChannel> fMeasurements;

};

#endif
