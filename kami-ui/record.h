#ifndef RECORD_H_
#define RECORD_H_
#include <packet.h>

typedef enum recording_state_t {
  RECORDING_DISABLED,
  RECORDING_STARTED,
  RECORDING_ENABLED,
  RECORDING_MAX
} recording_state_t;

typedef struct recorder_s {
  FILE *output_fd;
  enum recording_state_t state;
} recorder_t;

typedef enum recording_error_t {
  RECORDING_OPEN_FILE_ERR,
  RECORDING_WRITE_FILE_ERR,
  RECORDING_CLOSE_FILE_ERR,
  RECORDING_OK,
  RECORDING_ERR_MAX
} recording_error_t;

inline recording_state_t getRecorderState (const recorder_t *recorder)
{ return recorder->state; }

void initRecorder(recorder_t *recorder);
void recordButtonEvent(recorder_t *recorder);
recording_error_t recordData(recorder_t *recorder, const pkt_generic_t *data,
    const float *rec_params);
#endif
