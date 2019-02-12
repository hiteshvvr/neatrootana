#include "TIMEStamp.h"

#include <iomanip>
#include <iostream>


TIMEStamp::TIMEStamp(int bklen, int bktype, const char* name, void *pdata):
    TGenericData(bklen, bktype, name, pdata)
{
  timeinsec = GetData32()[0];
  timeinnanosec = GetData32()[1];
}


