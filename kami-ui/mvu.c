/* ===============================================================
 *                   UI INCLUDES AND DEFINES
 * ===============================================================*/

#include <mvu.h>
#include <packet_drone.h>
#include <tcpip.h>
#include <ring_buffer.h>
#include <record.h>
#include <serial.h>
#include <string.h>
#include <nk_utility.h>
#include <stdarg.h>
#ifdef __linux__
#include <linux/limits.h>
#else
#include <limits.h>
#endif


#define R_BUFF_SIZE 250
#define CHART_MAX 13
#define CHART_MIN -13
#define PRINTF_DEBUG
#define PORT 25550

#define DEFAULT_SERIAL "/dev/ttyUSB0"
#define CHART_HEIGHT 250

/* ===============================================================
 *                   STATE ENUMERATIONS
 * ===============================================================*/
enum graph_state_t {
  GRAPH_PLAY,
  GRAPH_PAUSE,
  GRAPH_STATE_MAX
};

enum port_state_t {
  PORT_CLOSED,
  PORT_OPEN,
  PORT_STATE_MAX
};

enum window_tab_t {
  TAB_OVERVIEW,
  TAB_MAX
};

/*
 * If you want to add another buffer, just add it here.
 * Alloc and dealloc will be automatically handled.
 */
enum ring_buffer_index_t {
  RB_ROLL,
  RB_PITCH,
  RB_YAW,
  RB_MAX
};

enum button_press_t {
  BTN_OPEN_PORT,
  BTN_GRAPH,
  BTN_MAX
};


/* ===============================================================
 *                   STATE LABELS
 * ===============================================================*/
const char *GRAPH_PLAY_BUTTON_LABELS[GRAPH_STATE_MAX] = {
  "Pause Graphs",
  "Play Graphs"
};

const char *RECORD_BUTTON_LABELS[RECORDING_MAX] = {
  "Record Data",
  "Stop Recording",
  "Stop Recording"
};

const char *PORT_BUTTON_LABELS[PORT_STATE_MAX] = {
  "Connect to IP",
  "Disconnect from IP",
};

#define PARAM_LEN 50

const char *PARAM_TITLE_STR[] = {
  FOREACH_PARAM(GENERATE_STRING)
};

struct mvu_model_s {
  struct nk_context *ctx;

  ring_buffer_t ring_buffers[RB_MAX];

  enum graph_state_t graph_state;
  enum port_state_t port_state;

  pkt_generic_t input_packet;

  sr_port_t serial_port;
  tcp_connection_t tcp_conn;

  bool read_data;

  float rec_params[PKT_PARAM_MAX]; //parameters recorded from the chip
  //parameters to send - may possibly not match rec_params
  float parameters[PKT_PARAM_MAX];
};

struct mvu_msg_s {
  char serial_addr[PATH_MAX];
  int serial_addr_len;
  char param_str[PKT_PARAM_MAX][PARAM_LEN];
  int param_str_lens[PKT_PARAM_MAX];
  bool button_presses[BTN_MAX];
};

static void mvu_printf(char *format, ...) {
#ifdef PRINTF_DEBUG
  va_list args;
  va_start (args, format);
  vprintf (format, args);
  va_end (args);
#endif
}
/*----------------------------------------------------------------------------*/
/* MSG FUNCTIONS */

//psoc doesn't like sending all at once for some reason.
static size_t mvu_sendData(mvu_model_t model, uint8_t *buff, size_t len) {
  size_t bytes_sent = 0;
  for (size_t i = 0; i < len; ++i) {
    tcp_sendData(model->tcp_conn, &buff[i], 1);
  }
  return bytes_sent;
}

static void mvu_clearMessage(mvu_msg_t msg) {
  for (size_t i = 0; i < BTN_MAX; ++i) {
    msg->button_presses[i];
  }
  //memset(msg->serial_addr, 0, PATH_MAX);
  //*msg.serial_addr_len = 0
}

mvu_msg_t mvu_createMessage() {
  return malloc(sizeof(struct mvu_msg_s));
}

void mvu_destroyMessage(mvu_msg_t msg) {
  free(msg);
}

/* MODEL FUNCTIONS */
mvu_model_t mvu_createModel() {
  return malloc(sizeof(struct mvu_model_s));
}


bool mvu_initModel(mvu_model_t model, struct nk_context *ctx) {
  model->ctx = ctx;

  for (size_t i = 0; i < RB_MAX; ++i) {
    model->ring_buffers[i] = rb_create(R_BUFF_SIZE);
  }

  pkt_init(&model->input_packet);

  model->read_data = true;
  model->tcp_conn = tcp_create();

  return true;
} //end mvu_createModel

void mvu_destroyModel(mvu_model_t model) {
  for (size_t i = 0; i < RB_MAX; ++i) {
    rb_destroy(model->ring_buffers[i]);
  }
  tcp_destroy(model->tcp_conn);
  free(model);
}

/*----------------------------------------------------------------------------*/

static void mvu_updateBuffers(mvu_model_t model) {
  /*
  pkt_log_data_t *pkt_log_payload =
    pkt_interp(pkt_log_data_t, model->input_packet);

  rb_pushBack(model->ring_buffers[RB_L_SETPOINT],
      pkt_log_payload->l_knee_angle_commanded);
  */
}

static void mvu_getPacket(mvu_model_t model) {
  uint8_t input_byte = 0;

  //wait for whole packet to populate
  do {
    tcp_receiveData(model->tcp_conn, &input_byte, 1);
  } while (!pkt_readByte(&model->input_packet, input_byte));
  pkt_decodePacket(&model->input_packet);
}

static void mvu_recordData(mvu_model_t model) {
#define rec_printerr(arg) \
  printf(#arg "\n")

  /*
  switch(recordData(&model->data_recorder, &model->input_packet,
        model->rec_params)) {
    case RECORDING_OK:
      break;
    case RECORDING_OPEN_FILE_ERR:
      rec_printerr(RECORDING_OPEN_FILE_ERR);
      break;

    case RECORDING_WRITE_FILE_ERR:
      rec_printerr(RECORDING_WRITE_FILE_ERR);
      break;

    case RECORDING_CLOSE_FILE_ERR:
      rec_printerr(RECORDING_CLOSE_FILE_ERR);
      break;
  }
  */
}

static void mvu_handleParams(mvu_model_t model) {
  pkt_get_params_t *params_packet = pkt_interp(pkt_get_params_t, model->input_packet);
  for (size_t i = 0; i < PKT_PARAM_MAX; ++i) {
    model->rec_params[i] = params_packet->float_params[i];
  }
}

static void mvu_handleData(mvu_model_t model) {
  mvu_getPacket(model);

  switch(model->input_packet.type) {
    /*
    case PKT_LOG_DATA:
      if (model->graph_state == GRAPH_PLAY) {
        mvu_updateBuffers(model);
      }

      mvu_recordData(model);
      break;
    case PKT_GET_PARAMS:
      mvu_handleParams(model);
      break;
      */
  }

  pkt_clear(&model->input_packet);
} //end mvu_readData

static void mvu_waitForAck(mvu_model_t model) {
  printf("wait for ack\n");
  while (model->input_packet.type != PKT_ACK) {
    printf("waiting for ack\n");
    mvu_getPacket(model);
  }
  mvu_printf("found ack\n");
  pkt_clear(&model->input_packet);
}

static void mvu_sendPacket(mvu_model_t model, pkt_generic_t *packet) {
  mvu_sendData(model, pkt_encodeBuffer(packet), pkt_getTotalLength(packet));
}

static void mvu_sendStartPacket(mvu_model_t model) {
  pkt_generic_t start_message;
  pkt_setHeaderTypeOnly(&start_message, PKT_DATA_START);
  mvu_sendPacket(model, &start_message);
}

static void mvu_sendHeaderMsg(mvu_model_t model, pkt_type_t type) {
  pkt_generic_t header_message;
  pkt_setHeaderTypeOnly(&header_message, type);
  mvu_sendPacket(model, &header_message);
}

static void mvu_sendSyncPacket(mvu_model_t model) {
  uint8_t stop_message = PKT_SYN;
  mvu_sendData(model, &stop_message, 1);
}

static void mvu_forceSync(mvu_model_t model) {
  mvu_sendSyncPacket(model);
  mvu_waitForAck(model);
}

static void mvu_tryOpenPort(const mvu_msg_t msg, mvu_model_t model) {

  printf("Connecting to host\n");
  if (tcp_getSocket(model->tcp_conn) == SOCKET_ERROR) {
    bool retval = tcp_connectToHost(model->tcp_conn, PORT, msg->serial_addr);
    if (retval) {
      model->port_state = PORT_OPEN;
      mvu_sendStartPacket(model);
      mvu_sendHeaderMsg(model, PKT_GET_PARAMS);
    }
  }
  else {
    tcp_closeSocket(model->tcp_conn);
    model->port_state = PORT_CLOSED;
  }
}


static void mvu_readParams(const mvu_msg_t msg, mvu_model_t model) {
  for (size_t i = 0; i < PKT_PARAM_MAX; ++i) {
    sscanf(msg->param_str[i], "%f", &model->parameters[i]);
  }
}

static void mvu_sendParamsMsg(mvu_model_t model) {
  pkt_generic_t params_message;
  pkt_init(&params_message);
  pkt_setHeader(&params_message, PKT_SET_PARAMS, sizeof(pkt_set_params_t));
  pkt_set_params_t *set_params_payload = pkt_interp(pkt_set_params_t,
      params_message);

  mvu_forceSync(model);
  for (size_t i = 0; i < PKT_PARAM_MAX; ++i) {
    set_params_payload->float_params[i] = model->parameters[i];
  }

  mvu_sendPacket(model, &params_message);
  //mvu_forceSync(model);
  mvu_sendStartPacket(model);
}


static void mvu_sendStateMsg(mvu_model_t model, uint8_t state) {
  pkt_generic_t state_message;
  pkt_init(&state_message);
  pkt_setHeader(&state_message, PKT_SET_STATE, sizeof(pkt_set_state_t));
  pkt_set_state_t *set_state_payload = pkt_interp(pkt_set_state_t,
      state_message);

  mvu_forceSync(model);
  set_state_payload->state = state;

  mvu_sendPacket(model, &state_message);
  //mvu_forceSync(model);
  mvu_sendStartPacket(model);
}

/*----------------------------------------------------------------------------*/
/* UPDATE FUNCTION */

void mvu_update(const mvu_msg_t msg, mvu_model_t model) {

  /* Open and close serial port */
  if (msg->button_presses[BTN_OPEN_PORT]) {
    mvu_tryOpenPort(msg, model);
  }

  if (msg->button_presses[BTN_GRAPH])
    model->graph_state = !model->graph_state;

  if (tcp_getSocket(model->tcp_conn) == SOCKET_ERROR);
    return;

  /*
  if (msg->button_presses[BTN_RECORD])
    recordButtonEvent(&model->data_recorder);

  if (msg->button_presses[BTN_TRIAL]) {

    if (model->trial_state == TRIAL_ENDED) {
      mvu_sendStateMsg(model, L_DBL_STANCE);
      model->trial_state = TRIAL_ONGOING;
    }

    else if(model->trial_state == TRIAL_ONGOING) {
      mvu_sendStateMsg(model, LOCKED);
      model->trial_state = TRIAL_ENDED;
    }

    recordButtonEvent(&model->data_recorder);
  }

  if (model->read_data) {
    mvu_handleData(model);
  }

  if (msg->button_presses[BTN_FORCE_ACK]) {
    mvu_forceSync(model);
    model->read_data = false;
  }

  if (msg->button_presses[BTN_REQUEST_DATA]) {
    mvu_sendStartPacket(model);
    model->read_data = true;
  }

  if (msg->button_presses[BTN_GET_PARAMS]) {
    mvu_sendHeaderMsg(model, PKT_GET_PARAMS);
  }

  if (msg->button_presses[BTN_SET_PARAMS]) {
    mvu_readParams(msg, model);
    mvu_sendParamsMsg(model);
    mvu_sendHeaderMsg(model, PKT_GET_PARAMS);
  }

  if (msg->button_presses[BTN_MOTOR_TOGGLE]) {
    mvu_readParams(msg, model);
    model->parameters[MOTOR_FORCE_OFF] = !(bool) model->rec_params[MOTOR_FORCE_OFF];
    mvu_sendParamsMsg(model);
    mvu_sendHeaderMsg(model, PKT_GET_PARAMS);
  }

  if (msg->button_presses[BTN_ZERO_L_HIP]) {
      mvu_sendHeaderMsg(model, PKT_CALIBRATE_LHIP);
  }

  if (msg->button_presses[BTN_ZERO_R_HIP]) {
      mvu_sendHeaderMsg(model, PKT_CALIBRATE_RHIP);
  }

  if (msg->button_presses[BTN_UNLOCK]) {
      mvu_sendStateMsg(model, UNLOCKED);
  }
  else if (msg->button_presses[BTN_LOCK]) {
      mvu_sendStateMsg(model, LOCKED);
  }
  else if (msg->button_presses[BTN_L_STEP]) {
      mvu_sendStateMsg(model, L_DBL_STANCE);
  }
  else if (msg->button_presses[BTN_R_STEP]) {
      mvu_sendStateMsg(model, R_DBL_STANCE);
  }
  */
}

/*----------------------------------------------------------------------------*/
/* VIEW FUNCTION */

static void mvu_drawPIDPlot(const mvu_model_t model, mvu_msg_t msg) {
  struct nk_context *ctx = model->ctx;
  /*
#define PLOT1_NUM_LINES 2
#define PLOT2_NUM_LINES 2
#define PLOT3_NUM_LINES 2
#define PLOT4_NUM_LINES 2

  float draw_setpoint_buff[R_BUFF_SIZE] = {0};
  float draw_pos_buff[R_BUFF_SIZE] = {0};

  float draw_speed_buff[R_BUFF_SIZE] = {0};
  float draw_com_speed_buff[R_BUFF_SIZE] = {0};

  float draw_current_buff[R_BUFF_SIZE] = {0};
  float draw_com_current_buff[R_BUFF_SIZE] = {0};

  float draw_motor_pwm_buff[R_BUFF_SIZE] = {0};
  float draw_motor_direction_buff[R_BUFF_SIZE] = {0};

  enum ring_buffer_index_t setpoint_index =
    leg_graph == LEG_GRAPH_LEFT ?  RB_L_SETPOINT : RB_R_SETPOINT;

  enum ring_buffer_index_t position_index =
    leg_graph == LEG_GRAPH_LEFT ?  RB_L_POSITION : RB_R_POSITION;

  enum ring_buffer_index_t velocity_index =
    leg_graph == LEG_GRAPH_LEFT ? RB_L_VELOCITY : RB_R_VELOCITY;

  enum ring_buffer_index_t velocity_command_index =
    leg_graph == LEG_GRAPH_LEFT ? RB_L_VELOCITY_COMMAND : RB_R_VELOCITY_COMMAND;

  enum ring_buffer_index_t current_index =
    leg_graph == LEG_GRAPH_LEFT ? RB_L_CURRENT : RB_R_CURRENT;

  enum ring_buffer_index_t current_command_index =
    leg_graph == LEG_GRAPH_LEFT ? RB_L_CURRENT_COMMAND : RB_R_CURRENT_COMMAND;

  struct plot_object_s plot1_lines[PLOT1_NUM_LINES] = {0};
  struct plot_object_s plot2_lines[PLOT2_NUM_LINES] = {0};
  struct plot_object_s plot3_lines[PLOT3_NUM_LINES] = {0};
  struct plot_object_s plot4_lines[PLOT4_NUM_LINES] = {0};

  rb_read(model->ring_buffers[setpoint_index], draw_setpoint_buff);
  rb_read(model->ring_buffers[position_index], draw_pos_buff);
  rb_read(model->ring_buffers[velocity_index], draw_speed_buff);
  rb_read(model->ring_buffers[velocity_command_index], draw_com_speed_buff);

  rb_read(model->ring_buffers[current_index], draw_current_buff);
  rb_read(model->ring_buffers[current_command_index], draw_com_current_buff);

  rb_read(model->motor_pwm_buff, draw_motor_pwm_buff);
  rb_read(model->motor_direction_buff, draw_motor_direction_buff);

  plot1_lines[0] = (struct plot_object_s) {
    .draw_buffer = draw_pos_buff,
      .length      = R_BUFF_SIZE,
      .title       = "Setpoint: red, Pos: black",
      .color       = nk_rgb(50, 50, 50),
      .height      = CHART_HEIGHT,
      .ymin        = DPS_CHART_MIN,
      .ymax        = DPS_CHART_MAX
  };

  plot1_lines[1] = (struct plot_object_s) {
    .draw_buffer = draw_setpoint_buff,
      .length      = R_BUFF_SIZE,
      .title       = "Setpoint: red, Pos: black",
      .color       = nk_rgb(255, 0, 0),
      .height      = CHART_HEIGHT,
      .ymin        = DPS_CHART_MIN,
      .ymax        = DPS_CHART_MAX
  };



  plot2_lines[0] = (struct plot_object_s) {
    .draw_buffer = draw_speed_buff,
      .length      = R_BUFF_SIZE,
      .title       = "Com Vel: red, Vel: black",
      .color       = nk_rgb(50, 50, 50),
      .height      = CHART_HEIGHT,
      .ymin        = DPS_CHART_MIN,
      .ymax        = DPS_CHART_MAX
  };

  plot2_lines[1] = (struct plot_object_s) {
    .draw_buffer = draw_com_speed_buff,
      .length      = R_BUFF_SIZE,
      .title       = "Com Vel: red, Vel: black",
      .color       = nk_rgb(255, 0, 0),
      .height      = CHART_HEIGHT,
      .ymin        = DPS_CHART_MIN,
      .ymax        = DPS_CHART_MAX
  };

  plot3_lines[0] = (struct plot_object_s) {
    .draw_buffer = draw_current_buff,
      .length      = R_BUFF_SIZE,
      .title       = "Com current: red, Current: black",
      .color       = nk_rgb(50, 50, 50),
      .height      = CHART_HEIGHT,
      .ymin        = CHART_MIN,
      .ymax        = CHART_MAX
  };


  plot3_lines[1] = (struct plot_object_s) {
    .draw_buffer = draw_com_current_buff,
      .length      = R_BUFF_SIZE,
      .title       = "Current: black, Com Current: red",
      .color       = nk_rgb(255, 0, 0),
      .height      = CHART_HEIGHT,
      .ymin        = CHART_MIN,
      .ymax        = CHART_MAX
  };


  plot4_lines[0] = (struct plot_object_s) {
    .draw_buffer = draw_motor_pwm_buff,
      .length      = R_BUFF_SIZE,
      .title       = "Com current: red, Current: black",
      .color       = nk_rgb(50, 50, 50),
      .height      = CHART_HEIGHT,
      .ymin        = PWM_CHART_MIN,
      .ymax        = PWM_CHART_MAX
  };


  plot4_lines[1] = (struct plot_object_s) {
    .draw_buffer = draw_motor_direction_buff,
      .length      = R_BUFF_SIZE,
      .title       = "Current: black, Com Current: red",
      .color       = nk_rgb(255, 0, 0),
      .height      = CHART_HEIGHT,
      .ymin        = PWM_CHART_MIN,
      .ymax        = PWM_CHART_MAX
  };

  nk_layout_row_dynamic(ctx, CHART_HEIGHT, 2);
  plotN(ctx, plot1_lines, PLOT1_NUM_LINES);
  plotN(ctx, plot2_lines, PLOT2_NUM_LINES);

  nk_layout_row_dynamic(ctx, CHART_HEIGHT, 2);
  plotN(ctx, plot3_lines, PLOT3_NUM_LINES);
  //plotN(ctx, plot4_lines, PLOT4_NUM_LINES);
  */
}

static void mvu_drawParamTextboxes(const mvu_model_t model, mvu_msg_t msg) {
  //for (size_t i = 0; i < PKT_PARAM_MAX; ++i) {
  for (int64_t i = PKT_PARAM_MAX-1; i >= 0; --i) {
    nk_layout_row_dynamic(model->ctx, 40, 3);
    char value[30];
    sprintf(value, "%f", model->rec_params[i]);
    nk_label(model->ctx, PARAM_TITLE_STR[i], NK_TEXT_LEFT);
    editFieldTitle(model->ctx, value, msg->param_str[i],
        &msg->param_str_lens[i], PARAM_LEN);
  }
}

void mvu_textDisplay(const mvu_model_t model, mvu_msg_t msg) {
  struct nk_context *ctx = model->ctx;

  /*
  nk_layout_row_dynamic(ctx, 40, 1);
  char buff[100];
  sprintf(buff, "State: %s", GED_STATE_STR[model->ged_state]);
  nk_label(ctx, buff, NK_TEXT_LEFT);

  nk_layout_row_dynamic(ctx, 40, 2);

  sprintf(buff, "L Hip Angle: %f", model->l_hip_angle);
  nk_label(ctx, buff, NK_TEXT_LEFT);

  sprintf(buff, "R Hip Angle: %f", model->r_hip_angle);
  nk_label(ctx, buff, NK_TEXT_LEFT);

  sprintf(buff, "L FSR FF: %f", model->l_fsr_ff);
  nk_label(ctx, buff, NK_TEXT_LEFT);

  sprintf(buff, "R FSR FF: %f", model->r_fsr_ff);
  nk_label(ctx, buff, NK_TEXT_LEFT);

  sprintf(buff, "L FSR Heel: %f", model->l_fsr_heel);
  nk_label(ctx, buff, NK_TEXT_LEFT);

  sprintf(buff, "R FSR Heel: %f", model->r_fsr_heel);
  nk_label(ctx, buff, NK_TEXT_LEFT);

  sprintf(buff, "L Knee Angle: %f", model->l_knee_angle);
  nk_label(ctx, buff, NK_TEXT_LEFT);

  sprintf(buff, "R Knee Angle: %f", model->r_knee_angle);
  nk_label(ctx, buff, NK_TEXT_LEFT);
  */
}

void  mvu_view(const mvu_model_t model, mvu_msg_t msg) {
  struct nk_context *ctx = model->ctx;
  mvu_clearMessage(msg);

  /* First row */
  nk_layout_row_dynamic(ctx, 35, 2);
  nk_edit_string(ctx, NK_EDIT_FIELD | NK_TEXT_EDIT_SINGLE_LINE, msg->serial_addr, &msg->serial_addr_len, PATH_MAX,
      nk_filter_default);
  msg->serial_addr[msg->serial_addr_len] = 0;

  msg->button_presses[BTN_OPEN_PORT] = multiLabelToggle(ctx, PORT_BUTTON_LABELS, model->port_state);

  /*
  tab selection row
  nk_layout_row_dynamic(ctx, 35, 3);
  model->current_tab = nk_option_label(ctx, "Overview", model->current_tab == TAB_OVERVIEW) ?
    TAB_OVERVIEW : model->current_tab;

  model->current_tab = nk_option_label(ctx, "Parameters", model->current_tab == TAB_PARAMETERS) ?
    TAB_PARAMETERS : model->current_tab;

  model->current_tab = nk_option_label(ctx, "State Override", model->current_tab == TAB_STATE) ?
    TAB_STATE : model->current_tab;

  switch (model->current_tab) {
    case TAB_OVERVIEW:
      nk_layout_row_dynamic(ctx, 40, 1);
      msg->button_presses[BTN_GRAPH] = //play button
        multiLabelToggle(ctx, GRAPH_PLAY_BUTTON_LABELS, model->graph_state);

      msg->button_presses[BTN_RECORD] = multiLabelToggle(ctx, RECORD_BUTTON_LABELS,
          getRecorderState(&model->data_recorder)); //record button

      msg->button_presses[BTN_TRIAL] = multiLabelToggle(ctx, TRIAL_BUTTON_LABELS,
          model->trial_state);

      msg->button_presses[BTN_MOTOR_TOGGLE] = multiLabelToggle(ctx,
          MOTOR_TOGGLE_LABELS, (bool) model->rec_params[MOTOR_FORCE_OFF]);

      //combobox for deciding what plots to plot
      nk_layout_row_dynamic(ctx, 40, 2);
      model->leg_graph = nk_option_label(ctx, "Left Leg Graph", model->leg_graph == LEG_GRAPH_LEFT) ?
        LEG_GRAPH_LEFT : model->leg_graph;

      model->leg_graph = nk_option_label(ctx, "Right Leg Graph", model->leg_graph == LEG_GRAPH_RIGHT) ?
        LEG_GRAPH_RIGHT : model->leg_graph;


      mvu_drawPIDPlot(model, msg, model->leg_graph);

      nk_layout_row_dynamic(ctx, 40, 2);
      msg->button_presses[BTN_ZERO_L_HIP] = nk_button_label(ctx, "Zero L Hip");
      msg->button_presses[BTN_ZERO_R_HIP] = nk_button_label(ctx, "Zero R Hip");

      mvu_textDisplay(model, msg);
      break;

    case TAB_PARAMETERS:

      nk_layout_row_dynamic(ctx, 40, 3);

      msg->button_presses[BTN_GET_PARAMS] = nk_button_label(ctx, "Get Parameters");
      msg->button_presses[BTN_SYNC_TEXTBOX] =
        nk_button_label(ctx, "Sync Textboxes");
      msg->button_presses[BTN_SET_PARAMS] = nk_button_label(ctx, "Set Parameters");

      mvu_drawParamTextboxes(model, msg);

      if(msg->button_presses[BTN_SYNC_TEXTBOX]) {
        for (size_t i = 0; i < PKT_PARAM_MAX; ++i) {
          sprintf(msg->param_str[i], "%f", model->rec_params[i]);
          msg->param_str_lens[i] = strlen(msg->param_str[i]);
        }
      }
      break;

    case TAB_STATE:
      nk_layout_row_dynamic(ctx, 40, 2);
      msg->button_presses[BTN_UNLOCK] = nk_button_label(ctx, "Unlock");
      msg->button_presses[BTN_LOCK] = nk_button_label(ctx, "Lock");

      nk_layout_row_dynamic(ctx, 40, 2);
      msg->button_presses[BTN_L_STEP] = nk_button_label(ctx, "Left Step");
      msg->button_presses[BTN_R_STEP] = nk_button_label(ctx, "Right Step");

      mvu_textDisplay(model, msg);
      break;
  }
*/
} //end mvu_view

