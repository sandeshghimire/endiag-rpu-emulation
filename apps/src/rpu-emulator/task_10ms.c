#include "task_10ms.h"

#include "../message/rpu_apu_msg_generated.h"
#include "mmap_lib.h"

int init_task_10ms(void* user_data) { return 0; }

void task_10ms(size_t timer_id, void* user_data) {
  printf("10 ms Task\n");
  // if (get_receive_flag()) {

  if (1) {
    static unsigned int counter;
    printf("10 ms Task\n");

    printf("Reading valid flag %X\n", get_receive_flag());
    printf("Rading rx counter  %X\n", get_receive_counter());
    set_send_counter(get_receive_counter() + 1);
    set_receive_flag();
    printf("Sending to tx counter  %X\n", get_receive_counter() + 1);
  }
}