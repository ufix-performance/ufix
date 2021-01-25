#include "utils.h"

namespace ufix {
  class SentMark {
  
  private:
    int mark_size;
    seq_sent_info * marks;
    
  public:
    
    SentMark(int size);
    void set_mark(int q_index, ULONG msg_seq, ULONG data_seq);
    seq_sent_info * get_mark(ULONG seq);
    seq_sent_info * get_last_mark(ULONG seq);
  };
}
