#ifndef MMAP_LIB_H
#define MMAP_LIB_H

int init_mmap_lib();
int cleanup_mmap_lib();

void set_tx_message_ready();
unsigned int get_rx_message_ready();

unsigned int get_receive_flag();
void clear_receive_flag();
void set_receive_flag();

unsigned int get_receive_counter();
unsigned int get_send_counter();
void set_send_counter(unsigned int counter);

void rx_fb_message();
void tx_fb_message();

void clear_buffer();

#endif  // !MMAP_LIB_H
