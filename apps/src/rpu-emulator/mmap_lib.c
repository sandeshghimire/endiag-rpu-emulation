#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../message/apu_rpu_msg_generated.h"
#include "../message/rpu_apu_msg_generated.h"

#define NPAGES 16
#define MAX_SIZE 4096

#define READ_DATA_SIZE 0x100
#define WRITE_DATA_SIZE 0x100

#define METADATA_OFFSET 0x00
#define APU_MESSAGE_OFFSET 0x100
#define RPU_MESSAGE_OFFSET 0x200

static int fd;
unsigned int *kadr;

int ret;
void *rx_buffer;
void *tx_buffer;
size_t size;

static unsigned int valid_flag = 0;
static unsigned int receive_counter = 0;
static unsigned int send_counter = 0;

flatcc_builder_t rpu_builder, *rpu_message;
flatcc_builder_t apu_builder, *apu_message;

int init_mmap_lib() {
  int len = NPAGES * getpagesize();

  if ((fd = open("/dev/rpu_emulator", O_RDWR | O_SYNC)) < 0) {
    perror("failed to open memap interace");
    return -1;
  }

  kadr = (unsigned int *)mmap(0, MAX_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
                              fd, 0);

  if (kadr == MAP_FAILED) {
    perror("failed to mmap");
    return -1;
  }
  memset(kadr, 0x00, 0x1000);

  fprintf(stderr, "mmap_alloc: mmap OK\n");

  rpu_message = &rpu_builder;
  flatcc_builder_init(rpu_message);

  apu_message = &apu_builder;
  flatcc_builder_init(apu_message);

  rx_buffer = (unsigned char *)(kadr + APU_MESSAGE_OFFSET);
  tx_buffer = (unsigned char *)(kadr + RPU_MESSAGE_OFFSET);

  return 0;
}

void rx_fb_message() {
  apu_apu_msg_table_t apu_msg = apu_apu_msg_as_root(rx_buffer);

  // memcpy(&valid_flag, read_data, 4);
  valid_flag = apu_apu_msg_valid_flag_get(apu_msg);
  printf("valid_flag %X\n", valid_flag);

  // memcpy(&receive_counter, read_data + 4, 4);
  receive_counter = apu_apu_msg_receive_counter_get(apu_msg);
  printf("receive_counter %X\n", receive_counter);

  // memcpy(&send_counter, read_data + 8, 4);
  send_counter = apu_apu_msg_send_counter_get(apu_msg);
  printf("send_counter %X\n\n", send_counter);
}

void tx_fb_message() {
    rpu_rpu_msg_table_t rpu_msg = rpu_rpu_msg_as_root(tx_buffer);
  // memcpy(write_data, &valid_flag, 4);
  printf("valid_flag %X\n", valid_flag);
  rpu_rpu_msg_valid_flag_add(rpu_message, valid_flag);

  // memcpy(write_data + 4, &receive_counter, 4);
  printf("receive_counter %X\n", receive_counter);
  rpu_rpu_msg_receive_counter_add(rpu_message, receive_counter);

  // memcpy(write_data + 8, &send_counter, 4);
  printf("send_counter %X\n\n", send_counter);
  rpu_rpu_msg_send_counter_add(rpu_message, send_counter);
}

int cleanup_mmap_lib() {
  munmap(kadr, MAX_SIZE);
  close(fd);
  return 0;
}

unsigned int get_receive_flag() { return valid_flag; }
unsigned int get_receive_counter() { return receive_counter; }

void set_send_counter(unsigned int counter) { send_counter = counter; }
void clear_receive_flag() { valid_flag = 0x00; }
void set_receive_flag() { valid_flag = 0x01; }

void clear_buffer() { memset(kadr, 0x00, MAX_SIZE / 4); }

void set_tx_message_ready() { kadr[0] = 1; }
unsigned int get_rx_message_ready() { return (kadr[1]); }