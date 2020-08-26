
#include "apu_app.h"

#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DEVICE "/dev/rpu_emulator"

#define READ_DATA_SIZE 0x10
#define WRITE_DATA_SIZE 0x10

int debug = 1, fd = 0, ppos = 0;

unsigned int valid_flag = 0;
unsigned int receive_counter = 0;
unsigned int send_counter = 0;

int main(int argc, char **argv) {
  int write_length = 0;

  ssize_t read_bytes;
  ssize_t write_bytes;

  if (access(DEVICE, F_OK) == -1) {
    printf("module %s not loaded\n", DEVICE);
    return 0;
  }

  fd = open(DEVICE, O_RDWR);
  if (fd < 0) {
    printf("module %s cannot open \n", DEVICE);
  }

  unsigned char *read_data =
      (unsigned char *)malloc(READ_DATA_SIZE * sizeof(char));

  unsigned char *write_data =
      (unsigned char *)malloc(WRITE_DATA_SIZE * sizeof(char));

  for (;;) {
    memset(read_data, 0x00, READ_DATA_SIZE);
    read_bytes = read(fd, read_data, READ_DATA_SIZE);

    memcpy(&valid_flag, read_data, 4);
    printf("valid_flag %X\n", valid_flag);

    memcpy(&receive_counter, read_data + 4, 4);
    printf("receive_counter %X\n", receive_counter);

    memcpy(&send_counter, read_data + 8, 4);
    printf("send_counter %X\n\n", send_counter);

    send_counter = receive_counter + 1;

    if (valid_flag) {
      valid_flag = 0;

      memset(write_data, 0x00, WRITE_DATA_SIZE);
      send_counter = receive_counter + 1;

      memcpy(write_data, &valid_flag, 4);
      printf("valid_flag %X\n", valid_flag);

      memcpy(write_data + 4, &receive_counter, 4);
      printf("receive_counter %X\n", receive_counter);

      memcpy(write_data + 8, &send_counter, 4);
      printf("send_counter %X\n\n", send_counter);

      write_bytes = write(fd, write_data, WRITE_DATA_SIZE);

      if (write_bytes == WRITE_DATA_SIZE) {
        printf("writing done  \n");
      }
    }
    sleep(1);
  }

  close(fd);

  return 0;
}