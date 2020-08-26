#include "task_40ms.h"

int init_task_40ms(void* user_data) { return 0; }

void task_40ms(size_t timer_id, void* user_data) {
  printf("40 ms timer expired. (%lu)\n", timer_id);
}