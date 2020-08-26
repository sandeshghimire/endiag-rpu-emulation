#ifndef MMAP_LIB_H
#define MMAP_LIB_H

int init_mmap_lib();
int cleanup_mmap_lib();

unsigned int get_receive_flag();
void clear_receive_flag();

unsigned int get_receive_counter();
void set_send_counter(unsigned int counter);

#endif  // !MMAP_LIB_H
