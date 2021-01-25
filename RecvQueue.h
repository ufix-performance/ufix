#ifndef _UFIX_RECV_QUEUE_
#define _UFIX_RECV_QUEUE_

#include "Locker.h"
#include "RMessage.h"
#include "Cmds.h"
#include "Logger.h"

namespace ufix {
  class RecvQueue : Locker {
  
  private:
    Logger * q_logger;
    
    Cmds * cmds;

    RMessage * msgs_recv;
    int q_size;
    int head;
    
    ULONG head_seq;
    
    ULONG tail_seq;

    int has_event();
    int is_empty(int index);
    int add_resend_request();

  public:
    RecvQueue(int size, Cmds * reqs, Logger * logger);
    
    void set_head_seq(ULONG seq);

    void put(RMessage * msg);

    void update_seq(ULONG current_seq_no, ULONG new_seq_no);

    RMessage * get();
  };
}
#endif
