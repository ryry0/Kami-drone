#include <stdio.h>
#include <ctype.h>
#include "fsm.h"

typedef struct keyboard_s keyboard_t;

struct keyboard_s {
  fsm_t super; /* extend the fsm class */

  /* other attributes of keyboard */
};

void keyboard_Ctor(keyboard_t *board);

void keyboard_initial(keyboard_t *board, fsm_event_t const *event);
void keyboard_default(keyboard_t *board, fsm_event_t const *event);
void keyboard_shifted(keyboard_t *board, fsm_event_t const *event);

typedef struct keyboard_event_s keyboard_event_t;

struct keyboard_event_s {
  fsm_event_t super; /* extend the event class */
  char keycode;
};

/* signals used by keyboard fsm */

enum {
  SHIFT_DEPRESSED_SIG,
  SHIFT_RELEASED_SIG,
  ANY_KEY_SIG
};

void keyboard_Ctor(keyboard_t *board) {
  fsm_Ctor_(&board->super, &keyboard_initial);
}

void keyboard_initial(keyboard_t *board, fsm_event_t const *event) {
  /* ..initialization of keyboard attributes */
  printf("keyboard initialized\n");
  fsm_Tran((fsm_t *) board, &keyboard_default);
}

void keyboard_default(keyboard_t *board, fsm_event_t const *event) {
  switch (event->signal) {
    case SHIFT_DEPRESSED_SIG:
      printf("default blah");
      fsm_Tran((fsm_t *) board, &keyboard_shifted);
      break;

    case ANY_KEY_SIG:
      printf("key %c", (char)tolower(((keyboard_event_t *)event)->keycode));
      break;
  }

}

void keyboard_shifted(keyboard_t *board, fsm_event_t const *event) {
  switch (event->signal) {
    case SHIFT_RELEASED_SIG:
      printf("shift is released");
      fsm_Tran((fsm_t *)board, &keyboard_default);
      break;
    case ANY_KEY_SIG:
      printf("key %c", (char)toupper(((keyboard_event_t *)event)->keycode));
      break;
  }
}

int main(int argc, char ** argv) {
  keyboard_t k;
  keyboard_Ctor(&k);
  fsm_Init((fsm_t *)&k, NULL);

  for(;;) {
    keyboard_event_t ke;
    printf("\nSignal<-");
    ke.keycode = getc(stdin);
    getc(stdin); /* discard '\n' */

    switch (ke.keycode) {
      case '^': ke.super.signal = SHIFT_DEPRESSED_SIG; break;
      case '6': ke.super.signal = SHIFT_RELEASED_SIG; break;
      case '.': return 0; /* terminate test */
      default: ke.super.signal = ANY_KEY_SIG; break;
    }
    fsm_Dispatch((fsm_t *) &k, (fsm_event_t *) &ke); /* dispatch */
  }
  return 0;
}
