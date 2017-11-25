#include <record.h>
#include <time.h>
#include <ctype.h>


void initRecorder(recorder_t *recorder) {
  recorder->output_fd = NULL;
  recorder->state = RECORDING_DISABLED;
}

void recordButtonEvent(recorder_t *recorder) {
  switch (recorder->state) {
    case RECORDING_DISABLED:
      recorder->state = RECORDING_STARTED;
      break;
    case RECORDING_STARTED:
      break;
    case RECORDING_ENABLED:
      recorder->state = RECORDING_DISABLED;
      break;
    default:
      break;
  }
}

recording_error_t recordData(recorder_t *recorder, const char *string_to_write,
    size_t length) {

  char buff[100];
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  switch (recorder->state) {
    case RECORDING_DISABLED:
      if (recorder->output_fd) {
        if (fclose(recorder->output_fd) < 0)
          return RECORDING_CLOSE_FILE_ERR;
        recorder->output_fd = NULL;
      }
      break;

    case RECORDING_STARTED:
      //make a file
      sprintf(buff, "%02d-%02d-%d_%02dh%02dm%02ds_trial.csv", tm.tm_mon +1, tm.tm_mday,
          tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

      recorder->output_fd = fopen(buff, "w");
      if (recorder->output_fd == NULL) {
        recorder->state = RECORDING_DISABLED;
        return RECORDING_OPEN_FILE_ERR;
      }

      recorder->state = RECORDING_ENABLED;

      // FALLS THROUGH
    case RECORDING_ENABLED:
      if (fprintf(recorder->output_fd, string_to_write) < 0)
          return RECORDING_WRITE_FILE_ERR;

      break;
    default:
      break;
  }
  return RECORDING_OK;
}
