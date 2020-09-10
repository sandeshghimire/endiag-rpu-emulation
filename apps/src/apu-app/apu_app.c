
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

#define __DEBUG__ 0

#undef ns
#define ns(x) FLATBUFFERS_WRAP_NAMESPACE(rpu, x)  // Specified in the schema

#define READ_DATA_SIZE 0x1024
#define WRITE_DATA_SIZE 0x1024

int debug = 1, fd = 0, ppos = 0;

unsigned int valid_flag = 0;
unsigned int receive_counter = 0;
unsigned int send_counter = 0;

int main(int argc, char **argv) {
  int write_length = 0;
  ssize_t read_bytes = 0;
  ssize_t write_bytes = 0;

  int ret;
  char *buffer;
  size_t size;
  int i;

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
  static unsigned long int frame = 0;
  for (;;) {
    system("clear");
    printf("T%lu\n", frame++);
    buffer = (char *)malloc(READ_DATA_SIZE * sizeof(char));
    memset(buffer, 0x00, READ_DATA_SIZE);
    read_bytes = read(fd, buffer, READ_DATA_SIZE);
    // printf("reading  bytes %lu done  \n\n\n", read_bytes);
    printf("Reading from RPU :: \n");

    ns(rpu_msg_table_t) mesg = ns(
        rpu_msg_as_root_with_identifier(buffer, ns(rpu_msg_file_identifier)));

    if (!mesg) {
      printf("the data is  not avaliable\n");
#if __DEBUG__
      for (i = 0; i < read_bytes; i++) {
        printf("%X ", buffer[i]);
      }
      printf("\n");
#endif /*__DEBUG__*/

    } else {
#if __DEBUG__
      printf("the data is  not avaliable\n");
      for (i = 0; i < read_bytes; i++) {
        printf("%X ", buffer[i]);
      }
      printf("\n");
#endif /*__DEBUG__*/

      valid_flag = ns(rpu_msg_valid_flag(mesg));
      receive_counter = ns(rpu_msg_receive_counter(mesg));
      send_counter = ns(rpu_msg_send_counter(mesg));
      send_counter = receive_counter + 1;

      printf("valid_flag %X\n", valid_flag);
      printf("receive_counter %X\n", receive_counter);
      printf("send_counter %X\n\n", send_counter);

      if (valid_flag) {
        printf("Writing to RPU :: \n");

        valid_flag = 0;
        flatcc_builder_init(apu_message);
        apu_apu_msg_start_as_root(apu_message);

        send_counter = receive_counter + 1;
        receive_counter = send_counter + 1;
        apu_apu_msg_valid_flag_add(apu_message, valid_flag);
        apu_apu_msg_receive_counter_add(apu_message, receive_counter);
        apu_apu_msg_send_counter_add(apu_message, send_counter);
        rpu_rpu_msg_end_as_root(apu_message);

        printf("valid_flag %X\n", valid_flag);
        printf("receive_counter %X\n", receive_counter);
        printf("send_counter %X\n\n", send_counter);

        buffer = flatcc_builder_finalize_buffer(apu_message, &size);
        printf("Sending bytes %lu\n", size);
        write_bytes = write(fd, buffer, size);
        flatcc_builder_clear(apu_message);
        printf("writing done  \n\n\n");
      }
    }
    sleep(1);
  }
  close(fd);
  return 0;
}