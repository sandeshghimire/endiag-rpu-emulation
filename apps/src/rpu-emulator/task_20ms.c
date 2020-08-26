#include "task_20ms.h"

int init_task_20ms(void* user_data) { return 0; }

void task_20ms(size_t timer_id, void* user_data) {
  printf("20 ms timer expired. (%lu)\n", timer_id);
}