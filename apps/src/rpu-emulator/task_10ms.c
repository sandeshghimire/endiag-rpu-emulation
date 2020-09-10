#include "task_10ms.h"

#include "../message/rpu_apu_msg_generated.h"
#include "mmap_lib.h"

int init_task_10ms(void* user_data) {
  printf("Init 10 ms Task\n");
  return 0;
}
static unsigned long int frame = 0;
void task_10ms(size_t timer_id, void* user_data) {
  if (1) {
    // static unsigned int counter;

    system("clear");
    printf("T%lu\n", frame++);
    printf("Reading from APU :: \n");
    rx_fb_message();

    printf("\n\n");
    printf("Writing to  APU :: \n");
    set_send_counter(get_receive_counter() + 1);
    set_receive_flag();
    tx_fb_message();
  }
}