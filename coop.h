/** \file
 * \author Ryan-David Reyes
 *
 * \brief This is an implementation of a cooperative scheduler.
 */
#ifndef COOP_H_
#define COOP_H_
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/** TODO: what I should have is the arguments be void pointers, and the task
 * structure should have a void pointer so that it can have a specific argument.
 */

/** \brief Struct that represents the arguments to the threads */
typedef struct coop_args_s {
    volatile struct exo_s *exo;
} coop_args_t;

/** \brief function signature of a cooperative thread */
typedef void (*coop_task_func_t)(coop_args_t *);

/** \brief Task structure, runs every period ticks */
typedef struct coop_task_s {
  uint32_t period;
  uint32_t countdown;
  bool scheduled;
  coop_task_func_t task_func;
} coop_task_t;

/** \brief Task list structure */
typedef struct coop_task_list_s {
  size_t num_tasks;
  coop_task_t *task_arr;
} coop_task_list_t;

//task_t* coop_getTaskList();
/**
 * \brief Schedule tasks and mark them ready for execution.
 *
 * This should run at a constant interval.
 *
 * \param tasks The list of tasks.
 */
void coop_scheduleTasks(coop_task_list_t *tasks);

/**
 * \brief Execute tasks marked ready.
 *
 * \param tasks The list of tasks.
 * \param args The arguments to pass to these threads.
 */
void coop_executeTasks(coop_task_list_t *tasks, coop_args_t *args);

/**
 * \brief Get number of tasks in the scheduler.
 *
 * \return Number of threads.
 */
size_t coop_getNumTasks();

/**
 * \brief Function that schedules and runs the tasks at the same time.
 */
void coop_schedExecuteTasks(coop_task_list_t *tasks, coop_args_t *args);

#endif
