#ifndef _U_FIX_UTILS_ 
#define _U_FIX_UTILS_ 

#include "portable.h"

namespace ufix {
#define convert_itoc(x) (x + 48)
#define convert_ctoi(x) (x - 48)

struct field_info {
  ULONG counter;
  int tag;
  const char * val;
  int val_len;
  field_info * next;
};

struct seq_sent_info {
  int q_index;
  ULONG msg_seq;
  ULONG data_seq;
};

struct send_queue_info {
  ULONG data_seq;
  char * data;
  int len;
};

void * mem_alloc(int size);

int convert_itoa(int value, char * buf);
int convert_itoa(int value, char * buf, int buf_end);
  
int convert_ltoa(long long value, char * buf);
int convert_ltoa(long long value, char * buf, int buf_end);

ULONG convert_atoll(const char * buf, int len);
ULONG convert_atoll(const char * buf);  
int convert_atoi(const char * buf, int len);
int convert_atoi(const char * buf);

int format_time(char * time_buf);

ULONG get_current_timestamp();

void fatal(const char * msg);
 
void error(const char * msg);
 
void warn(const char * msg);

int is_char_end(char val);

int is_session_msg(const char * msg_type);

int str_size(const char * str);

int str_match(const char * src_val, const char * dest_val); 

int seek(const char * data, int data_len, const char * pattern, int pattern_len);

void dir_create(const char * name); 

void poll_wait(int fd, int events, int timeout);
}

#endif
