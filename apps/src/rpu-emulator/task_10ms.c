#include "task_10ms.h"

#include "mmap_lib.h"

int init_task_10ms(void* user_data) { return 0; }

void task_10ms(size_t timer_id, void* user_data) {
  if (get_receive_flag()) {
    static unsigned int counter;
    printf("10 ms Task\n");
    printf("Rading from APU %d\n", get_receive_flag());
    set_send_counter(counter++);
    printf("Sending to APU %d\n", counter);
  }
}