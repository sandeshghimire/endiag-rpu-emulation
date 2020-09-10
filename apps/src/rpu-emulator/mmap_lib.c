#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#undef ns
#define ns(x) FLATBUFFERS_WRAP_NAMESPACE(apu, x)  // Specified in the schema

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
char *rx_buffer;
char *tx_buffer;
size_t size;

static unsigned int valid_flag = 0x01;
static unsigned int receive_counter = 0x00;
static unsigned int send_counter = 0x00;

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

  apu_message = &apu_builder;
  flatcc_builder_init(apu_message);

  rx_buffer = (unsigned char *)(kadr + RPU_MESSAGE_OFFSET);
  tx_buffer = (unsigned char *)(kadr + APU_MESSAGE_OFFSET);

  return 0;
}

void rx_fb_message() {
  ns(apu_msg_table_t) apu_msg = ns(apu_msg_as_root(rx_buffer));

  if (!apu_msg) {
    printf("message is not available \n");
  } else {
    // memcpy(&valid_flag, read_data, 4);
    valid_flag = ns(apu_msg_valid_flag(apu_msg));
    printf("valid_flag %X\n", valid_flag);

    // memcpy(&receive_counter, read_data + 4, 4);
    receive_counter = ns(apu_msg_receive_counter(apu_msg));
    printf("receive_counter %X\n", receive_counter);

    // memcpy(&send_counter, read_data + 8, 4);
    send_counter = ns(apu_msg_send_counter(apu_msg));
    printf("send_counter %X\n\n", send_counter);
    memset(rx_buffer, 0x00, READ_DATA_SIZE);
  }
}

void tx_fb_message() {
  rpu_message = &rpu_builder;
  flatcc_builder_init(rpu_message);
  void *buf;
  size_t size;
  rpu_rpu_msg_start_as_root(rpu_message);
  // memcpy(write_data, &valid_flag, 4);
  printf("valid_flag %X\n", valid_flag);
  rpu_rpu_msg_valid_flag_add(rpu_message, valid_flag);

  // memcpy(write_data + 4, &receive_counter, 4);
  printf("receive_counter %X\n", receive_counter);
  rpu_rpu_msg_receive_counter_add(rpu_message, receive_counter);

  // memcpy(write_data + 8, &send_counter, 4);

  send_counter = receive_counter + 1;
  printf("send_counter %X\n\n", send_counter);
  rpu_rpu_msg_send_counter_add(rpu_message, send_counter);

  rpu_rpu_msg_end_as_root(rpu_message);

  buf = flatcc_builder_get_direct_buffer(rpu_message, &size);
  memcpy(tx_buffer, buf, size);
  printf("Sending bytes %lu\n", size);
  flatcc_builder_clear(rpu_message);
}

int cleanup_mmap_lib() {
  munmap(kadr, MAX_SIZE);
  close(fd);
  return 0;
}

unsigned int get_receive_flag() { return valid_flag; }
unsigned int get_receive_counter() { return receive_counter; }
unsigned int get_send_counter() { return send_counter; }

void set_send_counter(unsigned int counter) { send_counter = counter; }
void clear_receive_flag() { valid_flag = 0x00; }
void set_receive_flag() { valid_flag = 0x01; }

void clear_buffer() { memset(kadr, 0x00, MAX_SIZE / 4); }

void set_tx_message_ready() { kadr[0] = 1; }
unsigned int get_rx_message_ready() { return (kadr[1]); }