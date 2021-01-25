#include "portable.h"

namespace ufix {

#define NUM_TIMESTAMP 8

  class Clock {
    
  private:
    char * utc_time[NUM_TIMESTAMP];
    ULONG timestamp[NUM_TIMESTAMP];
    int current_index;
    
  public:

    Clock();
    const char * get_utc_time();
    void update_utc_time();
    ULONG get_timestamp();
  };
}
