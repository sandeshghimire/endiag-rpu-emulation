#include "rpu_emulator.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mmap_lib.h"
#include "task_10ms.h"
#include "task_20ms.h"
#include "task_40ms.h"
#include "timer_lib.h"

int main(int argc, char** argv) {
  size_t timer1, timer2, timer3;
  initialize();

  init_task_10ms(NULL);
  init_task_10ms(NULL);
  init_task_10ms(NULL);

  if (init_mmap_lib() > 0) {
    printf("failed to init mmap interface \n");
    return -1;
  }

  timer1 = start_timer(500, task_10ms, TIMER_PERIODIC, NULL);
  // timer2 = start_timer(100, task_20ms, TIMER_PERIODIC, NULL);
  // timer3 = start_timer(2000, task_40ms, TIMER_PERIODIC, NULL);

  for (;;) {
  }

  stop_timer(timer1);
  // stop_timer(timer2);
  // stop_timer(timer3);

  finalize();

  return 0;
}
