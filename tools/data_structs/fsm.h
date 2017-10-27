/** @filex
 * @authoy Ryan-David Reyes */
#ifndef FSM_H_
#define FSM_H_

#include <stdint.h>
typedef uint16_t  fsm_signal_t;

typedef struct fsm_event_s fsm_event_t;
typedef struct fsm_s fsm_t;

/** @brief State is represented as a function pointer. */
typedef void (*fsm_state_t)(fsm_t*, fsm_event_t const *);

/** @brief Finite state machine event base class. */
struct fsm_event_s {
  fsm_signal_t signal;
};

/** @brief Finite state machine base class */
struct fsm_s {
  fsm_state_t state;
};

/** @brief This macro constructs the state machine. Should only be called by
 * state machine internals.
 *
 * This macro takes a state machine and an initial state, and sets the initial
 * state of the fsm to init_.
 *
 * @param fsm_ The finite state machine object pointer
 * @param init_ The initial state that the fsm_ object is set to.
 */
#define fsm_Ctor_(fsm_, init_) ((fsm_)->state = (fsm_state_t)(init_))

/** @brief This macro triggers the initial state transition. Function to be used
 * by clients of state machine.
 *
 * This macro takes a state machine and an event, and applies the event to the
 * state machine.
 *
 * @param fsm_ The finite state machine object pointer
 * @param event_ The event to apply
 */
#define fsm_Init(fsm_, event_) (*(fsm_)->state)((fsm_), (event_))

/** @brief This macro triggers a state transition. Function to be used by
 * clients of state machine.
 *
 * This macro takes a state machine and an event, and applies the event to the
 * state machine.
 *
 * @param fsm_ The finite state machine object pointer
 * @param event_ The event to apply
 */
#define fsm_Dispatch(fsm_, event_) (*(fsm_)->state)((fsm_), (event_))


/** @brief This macro changes the internal state pointer to point to another
 * state.
 *
 * This should be used internally by the state machine to implement state
 * transitions.
 *
 * @param fsm_ The finite state machine object pointer
 * @param targ_ The target state
 */
#define fsm_Tran(fsm_, targ_) ((fsm_)->state = (fsm_state_t)(targ_))
#endif
