#include "utils.h"
#include "PManager.h"

#define SEQS_PROCESSED "seqs_p"
#define SEQS_SENT "seqs_s"
#define SYSTEM_LOG "session.log"

namespace ufix {
  PManager::PManager(const char * p_dir, const char * s_dir, int num_send_queues) {
    
    dir_create(p_dir);
    
    dir_len = sprintf(dir, "%s%c%s", p_dir, DIR_DELIM, s_dir); 
    dir_create(dir);
    
    q_dir_len = sprintf(q_dir, "%s%c%s", dir, DIR_DELIM, "queue");                 
    dir_create(q_dir);

    sprintf(dir + dir_len, "%c", DIR_DELIM);
    dir_len++;

    sprintf(dir + dir_len, "%s", SEQS_PROCESSED);
    seqs_processed = fopen(dir, "ab");
    
    sprintf(dir + dir_len, "%s", SEQS_SENT);
    seqs_sent = fopen(dir, "ab");

    sprintf(dir + dir_len, "%s", SYSTEM_LOG);
    system_log = fopen(dir, "ab");
    
    data_queue = (FILEPtr *) mem_alloc(num_send_queues*sizeof(FILEPtr));
  }
  
  FILE * PManager::get_system_log() {
    return system_log;
  }

  FILE * PManager::init_data_queue(int q_index) {
    sprintf(q_dir + q_dir_len, "%c%d", DIR_DELIM, q_index);
    data_queue[q_index] = fopen(q_dir, "ab");
    return data_queue[q_index];
  }
  
  FILE * PManager::get_data_recover_file(int q_index) {
    sprintf(q_dir + q_dir_len, "%c%d", DIR_DELIM, q_index);
    return fopen(q_dir, "r");
  }
  
  void PManager::close(FILE * file) {
    if (file != NULL) {
      fclose(file);
    }
  }

  void PManager::write_seq_processed(ULONG seq) {
    fwrite((const char *) &seq, sizeof(char), sizeof(ULONG), seqs_processed);
    fflush(seqs_processed);
  }

  void PManager::write_seq_sent(int q_index, ULONG seq, ULONG data_seq) {
    int buf[1 + 2*sizeof(ULONG)/sizeof(int)];
    buf[0] = q_index;
    ULONG * seqs = (ULONG *) (buf + 1);
    seqs[0] = seq;
    seqs[1] = data_seq;
    fwrite((const char *) buf, sizeof(char), sizeof(int) + 2*sizeof(ULONG), seqs_sent);
    fflush(seqs_sent);
  }

  ULONG PManager::read_last_seq_processed() {
    sprintf(dir + dir_len, "%s", SEQS_PROCESSED);
    FILE * file_seqs_processed = fopen(dir, "rb");
    fseek(file_seqs_processed, 0, SEEK_END);
    int size = ftell(file_seqs_processed);
    if (size == 0) return 0;
    fseek(file_seqs_processed, 0 - sizeof(ULONG), SEEK_CUR);
    char buf[8];
    fread(buf, sizeof(char), sizeof(ULONG), file_seqs_processed);
    fclose(file_seqs_processed);
    return *((ULONG *) buf);
  }

  ULONG PManager::read_sent_marks(SentMark * marks, ULONG * last_seqs) {
    sprintf(dir + dir_len, "%s", SEQS_SENT);
    FILE * file_seqs_sent = fopen(dir, "rb");
    char buf[1 << 17];
    int max_read_size = (sizeof(int) + 2*sizeof(ULONG))*(1 << 12);
    
    ULONG last_seq = 0;
    int last_q_index = 0;

    while (true) {
      int len = fread(buf, sizeof(char), max_read_size, file_seqs_sent);
      int pos = 0;
      while (true) {
	if (pos >= len) break;
	int * info = (int *) (buf + pos); 
	int q_index = info[0];
	pos += sizeof(int);
	ULONG * seqs = (ULONG *) (buf + pos);
	marks->set_mark(q_index, seqs[0], seqs[1]);
	if (q_index != last_q_index) {
          last_seqs[last_q_index] += (seqs[0] - 1 - last_seq);
	  last_q_index = q_index;
	}
	last_seqs[q_index] = seqs[1];
	last_seq = seqs[0];
	pos += 2*sizeof(ULONG);
      }
      if (len != max_read_size) break;
    }
    fclose(file_seqs_sent);
    return last_seq;
  }
}
