#include <coop.h>
#include <stdbool.h>

/*
typedef enum coop_task_index_e {
  TASK_MAX
} coop_task_index_t;



static coop_task_t coop_task_list[TASK_MAX] = {

};

coop_task_t *coop_getTaskList() {
  return coop_task_list;
}

size_t coop_getNumTasks() {
  return TASK_MAX;
}
*/

/*
 * tasks countdown so you don't have to deal with overflow
 * Put this in a timer interrupt
 */
void coop_scheduleTasks(coop_task_list_t *tasks) {
  for (size_t i = 0; i < tasks->num_tasks; ++i) {
    if (tasks->task_arr[i].countdown == 0) {
      tasks->task_arr[i].scheduled = true;

      //always schedule period 0 tasks
      tasks->task_arr[i].countdown =
        (tasks->task_arr[i].period == 0) ? 0 : tasks->task_arr[i].period - 1;
    }
    else
      tasks->task_arr[i].countdown--;
  }
}

/*
 * execute tasks that are scheduled to run
 * put this in the main loop
 */
void coop_executeTasks(coop_task_list_t *tasks, coop_args_t *args) {
  for (size_t i = 0; i < tasks->num_tasks; ++i) {
    if (tasks->task_arr[i].scheduled == true) {
      (*tasks->task_arr[i].task_func)(args);
      tasks->task_arr[i].scheduled = false;
    }
  }
}

//function that combines the above into one
void coop_schedExecuteTasks(coop_task_list_t *tasks, coop_args_t *args) {
  for (size_t i = 0; i < tasks->num_tasks; ++i) {
    if (tasks->task_arr[i].countdown == 0) {
      (*tasks->task_arr[i].task_func)(args);
      //always schedule period 0 tasks
      tasks->task_arr[i].countdown =
        (tasks->task_arr[i].period == 0) ? 0 : tasks->task_arr[i].period - 1;
    }
    else
      tasks->task_arr[i].countdown--;
  }
}
