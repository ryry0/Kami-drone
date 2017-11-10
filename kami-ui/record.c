#include <record.h>
#include <time.h>
#include <packet_hybrid.h>
#include <ctype.h>


const char *PARAM_STR[] = {
  FOREACH_PARAM(GENERATE_STRING)
};

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

recording_error_t recordData(recorder_t *recorder, const pkt_generic_t *data,
    const float *rec_params) {

  char buff[100];
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  pkt_log_data_t *data_out = pkt_interpPtr(pkt_log_data_t, data);

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

      //make the parameter string names lower case
      for (size_t i = 0; i < PKT_PARAM_MAX; ++i) {
        char param_str[50] = {0};
        for (size_t j = 0; PARAM_STR[i][j]; ++j) {
          param_str[j] = tolower(PARAM_STR[i][j]);
        }

        sprintf(buff, "%s,",  param_str);

        //then print the titles into the files
        if (fprintf(recorder->output_fd, buff) < 0)
          return RECORDING_WRITE_FILE_ERR;
      }

      if (fprintf(recorder->output_fd, "\n") < 0)
          return RECORDING_WRITE_FILE_ERR;

      //Then print the actual values to the file
      for (size_t i = 0; i < PKT_PARAM_MAX; ++i) {
        sprintf(buff, "%f,", rec_params[i]);
        if (fprintf(recorder->output_fd, buff) < 0)
          return RECORDING_WRITE_FILE_ERR;
      }

      if (fprintf(recorder->output_fd, "\n\n") < 0)
          return RECORDING_WRITE_FILE_ERR;

      const char *log_string =
        "r_hip_angle,"
        "l_hip_angle,"

        "r_knee_current,"
        "r_knee_velocity,"
        "r_knee_angle,"

        "r_knee_current_commanded,"
        "r_knee_velocity_commanded,"
        "r_knee_angle_commanded,"

        "l_knee_current,"
        "l_knee_velocity,"
        "l_knee_angle,"

        "l_knee_current_commanded,"
        "l_knee_velocity_commanded,"
        "l_knee_angle_commanded,"

        "timestamp,"

        "an_0_l_fsr_heel,"
        "an_2_r_fsr_heel,"
        "an_1_l_fsr_ff,"
        "an_3_r_fsr_ff,"
        "an_11_bat_volt,"

        "state,"
        "time_marker"
        "\n";

      if (fprintf(recorder->output_fd, log_string) < 0)
        return RECORDING_WRITE_FILE_ERR;

    case RECORDING_ENABLED:
      //record the data in the file
      if (fprintf(recorder->output_fd,
            "%f,"
            "%f,"
            "%f,"
            "%f,"
            "%f,"
            "%f,"
            "%f,"
            "%f,"
            "%f,"
            "%f,"
            "%f,"
            "%f,"
            "%f,"
            "%f,"
            "%d,"
            "%d,"
            "%d,"
            "%d,"
            "%d,"
            "%d,"
            "%d,"
            "%d"
            "\n",
              data_out->r_hip_angle,
              data_out->l_hip_angle,

              data_out->r_knee_current,
              data_out->r_knee_velocity,
              data_out->r_knee_angle,

              data_out->r_knee_current_commanded,
              data_out->r_knee_velocity_commanded,
              data_out->r_knee_angle_commanded,

              data_out->l_knee_current,
              data_out->l_knee_velocity,
              data_out->l_knee_angle,

              data_out->l_knee_current_commanded,
              data_out->l_knee_velocity_commanded,
              data_out->l_knee_angle_commanded,

              data_out->timestamp,

              data_out->an_0_l_fsr_heel,
              data_out->an_2_r_fsr_heel,
              data_out->an_1_l_fsr_ff,
              data_out->an_3_r_fsr_ff,
              data_out->an_11_bat_volt,

              data_out->state,
              data_out->time_marker
              ) < 0)
                return RECORDING_WRITE_FILE_ERR;

      break;
    default:
      break;
  }
  return RECORDING_OK;
}
