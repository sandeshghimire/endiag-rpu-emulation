
#include "apu_app.h"

#include <fcntl.h>
#include <flatcc/support/hexdump.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../message/apu_rpu_msg_generated.h"
#include "../message/rpu_apu_msg_generated.h"

#define DEVICE "/dev/rpu_emulator"

#define READ_DATA_SIZE 0x1024
#define WRITE_DATA_SIZE 0x1024

int debug = 1, fd = 0, ppos = 0;

unsigned int valid_flag = 0;
unsigned int receive_counter = 0;
unsigned int send_counter = 0;

int main(int argc, char **argv) {
  int write_length = 0;
  ssize_t read_bytes;
  ssize_t write_bytes;

  int ret;
  void *buffer;
  size_t size;

  flatcc_builder_t rpu_builder, *rpu_message;
  flatcc_builder_t apu_builder, *apu_message;

  (void)argc;
  (void)argv;

  if (access(DEVICE, F_OK) == -1) {
    printf("module %s not loaded\n", DEVICE);
    return 0;
  }

  fd = open(DEVICE, O_RDWR);
  if (fd < 0) {
    printf("module %s cannot open \n", DEVICE);
  }

  rpu_message = &rpu_builder;
  flatcc_builder_init(rpu_message);

  apu_message = &apu_builder;
  flatcc_builder_init(apu_message);

#if 0
  unsigned char *read_data =
      (unsigned char *)malloc(READ_DATA_SIZE * sizeof(char));

  unsigned char *write_data =
      (unsigned char *)malloc(WRITE_DATA_SIZE * sizeof(char));
#endif

  for (;;) {
    buffer = (unsigned char *)malloc(READ_DATA_SIZE * sizeof(char));
    memset(buffer, 0x00, READ_DATA_SIZE);

    read_bytes = read(fd, buffer, READ_DATA_SIZE);

    printf("reading  bytes %lu done  \n\n\n", read_bytes);

    printf("Reading from RPU :: \n");

    rpu_rpu_msg_table_t rpu_msg = rpu_rpu_msg_as_root(buffer);

    // memcpy(&valid_flag, read_data, 4);
    valid_flag = rpu_rpu_msg_valid_flag_get(rpu_msg);
    printf("valid_flag %X\n", valid_flag);

    // memcpy(&receive_counter, read_data + 4, 4);
    receive_counter = rpu_rpu_msg_receive_counter_get(rpu_msg);
    printf("receive_counter %X\n", receive_counter);

    // memcpy(&send_counter, read_data + 8, 4);
    send_counter = rpu_rpu_msg_send_counter_get(rpu_msg);
    printf("send_counter %X\n\n", send_counter);

    send_counter = receive_counter + 1;

    if (valid_flag) {
      printf("Writing to RPU :: \n");
      valid_flag = 0;
      apu_apu_msg_start_as_root(apu_message);
      // memset(write_data, 0x00, WRITE_DATA_SIZE);
      send_counter = receive_counter + 1;

      // memcpy(write_data, &valid_flag, 4);
      printf("valid_flag %X\n", valid_flag);
      apu_apu_msg_valid_flag_add(apu_message, valid_flag);

      // memcpy(write_data + 4, &receive_counter, 4);
      printf("receive_counter %X\n", receive_counter);
      apu_apu_msg_receive_counter_add(apu_message, receive_counter);

      // memcpy(write_data + 8, &send_counter, 4);
      printf("send_counter %X\n\n", send_counter);
      apu_apu_msg_send_counter_add(apu_message, send_counter);

      buffer = flatcc_builder_finalize_buffer(apu_message, &size);

      write_bytes = write(fd, buffer, size);

      if (write_bytes == size) {
        printf("writing done  \n\n\n");
      }
    }
    sleep(1);
  }

  close(fd);

  return 0;
}