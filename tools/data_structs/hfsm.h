#ifndef HFSM_H_
#define HFSM_H_
#include "fsm.h"

static fsm_event_t const fsm_entry_evt = { ENTRY_SIG }
static fsm_event_t const fsm_exit_evt = { EXIT_SIG }

void hfsm_Tran_(fsm_t *fsm, fsm_state_t target) {
  fsm_Dispatch(fsm, fsm_exit_evt); /* exit the source, exit evt should call
                                       super last */
  fsm->state = target;
  fsm_Dispatch(fsm, fsm_entry_evt); /* enter the target, entry evt should call
  super first*/
}

void hfsm_Dispatch(fsm_t *fsm, fsm_event_t const *event) {
  fsm->source = fsm->state; // temp variable to hold initial caller

  do {
    fsm->source = (fsm_state_t)(*fsm->source)(fsm, event);
  } while (fsm->source != NULL);
}

#endif
