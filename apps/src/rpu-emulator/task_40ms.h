
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "timer_lib.h"
int init_task_40ms(void* user_data);
void task_40ms(size_t timer_id, void* user_data);