#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define NPAGES 16
#define MAX_SIZE 4096

static int fd;
char *kadr;

int init_mmap_lib() {
  int len = NPAGES * getpagesize();

  if ((fd = open("/dev/rpu_emulator", O_RDWR | O_SYNC)) < 0) {
    perror("failed to open memap interace");
    return -1;
  }

  kadr = (char *)mmap(0, MAX_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (kadr == MAP_FAILED) {
    perror("failed to mmap");
    return -1;
  }

  fprintf(stderr, "mmap_alloc: mmap OK\n");

  return 0;
}

int cleanup_mmap_lib() {
  munmap(kadr, MAX_SIZE);
  close(fd);
  return 0;
}

unsigned int get_receive_flag() { return (kadr[0]); }

void clear_receive_flag() { kadr[0] = 0x00; }

unsigned int get_receive_counter() { return (kadr[1]); }

void set_send_counter(unsigned int counter) { kadr[2] = counter; }
