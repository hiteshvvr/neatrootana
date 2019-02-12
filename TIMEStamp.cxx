#include "TV1740RawData.h"

#include <iomanip>
#include <iostream>

void TV1740RawData::HandlUncompressedData(){

  // Skip the header.
  uint32_t iPtr=4;
  uint32_t chgrMask    = GetData32()[1] & 0xFF;
  // const  uint32_t *fData = GetData32();
  // printf("%ld %ld %ld %ld \n",fData[0],fData[1],fData[2],fData[3]);

  int nActiveGroup=0;
  for(int iGr=0; iGr<8; iGr++){
    if(chgrMask & (1<<iGr))
      nActiveGroup++;
  }
  // Assume that we have readout the same number of samples for each Group and each Channel.
  // The number of 32 bit double-samples per channel is then
  // N32samples = (bank size - 4)/ ( 8 * nActiveGroup)
  int N32samples = (GetEventSize() - 4)/ (8 * nActiveGroup);

  // Loop over channels
  for(int iGr=0; iGr<8; iGr++)
    {
      if(!(chgrMask & (1<<iGr))){
      // Add empty channel data objects, to keep vector simple.
        for(int iCh=0; iCh<8 ; iCh++) {
          TV1740RawChannel channel(iCh + 8*iGr);
          fMeasurements.push_back(channel);
        }
      continue;
      }

      for(int iCh=0; iCh<8 ; iCh++)
      {

        TV1740RawChannel channel(iCh + 8*iGr);
      
        for(int j = 0; j < N32samples/3; j++){
        uint32_t samp1 = (GetData32()[iPtr]&0xFFF);
        uint32_t samp2 = ((GetData32()[iPtr]>>12)&0xFFF);
        uint32_t samp3 = ((GetData32()[iPtr]>>24)&0xFF) | ((GetData32()[iPtr+1]&0xF)<<8);
        uint32_t samp4 = ((GetData32()[iPtr+1]>>4)&0xFFF);
        uint32_t samp5 = ((GetData32()[iPtr+1]>>16)&0xFFF);
        uint32_t samp6 = ((GetData32()[iPtr+1]>>28)&0xF) | ((GetData32()[iPtr+2]&0xFF)<<4) ;
        uint32_t samp7 = ((GetData32()[iPtr+2]>>8)&0xFFF);
        uint32_t samp8 = ((GetData32()[iPtr+2]>>20)&0xFFF);
        channel.AddADCSample(samp1);
        channel.AddADCSample(samp2);      
        channel.AddADCSample(samp3);      
        channel.AddADCSample(samp4);      
        channel.AddADCSample(samp5);      
        channel.AddADCSample(samp6);      
        channel.AddADCSample(samp7);      
        channel.AddADCSample(samp8);      
        iPtr = iPtr+3;
        }
        fMeasurements.push_back(channel);
      }
    }  
}


TV1740RawData::TV1740RawData(int bklen, int bktype, const char* name, void *pdata):
    TGenericData(bklen, bktype, name, pdata)
{
  
  fGlobalHeader0 = GetData32()[0];
  fGlobalHeader1 = GetData32()[1];
  fGlobalHeader2 = GetData32()[2];
  fGlobalHeader3 = GetData32()[3];
  


    HandlUncompressedData();

}

void TV1740RawData::Print(){

  std::cout << "V1740 decoder for bank " << GetName().c_str() << std::endl;
  std::cout << "Bank size: " << GetEventSize() << std::endl;
  
  std::cout << "Channel Mask : " << GetChannelMask() << std::endl;
   
  std::cout << "Event counter : " << GetEventCounter() << std::endl;
  std::cout << "Trigger tag: " << GetTriggerTag() << std::endl;
  

  std::cout << "Number of channels with data: " << GetNChannels() << std::endl;
  for(int i = 0 ; i < GetNChannels(); i++){

    TV1740RawChannel channelData = GetChannelData(i);

    std::cout << "Channel: " << channelData.GetChannelNumber() << std::endl;
    
  }

}
