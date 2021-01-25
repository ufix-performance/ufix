#ifndef _UFIX_P_MANAGER_
#define _UFIX_P_MANAGER_

#include <stdio.h>

#include "constants.h"
#include "SentMark.h"

namespace ufix {

typedef FILE * FILEPtr;

  class PManager {

  private:
    
    char dir[1024]; 
    int dir_len;
    char q_dir[1024];
    int q_dir_len;

    FILE * seqs_processed;
    FILE * seqs_sent;
    FILEPtr * data_queue;
    FILE * system_log;

  public:

    PManager(const char * p_dir, const char * s_dir, int num_send_queues);
    
    FILE * get_system_log();

    FILE * init_data_queue(int q_index);
    
    FILE * get_data_recover_file(int q_index);
    
    void close(FILE * file);

    void write_seq_sent(int q_index, ULONG seq, ULONG data_seq);
    
    void write_seq_processed(ULONG seq);
    
    ULONG read_last_seq_processed();
    
    ULONG read_sent_marks(SentMark * marks, ULONG * last_seqs);
  };
}
#endif
